#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ir.h"
#include "motor.h"
#include "sonic.h"

// INT0 (IR 수신기)를 위한 인터럽트 서비스 루틴
ISR(INT0_vect)
{
    uint8_t time = TCNT0;                  // 현재 타이머 값 가져오기
    uint8_t overflow = TIFR & (1 << TOV0); // 타이머 오버플로우 확인

    if (bitCount == 32)
    {
        // 시작 비트 확인
        if ((time > 201) && (time < 221) && (overflow == 0))
        {
            receivedData = 0;
            bitCount = 0;
        }
        else if ((time > 166) && (time < 186) && (overflow == 0))
        {
            // 반복 데이터 처리
        }
        else
        {
            bitCount = 32; // 유효하지 않은 시작이면 리셋
        }
    }
    else
    {
        // 데이터 비트 처리
        if ((time > 40) || (overflow != 0))
        {
            bitCount = 32; // 유효하지 않은 비트 타이밍이면 리셋
        }
        else
        {
            // 비트 값 결정 및 receivedData로 시프트
            if (time > 26)
                receivedData = (receivedData << 1) + 1;
            else
                receivedData = (receivedData << 1);

            if (bitCount == 31)
            {
                // 데이터 수신 완료, 모터 제어
                if (receivedData == 0xFF18E7)
                {
                    motorForward();
                }
                else if (receivedData == 0xFF4AB5)
                {
                    motorBackward();
                }
                else if (receivedData == 0xFF38C7)
                {
                    motorStop();
                }
                else if (receivedData == 0xFF10EF)
                {
                    motorCrabLeft();
                }
                else if (receivedData == 0xFF5AA5)
                {
                    motorCrabRight();
                }
                else if (receivedData == 0xFF6897)
                {
                    motorRotateLeft();
                }
                else if (receivedData == 0xFFB04F)
                {
                    motorRotateRight();
                }
                bitCount = 32; // 다음 수신을 위해 리셋
            }
            else
            {
                bitCount++;
            }
        }
    }

    TCNT0 = 0;                 // 타이머 카운터 리셋
    TIFR = TIFR | (1 << TOV0); // 타이머 오버플로우 플래그 클리어
}

// 타이머2 오버플로우 ISR
ISR(TIMER2_OVF_vect)
{
    overflow_count++;
    if (overflow_count >= 235)
    {                     // 초음파 신호 타임아웃 (약 30ms)
        measure_flag = 0; // 측정 초기화
        overflow_count = 0;
        distance = 0; // 측정 실패로 거리 초기화
    }
}

// INT7 외부 인터럽트 ISR (Echo 신호 처리)
ISR(INT7_vect)
{
    static uint8_t edge_state = 0; // 상승/하강 엣지 상태 추적
    if (edge_state == 0)
    {                       // 상승 엣지 감지 (Echo 시작)
        TCNT2 = 0x00;       // 타이머2 초기화
        overflow_count = 0; // 오버플로우 카운터 초기화
        edge_state = 1;
    }
    else
    {                                                                     // 하강 엣지 감지 (Echo 종료)
        echo_time = 1000000 * (overflow_count * 256 + TCNT2) * 8 / F_CPU; // Echo 시간 계산(us)
        distance = echo_time / 58;                                        // 거리(cm) 계산
        edge_state = 0;
        measure_flag = 0; // 측정 완료
    }
}

int main(void)
{
    DDRE |= (1 << PE6); // PORTE6: 초음파 트리거 핀 (출력)

    // 타이머2 설정
    TCCR2 = (1 << CS21); // 분주비 8 설정, 타이머 tick = 0.5 μs
    TCNT2 = 0;
    TIMSK |= (1 << TOIE2); // 타이머2 오버플로우 인터럽트 활성화

    pwm_init(); // PWM 초기화 (50% 속도로 고정)

    // 타이머 0 설정
    TCCR0 |= (1 << CS00) | (1 << CS01) | (1 << CS02); // 분주비 1024로 설정

    EIMSK |= (1 << INT7);  // INT7 활성화
    EICRB |= (1 << ISC70); // 상승/하강 엣지 모두 인터럽트를 트리거시킴

    // 외부 인터럽트 설정
    EICRA |= (1 << ISC01); // 하강 에지에서 인터럽트 발생
    EIMSK |= (1 << INT0);  // INT0 인터럽트 활성화

    sei(); // 전역 인터럽트 활성화

    while (1)
    {
        if (measure_flag == 0)
        {
            PORTE |= (1 << PE6);  // 초음파 트리거 HIGH
            _delay_us(10);        // 10μs 유지
            PORTE &= ~(1 << PE6); // 초음파 트리거 LOW
            measure_flag = 1;     // 측정 시작
        }

        // 거리 기반 부저 제어
        if (distance < 10 && distance > 0)
        {                // 10cm 미만일 때 부저 ON
            motorStop(); // 모터 정지
        }
    }
    return 0;
}

#define F_CPU 12000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ir.h"
#include "servo.h"

// INT0 (IR 수신기)를 위한 인터럽트 서비스 루틴
ISR(INT0_vect)
{
    uint8_t time = TCNT0;                  // 현재 타이머 값 가져오기
    uint8_t overflow = TIFR & (1 << TOV0); // 타이머 오버플로우 확인

    // 시작 비트(리드 코드) 확인(버튼 한번 눌렀을때의 신호=일반데이터, 꾹눌렀을때 = 반복데이터)
    if (bitCount == 32)
    { // 시작 자체를 32로 시작

        // 일반데이터의 시작비트가 확인되면 데이터와 비트카운트를 0으로 세팅 이후 데이터비트 수신 준비
        if ((time > 150) && (time < 165) && (overflow == 0))
        {
            receivedData = 0;
            bitCount = 0;
        }

        // 반복 데이터 처리(안써서 안만들어둠)
        // else if ((time > 124) && (time < 139) && (overflow == 0)) {
        //}

        // 그외 유효하지 않은 데이터로 간주, bitCount를 32로 설정하여 다시 리드코드를 읽으러감
        else
        {
            bitCount = 32;
        }
    }

    // 데이터 비트(논리 데이터) 처리
    else
    {

        // 유효하지 않은 비트 타이밍이면(너무 길게 받거나(논리데이터는 최대가 26틱) 오버플로우가 발생할 경우) bitCount를 32로 설정하여 다시 리드코드를 읽으러감
        if ((time > 30) || (overflow != 0))
        {
            bitCount = 32;
        }

        // 비트 값 결정 및 receivedData 시프트
        else
        {

            // 논리 1의 경우 26틱, 0의 경우 13틱, 19틱 초과시 논리 1로 간주
            if (time > 19)
                receivedData = (receivedData << 1) + 1;
            else
                receivedData = (receivedData << 1);

            // 데이터 수신 완료, 모터 제어함수 불러오기
            if (bitCount == 31)
            {
                if (receivedData == 0xFFA25D)
                {
                    servo_Straight();
                }
                else if (receivedData == 0xFF629D)
                {
                    servo_Diagonal_L();
                }
                else if (receivedData == 0xFFE21D)
                {
                    servo_Diagonal_R();
                }
                else if (receivedData == 0xFF22DD)
                {
                    servo_Crab();
                }
                else if (receivedData == 0xFF9867)
                {
                    servo_Rotate();
                }

                // 다음 수신을 위해 bitCount를 32로 설정하여 다시 리드코드를 읽으러감
                bitCount = 32;
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

int main(void)
{
    // PWM 핀을 출력으로 설정
    DDRB |= (1 << Servo1) | (1 << Servo2); // PORTB의 PB5,6 출력 설정
    DDRE |= (1 << Servo3) | (1 << Servo4); // PORTE의 PE3,4 출력 설정

    InitializeTimer1(); // timer 1 초기화

    // 타이머 0 설정
    TCCR0 |= (1 << CS00) | (1 << CS01) | (1 << CS02); // 분주비 1024로 설정

    // 외부 인터럽트 설정
    EICRA |= (1 << ISC01); // 하강 에지에서 인터럽트 발생
    EIMSK |= (1 << INT0);  // INT0 인터럽트 활성화

    sei(); // 전역 인터럽트 활성화

    while (1)
    {
        // 메인 루프
    }

    return 0;
}
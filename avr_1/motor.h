
// 모터 제어 핀 정의
#define MOTOR1_IN1 PD1
#define MOTOR1_IN2 PD2
#define MOTOR2_IN3 PD4
#define MOTOR2_IN4 PD5
#define MOTOR3_IN1 PC0
#define MOTOR3_IN2 PC1
#define MOTOR4_IN3 PC3
#define MOTOR4_IN4 PC4

// PWM 핀 정의
#define MOTOR1_ENA PB5 // OC1A
#define MOTOR1_ENB PB6 // OC1B
#define MOTOR2_ENA PE3 // OC3A
#define MOTOR2_ENB PE4 // OC3B

void pwm_init()
{
    // Timer/Counter 1 설정 (Fast PWM 모드, TOP = ICR1)
    TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11); // 분주비 8

    // Timer/Counter 3 설정 (Fast PWM 모드, TOP = ICR3)
    TCCR3A |= (1 << WGM31) | (1 << COM3A1) | (1 << COM3B1);
    TCCR3B |= (1 << WGM33) | (1 << WGM32) | (1 << CS31); // 분주비 8

    // PWM 주파수와 분주비에 맞춘 ICR 값 설정 (25 kHz)
    ICR1 = 79; // 16MHz / (8 * 25000) - 1 = 79
    ICR3 = 79;

    // 초기 듀티 사이클 설정 (59%)
    OCR1A = 46; // OC1A 듀티 사이클
    OCR1B = 46; // OC1B 듀티 사이클
    OCR3A = 46; // OC3A 듀티 사이클
    OCR3B = 46; // OC3B 듀티 사이클

    // PWM 핀을 출력으로 설정
    DDRB |= (1 << MOTOR1_ENA) | (1 << MOTOR1_ENB);
    DDRE |= (1 << MOTOR2_ENA) | (1 << MOTOR2_ENB);
}
// 모터를 전진 방향으로 움직이는 함수
void motorForward()
{
    PORTD |= (1 << MOTOR1_IN1);  // MOTOR1 IN1을 HIGH로 설정
    PORTD &= ~(1 << MOTOR1_IN2); // MOTOR1 IN2를 LOW로 설정

    PORTD |= (1 << MOTOR2_IN4);  // MOTOR2 IN4을 HIGH로 설정
    PORTD &= ~(1 << MOTOR2_IN3); // MOTOR2 IN3를 LOW로 설정

    PORTC |= (1 << MOTOR3_IN1);  // MOTOR3 IN1을 HIGH로 설정
    PORTC &= ~(1 << MOTOR3_IN2); // MOTOR3 IN2를 LOW로 설정

    PORTC |= (1 << MOTOR4_IN4);  // MOTOR4 IN4을 HIGH로 설정
    PORTC &= ~(1 << MOTOR4_IN3); // MOTOR4 IN3를 LOW로 설정
}

// 모터를 후진 방향으로 움직이는 함수
void motorBackward()
{
    PORTD &= ~(1 << MOTOR1_IN1); // MOTOR1 IN1을 LOW로 설정
    PORTD |= (1 << MOTOR1_IN2);  // MOTOR1 IN2를 HIGH로 설정

    PORTD &= ~(1 << MOTOR2_IN4); // MOTOR2 IN4을 LOW로 설정
    PORTD |= (1 << MOTOR2_IN3);  // MOTOR2 IN3를 HIGH로 설정

    PORTC &= ~(1 << MOTOR3_IN1); // MOTOR3 IN1을 LOW로 설정
    PORTC |= (1 << MOTOR3_IN2);  // MOTOR3 IN2를 HIGH로 설정

    PORTC &= ~(1 << MOTOR4_IN4); // MOTOR4 IN4을 LOW로 설정
    PORTC |= (1 << MOTOR4_IN3);  // MOTOR4 IN3를 HIGH로 설정
}

// 모터를 정지 시키는 함수
void motorStop()
{
    PORTD &= ~((1 << MOTOR1_IN1) | (1 << MOTOR1_IN2) | (1 << MOTOR2_IN3) | (1 << MOTOR2_IN4));
    PORTC &= ~((1 << MOTOR3_IN1) | (1 << MOTOR3_IN2) | (1 << MOTOR4_IN3) | (1 << MOTOR4_IN4));
}

// 크랩 모드시 오른쪽 방향으로 움직이는 함수
void motorCrabRight()
{
    PORTD |= (1 << MOTOR1_IN1);  // MOTOR1 IN1을 HIGH로 설정
    PORTD &= ~(1 << MOTOR1_IN2); // MOTOR1 IN2를 LOW로 설정

    PORTD |= (1 << MOTOR2_IN3);  // MOTOR2 IN3을 HIGH로 설정
    PORTD &= ~(1 << MOTOR2_IN4); // MOTOR2 IN4를 LOW로 설정

    PORTC |= (1 << MOTOR3_IN2);  // MOTOR3 IN2을 HIGH로 설정
    PORTC &= ~(1 << MOTOR3_IN1); // MOTOR3 IN1를 LOW로 설정

    PORTC |= (1 << MOTOR4_IN4);  // MOTOR4 IN4을 HIGH로 설정
    PORTC &= ~(1 << MOTOR4_IN3); // MOTOR4 IN3를 LOW로 설정
}

// 크랩 모드시 왼쪽 방향으로 움직이는 함수
void motorCrabLeft()
{
    PORTD |= (1 << MOTOR1_IN2);  // MOTOR1 IN2을 HIGH로 설정
    PORTD &= ~(1 << MOTOR1_IN1); // MOTOR1 IN1를 LOW로 설정

    PORTD |= (1 << MOTOR2_IN4);  // MOTOR2 IN4을 HIGH로 설정
    PORTD &= ~(1 << MOTOR2_IN3); // MOTOR2 IN3를 LOW로 설정

    PORTC |= (1 << MOTOR3_IN1);  // MOTOR3 IN1을 HIGH로 설정
    PORTC &= ~(1 << MOTOR3_IN2); // MOTOR3 IN2를 LOW로 설정

    PORTC |= (1 << MOTOR4_IN3);  // MOTOR4 IN3을 HIGH로 설정
    PORTC &= ~(1 << MOTOR4_IN4); // MOTOR4 IN4를 LOW로 설정
}

// 로테이트 모드시 오른쪽 방향으로 회전하는 함수
void motorRotateRight()
{
    PORTD |= (1 << MOTOR1_IN1);  // MOTOR1 IN1을 HIGH로 설정
    PORTD &= ~(1 << MOTOR1_IN2); // MOTOR1 IN2를 LOW로 설정

    PORTD |= (1 << MOTOR2_IN3);  // MOTOR2 IN4을 HIGH로 설정
    PORTD &= ~(1 << MOTOR2_IN4); // MOTOR2 IN3를 LOW로 설정

    PORTC |= (1 << MOTOR3_IN1);  // MOTOR3 IN1을 HIGH로 설정
    PORTC &= ~(1 << MOTOR3_IN2); // MOTOR3 IN2를 LOW로 설정

    PORTC |= (1 << MOTOR4_IN3);  // MOTOR4 IN4을 HIGH로 설정
    PORTC &= ~(1 << MOTOR4_IN4); // MOTOR4 IN3를 LOW로 설정
}

// 로테이트 모드시 왼쪽 방향으로 회전하는 함수
void motorRotateLeft()
{
    PORTD |= (1 << MOTOR1_IN2);  // MOTOR1 IN1을 HIGH로 설정
    PORTD &= ~(1 << MOTOR1_IN1); // MOTOR1 IN2를 LOW로 설정

    PORTD |= (1 << MOTOR2_IN4);  // MOTOR2 IN4을 HIGH로 설정
    PORTD &= ~(1 << MOTOR2_IN3); // MOTOR2 IN3를 LOW로 설정

    PORTC |= (1 << MOTOR3_IN2);  // MOTOR3 IN1을 HIGH로 설정
    PORTC &= ~(1 << MOTOR3_IN1); // MOTOR3 IN2를 LOW로 설정

    PORTC |= (1 << MOTOR4_IN4);  // MOTOR4 IN4을 HIGH로 설정
    PORTC &= ~(1 << MOTOR4_IN3); // MOTOR4 IN3를 LOW로 설정
}
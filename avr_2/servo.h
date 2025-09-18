#define Servo1 PB5
#define Servo2 PB6
#define Servo3 PE3
#define Servo4 PE4

void InitializeTimer1()
{
    ////////////////////고속 PWM 모드 설정/////////////////////
    // 타이머1번
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13);

    // 타이머3번
    TCCR3A |= (1 << WGM11);
    TCCR3B |= (1 << WGM12) | (1 << WGM13);

    //////////////// 비반전 모드 및 분주율 설정 ////////////////
    // 타이머 1번
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
    TCCR1B |= (1 << CS11); // 분주율 8
    ICR1 = 29999;          // 20ms 주기

    // 타이머 3번
    TCCR3A |= (1 << COM3A1) | (1 << COM3B1);
    TCCR3B |= (1 << CS11); // 분주율 8
    ICR3 = 29999;          // 20ms 주기

    // PWM 핀을 출력으로 설정
    DDRB |= (1 << Servo1) | (1 << Servo2); // PORTB의 PB5,6 출력 설정
    DDRE |= (1 << Servo3) | (1 << Servo4); // PORTE의 PE3,4 출력 설정
}

void servo_Straight()
{                 // 1
    OCR1A = 2350; // 0도 회전, FL모터
    OCR1B = 2150; // 0도 회전, FR모터

    OCR3A = 2300; // 0도 회전, RR모터
    OCR3B = 800;  // 0도 회전, RL모터

    // PWM 핀을 출력으로 설정
    DDRB |= (1 << Servo1) | (1 << Servo2); // PORTB의 PB5,6 출력 설정
    DDRE |= (1 << Servo3) | (1 << Servo4); // PORTE의 PE3,4 출력 설정
}

void servo_Diagonal_L()
{                 // 2
    OCR1A = 1700; //   FL모터
    OCR1B = 1400; //   FR모터

    OCR3A = 1550; //  RL모터
    OCR3B = 3000; //  RR모터

    // PWM 핀을 출력으로 설정
    DDRB |= (1 << Servo1) | (1 << Servo2); // PORTB의 PB5,6 출력 설정
    DDRE |= (1 << Servo3) | (1 << Servo4); // PORTE의 PE3,4 출력 설정
}

void servo_Diagonal_R()
{                 // 3
    OCR1A = 3100; // FL모터
    OCR1B = 2900; //  FR모터

    OCR3A = 3050; //  RL모터
    OCR3B = 1550; //  RR모터

    // PWM 핀을 출력으로 설정
    DDRB |= (1 << Servo1) | (1 << Servo2); // PORTB의 PB5,6 출력 설정
    DDRE |= (1 << Servo3) | (1 << Servo4); // PORTE의 PE3,4 출력 설정
}

void servo_Crab()
{                 // 4
    OCR1A = 3800; // -90도 회전, FL모터
    OCR1B = 700;  //  90도 회전, FR모터

    OCR3A = 900;  // -90도 회전, Bl모터
    OCR3B = 2250; // 90도 회전, Br모터

    // PWM 핀을 출력으로 설정
    DDRB |= (1 << Servo1) | (1 << Servo2); // PORTB의 PB5,6 출력 설정
    DDRE |= (1 << Servo3) | (1 << Servo4); // PORTE의 PE3,4 출력 설정
}

void servo_Rotate()
{                 // 0
    OCR1A = 3100; //  FL모터
    OCR1B = 1400; //  FR모터

    OCR3A = 1550; // Bl모터
    OCR3B = 1550; // Br모터

    // PWM 핀을 출력으로 설정
    DDRB |= (1 << Servo1) | (1 << Servo2); // PORTB의 PB5,6 출력 설정
    DDRE |= (1 << Servo3) | (1 << Servo4); // PORTE의 PE3,4 출력 설정
}

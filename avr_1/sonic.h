volatile uint16_t echo_time = 0;      // 에코 신호의 시간
volatile uint16_t distance = 0;       // 물체와의 거리
volatile uint8_t measure_flag = 0;    // 측정 플래그
volatile uint16_t overflow_count = 0; // 타이머 오버플로우 카운터
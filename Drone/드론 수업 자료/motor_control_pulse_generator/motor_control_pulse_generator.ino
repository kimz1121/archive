// 모터 제어 펄스 발생 실험




int channel_3, channel_4; // 스로틀과 러더 의미
unsigned long timer_channel_1; // 모터 구동 시간 설정
unsigned long esc_loop_timer; // 모터 구동 제한 시간 설정
unsigned long zero_timer; // 시간 측정 기준 시간
int count = 0; // 초기화 확인 수


void setup() {
  DDRD |= B00010000; // port 4를 출력으로 지정
  Serial.begin(9600);
  channel_3 = 1000; // 스로틀 최소화 의미
  channel_4 = 1300; // 러더 최소화 의미
  while (channel_4 < 1400) 
  { //모터 초기화
    PORTD |= B00010000; //set port 4 high 최대값 설정
    delayMicroseconds(1000); //1m초 지연
    PORTD &= B11101111; //Set port 4 low. 최소값 설정
    delay(3); //3ms 지연
    if (++count > 200) 
      channel_4 = 1500; //while loop 벗어나기
  }
  zero_timer = micros(); //첫 번 째 루프의 시작 시간
}


void loop() {
  if (channel_3 < 2000) 
    channel_3 += 1; //스로틀을 1200까지 반복
  else 
    channel_3 = 1000; // 스로틀 최저값

  while (zero_timer + 4000 > micros()); //4ms 지연, 내부가 비어 있는 While문 delay의 역할을 대신 하고 있되 드러나는 현상이 다른 듯.

  zero_timer = micros(); //Reset the zero timer.
  PORTD |= B00010000; //Set digital port 4 high.
  timer_channel_1 = channel_3 + zero_timer; // 지연 시간 설정
  //4번 포트가 low일 때 시간 저장
  while (PORTD &= B00010000) 
  {
  //4번 포트가 low가 될 때까지 모터 구동
    esc_loop_timer = micros(); //현재 시간 저장
    if (timer_channel_1 <= esc_loop_timer) PORTD &= B11101111;
  //지연 시간이 종료되면(channel_3 시간이 초과되면) 포트 4번을 low로
  }
}

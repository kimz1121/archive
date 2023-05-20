void setup() {
  // put your setup code here, to run once:
 
 //pinMode(13,OUTPUT);
 //pinMode(8, INPUT_PULLUP); //핀모드 명령어와 저수준 포트제어를 동시에 할 수 있는가??
 DDRC &= B00000000;
 DDRC |= B00000011;
 DDRB &= B00000000;
 DDRB |= B00000011;
}

void loop() {
  // put your main code here, to run repeatedly:
  //PORTC |= B00000001;
  
  if(PINB & B00000100)// &(엠퍼센드)는 어떻게 참값을 내보내는 걸까?? 
  {
    PORTC |= B00000010;
  }
  else
  {
    PORTC &= ~(B00000010);  
  }

  
  if(PINC & B00000100)// &(엠퍼센드)는 어떻게 참값을 내보내는 걸까?? 
  {
    PORTC |= B00000001;
  }
  else
  {
    PORTC &= ~(B00000001);
  }
  
  /*
  PORTC |= B00100000;
  delay(1000);
  PORTC &= ~(B00100000);
  delay(1000);
  */
}

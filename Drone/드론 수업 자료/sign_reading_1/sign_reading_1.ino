int current_count = 0;
byte last_CH1_state = 0;
byte last_CH2_state = 0;
byte last_CH3_state = 0;
byte last_CH4_state = 0;
unsigned long counter_1 = 0;
unsigned long counter_2 = 0;
unsigned long counter_3 = 0;
unsigned long counter_4 = 0; 
int input_ROLL = 0;
int input_PITCH = 0;
int input_YAW = 0;
int input_THROTTLE = 0;

void setup() {

  DDRC |= B00000000;
  //DDRC &= B00001111;

  PCICR |= (1 << PCIE1);    //enable PCMSK1(group PCINT 8~14) scan                                                 
  PCMSK1 |= (1 << PCINT8);  //Set pin A0 trigger an interrupt on state change. 
  PCMSK1 |= (1 << PCINT9);  //Set pin A1 trigger an interrupt on state change.                                             
  PCMSK1 |= (1 << PCINT10);  //Set pin A2 trigger an interrupt on state change.                                               
  PCMSK1 |= (1 << PCINT11);  //Set pin A3 trigger an interrupt on state change.  


  Serial.begin(250000);

}

void loop() {



  Serial.print("thro: ");
  Serial.print(input_THROTTLE);
  Serial.print("   |   ");
  Serial.print("rudd: ");
  Serial.print(input_YAW);
  Serial.print("   |   ");
  Serial.print("aile: ");
  Serial.print(input_ROLL);
  Serial.print("   |   ");
  Serial.print("pitch: ");
  Serial.println(input_PITCH);

}

ISR(PCINT1_vect)
{
  current_count = micros();
  ///////////////////////////////////////Channel 1
  if(PINC & B00001000){                              //We make an AND with the pin state register, We verify if pin 8 is HIGH???
    if(last_CH1_state == 0){                         //If the last state was 0, then we have a state change...
      last_CH1_state = 1;                            //Store the current state into the last state for the next loop
      counter_1 = current_count;                     //Set counter_1 to current value.
    }
  }
  else if(last_CH1_state == 1){                      //If pin 8 is LOW and the last state was HIGH then we have a state change      
    last_CH1_state = 0;                              //Store the current state into the last state for the next loop
    input_ROLL = current_count - counter_1;   //We make the time difference. Channel 1 is current_time - timer_1.
  }



  ///////////////////////////////////////Channel 2
  if(PINC & B00000010 ){                             //pin D9 -- B00000010                                              
    if(last_CH2_state == 0){                                               
      last_CH2_state = 1;                                                   
      counter_2 = current_count;                                             
    }
  }
  else if(last_CH2_state == 1){                                           
    last_CH2_state = 0;                                                     
    input_PITCH = current_count - counter_2;                             
  }


  
  ///////////////////////////////////////Channel 3
  if(PINC & B00000100 ){                             //pin D10 - B00000100                                         
    if(last_CH3_state == 0){                                             
      last_CH3_state = 1;                                                  
      counter_3 = current_count;                                               
    }
  }
  else if(last_CH3_state == 1){                                             
    last_CH3_state = 0;                                                    
    input_THROTTLE = current_count - counter_3;                            

  }


  
  ///////////////////////////////////////Channel 4
  if(PINC & B00001000 ){                             //pin D12  -- B00010000                      
    if(last_CH4_state == 0){                                               
      last_CH4_state = 1;                                                   
      counter_4 = current_count;                                              
    }
  }
  else if(last_CH4_state == 1){                                             
    last_CH4_state = 0;                                                  
    input_YAW = current_count - counter_4;                            
  }
}

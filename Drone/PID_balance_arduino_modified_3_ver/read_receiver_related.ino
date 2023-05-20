





void set_PCICR()
{
  //This part is for the PWM input interruptions
  PCICR |= (1 << PCIE0);    //enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT0);  //Set pin D8 trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT1);  //Set pin D9 trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT2);  //Set pin D10 trigger an interrupt on state change.
  PCMSK0 |= (1 << PCINT4);  //Set pin D12 trigger an interrupt on state change.
}


ISR(PCINT0_vect) {
  //First we take the current count value in micro seconds using the micros() function
  current_count = micros();
  ///////////////////////////////////////Channel 1
  if (PINB & B00000001) {                            //We make an AND with the pin state register, We verify if pin 8 is HIGH???
    if (last_CH1_state == 0) {                       //If the last state was 0, then we have a state change...
      last_CH1_state = 1;                            //Store the current state into the last state for the next loop
      counter_1 = current_count;                     //Set counter_1 to current value.
    }
  }
  else if (last_CH1_state == 1) {                    //If pin 8 is LOW and the last state was HIGH then we have a state change
    last_CH1_state = 0;                              //Store the current state into the last state for the next loop
    input_ROLL = (unsigned long)(current_count - counter_1);   //We make the time difference. Channel 1 is current_time - timer_1.
  }
  ///////////////////////////////////////Channel 2
  if (PINB & B00000010 ) {                           //pin D9 -- B00000010
    if (last_CH2_state == 0) {
      last_CH2_state = 1;
      counter_2 = current_count;
    }
  }
  else if (last_CH2_state == 1) {
    last_CH2_state = 0;
    input_PITCH = (unsigned long)(current_count - counter_2);
  }
  ///////////////////////////////////////Channel 3
  if (PINB & B00000100 ) {                           //pin D10 - B00000100
    if (last_CH3_state == 0) {
      last_CH3_state = 1;
      counter_3 = current_count;
    }
  }
  else if (last_CH3_state == 1) {
    last_CH3_state = 0;
    input_THROTTLE = (unsigned long)(current_count - counter_3);
  }
  ///////////////////////////////////////Channel 4
  if (PINB & B00010000 ) {                           //pin D12  -- B00010000
    if (last_CH4_state == 0) {
      last_CH4_state = 1;
      counter_4 = current_count;
    }
  }
  else if (last_CH4_state == 1) {
    last_CH4_state = 0;
    input_YAW = (unsigned long)(current_count - counter_4);
  }
}

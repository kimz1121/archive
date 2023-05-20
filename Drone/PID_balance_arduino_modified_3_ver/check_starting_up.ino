void set_start_up_Lamp()
{
  DDRB |= B00100000;  //D13 as output
  PORTB &= B11011111; //D13 set to LOW
}

void check_start_up()
{
  if (input_THROTTLE < 1150 && input_YAW > 1800 && !mot_activated)
  {
    if (activate_count == 200)
    {
      mot_activated = 1;
      PORTB |= B00100000; //D13 HIGH

    }
    activate_count = activate_count + 1;
  }
  if (!(input_THROTTLE < 1150 && input_YAW > 1800) && !mot_activated)
  {
    activate_count = 0;
  }

  if (input_THROTTLE < 1150 && input_YAW < 1150 && mot_activated)
  {
    if (des_activate_count == 300)
    {
      mot_activated = 0;
      PORTB &= B11011111; //D13 LOW
    }
    des_activate_count = des_activate_count + 1;
  }
  if (!(input_THROTTLE < 1150 && input_YAW < 1150) && mot_activated)
  {
    des_activate_count = 0;
  }
}

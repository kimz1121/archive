void set_OUTPUT()
{
  pwmLeft = throttle + PID;
  pwmRight = throttle - PID;
}

void OUTPUT_saturation()
{
  //Right
  if (pwmRight < 1000)
  {
    pwmRight = 1000;
  }
  if (pwmRight > 2000)
  {
    pwmRight = 2000;
  }
  //Left
  if (pwmLeft < 1000)
  {
    pwmLeft = 1000;
  }
  if (pwmLeft > 2000)
  {
    pwmLeft = 2000;
  }
}

void write_OUTPUT()
{
  left_prop.writeMicroseconds(pwmLeft);
  right_prop.writeMicroseconds(pwmRight);
}

void calc_error()
{
  desired_angle = map(input_ROLL,1000,2000,-10,10);
  
  previous_error = error; //Remember to store the previous error.
  error = Total_angle[1] - desired_angle;
}



void calc_PID()
{
  //error, previous_error
  //kp pid_p
  //ki pid_i
  //kd pid_d
  //PID 
}

void PID_saturation()
{
  if (PID < -1000)
  {
    PID = -1000;
  }
  if (PID > 1000)
  {
    PID = 1000;
  }
}

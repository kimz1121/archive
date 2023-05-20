void test_print_0()
{
  Serial.println(Total_angle[1]);
  Serial.print(pid_p);
  Serial.print(',');
  Serial.print(pid_i);
  Serial.print(',');
  Serial.print(pid_d);
  Serial.print(',');
  Serial.println(pwmRight);
}

void test_print_1()
{
  Serial.print("RF: ");
  Serial.print(pwmRight);
  //Serial.print("   |   ");
  //Serial.print("RB: ");
  //Serial.print(pwm_R_B);
  //Serial.print("   |   ");
  //Serial.print("LB: ");
  //Serial.print(pwm_L_B);
  Serial.print("   |   ");
  Serial.print("LF: ");
  Serial.print(pwmLeft);
  Serial.print("   |   ");
  Serial.print("thro: ");
  Serial.print(input_THROTTLE);
  Serial.print("   |   ");
  Serial.print("rudd: ");
  Serial.print(input_YAW);
  Serial.print("   |   ");
  Serial.print("aile: ");
  Serial.print(input_ROLL);

  Serial.print("   |   ");
  Serial.print("Xº: ");
  Serial.print(Total_angle[0]);
  Serial.print("   |   ");
  Serial.print("Yº: ");
  Serial.print(Total_angle[1]);
  Serial.print("   |   ");
  Serial.print("M: ");
  Serial.print(mot_activated);
  Serial.println(" ");
}

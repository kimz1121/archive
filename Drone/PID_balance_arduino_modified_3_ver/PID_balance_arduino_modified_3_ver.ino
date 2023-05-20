/* http://www.youtube.com/c/electronoobs
 * 
 * This is an example where we control one axis bar with PID 
 * and data from the radio receiver and the MPU6050 module
 * 
 * Arduino pin    |   MPU6050
 * 5V             |   Vcc
 * GND            |   GND
 * A4             |   SDA
 * A5             |   SCL
 * 
 * Left__motor    |   D4
 * Right__motor   |   D7
  */

#include <Wire.h>
#include <Servo.h>


Servo right_prop;
Servo left_prop;

/*MPU-6050 gives you 16 bits data so you have to create some 16int constants
 * to store the data for accelerations and gyro*/

int16_t Acc_rawX, Acc_rawY, Acc_rawZ,Gyr_rawX, Gyr_rawY, Gyr_rawZ;
 

float Acceleration_angle[2];
float Gyro_angle[2];
float Total_angle[2];

//We create variables for the time width values of each PWM input signal
unsigned long counter_1, counter_2, counter_3, counter_4, current_count;

//We create 4 variables to stopre the previous value of the input signal (if LOW or HIGH)
byte last_CH1_state, last_CH2_state, last_CH3_state, last_CH4_state;

//To store the 1000us to 2000us value we create variables and store each channel
int input_YAW;      //In my case channel 4 of the receiver and pin D12 of arduino
int input_PITCH;    //In my case channel 2 of the receiver and pin D9 of arduino
int input_ROLL;     //In my case channel 1 of the receiver and pin D8 of arduino
int input_THROTTLE; //In my case channel 3 of the receiver and pin D10 of arduino

//More variables for the code
//int i;
int mot_activated=0;
long activate_count=0;
long des_activate_count=0;

float elapsedTime, time, timePrev;
//int i;
float rad_to_deg = 180/3.141592654;

float PID, pwmLeft, pwmRight, error, previous_error;
float pid_p=0;
float pid_i=0;
float pid_d=0;
/////////////////PID CONSTANTS/////////////////
double kp=1;//3.55
double ki=0.000;//0.003
double kd=0.00;//2.05
///////////////////////////////////////////////

double throttle=1200; //initial value of throttle to the motors     ★★★★★★★★★★★★★★★★★★★★
float desired_angle = 0; //This is the angle in which we whant the ★★★★★★★★★★★★★★★★★★★★
                         //balance to stay steady


void setup() {

  Serial.begin(9600);
  right_prop.attach(3); //attatch the right motor to pin 3
  left_prop.attach(5);  //attatch the left motor to pin 5

  time = millis(); //Start counting time in milliseconds
  /*In order to start up the ESCs we have to send a min value
   * of PWM to them before connecting the battery. Otherwise
   * the ESCs won't start up or enter in the configure mode.
   * The min value is 1000us and max is 2000us, REMEMBER!*/
  delay(1000);
  left_prop.writeMicroseconds(2000); 
  right_prop.writeMicroseconds(2000);
  delay(1000);
  left_prop.writeMicroseconds(1000); 
  right_prop.writeMicroseconds(1000);
  delay(7000); /*Give some delay, 7s, to have time to connect
                *the propellers and let everything start up*/ 
}//end of setup void

void loop() {

    timePrev = time;  // the previous time is stored before the actual time read
    time = millis();  // actual time read
    elapsedTime = (time - timePrev) / 1000; 



}//end of loop void

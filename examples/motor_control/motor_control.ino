/*
 * Basic example code on how to control via I2C your geared DC motor with quadrature
 * encoder which is already connected to the Easy PID Motor Controller module and have already
 * succesfully set up their velocity PID control using the epmc_setup_application
 *
 * The code basically sends a low target velocity (in rad/s), waits for some time and then
 * sends a high target velocity (in rad/s). it also prints out the motors' angular positions
 * (in rad) and angular velocities (in rad/s).
 *
 * you can copy the code and use it in your project as you will.
 */

// Easy PID Motor Control i2c communication library
#include <epmc.h>

int i2c_address = 0x55; // set this address to the same address you have during setup via the GUI app
EPMC epmc(i2c_address);

///////// my sepcial delay function ///////////////
void delayMs(int ms)
{
  for (int i = 0; i < ms; i += 1)
  {
    delayMicroseconds(1000);
  }
}
//////////////////////////////////////////////////

float pos0, pos1; // (in rad)
float v0, v1; // (in rad/sec)

float lowTargetVel = -3.142; // rad/sec
float highTargetVel = 3.142; // rad/sec
bool sendHigh = true;

long prevTime;
long sampleTime = 20; // millisec

long ctrlPrevTime;
long ctrlSampleTime = 4000; // millisec

void setup()
{
  // start i2c communication
  Wire.begin();

  // setup serial communication to print result on serial minitor
  Serial.begin(115200);

  for (int i = 0; i < 4; i += 1)
  {
    delayMs(1000);
    Serial.print("configuring controller: ");
    Serial.println(i);
  }

  epmc.clearDataBuffer();
  epmc.writeSpeed(0.0, 0.0);

  int cmd_vel_timeout = 5000; // 0 to deactivate.
  epmc.setCmdTimeout(cmd_vel_timeout); // set motor command velocity timeout
  cmd_vel_timeout = epmc.getCmdTimeout(); // get the stored command velocity timeout
  Serial.print("motor command vel timeout in ms: ");
  Serial.println(cmd_vel_timeout);

  sendHigh = true;

  prevTime = millis();
  ctrlPrevTime = millis();
}

void loop()
{
  if ((millis() - ctrlPrevTime) >= ctrlSampleTime)
  {
    if (sendHigh)
    {
      epmc.writeSpeed(highTargetVel, highTargetVel);
      sendHigh = false;
    }
    else
    {
      epmc.writeSpeed(lowTargetVel, lowTargetVel);
      sendHigh = true;
    }
    ctrlPrevTime = millis();
  }

  if ((millis() - prevTime) >= sampleTime)
  {
    /* CODE SHOULD GO IN HERE*/

    // epmc.readPos(pos0, pos1);
    // epmc.readVel(v0, v1);

    epmc.readMotorData(pos0, pos1, v0, v1);

    // Print results
    Serial.println("-----------------------------------");
    Serial.print("Motor A: ");
    Serial.print(pos0); Serial.print("\t"); Serial.println(v0, 4);
    Serial.print("Motor B: ");
    Serial.print(pos1); Serial.print("\t"); Serial.println(v1, 4);
    Serial.println("------------------------------------");
    
    // Serial.println();

    prevTime = millis();
  }
}
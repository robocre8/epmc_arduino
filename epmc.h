#ifndef EPMC_H
#define EPMC_H

#include <Arduino.h>
#include <Wire.h>

class EPMC
{
public:
  EPMC(int);

  void writeSpeed(float v0, float v1);
  void writePWM(int pwm0, int pwm1);
  void readPos(float &pos0, float &pos1);
  void readSpeed(float &v0, float &v1);
  void setCmdTimeout(int timeout_ms);
  float getMaxSpeed(int motor_no);
  int getCmdTimeout();
  void setPidMode(int mode);
  int getPidMode();
  bool clearDataBuffer();


private:
  int slaveAddr;
  void send(float cmd, float arg1, float arg2);
  void recv(float cmd, float pos, float &data1, float &data2);

  // Serial Protocol Command IDs -------------
  const int WRITE_SPEED = 10;
  const int READ_SPEED = 11;
  const int READ_TSPEED = 12;
  const int READ_POS = 13;
  const int WRITE_PWM = 14;
  const int SET_KP = 15;
  const int GET_KP = 16;
  const int SET_KI = 17;
  const int GET_KI = 18;
  const int SET_KD = 19;
  const int GET_KD = 20;
  const int SET_PPR = 21;
  const int GET_PPR = 22;
  const int SET_CF = 23;
  const int GET_CF = 24;
  const int SET_RDIR = 25;
  const int GET_RDIR = 26;
  const int SET_PID_MODE = 27;
  const int GET_PID_MODE = 28;
  const int SET_CMD_TIMEOUT = 29;
  const int GET_CMD_TIMEOUT = 30;
  const int SET_I2C_ADDR = 31;
  const int GET_I2C_ADDR = 32;
  const int SET_MAX_SPEED = 33;
  const int GET_MAX_SPEED = 34;
  const int RESET = 35;
  const int CLEAR = 36;
  //---------------------------------------------
};

#endif
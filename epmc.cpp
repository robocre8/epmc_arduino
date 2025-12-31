#include "epmc.h"

EPMC::EPMC(int slave_addr)
{
  slaveAddr = slave_addr;
}

void EPMC::send(float cmd, float arg1=0.0, float arg2=0.0)
{
  uint8_t packet[12];
  memcpy(&packet[0], &cmd, sizeof(float));
  memcpy(&packet[4], &arg1, sizeof(float));
  memcpy(&packet[8], &arg2, sizeof(float));

  Wire.beginTransmission(slaveAddr);
  Wire.write(packet, sizeof(packet));
  Wire.endTransmission(true);
}

void EPMC::recv(float cmd, float pos, float &data1, float &data2)
{
  send(cmd, pos);

  uint8_t buffer[8];
  uint8_t dataSizeInBytes = Wire.requestFrom(slaveAddr, 8);
  for (size_t i = 0; i < dataSizeInBytes; i += 1)
  {
    uint8_t data = Wire.read();
    buffer[i] = data;
  }
  memcpy(&data1, &buffer[0], sizeof(float));
  memcpy(&data2, &buffer[4], sizeof(float));
}

void EPMC::writeSpeed(float v0, float v1){
  send((float)WRITE_SPEED, v0, v1);
}

void EPMC::writePWM(int pwm0, int pwm1){
  send((float)WRITE_PWM, (float)pwm0, (float)pwm1);
}

void EPMC::readPos(float &pos0, float &pos1){
  float pos = 0.0;
  recv((float)READ_POS, pos, pos0, pos1);
}

void EPMC::readSpeed(float &v0, float &v1){
  float pos = 0.0;
  recv((float)READ_SPEED, pos, v0, v1);
}

float EPMC::getMaxSpeed(int motor_no){
  float max_vel, dummy;
  recv((float)READ_SPEED, (float)motor_no, max_vel, dummy);
  return max_vel;
}

void EPMC::setCmdTimeout(int timeout_ms){
  send((float)SET_CMD_TIMEOUT, 0.0, (float)timeout_ms);
}

int EPMC::getCmdTimeout(){
  float timeout, dummy;
  recv((float)GET_CMD_TIMEOUT, 0.0, timeout, dummy);
  return (int)timeout;
}

void EPMC::setPidMode(int mode){
  send((float)SET_PID_MODE, 0.0, (float)mode);
}

int EPMC::getPidMode(){
  float mode, dummy;
  recv((float)GET_PID_MODE, 0.0, mode, dummy);
  return (int)mode;
}

bool EPMC::clearDataBuffer(){
  float res, dummy;
  recv((float)CLEAR, 0.0, res, dummy);
  return ((int)res == 1);
}
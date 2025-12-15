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
  void readVel(float &v0, float &v1);
  void setCmdTimeout(int timeout_ms);
  float getMaxVel(int motor_no);
  int getCmdTimeout();
  void setPidMode(int mode);
  int getPidMode();
  bool clearDataBuffer();
  void readMotorData(float &pos0, float &pos1, float &v0, float &v1);


private:
  int slaveAddr;
  void send_packet_without_payload(uint8_t cmd);
  void write_data1(uint8_t cmd, float val=0.0, uint8_t pos=100);
  void write_data2(uint8_t cmd, float val0, float val1);
  void read_data1(float &val0);
  void read_data2(float &val0, float &val1);
  void read_data4(float &val0, float &val1, float &val2, float &val3);

  // Serial Protocol Command IDs -------------
  const uint8_t START_BYTE = 0xAA;
  const uint8_t WRITE_VEL = 0x01;
  const uint8_t WRITE_PWM = 0x02;
  const uint8_t READ_POS = 0x03;
  const uint8_t READ_VEL = 0x04;
  const uint8_t READ_UVEL = 0x05;
  const uint8_t GET_MAX_VEL = 0x14;
  const uint8_t SET_PID_MODE = 0x15;
  const uint8_t GET_PID_MODE = 0x16;
  const uint8_t SET_CMD_TIMEOUT = 0x17;
  const uint8_t GET_CMD_TIMEOUT = 0x18;
  const uint8_t READ_MOTOR_DATA = 0x2A;
  const uint8_t CLEAR_DATA_BUFFER = 0x2C;
  //---------------------------------------------
};

#endif
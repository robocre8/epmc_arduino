#include "epmc.h"

EPMC::EPMC(int slave_addr)
{
  slaveAddr = slave_addr;
}

uint8_t computeChecksum(uint8_t *packet, uint8_t length) {
  uint8_t sum = 0;
  for (size_t i = 0; i < length; i++) {
    sum += packet[i]; 
  }
  return sum & 0xFF; 
}

void EPMC::send_packet_without_payload(uint8_t cmd)
{
  // Build packet: start_byte + cmd + length + checksum
  uint8_t packet[4];
  packet[0] = START_BYTE;
  packet[1] = cmd;
  packet[2] = 0; // msg length = 0

  // Compute checksum
  uint8_t checksum = computeChecksum(packet, 3);
  packet[3] = checksum;

  Wire.beginTransmission(slaveAddr);
  Wire.write(packet, sizeof(packet));
  Wire.endTransmission(true);
}

void EPMC::write_data1(uint8_t cmd, float val=0.0, uint8_t pos=100)
{
  // Build packet: start_byte + cmd + length + pos + float + checksum
  uint8_t packet[1 + 1 + 1 + 1 + 4 + 1];
  packet[0] = START_BYTE;
  packet[1] = cmd;
  packet[2] = 5; // msg is uint8 + float = 5byte length
  packet[3] = pos;
  memcpy(&packet[4], &val, sizeof(float));

  // Compute checksum
  uint8_t checksum = computeChecksum(packet, 8);
  packet[8] = checksum;

  Wire.beginTransmission(slaveAddr);
  Wire.write(packet, sizeof(packet));
  Wire.endTransmission(true);
}

void EPMC::write_data2(uint8_t cmd, float val0, float val1)
{
  // Build packet: start_byte + cmd + length + float*4 + checksum
  uint8_t packet[1 + 1 + 1 + 8 + 1];
  packet[0] = START_BYTE;
  packet[1] = cmd;
  packet[2] = 8; // msg is 2 float = 8byte length
  memcpy(&packet[3], &val0, sizeof(float));
  memcpy(&packet[7], &val1, sizeof(float));

  // Compute checksum
  uint8_t checksum = computeChecksum(packet, 11);
  packet[11] = checksum;

  Wire.beginTransmission(slaveAddr);
  Wire.write(packet, sizeof(packet));
  Wire.endTransmission(true);
}

void EPMC::read_data1(float& val0)
{
  uint8_t buffer[4];
  uint8_t dataSizeInBytes = Wire.requestFrom(slaveAddr, 4);
  for (size_t i = 0; i < dataSizeInBytes; i += 1)
  {
    uint8_t data = Wire.read();
    buffer[i] = data;
  }
  memcpy(&val0, &buffer[0], sizeof(float));
}

void EPMC::read_data2(float &val0, float &val1)
{
  uint8_t buffer[8];
  uint8_t dataSizeInBytes = Wire.requestFrom(slaveAddr, 8);
  for (size_t i = 0; i < dataSizeInBytes; i += 1)
  {
    uint8_t data = Wire.read();
    buffer[i] = data;
  }
  memcpy(&val0, &buffer[0], sizeof(float));
  memcpy(&val1, &buffer[4], sizeof(float));
}

void EPMC::read_data4(float &val0, float &val1, float &val2, float &val3)
{
  uint8_t buffer[16];
  uint8_t dataSizeInBytes = Wire.requestFrom(slaveAddr, 16);
  for (size_t i = 0; i < dataSizeInBytes; i += 1)
  {
    uint8_t data = Wire.read();
    buffer[i] = data;
  }
  memcpy(&val0, &buffer[0], sizeof(float));
  memcpy(&val1, &buffer[4], sizeof(float));
  memcpy(&val2, &buffer[8], sizeof(float));
  memcpy(&val3, &buffer[12], sizeof(float));
}

void EPMC::readMotorData(float &pos0, float &pos1, float &v0, float &v1){
  send_packet_without_payload(READ_MOTOR_DATA);
  read_data4(pos0, pos1, v0, v1);
}

void EPMC::writeSpeed(float v0, float v1){
  write_data2(WRITE_VEL, v0, v1);
}

void EPMC::writePWM(int pwm0, int pwm1){
  write_data2(WRITE_VEL, (float)pwm0, (float)pwm1);
}

void EPMC::readPos(float &pos0, float &pos1){
  send_packet_without_payload(READ_POS);
  read_data2(pos0, pos1);
}

void EPMC::readVel(float &v0, float &v1){
  send_packet_without_payload(READ_VEL);
  read_data2(v0, v1);
}

float EPMC::getMaxVel(int motor_no){
  float max_vel;
  write_data1(GET_MAX_VEL, 0.0, motor_no);
  read_data1(max_vel);
  return max_vel;
}

void EPMC::setCmdTimeout(int timeout_ms){
  float res;
  write_data1(SET_CMD_TIMEOUT, (float)timeout_ms);
}

int EPMC::getCmdTimeout(){
  float timeout_ms;
  write_data1(GET_CMD_TIMEOUT);
  read_data1(timeout_ms);
  return (int)timeout_ms;
}

void EPMC::setPidMode(int mode){
  float res;
  write_data1(SET_PID_MODE, (float)mode);
}

int EPMC::getPidMode(){
  float mode;
  write_data1(GET_PID_MODE);
  read_data1(mode);
  return (int)mode;
}

bool EPMC::clearDataBuffer(){
  float res;
  write_data1(CLEAR_DATA_BUFFER);
  read_data1(res);
  return ((int)res == 1);
}
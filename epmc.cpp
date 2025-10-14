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
  // Build packet: start_byte + cmd + length + pos + float + checksum
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

void EPMC::write_data1(uint8_t cmd, uint8_t pos, float val)
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
  // Build packet: start_byte + cmd + length + float*3 + checksum
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

void EPMC::write_data4(uint8_t cmd, float val0, float val1, float val2, float val3)
{
  // Build packet: start_byte + cmd + length + float*4 + checksum
  uint8_t packet[1 + 1 + 1 + 16 + 1];
  packet[0] = START_BYTE;
  packet[1] = cmd;
  packet[2] = 16; // msg is 4 float = 16byte length
  memcpy(&packet[3], &val0, sizeof(float));
  memcpy(&packet[7], &val1, sizeof(float));
  memcpy(&packet[11], &val2, sizeof(float));
  memcpy(&packet[15], &val3, sizeof(float));
  // Compute checksum
  uint8_t checksum = computeChecksum(packet, 19);
  packet[19] = checksum;

  Wire.beginTransmission(slaveAddr);
  Wire.write(packet, sizeof(packet));
  Wire.endTransmission(true);
}

float EPMC::read_data1()
{
  uint8_t buffer[4];
  float res;
  uint8_t dataSizeInBytes = Wire.requestFrom(slaveAddr, 4);
  for (size_t i = 0; i < dataSizeInBytes; i += 1)
  {
    uint8_t data = Wire.read();
    buffer[i] = data;
  }
  memcpy(&res, &buffer[0], sizeof(float));
  return res;
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
  float res;
  write_data2(WRITE_VEL, v0, v1);
}
void EPMC::writePWM(int pwm0, int pwm1){
  float res;
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
void EPMC::readUVel(float &v0, float &v1){
  send_packet_without_payload(READ_UVEL);
  read_data2(v0, v1);
}
int EPMC::setCmdTimeout(int timeout_ms){
  float res;
  write_data1(SET_CMD_TIMEOUT, 100, (float)timeout_ms);
  res = read_data1();
  return (int)res;
}
int EPMC::getCmdTimeout(){
  float res;
  write_data1(GET_CMD_TIMEOUT, 100, 0.0);
  res = read_data1();
  return (int)res;
}
int EPMC::setPidMode(int motor_no, int mode){
  float res;
  write_data1(SET_PID_MODE, motor_no, (float)mode);
  res = read_data1();
  return (int)res;
}
int EPMC::getPidMode(int motor_no){
  float res;
  write_data1(GET_PID_MODE, motor_no, 0.0);
  res = read_data1();
  return (int)res;
}
int EPMC::clearDataBuffer(){
  float res;
  write_data1(CLEAR_DATA_BUFFER, 100, 0.0);
  res = read_data1();
  return (int)res;
}
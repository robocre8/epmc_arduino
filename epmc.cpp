#include "epmc.h"

EPMC::EPMC(int slave_addr)
{
  slaveAddr = slave_addr;
}

bool EPMC::sendTargetVel(float valA = 0.0, float valB = 0.0)
{
  return send("/tag", valA, valB);
}

bool EPMC::sendPwm(int valA = 0, int valB = 0)
{
  return send("/pwm", valA, valB);
}

bool EPMC::setCmdTimeout(int timeout_ms = 0)
{
  return send("/timeout", timeout_ms, 0);
}

void EPMC::getCmdTimeout(int &timeout_ms)
{
  get("/timeout");

  timeout_ms = valA;

  valA = 0.0;
  valB = 0.0;
}

void EPMC::getMotorsPos(float &angPosA, float &angPosB)
{
  get("/pos");

  angPosA = valA;
  angPosB = valB;

  valA = 0.0;
  valB = 0.0;
}

void EPMC::getMotorsVel(float &angVelA, float &angVelB)
{
  get("/vel");

  angVelA = valA;
  angVelB = valB;

  valA = 0.0;
  valB = 0.0;
}

void EPMC::getMotorAData(float &angPos, float &angVel)
{
  get("/dataA");

  angPos = valA;
  angVel = valB;

  valA = 0.0;
  valB = 0.0;
}

void EPMC::getMotorBData(float &angPos, float &angVel)
{
  get("/dataB");

  angPos = valA;
  angVel = valB;

  valA = 0.0;
  valB = 0.0;
}

void EPMC::getMotorAMaxVel(float &maxVel)
{
  get("/maxVelA");

  maxVel = valA;

  valA = 0.0;
  valB = 0.0;
}

void EPMC::getMotorBMaxVel(float &maxVel)
{
  get("/maxVelB");

  maxVel = valA;

  valA = 0.0;
  valB = 0.0;
}

void EPMC::get(String cmd_route)
{
  masterSendData(cmd_route);
  dataMsg = masterReceiveData();

  int indexPos = 0, i = 0;
  do
  {
    indexPos = dataMsg.indexOf(',');
    if (indexPos != -1)
    {
      dataMsgBuffer = dataMsg.substring(0, indexPos);
      dataMsg = dataMsg.substring(indexPos + 1, dataMsg.length());
      dataBuffer[i] = dataMsgBuffer;
      dataMsgBuffer = "";
    }
    else
    {
      if (dataMsg.length() > 0)
        dataBuffer[i] = dataMsg;
    }
    i += 1;
  } while (indexPos >= 0);

  valA = dataBuffer[0].toFloat();
  valB = dataBuffer[1].toFloat();

  dataMsg = "";
  dataMsgBuffer = "";
  dataBuffer[0] = "";
  dataBuffer[1] = "";
}

bool EPMC::send(String cmd_route, float valA, float valB)
{
  String msg_buffer = cmd_route;
  msg_buffer += ",";
  msg_buffer += String(valA, 3);
  msg_buffer += ",";
  msg_buffer += String(valB, 3);

  masterSendData(msg_buffer);
  String data = masterReceiveCharData();
  if (data == "1")
    return true;
  else
    return false;
}

void EPMC::masterSendData(String i2c_msg)
{
  char charArray[i2c_msg.length() + 1];
  i2c_msg.toCharArray(charArray, i2c_msg.length() + 1);

  Wire.beginTransmission(slaveAddr);
  Wire.write(charArray);
  Wire.endTransmission();
}

String EPMC::masterReceiveData()
{
  String i2c_msg = "";
  Wire.requestFrom(slaveAddr, 32);
  while (Wire.available())
  {
    char c = Wire.read();
    i2c_msg += c;
  }
  int indexPos = i2c_msg.indexOf((char)255);
  if (indexPos != -1)
  {
    return i2c_msg.substring(0, indexPos);
  }
  return i2c_msg;
}

String EPMC::masterReceiveCharData()
{
  String i2c_msg = "";
  Wire.requestFrom(slaveAddr, 1);
  while (Wire.available())
  {
    char c = Wire.read();
    i2c_msg += c;
  }
  int indexPos = i2c_msg.indexOf((char)255);
  if (indexPos != -1)
  {
    return i2c_msg.substring(0, indexPos);
  }
  return i2c_msg;
}
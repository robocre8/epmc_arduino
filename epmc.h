#ifndef EPMC_H
#define EPMC_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Wire.h>

class EPMC
{
public:
  EPMC(int);

  bool sendTargetVel(float, float);

  bool sendPwm(int, int);

  bool setCmdTimeout(int);

  void getCmdTimeout(int &);

  void getMotorsPos(float &, float &);

  void getMotorsVel(float &, float &);

  void getMotorAData(float &, float &);

  void getMotorBData(float &, float &);

  void getMotorAMaxVel(float &);

  void getMotorBMaxVel(float &);

private:
  int slaveAddr;
  String dataMsg = "", dataMsgBuffer = "", dataBuffer[2];
  float valA, valB;

  void get(String);

  bool send(String, float, float);

  void masterSendData(String);

  String masterReceiveData();

  String masterReceiveCharData();
};

#endif
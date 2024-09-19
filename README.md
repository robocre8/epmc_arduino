## Easy PID Motor Controller (EPMC) Arduino I2C Lib
This library helps communicate with the already setup **`Easy PID Motor Controller Module`** (i.e **`L298N EPMC Module`**) with  arduino microcontroller projects via I2C, after successful setup with the [epmc_setup_application](https://github.com/samuko-things-company/epmc_setup_application).

> you can use it in your Arduino-based robotics project (e.g Arduino UNO, Arduino NANO, Arduino MEGA, Esp32, etc.)

A simple way to get started is simply to try out and follow the example code


## How to Use the Library
- Ensure you have the **`Easy PID Motor Controller Module`** (i.e **`L298N EPMC Module`**). Calibrate it and set it up using the **`epmc_setup_application`**.

- Download download the library by clicking on the green Code button above (or clone it)
  > if you download it, extract it and change the folder name to `epmc_arduino`

- Move the downloaded library file - **`epmc_arduino`** - to your Arduino library folder
  > e.g on linux: ... home/Arduino/libraries/
  >
  > e.g on windows: ... Documents/Arduino/libraries/
  >
  > (or any where your arduino libraries are stored)

- restart your ArduinoIDE and navigate to examples and run the example code and see how to control the motors.

- you can copy this example code into your project and modify it to your taste.


## Basic Library functions and usage

- initialize the EPMC i2c communication by creating an object
  > `EPMC` motor(epmc_i2c_address)

- send target angular velocity command
  > motor.`sendTargetVel`(motorATargetVel, motorBTargetVel)

- send PWM command
  > motor.`sendPwm`(motorA_PWM, motorB_PWM)

- read motors angular position
  > motor.`getMotorsPos`(&angPosA, &angPosB) // gets and copies the motors A and B angular position into angPosA, angPosB

- read motors angular velocity
  > motor.`getMotorsVel`(&angVelA, &angVelB) // gets and copies the motors A and B angular velocity angVelA, angVelB

- read motorA angular position and velocity together
  > motor.`getMotorAData`(&angPosA, &angVelA) // gets and copies the motorA ang pos and vel into angPosA, angVelA

- read motorB angular position and velocity together
  > motor.`getMotorBData`(&angPosB, &angVelB) // gets and copies the motorB ang pos and vel into angPosB, angVelB

- read motorA maximum commandable angular velocity
  > motor.`getMotorAData`(&maxVelA)

- read motorB maximum commandable angular velocity
  > motor.`getMotorBData`(&maxVelB)

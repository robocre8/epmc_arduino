## Easy PID Motor Controller (EPMC) Arduino I2C Lib
This library helps communicate with the already setup **`Easy PID Motor Controller Module`** (i.e **`L298N EPMC Module`**) with  arduino microcontroller projects via I2C, after successful setup with the [epmc_setup_application](https://github.com/samuko-things-company/epmc_setup_application).

> you can use it in your Arduino-based robotics project (e.g Arduino UNO, Arduino NANO, Arduino MEGA, Esp32, etc.)

A simple way to get started is simply to try out and follow the example code


## How to Use the Library
- Ensure you have the **`Easy PID Motor Controller Module`**. Calibrate it and set it up using the **`epmc_setup_application`**.

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

- connect to epmc_driver shield module
  > EPMC epmc(i2c_address)
  > .clearDataBuffer()

- send target angular velocity command
  > .writeSpeed(motor0_TargetVel, motor1_TargetVel)

- send PWM command
  > .writePWM(motor0_PWM, motor1_PWM)

- set motor command timeout
  > .setCmdTimeout(timeout_ms)

- get motor command timeout
  > .getCmdTimeout() # returns motor command timeout in ms

- read motors angular position
  > .readPos(&motor0_angPos, &motor1_angPos) # returns angPos0, angPos1

- read motors angular velocity
  > .readVel(&motor0_angVel, &motor1_angVel) # returns angVel0, angVel1

- read motorA maximum commandable angular velocity
  > .getMaxVel(motor_no) # returns maxVel0 or maxVel1 based on the specified motor number

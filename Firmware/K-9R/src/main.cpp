#include "Arduino.h"
#include "SimpleFOC.h"
#include "SimpleFOCDrivers.h"
#include "encoders/as5047/MagneticSensorAS5047.h"
#include "TCAN4550.h"

// Change pins to V2 pins
#define   PIN_A_H 10
#define   PIN_A_L 9
#define   PIN_B_H 8
#define   PIN_B_L 12
#define   PIN_C_H 11
#define   PIN_C_L 24
#define   DRV8353_ENABLE 14

#define AS5047P_CS 16

// === Encoder (AS5047P over SPI) ===
MagneticSensorAS5047 sensor = MagneticSensorAS5047(AS5047P_CS);

// === BLDC Motor & Driver ===
BLDCMotor motor = BLDCMotor(7); // pole pairs 

BLDCDriver6PWM driver = BLDCDriver6PWM(PIN_A_H, PIN_A_L, PIN_B_H, PIN_B_L, PIN_C_H, PIN_C_L, DRV8353_ENABLE);

// === Variables ===
float target_velocity = 5;  // rad/s open loop speed

void setup() {
  Serial.begin(115200);
  _delay(1000);

  // Initialize SPI encoder
  sensor.init();
  Serial.println("AS5047P initialized.");

  // Driver setup
  driver.voltage_power_supply = 12;  // supply voltage
  driver.init();
  motor.linkDriver(&driver);

  // Set control mode
  motor.controller = MotionControlType::velocity_openloop;

  // Initialize motor
  motor.init();

  // Optionally, enable gate driver pin
  pinMode(DRV8353_ENABLE, OUTPUT);
  digitalWrite(DRV8353_ENABLE, HIGH); // Enable DRV8353

  Serial.println("Setup complete. Spinning...");
}

void loop() {
  // SimpleFOC loop for open-loop control
  motor.move(target_velocity);

  // Read encoder angle
  sensor.update();
  Serial.print("Angle: ");
  Serial.println(sensor.getAngle());

  delay(5);
}
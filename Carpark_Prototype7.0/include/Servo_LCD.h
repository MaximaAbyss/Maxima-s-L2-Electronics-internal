#pragma once
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

extern Servo Servo_Entrance;
extern Servo Servo_Exit;

extern LiquidCrystal_I2C display;

void initservo(void);

void initLCD(void *param);
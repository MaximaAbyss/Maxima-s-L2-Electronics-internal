#pragma once
#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C display(0x27,16, 2);
Servo Servo_Entrance;
Servo Servo_Exit;

bool gate1running = false;
bool gate2running = false;

const int MUX_A = 12;
const int MUX_B = 11;
const int MUX_C = 10;
const int SR_DATAPIN = 9;
const int SR_CLOCKPIN = 8;

extern bool ledOutput[8]={0,0,0,0,0,0,0,0};
int cars_in_lot = 0;
int cars_parked;
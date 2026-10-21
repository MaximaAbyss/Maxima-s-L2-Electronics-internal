#include "Servo_LCD.h"

Servo Servo_Entrance;
Servo Servo_Exit;

LiquidCrystal_I2C display(0x27,16, 2);

void initservo(void){
    Servo_Entrance.attach(6);
    Servo_Exit.attach(7);
}

void initLCD(void *param){
    (void) param;
    display.init();
    display.backlight();
}
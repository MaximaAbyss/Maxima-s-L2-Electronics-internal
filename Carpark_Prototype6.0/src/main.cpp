// separated into many files

#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#include "ldr.h"
#include "led.h"
#include "parkingSpace.h"
#include "gate.h"
#include "constants.h"
#include "objects.h"


void do_screen(){
  display.clear();
  display.setCursor(0, 0);
  display.println("Spaces available:");
  display.setCursor(0, 1);
  display.println((8-cars_in_lot));
}


void setup(){
  // Setup the pins
  pinMode(MUX_A,OUTPUT);
  pinMode(MUX_B,OUTPUT);
  pinMode(MUX_C,OUTPUT);
  pinMode(SR_CLOCKPIN,OUTPUT);
  pinMode(SR_DATAPIN,OUTPUT);

  Servo_Entrance.attach(6);
  Servo_Exit.attach(7);

  // test the display
  display.init();
  display.backlight();
  display.setCursor(0,0);
  display.println("Calibrating...");
  ldr::calibrateall();
  Servo_Entrance.write(0);
  Servo_Exit.write(0);

}


void loop() {
  ldr::updateall();
  parkingSpace::updateall();
  shiftregisterwrite(ledOutput); 
  check_gate_status();
  do_screen();
  delay(50);
  // total delay currently is 10 + 10.002 (per led) ms
}


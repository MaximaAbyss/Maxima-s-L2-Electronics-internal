// added threading
// note : swap D8 and D9

#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "ldr.h"
#include "led.h"
#include "parkingSpace.h"
#include "gate.h"
#include "constants.h"
#include "objects.h"
#include "Servo_LCD.h"

SemaphoreHandle_t xSerialMutex;
// defines mutex


void gate_loop(void *parameter);
void parking_loop(void *parameter);




void setup(){
  initLCD(NULL);
  // Setup the pins
  pinMode(MUX_A,OUTPUT);
  pinMode(MUX_B,OUTPUT);
  pinMode(MUX_C,OUTPUT);
  pinMode(SR_CLOCKPIN,OUTPUT);
  pinMode(SR_DATAPIN,OUTPUT);


  // test the display
  initservo();
  
  display.setCursor(0,0);
  display.println("Calibrating...");
  ldr::calibrateall();
  Servo_Entrance.write(0);
  Servo_Exit.write(0);
  display.clear();

  xSerialMutex = xSemaphoreCreateMutex();
  // creates mutex

  if(xSerialMutex != NULL){
    // Create Tasks (Task Name, Stack Size, Parameter, Priority, Task Handle)
    
    xTaskCreate(gate_loop, "Gate loop", 128, NULL, 1, NULL);
    xTaskCreate(parking_loop,  "Parking loop",  128, NULL, 1, NULL);
    display.println("mutex made");
  }
  else{
    display.println("no ram :(");
  }
  // checks if mutex has formed
}

void do_screen(){
  display.clear();
  display.setCursor(0, 0);
  display.println("Spaces available:");
  display.setCursor(0, 1);
  display.println((8-cars_in_lot));
}


void loop() {}

void gate_loop(void *parameter){
  (void) parameter;

  for(;;){
    if(xSemaphoreTake(xSerialMutex,portMAX_DELAY) == pdTRUE){
      // try take mutex, or wait until mutex available

      check_gate_status();
      // run gate check

      xSemaphoreGive(xSerialMutex);
      // gives mutex back

    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void parking_loop(void *parameter){
  (void) parameter;

  for(;;){
    if(xSemaphoreTake(xSerialMutex,portMAX_DELAY) == pdTRUE){
      // try take mutex, or wait until mutex available

      ldr::updateall();
      parkingSpace::updateall();
      shiftregisterwrite(ledOutput); 
      do_screen();
      // run parking stuff function

      xSemaphoreGive(xSerialMutex);
      // gives mutex back

    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
#pragma once
#include <Arduino.h>
#include <Servo.h>
#include "led.h"
#include "ldr.h"
#include "Servo_LCD.h"

class gate{
  private:

    ldr& front;
    ldr& back;
    Servo& motor;
    bool is_in_use = false;
    int func;

  public:
    gate(ldr& l1, ldr& l2, Servo& m, int f);
    
    void gate_down();

    void do_gate();

};

void gate1run();
void gate2run();
void check_gate_status();
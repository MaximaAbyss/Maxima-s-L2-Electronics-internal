#include <Arduino.h>
#include "gate.h"
#include "objects.h"
#include "constants.h"
#include <Servo.h>
#include "Servo_LCD.h"


gate::gate(ldr& l1, ldr& l2, Servo& m, int f):
front(l1), back(l2), motor(m), func(f)
{}


void gate::gate_down(){ motor.write(0); }


void gate::do_gate(){
    is_in_use = true;
    motor.write(90);
    delay(100);
    front.get_value();
    while(front.is_detected()){
      front.get_value();
      front.reportdetect();
      delay(50);
    }      
    while(!back.is_detected()){
      back.get_value();
      back.report();
      delay(50);
    }
    while(back.is_detected()){
      back.get_value();
      back.report();
      delay(50);
    }
    motor.write(0);
    
    cars_in_lot+= func;
}

void gate1run(){
  gate1running = true;
  Entrance.do_gate();
  gate1running = false;
}
void gate2run(){
  gate2running = true;
  Exit.do_gate();
  gate2running = false;
}

void check_gate_status(){
  if(Entrance1.is_detected() && !gate1running){
    gate1run();
  }
  if(Exit1.is_detected() && !gate2running){
    gate2run();
  }
}
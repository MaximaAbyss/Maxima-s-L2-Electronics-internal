#include <Arduino.h>
#include "parkingSpace.h"
#include "Servo_LCD.h"



parkingSpace::parkingSpace(led& l, ldr& d):
    light(l),
    detector(d)
{
    if(spaces < 8){
    all_spaces[spaces] = this;
    spaces++;
};
}

void parkingSpace::is_open_space(){
    if(detector.is_detected()){
    light.set_led(false);
    }else{
    light.set_led(true);
    }
}

void parkingSpace::load_one(){
    is_open_space();
}

// make the load_one function occur for all objects of the parking space class
void parkingSpace::updateall() {
    for (int i = 0; i < spaces; i++) {
    all_spaces[i]->load_one();
    }
}
void parkingSpace::calibrateall(){
    display.clear();
    display.setCursor(0, 0);
    display.println("Calibrating...");
    for (int i = 0; i < spaces; i++) {
    all_spaces[i]->detector.calibrate();
    delay(50);
    }
    display.clear();
}

parkingSpace* parkingSpace::all_spaces[8] = {nullptr};
int parkingSpace::spaces = 0;
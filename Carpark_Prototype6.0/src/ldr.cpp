#include "ldr.h"
#include <LiquidCrystal_I2C.h>
#include "constants.h"


ldr::ldr(String pname, bool is_p_space, bool a,bool b,bool c, int pn, int thre):
name(pname), is_parking_space(is_p_space), pinState{a,b,c},pin(pn), pre_threshold(thre)
{
if(amount_of_ldrs < 12){
    all_ldrs[amount_of_ldrs] = this;
    amount_of_ldrs++;
};
}

void ldr::call_ldr(){
    if (is_parking_space){
    digitalWrite(MUX_A,pinState[0]);
    digitalWrite(MUX_B,pinState[1]);
    digitalWrite(MUX_C,pinState[2]);
    }
}

void ldr::get_value(){
    call_ldr();
    reading = analogRead(pin);
}

void ldr::calibrate(){
    get_value();
    threshold = reading - pre_threshold;
}

bool ldr::is_detected(){
    if(reading < threshold){
        return true;
    }else{
        return false;
    }
}

void ldr::reportdetect(){
    if(reading < threshold){
    display.clear();
    display.setCursor(0, 0);
    display.println(name);
    display.setCursor(0, 1);
    display.println("1");
    }else{
    display.clear();
    display.setCursor(0, 0);
    display.println(name);
    display.setCursor(0, 1);
    display.println("0");
    }
}

void ldr::report(){
    display.clear();
    display.setCursor(0, 0);
    display.println(name + " reading is:");
    display.setCursor(0, 1);
    display.println(reading);
}

void ldr::updateall() {
    for (int i = 0; i < amount_of_ldrs; i++) {
    all_ldrs[i]->get_value();
    }
}

void ldr::calibrateall(){
    for (int i = 0; i < amount_of_ldrs; i++) {
    all_ldrs[i]->calibrate();
    }
}

ldr* ldr::all_ldrs[12] = {nullptr};
int ldr::amount_of_ldrs = 0;
#include "led.h"
#include "constants.h"

led::led(int p):
position(p){};

void led::set_led(bool will_be_on){
    ledOutput[position - 1] = will_be_on;
}

void shiftregisterwrite(bool binary_out[8]){

  digitalWrite(SR_CLOCKPIN, LOW);
  digitalWrite(SR_DATAPIN, HIGH);
  delay(10); // change to microseconds?
  digitalWrite(SR_DATAPIN, LOW);
  digitalWrite(SR_CLOCKPIN, HIGH);

  for(int i; i<8; i++){
    delayMicroseconds(2);
    digitalWrite(SR_CLOCKPIN, LOW);
    digitalWrite(SR_DATAPIN,binary_out[i]);
    // inputs each value each loop x8
    delayMicroseconds(2);
    digitalWrite(SR_CLOCKPIN, HIGH);
  }
}
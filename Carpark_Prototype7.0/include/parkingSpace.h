#pragma once
#include "led.h"
#include "ldr.h"

class parkingSpace{

  private:
  
    static int spaces;
    static parkingSpace* all_spaces[8];

    led& light;
    ldr& detector;

  public:
    
    parkingSpace(led& l, ldr& d);

    void is_open_space();

    void load_one();

    static void updateall();

    static void calibrateall();

};
#pragma once
#include <Arduino.h>



class ldr{
    private:
        static int amount_of_ldrs;
        static ldr* all_ldrs[12];

        bool is_parking_space;
        bool pinState[3]; 
        int pin;

        int pre_threshold;

    public:
        int reading;
        int threshold;

        ldr(bool is_p_space, bool a,bool b,bool c, int pn, int thre);
        
        void call_ldr();

        void get_value();

        void calibrate();

        bool is_detected();

        void reportdetect();

        void report();

        static void updateall();

        static void calibrateall();
        
};
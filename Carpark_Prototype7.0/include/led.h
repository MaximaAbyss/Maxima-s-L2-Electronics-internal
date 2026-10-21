#pragma once

class led{
    private:
        int position;
    
    public:
        led(int p);

        void set_led(bool will_be_on);

};

void shiftregisterwrite(bool binary_out[8]);
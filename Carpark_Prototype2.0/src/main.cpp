#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C display(0x27,16, 2);



/*        Constants and Variables       */
const int MUX_A = 12;
const int MUX_B = 11;
const int MUX_C = 10;
const int SR_DATAPIN = 9;
const int SR_CLOCKPIN = 8;

bool ledOutput[8]={0,0,0,0,0,0,0,0};



/*        Classes       */

// define LDR class
class ldr{
  private:
    int reading;
    bool pinState[3]; 
    int threshold;
  public:
    String name;

    ldr(String pname, bool a,bool b,bool c, int t)
    : pinState{a,b,c} {name = pname; threshold = t;};

    void call_lcd(){
      digitalWrite(MUX_A,pinState[0]);
      digitalWrite(MUX_B,pinState[1]);
      digitalWrite(MUX_C,pinState[2]);
    }

    void get_value(){
      call_lcd();
      reading = analogRead(A0);
    }

    void calibrate(){
      get_value();
      threshold = reading - 30;
    }

    bool is_over_threshold(){
      if(reading >= threshold){
        return true;
      }else{
        return false;
      }
    }

    void report(){
      display.clear();
      display.setCursor(0, 0);
      display.println(name + " reading is:");
      display.setCursor(0, 1);
      display.println(reading);
    }

};

// define led class
class led{

  public:
    int position;
    led(int positionne)
    {position = positionne;};

    void set_led(bool will_be_on){
      ledOutput[position - 1] = will_be_on;
    }

};


class parkingSpace{

  private:
  
    static int spaces;
    static parkingSpace* all_spaces[8];

  public:

    led& light;
    ldr& detector;
    
    parkingSpace(led& l, ldr& d):
     light(l),
     detector(d)
    {
     if(spaces < 8){
      all_spaces[spaces] = this;
      spaces++;
    };
    }

    void is_open_space(){
      if(detector.is_over_threshold()){
        light.set_led(true);
      }else{
        light.set_led(false);
      }
    }

    void load_one(){
      detector.get_value();
      is_open_space();
    }

    // make the load_one function occur for all objects of the parking space class
    static void updateall() {
      for (int i = 0; i < spaces; i++) {
        all_spaces[i]->load_one();
      }
    }
    static void calibrateall(){
      display.clear();
      display.setCursor(0, 0);
      display.println("Calibrating...");
      for (int i = 0; i < spaces; i++) {
        all_spaces[i]->detector.calibrate();
        delay(10);
      }
      display.clear();
    }
};

parkingSpace* parkingSpace::all_spaces[8] = {nullptr};
int parkingSpace::spaces = 0;


/*        Objects       */

// creating LDR objects
ldr LDR1("LDR1",false,false,false,    500);
ldr LDR2("LDR2",false,false,true,     500);
ldr LDR3("LDR3",false,true,false,     500);
ldr LDR4("LDR4",false,true,true,      500);
ldr LDR5("LDR5",true,false,false,     500);
ldr LDR6("LDR6",true,true,false,      500);
ldr LDR7("LDR7",true,true,true,       500);
ldr LDR8("LDR8",true,false,true,      500);

led LED1(1);
led LED2(2);
led LED3(3);
led LED4(4);
led LED5(5);
led LED6(6);
led LED7(7);
led LED8(8);

parkingSpace one(LED1,LDR1);
parkingSpace two(LED5,LDR2);
parkingSpace three(LED3,LDR3);
parkingSpace four(LED4,LDR6);
parkingSpace five(LED2,LDR5);
parkingSpace six(LED6,LDR4);
parkingSpace seven(LED7,LDR7);
parkingSpace eight(LED8,LDR8);




/*        Functions       */

// function to turn on certain LEDs
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


void setup(){
  // Setup the pins
  pinMode(MUX_A,OUTPUT);
  pinMode(MUX_B,OUTPUT);
  pinMode(MUX_C,OUTPUT);
  pinMode(SR_CLOCKPIN,OUTPUT);
  pinMode(SR_DATAPIN,OUTPUT);
  
  // test the display
  display.init();
  display.backlight();
  display.setCursor(0,0);
  display.println("Calibrating...");
  parkingSpace::calibrateall();
}



void loop() {
  parkingSpace::updateall();
  shiftregisterwrite(ledOutput); 
  delay(10);
  // total delay currently is 10 + 10.002 (per led) ms
}


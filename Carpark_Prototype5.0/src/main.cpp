// added separate files

#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <TaskScheduler.h>

LiquidCrystal_I2C display(0x27,16, 2);
Servo Servo_Entrance;
Servo Servo_Exit;



bool gate1running = false;
bool gate2running = false;

/*        Constants and Variables       */
const int MUX_A = 12;
const int MUX_B = 11;
const int MUX_C = 10;
const int SR_DATAPIN = 9;
const int SR_CLOCKPIN = 8;

bool ledOutput[8]={0,0,0,0,0,0,0,0};
int cars_in_lot = 0;
int cars_parked;


/*        Classes       */

// define LDR class
class ldr{
  private:

    static int amount_of_ldrs;
    static ldr* all_ldrs[12];

    String name;
    bool is_parking_space;
    bool pinState[3]; 
    int pin;

    int pre_threshold;
    
  public:
    int reading;
    int threshold;


    ldr(String pname, bool is_p_space, bool a,bool b,bool c, int pn, int thre):
    name(pname), is_parking_space(is_p_space), pinState{a,b,c},pin(pn), pre_threshold(thre)
    {
    if(amount_of_ldrs < 12){
      all_ldrs[amount_of_ldrs] = this;
      amount_of_ldrs++;
    };
    }

    void call_ldr(){
      if (is_parking_space){
        digitalWrite(MUX_A,pinState[0]);
        digitalWrite(MUX_B,pinState[1]);
        digitalWrite(MUX_C,pinState[2]);
      }
    }

    void get_value(){
      call_ldr();
      reading = analogRead(pin);
    }

    void calibrate(){
      get_value();
      threshold = reading - pre_threshold;
    }

    bool is_detected(){
      if(reading < threshold){
        return true;
      }else{
        return false;
      }
    }

    void reportdetect(){
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

    void report(){
      display.clear();
      display.setCursor(0, 0);
      display.println(name + " reading is:");
      display.setCursor(0, 1);
      display.println(reading);
    }

    static void updateall() {
      for (int i = 0; i < amount_of_ldrs; i++) {
        all_ldrs[i]->get_value();
      }
    }

    static void calibrateall(){
      for (int i = 0; i < amount_of_ldrs; i++) {
        all_ldrs[i]->calibrate();
      }
    }

};

ldr* ldr::all_ldrs[12] = {nullptr};
int ldr::amount_of_ldrs = 0;

// define led class
class led{
  private:
    int position;

  public:
    led(int p):
    position(p)
    {};

    void set_led(bool will_be_on){
      ledOutput[position - 1] = will_be_on;
    }

};


class parkingSpace{

  private:
  
    static int spaces;
    static parkingSpace* all_spaces[8];

    led& light;
    ldr& detector;

  public:
    
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
      if(detector.is_detected()){
        light.set_led(false);
      }else{
        light.set_led(true);
      }
    }

    void load_one(){
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
        delay(50);
      }
      display.clear();
    }
};

parkingSpace* parkingSpace::all_spaces[8] = {nullptr};
int parkingSpace::spaces = 0;


class gate{
  private:



    ldr& front;
    ldr& back;
    Servo& motor;
    bool is_in_use = false;
    int func;

  public:
    gate(ldr& l1, ldr& l2, Servo& m, int f):
    front(l1), back(l2), motor(m), func(f)
    {}
    

    void gate_down(){ motor.write(0);   }


    void do_gate(){
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

  
};


/*        Objects       */

// creating LDR objects
ldr LDR1("LDR1",true,false,false,false,    A0, 30);
ldr LDR2("LDR2",true,false,false,true,     A0, 30);
ldr LDR3("LDR3",true,false,true,false,     A0, 30);
ldr LDR4("LDR4",true,false,true,true,      A0, 30);
ldr LDR5("LDR5",true,true,false,false,     A0, 40);
ldr LDR6("LDR6",true,true,true,false,      A0, 30);
ldr LDR7("LDR7",true,true,true,true,       A0, 30);
ldr LDR8("LDR8",true,true,false,true,      A0, 30);

ldr Entrance1("Entrance1",false,false,false,false, A1, 30);
ldr Entrance2("Entrance2",false,false,false,false, A2, 20);
ldr Exit1("Exit1",false,false,false,false,         A3, 35);
ldr Exit2("Exit2",false,false,false,false,         A6, 30);

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

gate Entrance(Entrance1, Entrance2,Servo_Entrance, 1);
gate Exit(Exit1, Exit2,Servo_Exit, -1);



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



void do_screen(){
  display.clear();
  display.setCursor(0, 0);
  display.println("Spaces available:");
  display.setCursor(0, 1);
  display.println((8-cars_in_lot));
}


void setup(){
  // Setup the pins
  pinMode(MUX_A,OUTPUT);
  pinMode(MUX_B,OUTPUT);
  pinMode(MUX_C,OUTPUT);
  pinMode(SR_CLOCKPIN,OUTPUT);
  pinMode(SR_DATAPIN,OUTPUT);

  Servo_Entrance.attach(6);
  Servo_Exit.attach(7);

 
  // test the display
  display.init();
  display.backlight();
  display.setCursor(0,0);
  display.println("Calibrating...");
  ldr::calibrateall();
  Servo_Entrance.write(0);
  Servo_Exit.write(0);

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



void loop() {
  ldr::updateall();
  parkingSpace::updateall();
  shiftregisterwrite(ledOutput); 
  check_gate_status();
  
  display.clear();
  display.setCursor(0, 0);
  display.println(Entrance1.reading);
  display.setCursor(0, 1);
  display.println(Entrance1.threshold);
  delay(50);
  // total delay currently is 10 + 10.002 (per led) ms
}


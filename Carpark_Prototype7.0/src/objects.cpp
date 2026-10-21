#include <Arduino.h>

#include "objects.h"

#include "ldr.h"
#include "led.h"
#include "parkingSpace.h"
#include "gate.h"


ldr LDR2(true,false,false,true,     A0, 30);
ldr LDR1(true,false,false,false,    A0, 30);
ldr LDR3(true,false,true,false,     A0, 30);
ldr LDR4(true,false,true,true,      A0, 30);
ldr LDR5(true,true,false,false,     A0, 40);
ldr LDR6(true,true,true,false,      A0, 30);
ldr LDR7(true,true,true,true,       A0, 30);
ldr LDR8(true,true,false,true,      A0, 30);

ldr Entrance1(false,false,false,false, A1, 30);
ldr Entrance2(false,false,false,false, A2, 20);
ldr Exit1(false,false,false,false,         A3, 35);
ldr Exit2(false,false,false,false,         A6, 30);

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
/*
Port_1 = RGB  
Port_2 = ULTRASONIC
Port_3 = Kleur

Port_B = Rechter wiel
Port_C = Linker wiel
*/

#include "BrickPi3.h"
#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

uint16_t MIN;
uint16_t MAX;
sensor_color_t Color1;
sensor_light_t Light3;
sensor_ultrasonic_t Ultrasonic2;


void goLeft(int valWheel1, int valWheel2){
	BP.set_motor_power(PORT_C, valWheel1);
    BP.set_motor_power(PORT_B, valWheel2); // moet harder gaan dan motor C (links)
}

void goRight(int valWheel1, int valWheel2){
    BP.set_motor_power(PORT_C, valWheel1); // moet harder gaan dan motor B (rechts)
    BP.set_motor_power(PORT_B, valWheel2); 
}

void goForward(int valWheel1, int valWheel2){
    //Motoren gaan even snel
    BP.set_motor_power(PORT_C, valWheel1);
    BP.set_motor_power(PORT_B, valWheel2); 
}

int16_t measureColor() {
    BP.get_sensor(PORT_1, Color1);
    uint16_t val = Color1.Color;
    if (val < MIN) val = MIN;
    if (val > MAX) val = MAX;
    return (100*(val - MIN))/(MAX - MIN);
}

int16_t measureLight() {
    BP.get_sensor(PORT_3, Light3);
    uint16_t val = Light3.reflected;
    if (val < MIN) val = MIN;
    if (val > MAX) val = MAX;
    return (100*(val - MIN))/(MAX - MIN);
}

int16_t measureDistance(){
    return Ultrasonic2.cm;
}

int main(){
    signal(SIGINT, exit_signal_handler);
    BP.detect();
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_RED);
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
    BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
    
    while(true){
        cout << "RGB SENSOR (PORT3)" : << Light3.reflected << "\n";
        cout << "LIGHT SENSOR (PORT1)" : << Color1.reflected_red << "\n";
        cout << "ULTRASONIC SENSOR (PORT2): " << measureDistance() << "cm" << "\n";
        //Doe iets met de waarde
        sleep(2);
    }
  
}

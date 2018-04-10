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

uint16_t MINRGB;
uint16_t MAXRGB;
uint16_t MINLGT;
uint16_t MAXLGT;
sensor_color_t Color;
sensor_light_t Light3;
sensor_ultrasonic_t Ultrasonic2;

void hardLinks(){
	BP.set_motor_position_relative(PORT_C, -180);
	BP.set_motor_position_relative(PORT_B, 180);
}

void hardRechts(){
	BP.set_motor_position_relative(PORT_C, 180);
	BP.set_motor_position_relative(PORT_B, -180);
}

void hardForward(){
	BP.set_motor_position_relative(PORT_C, 180);
	BP.set_motor_position_relative(PORT_B, 180);
}

void goLeft(int valWheel1, int valWheel2){
    BP.set_motor_power(PORT_C, valWheel1);
    BP.set_motor_power(PORT_B, valWheel2); // moet harder gaan dan motor C (links)
}

void goRight(int valWheel1, int valWheel2){
    BP.set_motor_power(PORT_C, valWheel1); // moet harder gaan dan motor B (rechts)
    BP.set_motor_power(PORT_B, valWheel2); 
}

void goForward(){
    //Motoren gaan even snel
    BP.set_motor_power(PORT_C, 20);
    BP.set_motor_power(PORT_B, 20); 
}

void stop(){
    BP.set_motor_power(PORT_C, 0);
    BP.set_motor_power(PORT_B, 0);
}

int16_t measureColor() {
    BP.get_sensor(PORT_1, Color);
    uint16_t val = Color.reflected_red;
    return val;
}

int16_t measureLight() {
    BP.get_sensor(PORT_3, Light3);
    uint16_t val = Light3.reflected;
    return val;
}

float measureDistance(){
    BP.get_sensor(PORT_2, Ultrasonic2);
    float val = Ultrasonic2.cm;
    return val;
}

void objectLoop(){
	hardLinks();
	hardForward();
	hardRechts();
}

void doit(){
	    while(true){
        cout << "ULTRASONIC SENSOR (PORT2): " << measureDistance() << "cm" << "\n";
        //Doe iets met de waarde
	if(measureDistance() < 10){
		objectLoop();
		sleep(1);
	}
		
        if(int(measureLight()) < 2000 && int(measureColor()) < 400){
		goForward();}
	    //<2000 is wit, < 400 is wit
	else if(int(measureLight()) > 2000 && int(measureColor()) < 400){
		goLeft(-20, 50);}
	    //>2000 is zwart, < 400 is wit
	else if(int(measureLight()) < 2000 && int(measureColor()) > 400){
		goRight(50, -20);}
	    //<2000 is wit, > 400 is zwart
	else if(int(measureLight()) > 2000 && int(measureColor()) > 400){
			goLeft(-20, 50);}
	    //>2000 is zwart, >400 is zwart
	usleep(50000);
	stop();
  	usleep(10000);
    }	
}


int main(){
    signal(SIGINT, exit_signal_handler);
    BP.detect();
    int error;
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_RED);
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
    BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
    //BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
    //BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
    
    //doit();
	cout << measureDistance();
}



void exit_signal_handler(int signo){
  if(signo == SIGINT){
    BP.reset_all();    // Reset everything so there are no run-away motors
    exit(-2);
  }
}




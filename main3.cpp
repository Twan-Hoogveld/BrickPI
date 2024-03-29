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
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

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
	BP.set_motor_position_relative(PORT_C, -270);
	BP.set_motor_position_relative(PORT_B, 270);
}

void hardRechts(){
	BP.set_motor_position_relative(PORT_C, 260);
	BP.set_motor_position_relative(PORT_B, -260);
}

void hardForward(int val){
	BP.set_motor_position_relative(PORT_C, val);
	BP.set_motor_position_relative(PORT_B, val);
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
    BP.set_motor_power(PORT_C, 30);
    BP.set_motor_power(PORT_B, 34); 
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

void doit(){
	int counter = 0;
	while(true){
	if(measureDistance() <= 15 && measureDistance() > 0){
		hardLinks();
		sleep(1);
		hardLinks();
		sleep(1);
	}	
        if(int(measureLight()) < 2000 && int(measureColor()) > 400){
		goForward();
		counter++;
		if(counter > 450){
			hardLinks();
			sleep(1);
			hardLinks();
			sleep(1);
			counter = 0;
		}
		//< 2000 is wit, >450 is wit
	}
	else if(int(measureLight()) > 2000 && int(measureColor()) > 400){
		goLeft(-20, 50);
		counter = 0;
		//>2200 is zwart, >450 is wit
	}
	else if(int(measureLight()) < 2000 && int(measureColor()) < 400){
		goRight(50, -20);
		counter = 0;
	
		//<2000 is wit, < 400 is zwart
	}
	else if(int(measureLight()) > 2000 && int(measureColor()) < 400){
		srand (time(NULL));
		int dice = rand()%2;
		if (dice == 0){
			goLeft(-20, 50);
			usleep(110000);
		}
		else if (dice == 1){
			goRight(50, -20);
			usleep(110000);
		}
		// >2000 is zwart, <400 is zwart
	}
	usleep(50000);
	stop();
  	usleep(10000);
	cout << counter << "\n";
    }	
}


int main(){
    signal(SIGINT, exit_signal_handler);
    BP.detect();
    int error;
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_RED);
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
    BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
    BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
    BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
    doit();
}



void exit_signal_handler(int signo){
  if(signo == SIGINT){
    BP.reset_all();    // Reset everything so there are no run-away motors
    exit(-2);
  }
}

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

void checkObject(){
		sleep(2);
		cout << "Links draaien om object van zijkant te scannen." << "\n";
		hardLinks();
		cout << "Check of object er nog staat." << "\n";
		sleep(2);
		//Je kijkt nu tegen de zijkant van het object
		if (measureDistance() < 40){
			cout << "Object staat nog voor je" << "\n";
			cout << "Naar rechts draaien" << "\n";
			hardRechts();
			sleep(2);
			cout << "Vooruit rijden" << "\n";
			hardForward(360);
			cout << "rerun checkObject functie" << "\n";
			checkObject();
		}
		else {
			cout << "afstand is meer dan 40 cm, loop eindigt, zoek lijn op" << "\n";
			hardForward(450);
			sleep(2);
			hardRechts();
		}
	}

void objectLoop(){
	cout << "Het object is gescanned, robot gaat naar rechts." << "\n";
	hardRechts();
	sleep(2);
	cout << "Vooruit gaan om object te kunnen dodgen." << "\n";
	hardForward(540);
	sleep(2);
	cout << "Ga naar links om object te scannen." << "\n";
	hardLinks();
	sleep(2);
	cout << "vooruit gaan om object te dodgen" << "\n";
	hardForward(540);
	sleep(2);
	cout << "Je zou nu naast het object moeten staan" << "\n";
	//Check of je het object nog ziet
	checkObject();
}

void doit(){
	while(true){
	if(measureDistance() <= 15 && measureDistance() > 0){
		objectLoop();
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
	cout << "Lijn volgen" << "\n";
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




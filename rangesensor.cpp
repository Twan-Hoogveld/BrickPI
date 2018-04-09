#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <iomanip>

using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);


int main(){
	signal(SIGINT, exit_signal_handler);
	
	BP.detect();
	
	int error;
	
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
	sensor_ultrasonic_t Ultrasonic2;
	
	while(true){
		error = 0;
		if(BP.get_sensor(PORT_2, Ultrasonic2) == 0){
			cout << "Afstand met object: " << Ultrasonic2.cm << "cm" << "\n";
		}
	}
}

void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}
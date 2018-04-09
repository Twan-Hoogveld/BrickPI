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
	
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
	sensor_light_t light3;
	
	while(true){
		error = 0;
		if(BP.get_sensor(PORT_3, light3) == 0){
			cout << "lichtreflectie: " << light3.reflected_red <<  "\n";
			//zwart is over 2000? (2517, 2544, 2109)
			// wit is onder 2000? (1696, 1653, 1694, 1638, 1744)
		}
		sleep(3);
	}
}

void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}
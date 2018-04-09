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
	
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_RED);
	sensor_color_t color1;
	
	while(1){
		error = 0;
		BP.get_sensor(PORT_1, color1);
		uint16_t roodwaarde = color1.reflected_red;
		cout << roodwaarde;
		sleep(1);
		}
}

void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}
#include "BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <iomanip>

using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

//port c is linker wiel
//port b is rechter wiel

void stop(void){
	BP.set_motor_power(PORT_B, 0);
	BP.set_motor_power(PORT_C, 0);
}

void drive(void){
	BP.set_motor_power(PORT_B, 100);
	BP.set_motor_power(PORT_C, -100);
}

void links(void){
	BP.set_motor_position_relative(PORT_C, -180);
	BP.set_motor_position_relative(PORT_B, 180);
}

void rechts(void){
	BP.set_motor_position_relative(PORT_C, 180);
	BP.set_motor_position_relative(PORT_B, -180);
}



int main(){
	signal(SIGINT, exit_signal_handler);
	
	BP.detect();
	
	int error;
	
	BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
	BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
	
	int input;
	
	while(true){
		error = 0;
		
		cin >> input;
		cout << input;
		
		if(input == 1){
			drive();
		}
		else if(input == 4){
			stop();
		}
	}
}

void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}
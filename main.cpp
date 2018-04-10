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
    BP.get_sensor(PORT_1, Color);
    uint16_t val = Color.reflected_red;
    if (val < MINRGB) val = MINRGB;
    if (val > MAXRGB) val = MAXRGB;
    return (100*(val - MINRGB))/(MAXRGB - MINRGB);
}

int16_t measureLight() {
    BP.get_sensor(PORT_3, Light3);
    uint16_t val = Light3.reflected;
    if (val < MINLGT) val = MINLGT;
    if (val > MAXLGT) val = MAXLGT;
    return (100*(val - MINLGT))/(MAXLGT - MINLGT);
}

float measureDistance(){
    BP.get_sensor(PORT_2, Ultrasonic2);
    return Ultrasonic2.cm;
}

int main(){
    signal(SIGINT, exit_signal_handler);
    BP.detect();
    int error;
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_RED);
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
    BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
	
  //calibrate
  string regel;
  cout << "plaats RGB recht boven de lijn (zwart) en voer in a gevolgd door enter" << endl;
  cin >> regel;
  BP.get_sensor(PORT_1, Color);
  MINRGB = Color.reflected_red;
  cout << "MIN = " << MINRGB << endl;
  cout << "plaats RGB helemaal naast de lijn (wit) en voer in b gevolgd door enter" << endl;
  cin >> regel;
  BP.get_sensor(PORT_1, Color);
  MAXRGB = Color.reflected_red;
  cout << "MAX = " << MAXRGB << endl;
  cout << "plaats het voertuig met de RGB half boven de lijn en voer in c gevolgd door enter" << endl;
  cin >> regel;
	
  //calibrate
  
  cout << "plaats KLEUR recht boven de lijn (zwart) en voer in a gevolgd door enter" << endl;
  cin >> regel;
  BP.get_sensor(PORT_3, Light3);
  MINLGT = Light3.reflected;
  cout << "MIN = " << MINLGT << endl;
  cout << "plaats KLEUR helemaal naast de lijn (wit) en voer in b gevolgd door enter" << endl;
  cin >> regel;
  BP.get_sensor(PORT_3, Light3);
  MAXLGT = Light3.reflected;
  cout << "MAX = " << MAXLGT << endl;
  cout << "plaats KLEUR met de sensor half boven de lijn en voer in c gevolgd door enter" << endl;
  cin >> regel;
    
    while(true){
        cout << "LIGHT SENSOR (PORT3) :" << measureLight() << "\n";
        cout << "RGB SENSOR (PORT1) :" << measureColor() << "\n";
        cout << "ULTRASONIC SENSOR (PORT2): " << measureDistance() << "cm" << "\n";
        //Doe iets met de waarde
        sleep(2);
    }
  
}

void exit_signal_handler(int signo){
  if(signo == SIGINT){
    BP.reset_all();    // Reset everything so there are no run-away motors
    exit(-2);
  }
}

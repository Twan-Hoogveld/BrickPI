#include "BrickPi3.h"
#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

BrickPi3 BP;

void exit_signal_handler(int signo);

uint16_t MIN;
uint16_t MAX;
sensor_color_t mycolor;

int16_t measureLight() {
    
    BP.get_sensor(PORT_1, mycolor);
    uint16_t val = mycolor.reflected_red;
    if (val < MIN) val = MIN;
    if (val > MAX) val = MAX;
    return (100*(val - MIN))/(MAX - MIN);
}

int main(){
    signal(SIGINT, exit_signal_handler);
    BP.detect();
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_COLOR_RED);
  
}

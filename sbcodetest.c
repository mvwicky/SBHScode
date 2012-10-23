#include <stdio.h>
#include "drive.h"
#include "gen_configs.h"
#include "servo.h"


#ifndef PI
#define PI 3.14159
#endif


int main ()
{
    build_left_wheel(0 , .1 , 1100 , .05);
    build_right_wheel(3 , .1 , 1100 , .05);
    build_servo(0 , 0 , 2047 , 0);
    build_servo(1 , 1 , 2047 , 0);
    build_generic_motor(0 , 2 , 1100 , .05);
    build_generic_motor(1 , 3 , 1100 , .05);
    build_cbc(left.port , right.port , servo_gen[0].port , servo_gen[1].port , gen[0].port , gen[1].port);
    drive_arc(750 , .5 , 90 , -1);

}

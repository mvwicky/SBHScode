#include <stdio.h>
#include "drive.h"
#include "servo.h"


#ifndef PI
#define PI 3.14159
#endif


int main ()
{
    build_left_wheel(0 , .07 , 1100 , .04);
    build_right_wheel(3 , .07 , 1100 , .04);
    drive_arc(750 , .25 , 90 , 1);
	
}

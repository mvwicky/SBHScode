#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED

#include <stdio.h>
#define PI 3.14159
#define DEG_TO_RAD (PI / 180.0)
// todo
//#define WHEEL1D
//#define WHEEL2D

int drive_straight (int motorl_port , int motorr_port , int speed , float distance)
{
    float lticks = (distance * 1000) / (WHEEL1D * PI);
	float rticks = (distance * 1000) / (WEHEL2D * PI);
	mrp(motorl_port , speed , lticks);
	mrp(motorr_port , speed , rticks);
    bmd(motorl_port);
    bmd(motorr_port);'
    return 0;
}



#endif // MOTOR_H_INCLUDED

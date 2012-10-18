#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED
/*
This library contains the code to drive motors that move the robot
created 10/17/12

*/
#include <stdio.h>
#define PI 3.14159
#define DEG_TO_RAD (PI / 180.0)
// todo
//#define WHEEL1D
//#define WHEEL2D
struct drive_motors {
    int port; // port that the motor is in
    int radius_to_middle; // 1/2 of the distance from one motor to the other
    int ticks; //  ticks per revolution (1000)
    int diameter; // diameter of the wheel
} left , right;

void build_left_wheel(int port , int rad , int ticks , int diameter)
{
    left.port = port;
    left.radius_to_middle = rad;
    left.ticks = ticks;
    left.diameter = diameter;
}

void build_right_wheel(int port , int rad , int ticks , int diameter)
{
    right.port = port;
    right.radius_to_middle = rad;
    right.ticks = ticks;
    right.diameter = diameter;
}

int drive_straight (int speed , float distance)
{
    float lticks = (distance * left.ticks) / (left.diameter * PI);
	float rticks = (distance * right.ticks) / (right.ticks * PI);
	mrp(left.port , speed , (int)lticks);
	mrp(right.port , speed , (int)rticks);
    bmd(left.port);
    bmd(right.port);'
    return 0;
}

int drive_arc(int motorl_port , int motorr_port , int speed , float radius , float angle)
{

}


#endif // MOTOR_H_INCLUDED

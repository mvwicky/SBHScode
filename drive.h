#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED
/*
This library contains the code to drive motors that move the robot.
Also contains code to drive motors not associated with movement.
created 10/17/12


>struct drive motors: this allows the user to store the information for the motors used in driving the robot.
                      normally there are only two: left and right. More can be generated for use in non-starndard
                      configurations.

>build_left-wheel/build_right_wheel: this gives the information for the dirve motors.

>drive_straight: this function drives the robot in a straight line. All values are in meters.

>drive_arc: This moves the robot on an arc defined by a radius and an angle. All values are in meters. !! TO REVIEW !!

*/
#include <stdio.h>
#define PI 3.14159
#define DEG_TO_RAD (PI / 180.0)
// todo

struct drive_motors {
    int port; // port that the motor is in
    float radius_to_middle; // 1/2 of the distance from one motor to the other
    float ticks; //  ticks per revolution (1000)
    float diameter; // diameter of the wheel
} left , right;

struct gen_motor {
    int port; // port
    float ticks; // ticks per revolution (1000)
    float diameter; // diameter of wheel
} gen[10];

void build_left_wheel(int port , float rad , float ticks , float diameter)
{
    left.port = port;
    left.radius_to_middle = rad;
    left.ticks = ticks;
    left.diameter = diameter;
}

void build_right_wheel(int port , float rad , float ticks , float diameter)
{
    right.port = port;
    right.radius_to_middle = rad;
    right.ticks = ticks;
    right.diameter = diameter;
}

int drive_straight (int speed , float distance) // both values in meters
{
    float lticks = (distance * left.ticks) / (left.diameter * PI);
	float rticks = (distance * right.ticks) / (right.ticks * PI);
	mrp(left.port , speed , (int)lticks);
	mrp(right.port , speed , (int)rticks);
    bmd(left.port);
    bmd(right.port);
    return 0;
}
// FIX THIS: Wheels need to move at different speeds
int drive_arc(int speed , float radius , float angle , float direction) // radius in meters , angle in degrees
// if direction == 1 then left wheel is inner
// if direction == -1 then right wheel is inner
// if direction != -1 && direction != 1 then error
{
    float Sinner;
    float Souter;
    float inTicks;
    float outTicks;
    if (direction == 1) // CCW
    {
        Sinner = (radius - left.radius_to_middle) * (angle * DEG_TO_RAD); // inner arc length
        Souter = (radius + right.radius_to_middle) * (angle * DEG_TO_RAD); // outer arc length
        inTicks = (1000 * Sinner) / (PI * right.diameter); // inner wheel ticks to move
        outTicks = (1000 * Souter) / (PI * left.diameter); // outer wheel ticks to move
        mrp(right.port , speed , (int)inTicks);
        mrp(left.port, speed , (int)outTicks);
        bmd(right.port);
        bmd(left.port);

    }
    if (direction == -1) // CW
    {
        Sinner = (radius - right.radius_to_middle) * (angle * DEG_TO_RAD);// inner arc length
        Souter = (radius + left.radius_to_middle) * (angle * DEG_TO_RAD); // outer arc length
        inTicks = (1000 * Sinner) / (PI * right.diameter); // inner wheel ticks to move
        outTicks = (1000 * Souter) / (PI * left.diameter); // outer wheel ticks to move
        mrp(left.port , speed , (int)inTicks);
        mrp(right.port , speed , (int)outTicks);
        bmd(left.port);
        bmd(right.port);
    }
    if (direction != 1 && direction != -1)
    {
        printf("error");

    }
    // to review
    return 0;
}

int drive_spin(int speed , float theta , int direction) // angle in degrees
// if direction == 1 then left wheel is inner
// if direction == -1 then right wheel is inner
// if direction != -1 && direction != 1 then error
{
    float lticks;
    float rticks;
    if (direction == 1) // CCw
    {
        rticks = (1000) / (PI * right.diameter);
        lticks = -1.0 * rticks;
        mrp(right.port , speed , (int)rticks);
        mrp(left.port , speed , (int)lticks);
        bmd(right.port);
        bmd(left.port);
    }
    if (direction == -1)
    {
        lticks = (1000) / (PI * left.diameter);
        rticks = -1.0 * lticks;
        mrp(left.port , speed , (int)lticks);
        mrp(right.port , speed , (int)rticks);
        bmd(left.port);
        bmd(right.port);
    }
    if (direction != 1 && direction != -1)
    {
        printf("error");
    }
    // todo
    return 0;
}



void build_generic_motor (int n , int port , float ticks , float diameter)
{
    gen[n].port = port;
    gen[n].ticks = ticks;
    gen[n].diameter = diameter;
}

int motor_spin_for (int n , float speed , float time)
{
    float ticks = (speed * time);
    mrp(gen[n].port , speed , (int)ticks);
    bmd(gen[n].port);
}



#endif // MOTOR_H_INCLUDED

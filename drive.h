#ifndef DRIVE_H_INCLUDED
#define DRIVE_H_INCLUDED
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
#include <iostream>
#define PI 3.14159
#define DEG_TO_RAD (PI / 180.0)// todo


struct drive_motors { 
    int port; // port that the motor is in
    float radius_to_middle; // 1/2 of the distance from one motor to the other
    float ticks; //  ticks per revolution (1000)
    float diameter; // diameter of the wheel in meters
} left , right;

struct gen_motor { 
    int port; // port
    float ticks; // ticks per revolution (1000)
    float diameter; // diameter of wheel
} gen[10];

struct servo {
    int port;
    float ticks;
    int min;
    int max;
    float diameter;
} servo[10];

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

void build_generic_motor(int n ,int port , float ticks , float diameter)
{
    gen[n].port = port;
    gen[n].ticks = ticks;
    gen[n].diameter = diameter;
}

void build_servo (int n , int port , float ticks , int max , int min , float diameter)
{
    servo[n].port = port;
    servo[n].ticks = ticks;
    servo[n].max = max;
    servo[n].min = min;
    servo[n].diameter = diameter;
}

int drive_straight (int speed , float distance) // both values in meters
{
    float lticks = (distance * left.ticks) / (left.diameter * PI);
	float rticks = (distance * right.ticks) / (right.ticks * PI);
	mrp(left.port , speed , (int)lticks);
	mrp(right.port , speed , (int)rticks);
    bmd(left.port);
    bmd(right.port);
    return 1;
}

// FIX THIS: Goes double the radius
int drive_arc(int speed , float radius , float angle , float direction) // radius in meters , angle in degrees
// if direction == 1 then left wheel is inner
// if direction == -1 then right wheel is inner
// if direction != -1 && direction != 1 then error
// Idea: Base the speed on an arbitrary time variable. Would be easier to implement but doesn't seem ideal.
//
{
    float S;
    float Sinner;
    float Souter;
    float inTicks;
    float outTicks;
    float sprop;
    float lspeed;
    float rspeed;
    if (direction == 1) // CCW
    {
        S = radius * (angle * DEG_TO_RAD);
        Sinner = (radius - left.radius_to_middle) * (angle * DEG_TO_RAD); // inner arc length
        Souter = (radius + right.radius_to_middle) * (angle * DEG_TO_RAD); // outer arc length
        sprop = Souter/Sinner;
        inTicks = (right.ticks * Sinner) / (PI * right.diameter); // inner wheel ticks to move
        outTicks = (left.ticks * Souter) / (PI * left.diameter); // outer wheel ticks to move
        lspeed = (float)speed * sprop * Souter / S;
        rspeed = (float)speed * sprop * Sinner / S;
        mrp(right.port , ((int)rspeed) , ((int)inTicks));
        mrp(left.port, ((int)lspeed) , ((int)outTicks));
        bmd(right.port);
        bmd(left.port);

    }
    if (direction == -1) // CW
    {
        S = radius * (angle * DEG_TO_RAD);
        Sinner = (radius - right.radius_to_middle) * (angle * DEG_TO_RAD);// inner arc length
        Souter = (radius + left.radius_to_middle) * (angle * DEG_TO_RAD); // outer arc length
        sprop = Souter/Sinner;
        inTicks = (right.ticks * Sinner) / (PI * right.diameter); // inner wheel ticks to move
        outTicks = (left.ticks * Souter) / (PI * left.diameter); // outer wheel ticks to move
        lspeed = (float)speed * sprop * Sinner / S;
        rspeed = (float)speed * sprop * Souter / S;
        mrp(left.port , ((int)lspeed) , ((int)inTicks));
        mrp(right.port , ((int)rspeed) , ((int)outTicks));
        bmd(left.port);
        bmd(right.port);
    }
    if (direction != 1 && direction != -1)
    {
        return -1;

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
    if (direction == 1) // CCW
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
        
    }
    // todo
    return 0;
}

int motor_spin_for (int n , float speed , float time)
{
    float ticks = (speed * time);
    mrp(gen[n].port , speed , (int)ticks);
    bmd(gen[n].port);
}



#endif // MOTOR_H_INCLUDED

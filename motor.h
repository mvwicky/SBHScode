#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED
/*
This library contains the code to drive motors not associated with movement
created 10/17/12

*/
#include <stdio.h>
#define PI 3.14159
#define DEG_TO_RAD (PI / 180.0)
// todo
//#define WHEEL1D
//#define WHEEL2D

struct gen_motor {
    int port; // port
    int ticks; // ticks per revolution (1000)
    int diameter; // diameter of wheel
} gen[10];

void build_generic_motor (int n , int port , int ticks , int diameter)
{
    gen[n].port = port;
    gen[n].ticks = ticks;
    gen[n].diameter = diameter;
}

int motor_spin_for (int n , float speed , float time)
{
    float ticks = (speed * time);
    mrp(gen[n].port , speed , (int)ticks);
    bmc(gen[n].port);
}

#endif // MOTOR_H_INCLUDED

#ifndef GEN_CONFIGS_H_INCLUDED
#define GEN_CONFIGS_H_INCLUDED
/*
This file includes tests of basic cbc configurations

*/

#include <stdio.h>
#include "drive.h"
#include "motor.h"
#include "servo.h"

struct cbc {
    int left_motor;
    int right_motor;
    int servo1;
    int servo2;
    int gen_motor1;
    int gen_motor2;
} the_cbc;

void build_cbc (int lmp , int rmp , int s1p , int s2p , int g1p , int g2p)
{
    the_cbc.left_motor = lmp;
    the_cbc.right_motor = rmp;
    the_cbc.servo1 = s1p;
    the_cbc.servo2 = s2p;
    the_cbc.gen_motor1 = g1p;
    the_cbc.gen_motor2 = g2p;
}


#endif // GEN_CONFIGS_H_INCLUDED

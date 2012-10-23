#ifndef SERVO_H_INCLUDED
#define SERVO_H_INCLUDED
/*
This library contains functions to
drive servos
created 10/18/12
*/

#include <stdio.h>
#define PI 3.14159
#define DEG_TO_RAD (PI / 180.0)

struct gen_servo {
    int port;
    int max;
    int min;
} servo_gen[4];

void build_servo (int n , int port , int max , int min)
{
    servo_gen[n].port = port;
    servo_gen[n].max = max;
    servo_gen[n].min = min;
}

int ssp(int n , int fin_pos)
{
    int current_pos = get_servo_position(servo_gen[n].port);
    if (current_pos < fin_pos)
    {
        while (current_pos < fin_pos)
        {
            set_servo_position(servo_gen[n].port , current_pos);
            current_pos+=1;
            msleep(1);

        }
        return 0;
    }
    if (current_pos > fin_pos)
    {
        while (current_pos > fin_pos)
        {
            set_servo_position(servo_gen[n].port , current_pos);
            current_pos-=1;
            msleep(1);
        }
    }
    return 0;
    if (current_pos == fin_pos)
    {
        return 0;
    }

}

double_servo_move(int n , int m , int fin_pos)
{
    int current_posn = get_servo_position(servo_gen[n]);
    int current_posm = get_servo_position(servo_gen[m]);
    if (current_posn != current_posm)
    {
        current_posn = current_posm;
        set_servo_position(gen_servo[m] , current_posm);
        set_servo_position(gen_servo[n] , current_posn);
        current_posn = get_servo_position(servo_gen[n]);
        current_posm = get_servo_position(servo_gen[m]);
    }
    if (current_posn < fin_pos && current_posm < fin_pos)
    {
        while (current_posn < fin_pos || current_posm < fin_pos)
        {
            current_posn+=1;
            current_posm+=1;
            set_servo_position(gen_servo[n] , current_posn);
            set_servo_position(gen_servo[m] , current_posm);
            if (current_posn >= fin_pos || current_posm >= fin_pos)
            {
                break;
            }
        }
    }
    if (current_posn > fin_pos && current_posm > fin_pos)
    {

    }
    if (current_posn == fin_pos || current_posn == fin_pos)
    {

    }
}

#endif // SERVO_H_INCLUDED

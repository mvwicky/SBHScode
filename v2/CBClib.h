#ifndef CBClib2_H_INCLUDED
#define CBClib2_H_INCLUDED

#include <stdbool.h>

#define PI 3.14159265359
#define E 2.71828182845
#define DEG_TO_RAD (PI / 180)

int motors_p[] =  {0 , 1 , 2 , 3};
int servos_p[] = {0 , 1 , 2 , 3};
int analog_p[] =  {0 , 1 , 2 , 3 , 4 , 5 , 6 , 7};
int digital_p[] =  {8 , 9 , 10 , 11 , 12 , 13 , 14 , 15};

struct cbc {


	struct drive_motors {
		int port;
		float radius_to_middle;
		int ticks;
		float diameter;
	} left , right;
	struct gen_motor {
		int port;
		int ticks;
		float diameter;
	} gen[4];
	struct servo {
		int port;
		int ticks;
		int min;
		int max;
	}servo[6];
	struct s_analog {
		int port;
		int value;
		int thresh;
	}top_hat , ET, s_top_hat[5] , slot[2] , light[3] , pot , slide;
	struct s_digital {
		int port;
		int value;
	}lever[2] , s_touch[4] , l_touch[6] , slot_s[2];
}lego , create;

struct cbc lego;

bool check_in(int a[] , int p)
{
	int num = sizeof(a) / 4;
	int n = 0;
	int i = 0;
	bool is_in = false;
	while (n < num)
	{
		if (a[n] == p)
		{
			is_in = true;
		}
		if (is_in == true)
			break;
		n++;
	}
	return is_in;
}

int build_left_motor(int port , float radius , int ticks , float diameter)
{
	lego.left.port = port;
	lego.left.radius_to_middle = radius;
	lego.left.ticks = ticks;
	lego.left.diameter = diameter;
	return 0;
}

int build_right_motor(int port , float radius , int ticks , float diameter)
{
	lego.right.port = port;
	lego.right.radius_to_middle = radius;
	lego.right.ticks = ticks;
	lego.right.diameter = diameter;
	return 0;
}

int build_gen_motor(int index , int port , int ticks)
{
	lego.gen[index].port = port;
	lego.gen[index].ticks = ticks;
	return 0;
}

int build_servo(int index , int port , int ticks , int min , int max)
{
	lego.servo[index].port = port;
	lego.servo[index].ticks = ticks;
	lego.servo[index].min = min;
	lego.servo[index].max = max;
	return 0;
}

int build_top_hat(int port , int thresh)
{
	lego.top_hat.port = port;
	lego.top_hat.thresh = thresh;
	return 0;
}

int build_ET(int port)
{
	lego.ET.port = port;
	return 0;
}

int build_s_top_hat(int index , int port)
{
	lego.s_top_hat[index].port = port;
	return 0;
}

int build_light(int index , int port)
{
	lego.s_top_hat[index].port = port;
	return 0;
}

int build_slide(int port)
{
	lego.slide.port = port;
	return 0;
}

int build_pot(int port)
{
	lego.pot.port = port;
	return 0;
}

int build_lever(int index , int port)
{
	lego.lever[index].port = port;
	return 0;
}

int build_s_touch(int index , int port)
{
	lego.s_touch[index].port  = port;
	return 0;
}

int build_l_touch(int index , int port)
{
	lego.l_touch[index].port = port;
	return 0;
}

int build_slot(int index , int port)
{
	lego.slot[index].port = port;
	return 0;
}

float mm_to_ticks(float dist)
{
	float ticks = ((dist * lego.left.ticks) / (lego.left.diameter * PI));
	return ticks;
}

float ticks_to_mm(float ticks)
{
	float dist = ((ticks * (lego.left.diameter * PI)) / lego.left.ticks);
	return dist;
}

float arc_length(float angle , float radius)
{
	float alength = ((DEG_TO_RAD * angle) * radius);
	return alength;
}

void bmd_both()
{
	bmd(lego.left.port);
	bmd(lego.right.port);
}

int drive_straight(int speed , int distance)
{
	float mticks = mm_to_ticks(distance);
	mrp(lego.left.port , (int)mticks , speed);
	mrp(lego.left.port , (int)mticks , speed);
	bmd_both();
	return 0;	
}

int drive_arc(int speed , float radius , float angle , int direction)
{
	float S = arc_length(angle , radius);
	float sinner;
	float souter;
	float in_ticks;
	float out_ticks;
	float sprop;
	float lspeed;
	float rspeed;
	if (direction == 1) // CCW
	{
		sinner = arc_length((radius - lego.left.radius_to_middle) , angle);
		souter = arc_length((radius + lego.left.radius_to_middle) , angle);
		sprop = souter/sinner;
		in_ticks = mm_to_ticks(sinner);
		out_ticks = mm_to_ticks(souter);
		lspeed = (float)speed * sprop * (souter / S);
		rspeed = (float)speed * sprop * (sinner / S);
		mrp(lego.right.port , (int)rspeed , (int)in_ticks);
		mrp(lego.left.port , (int)lspeed , (int)out_ticks);
		bmd_both();
		return 0;
	}
	if (direction == -1) // CW
	{
		sinner = arc_length((radius - lego.right.radius_to_middle) , angle);
		souter = arc_length((radius + lego.left.radius_to_middle) , angle);
		sprop = souter/sinner;
		in_ticks = mm_to_ticks(sinner);
		out_ticks = mm_to_ticks(souter);
		lspeed = (float)speed * sprop * (sinner / S);
		rspeed = (float)speed * sprop * (souter / S);
		mrp(lego.right.port , (int)rspeed , (int)in_ticks);
		mrp(lego.left.port , (int)lspeed , (int)out_ticks);
		bmd_both();
		return 0;
	}
	if (direction != 1 && direction != -1)
		return -1;
}

int drive_spin(int speed , float angle , int direction)
{
	float ldistance;
	float rdistance;
	float lticks;
	float rticks;
	float lspeed;
	float rspeed;
	if (direction == 1) // CCW
	{
		ldistance = -1.0 * arc_length(lego.left.radius_to_middle , angle);
		rdistance = arc_length(lego.right.radius_to_middle , angle);
		lticks = mm_to_ticks(ldistance);
		rticks = mm_to_ticks(rdistance);
		lspeed = (float)speed * lego.left.radius_to_middle / (2 * lego.left.radius_to_middle);
		rspeed = (float)speed * lego.right.radius_to_middle / (2 * lego.right.radius_to_middle);
		mrp(lego.left.port , (int)lspeed , (int)lticks);
		mrp(lego.right.port , (int)rspeed , (int)rticks);
		bmd_both();
	}
	if (direction == -1) // CW
	{
		ldistance = arc_length(lego.left.radius_to_middle , angle);
		rdistance = -1.0 * arc_length(lego.right.radius_to_middle , angle);
		lticks = mm_to_ticks(ldistance);
		rticks = mm_to_ticks(rdistance);
		lspeed = (float)speed , lego.left.radius_to_middle / (2 * lego.left.radius_to_middle);
		rspeed = (float)speed , lego.right.radius_to_middle / (2 * lego.right.radius_to_middle);
		mrp(lego.left.port , (int)lspeed , (int)lticks);
		mrp(lego.right.port , (int)rspeed , (int)rticks);
		bmd_both();
	}
	if (direction != 1 && direction != -1)
		return -1;
}

int motor_spin_for(int index , int speed , int time)
{
	float ticks = ((float)speed * (float)time);
	mrp(lego.gen[index].port , speed , (int)ticks);
	bmd(lego.gen[index].port);
	return 0;
}

int msf(int index , int speed , int time)
{
	return motor_spin_for(index , speed , time);
}

int motor_spin_ticks(int index , int speed , int ticks)
{
	mrp(lego.gen[index].port , speed , ticks);
	bmd(lego.gen[index].port);
	return 0;
}

int mst(int index , int speed , int ticks)
{
	return motor_spin_ticks(index, speed , ticks);
}


#endif
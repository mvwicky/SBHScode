#ifndef CBCclass_H_INCLUDED
#define CBCclass_H_INCLUDED
#include <iostream>
#include <cmath>

#define PI 3.14159
#define E 2.71828
#define DEG_TO_RAD (PI / 180.0)
#define EC 1023 // Closest value of the ET
#define EF 0 // Furthest value of the ET
#define TOL 50
#define ET_DIST(x) ( ( (-1.0 * 0.07331378) * (x) ) + 80 ) // I literally have no idea if this is the right function
#define ET_SPEED(x) ((-1.0 * 252.665) * (log(.00102755 * (x))))
#define A_DEL 3

class cbc {
	struct drive_motors {
		int port;
		float radius_to_middle;
		float ticks;
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
		int high_thresh;
		int low_thresh;
	}top_hat , ET, s_top_hat[5] , slot[2] , light[3];
	struct s_digital {
		int port;
		int value;
	}lego , create;
	public:
		void build_left_motor(int , float , float , float); // port , radius to middle (of chassis) , # of ticks ,  diameter (of wheel)
		void build_right_motor(int , float , float , float); // port , radius to middle (of chassis) , # of ticks ,  diameter (of wheel)
		void build_generic_motor(int , int , int , float); // port , # of ticks ,  diameter (of wheel)
		void build_servo(int , int , int , int , int); //port , number of ticks , min , max
		int build_top_hat(int , int , int); // port , high threshold (black) , low threshold (white)
		int build_s_top_hat(int , int , int , int); // index , port , high threshold (black) , low threshold (white)
		int build_ET(int); // port
		float mm_to_ticks(float); // distance in mm
		float ticks_to_mm(float); // ticks in ticks
		int drive_straight(int , float); // speed , distance
		int drive_arc(int , float , float , float); // speed , radius , angle , direction
		int drive_spin(int , float , int); // speed , angle , direction
		int motor_spin_for(int , float , float); // motor index, speed , time
		int motor_spin_ticks(int , int , int); // motor index , speed , ticks
		int move_servo_to(int , int); // servo index , servo final position
		int double_servo_move(int , int , int); // servo index 1 , servo index 2 , final position
		int average(int , int); // port (analog sensor) , amount of samples
		int ramp_up(float , float); // speed , distance
		int bmd_both(); // 
		int ET_drive(int , int , int); // index , direction to turn  , distance to move back (mm) , direction to turn 
		void set_lines(int , int); // left , right (indicies (of small top hats))
		int line_follow(int , int , int); // base speed , left , right (indicies (of small top hats))
		int num_of_in_ch(int); // channel
}lego , create;

void cbc::build_left_motor(int p , float r , float t , float d)
{
	left.port = p;
	left.radius_to_middle = r;
	left.ticks = t;
	left.diameter = d;
}

void cbc::build_right_motor(int p , float r , float t , float d)
{
	right.port = p;
	right.radius_to_middle = r;
	right.ticks = t;
	right.diameter = d;
}

void cbc::build_generic_motor(int n , int p , int t , float d)
{
	gen[n].port = p;
	gen[n].ticks = t;
	gen[n].diameter = d;
}

void cbc::build_servo(int n , int p , int t , int mi , int ma)
{
	servo[n].port = p;
	servo[n].ticks = t;
	servo[n].min = mi;
	servo[n].max = ma;
}

int cbc::build_top_hat(int p , int h , int l)
{
	top_hat.port = p;
	top_hat.high_thresh = h;
	top_hat.low_thresh = l;
	return 0;
}

int cbc::build_s_top_hat(int i , int p , int h , int l)
{
	s_top_hat[i].port = p;
	s_top_hat[i].high_thresh = h;
	s_top_hat[i].low_thresh = l;
	return 0;
}

int cbc::build_ET(int p)
{
	ET.port = p;
	return 0;
}

float cbc::mm_to_ticks(float mm)
{
	float ticks = ((mm * left.ticks) / (left.diameter * PI));
	return ticks;
}

float cbc::ticks_to_mm(float ticks)
{
	float mm = ((ticks * (left.diameter * PI)) / left.ticks);
	return mm;
}

int cbc::drive_straight(int s , float d)
{
	float lticks = ((d * left.ticks) / (left.diameter * PI));
	float rticks = ((d * right.ticks) / (right.diameter * PI));
	mrp(left.port , s , ((int)lticks) );
	mrp(right.port , s , ((int)rticks) );
	bmd(left.port);
	bmd(right.port);
	return 1;
}

int cbc::drive_arc(int s , float r , float a , float d) // BTW: Goes double the radius
{
	float S;
	float sinner;
	float souter;
	float inTicks;
	float outTicks;
	float sprop;
	float lspeed;
	float rspeed;
	if (d == 1) //CCW
	{
		S = r * (a * DEG_TO_RAD);
		sinner = ( (r - left.radius_to_middle) * (a * DEG_TO_RAD) );
		souter = ( (r + right.radius_to_middle) * (a * DEG_TO_RAD) );
		sprop = souter/sinner;
		inTicks = ( (right.ticks * sinner) / (PI * right.diameter) );
		outTicks = ( (left.ticks * souter) / (PI * left.diameter) );
		lspeed = (float)s * sprop * souter / S;
        rspeed = (float)s * sprop * sinner / S;
        mrp(right.port , ((int)rspeed) , ((int)inTicks));
        mrp(left.port, ((int)lspeed) , ((int)outTicks));
        bmd_both();
        return 0;
	}
	if (d == -1) //CW
	{
		S = r * (a * DEG_TO_RAD);
		sinner = ( (r - right.radius_to_middle) * (a * DEG_TO_RAD) );
		souter = ( (r + left.radius_to_middle) * (a * DEG_TO_RAD) );
		sprop = souter/sinner;
		inTicks = ( (left.ticks * sinner) / (PI * left.diameter) );
		outTicks = ( (right.ticks * souter) / (PI * right.diameter) );
		lspeed = (float)s * sprop * sinner / S;
        rspeed = (float)s * sprop * souter / S;
        mrp(right.port , ((int)lspeed) , ((int)inTicks));
        mrp(left.port, ((int)rspeed) , ((int)outTicks));
        bmd_both();
        return 0;
	}
	if (d != 1 && d != -1)
	{
		return -1;
	}
}

int cbc::drive_spin(int speed , float theta , int d)
// if direction == 1 then left wheel is inner
// if direction == -1 then right wheel is inner
// if direction != -1 && direction != 1 then error
{
	float ldistance;
	float rdistance;
	float lticks;
	float rticks;
	float lspeed;
	float rspeed;
	if (d == 1) // CCW
	{
		 ldistance = -1.0 * left.radius_to_middle * theta * DEG_TO_RAD;
		 rdistance = right.radius_to_middle * theta * DEG_TO_RAD;
		 lticks = (ldistance * left.ticks) / (left.diameter * PI);
		 rticks = (rdistance * right.ticks) / (right.diameter * PI);
		 lspeed = (float)speed * left.radius_to_middle / (left.radius_to_middle + left.radius_to_middle);
		 rspeed = (float)speed * right.radius_to_middle / (right.radius_to_middle + right.radius_to_middle);
		 mrp(left.port , (int)lspeed , (int)lticks);
		 mrp(right.port , (int)rspeed , (int)rticks);
		 bmd(left.port);
		 bmd(right.port);
		 return 0;
	}
	if (d == -1) // CW
	{
		 ldistance = left.radius_to_middle * theta * DEG_TO_RAD;
		 rdistance = -1.0 * right.radius_to_middle * theta * DEG_TO_RAD;
		 lticks = (ldistance * left.ticks) / (left.diameter * PI);
		 rticks = (rdistance * right.ticks) / (right.diameter * PI);
		 lspeed = (float)speed * left.radius_to_middle / (left.radius_to_middle + left.radius_to_middle);
		 rspeed = (float)speed * right.radius_to_middle / (right.radius_to_middle + right.radius_to_middle);
		 mrp(left.port , (int)lspeed , (int)lticks);
		 mrp(right.port , (int)rspeed , (int)rticks);
		 bmd(left.port);
		 bmd(right.port);
		 return 0;
	}
	if (d != 1 && d != -1)
	{
		return -1;
	}
}

int cbc::motor_spin_for(int n , float s , float t)
{
	float ticks = (s * t);
	mrp(gen[n].port , (int)s , (int)ticks);
	return 0;
}

int cbc::motor_spin_ticks(int n , int s , int t)
{
	mrp(gen[n].port , s , t);
	return 0;
}

int cbc::move_servo_to(int n , int f)
{
	int c_pos = get_servo_position(servo[n].port);
	if (c_pos < f)
	{
		while (c_pos < f)
		{
			set_servo_position(servo[n].port , c_pos);
			c_pos += 1;
			msleep(10);
		}
		return 0;
	}
	if (c_pos > f)
	{
		while (c_pos > f)
		{
			set_servo_position(servo[n].port , c_pos);
			c_pos -= 1;
			msleep(10);
		}
		return 0;
	}
	if (c_pos == f)
	{
		return 0;
	}
}

int cbc::double_servo_move(int n , int m , int f)
{
	int c_posn = get_servo_position(servo[n].port);
	int c_posm = get_servo_position(servo[m].port);
	if (c_posn != c_posm)
	{
		c_posn = c_posm;
		set_servo_position(servo[n].port , c_posn);
		set_servo_position(servo[m].port , c_posm);
		c_posn = get_servo_position(servo[n].port);
		c_posm = get_servo_position(servo[m].port);
	}
	if (c_posn < f && c_posm < f)
	{
		while (c_posn < f || c_posm < f)
		{
			c_posn += 1;
			c_posm += 1;
			set_servo_position(servo[n].port , c_posn);
			set_servo_position(servo[m].port , c_posm);
			if (c_posn >= f || c_posm >= f)
			{
				break;
			}
		}
	}
	if (c_posn > f && c_posm > f)
	{
		while (c_posn > f || c_posm > f)
		{
			c_posn -= 1;
			c_posm -= 1;
			set_servo_position(servo[n].port , c_posn);
			set_servo_position(servo[m].port , c_posm);
			if (c_posn <= f || c_posm <= f)
			{
				break;
			}
		}
	}
	if (c_posn == f || c_posm == f)
    {
    	return 0;

    }
}

int cbc::average(int port , int samples)
{
	int nums[samples];
	int sum = 0; 
	float average;
	for (int n = 0; n < samples; n++)
	{
		nums[n] = analog10(port);
		sum += nums[n];
	}
	average = ((float)sum / (float)samples);
	return ((int)average);
}

__inline int cbc::ramp_up(float speed , float distance) // this function makes the speed of the CBC go from a low speed to a top speed and back down
{
	// the way that this works is that it calculates the total time needed. Then it divides the time into five segments. The first segment is for
	// the low speed. The middle three segments are for going at top speed. The last segment is for slowing down. 
	float ticks = mm_to_ticks(1000 * distance);
	float time_f = (ticks / speed);
	float time_s = 0;
	float seg1 = (time_f / 5);
	float acc = ((5 * speed) / time_f);
	float v_subn = 0; // the velocity at during each segment
	float ttm = 0; // ticks to move
	while (time_s < seg1) // ramp up
	{
		v_subn = v_subn + (acc * time_s);
		ttm = (((v_subn * (time_f / 25)) * left.ticks) / (right.diameter * PI));
		time_s += (time_f / 25);
		mrp(left.port , (int)v_subn , (int)ttm);
		mrp(right.port , (int)v_subn , (int)ttm);
		bmd_both();
	}
	ttm = ((3 * speed * time_f * left.ticks) / (5 * right.diameter * PI));  
	mrp(left.port , (int)speed , (int)ttm); // top speed
	mrp(right.port , (int)speed , (int)ttm);
	time_s += ((3 * time_s) / 5);
	bmd_both();
	while (time_s < time_f) // ramp down
	{
		v_subn = v_subn + ((-1.0 * acc) * time_s);
		ttm = (((v_subn * (time_f / 25)) * left.ticks) / (right.diameter * PI));
		time_s += (time_f / 25);
		mrp(left.port , (int)v_subn , (int)ttm);
		mrp(right.port , (int)v_subn , (int)ttm);
		bmd_both();
	}
	return 0;
}

int cbc::bmd_both()
{
	bmd(left.port);
	bmd(right.port);
	return 0;
}

int cbc::ET_drive(int dir , int dis , int theta) // direction to turn  , distance to move back (mm) , angle to turn
{

	// Top hat sensor = highest values when closest
	// Top hat sensor = lowest values when most far
	// max speed = 750 t/s
	int mspeed = 750;
	int f = ET.port;
	int ports[8] = {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
	ports[f] = 1;
	int too_close = EC - TOL; // high
	int far_away = EF + TOL; // low
	set_each_analog_state(ports[0] , ports[1] , ports[2] , ports[3] , ports[4] , ports[5] , ports[6] , ports[7]);
	int s_dist = analog10(f);
	while (s_dist < too_close)
	{
		mav(left.port , ET_SPEED(s_dist));
		mav(right.port , ET_SPEED(s_dist));
		s_dist = analog10(f);
		if (s_dist >= too_close)
			break;
	}
	drive_straight(-250 , dis);
	drive_spin(250 , float(theta) , dir);
	return 0;
}

void cbc::set_lines(int l , int r) // the left and right sensors indicies
{
	s_top_hat[l].value = analog10(s_top_hat[l].port);
	s_top_hat[r].value = analog10(s_top_hat[r].port);

}

int cbc::line_follow(int base_speed , int l , int r) // base speed , left , right (indicies (of small top hats))
{
	int left_speed = base_speed;
	int right_speed = base_speed;
	set_lines(l , r);
	if (s_top_hat[l].value <= s_top_hat[l].low_thresh || s_top_hat[r].value >= s_top_hat[r].high_thresh)
	{
		left_speed += 5;
		right_speed -= 5;
	}
	if (s_top_hat[l].value >= s_top_hat[l].high_thresh || s_top_hat[r].value <= s_top_hat[r].low_thresh)
	{
		left_speed -= 5;
		right_speed +=5;
	}
	mav(s_top_hat[l].port , left_speed);
	mav(s_top_hat[r].port , right_speed);
	msleep(10); 
}

int cbc::num_of_in_ch(int chan)
{
	
}

#endif // CBCclass_H_INCLUDED

// theoretical furthest distance 80cm = 0
// theoretical closest distance 5cm = 1023
// (0 , 80)
// (1023 , 5)
// (80 , 0)
// ()
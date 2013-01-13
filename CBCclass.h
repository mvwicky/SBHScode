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
	} gen[10];
	struct servo {
		int port;
		int ticks;
		int min;
		int max;
	}servo[10];
	struct s_analog {
		int port;
		int value;
	}top_hat[2] , ET[2] ;
	struct s_digital {
		int port;
		int value;
	};
	public:
		void build_left_motor(int , float , float , float);
		void build_right_motor(int , float , float , float);
		void build_generic_motor(int , int , int , float);
		void build_servo(int , int , int , int , int);
		int build_top_hat(int , int);
		int build_ET(int , int);
		float mm_to_ticks(float);
		float ticks_to_mm(float);
		int drive_straight(int , float);
		int drive_arc(int , float , float , float);
		int drive_spin(int , float , int); // DO THIS
		int motor_spin_for(int , float , float , int); // 
		int motor_spin_ticks(int , int , int , int);
		int move_servo_to(int , int);
		int double_servo_move(int , int , int);
		int average(int , int);
		int ramp_up(float , float);
		int bmd_both();
		int ET_drive(int , int , int , int); // index , direction to turn  , distance to move back (mm) , direction to turn 
		int ET_align(int , int , int , int); // index , index , distance from wall , distance to go , side the sensors are on (-1 = L , 1 = R)
										     // only works for one side, assumes that you have the two ET sendors on one side
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

int cbc::build_top_hat(int n , int p)
{
	if (n > 2)
		return -1;
	top_hat[n].port = p;
	return 0;
}

int cbc::build_ET(int n , int p)
{
	if (n > 2)
		return -1;
	ET[n].port = p;
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
		souter = ( (r - right.radius_to_middle) * (a * DEG_TO_RAD) );
		sprop = souter/sinner;
		inTicks = ( (right.ticks * sinner) / (PI * right.diameter) );
		outTicks = ( (left.ticks * sinner) / (PI * left.diameter) );
		lspeed = (float)s * sprop * souter / S;
        rspeed = (float)s * sprop * sinner / S;
        mrp(right.port , ((int)rspeed) , ((int)inTicks));
        mrp(left.port, ((int)lspeed) , ((int)outTicks));
        bmd(right.port);
        bmd(left.port);
        return 0;
	}
	if (d == -1) //CW
	{
		S = r * (a * DEG_TO_RAD);
		sinner = ( (r - right.radius_to_middle) * (a * DEG_TO_RAD) );
		souter = ( (r - left.radius_to_middle) * (a * DEG_TO_RAD) );
		sprop = souter/sinner;
		inTicks = ( (left.ticks * sinner) / (PI * left.diameter) );
		outTicks = ( (right.ticks * sinner) / (PI * right.diameter) );
		lspeed = (float)s * sprop * sinner / S;
        rspeed = (float)s * sprop * souter / S;
        mrp(right.port , ((int)lspeed) , ((int)inTicks));
        mrp(left.port, ((int)rspeed) , ((int)outTicks));
        bmd(left.port);
        bmd(right.port);
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

int cbc::motor_spin_for(int n , float s , float t , int c)
{
	float ticks = (s * t);
	mrp(gen[n].port , (int)s , (int)ticks);
	if (c == 0)
	{
		bmd(gen[n].port);
	}
	else;
	return 0;
}

int cbc::motor_spin_ticks(int n , int s , int t , int c)
{
	mrp(gen[n].port , s , t);
	if (c == 0)
	{
		bmd(gen[n].port);
	}
	else;
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
	for (int n = 0; n < 4; n++)
	{
		nums[n] = analog10(port);
		sum += nums[n];
	}
	average = ((float)sum / (float)samples);
	return ((int)average);
}

__inline int cbc::ramp_up(float speed , float distance)
{
	float ticks = mm_to_ticks(1000 * distance);
	float time_f = (ticks / speed);
	float time_s = 0;
	float seg1 = (time_f / 5);
	float acc = ((5 * speed) / time_f);
	float v_subn = 0; 
	float ttm = 0;
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

int cbc::ET_drive(int n , int dir , int dis , int theta) // direction to turn  , distance to move back (mm) , angle to turn
{

	// Top hat sensor = highest values when closest
	// Top hat sensor = lowest values when most far
	// max speed = 750 t/s
	int mspeed = 750;
	int f = ET[n].port;
	int ports[8] = {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
	ports[n] = 1;
	int too_close = EC - TOL; // high
	int far_away = EF + TOL; // low
	set_each_analog_state(ports[0] , ports[1] , ports[2] , ports[3] , ports[4] , ports[5] , ports[6] , ports[7]);
	int s_dist = analog10(n);
	while (s_dist < too_close)
	{
		mav(left.port , ET_SPEED(s_dist));
		mav(right.port , ET_SPEED(s_dist));
		s_dist = analog10(n);
		if (s_dist >= too_close)
			break;
	}
	drive_straight(-250 , dis);
	drive_spin(250 , float(theta) , dir);
	return 0;
}

int cbc::ET_align(int n , int m ,  int dist , int s)// n = front , m = back , dist = dist from wall , d_t_g = distance to move , s = side the sensors are on
{
	int mspeed = 750;
	int clspeed = mspeed;
	int crspeed = mspeed;
	int n_p = ET[n].port;
	int m_p = ET[n].port;
	int ports[8] = {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};
	ports[n_p] = 1;
	ports[m_p] = 1;
	set_each_analog_state(ports[0] , ports[1] , ports[2] , ports[3] , ports[4] , ports[5] , ports[6] , ports[7]);
	int f_sens = analog10(n_p);
	int b_sens = analog10(m_p);
	float f_dist =  ET_DIST(f_sens);
	float b_dist = ET_DIST(b_sens);
	float avg_dist = ((f_dist + b_dist) / 2);
	float too_close = dist - (dist / 5);
	float far_away = dist + (dist / 5);
	if (avg_dist == dist)
		return 0;
	if (s == 1)
	{
		while (1)
		{
			f_sens = analog10(n_p);
			b_sens = analog10(m_p);
			f_dist = ET_DIST(f_sens);
			b_dist = ET_DIST(b_sens);
			avg_dist = ((f_dist + b_dist) / 2);
			mav(left.port , clspeed);
			mav(right.port , crspeed);
			if (f_dist == b_dist);
			if (f_dist > b_dist) // make right go faster , left slower
			{
				clspeed++;
				crspeed--;
				mav(left.port , clspeed);
				mav(right.port , crspeed);
			}
			if (b_dist > f_dist) // make left go faster , left slower
			{
				clspeed--;
				crspeed++;
				mav(left.port , clspeed);
				mav(right.port , crspeed);
			}
			if (avg_dist > dist)
			{
				clspeed+=A_DEL;
				clspeed-=A_DEL;
				mav(left.port , clspeed);
				mav(right.port , crspeed);
			}
			if (dist > avg_dist)
			{
				clspeed-=A_DEL;
				crspeed+=A_DEL;
				mav(left.port , clspeed);
				mav(right.port , crspeed);
			}
		}
	}
	if (s == -1)
	{
		while (1)
		{
			f_sens = analog10(n_p);
			b_sens = analog10(m_p);
			f_dist = ET_DIST(f_sens);
			b_dist = ET_DIST(b_sens);
			mav(left.port , clspeed);
			mav(right.port , crspeed);
			if (f_dist == b_dist);
			if (f_dist > b_dist)
			{
				clspeed--;
				crspeed++;
				mav(left.port , clspeed);
				mav(right.port , crspeed);
			}
			if (b_dist > f_dist)
			{
				clspeed++;
				crspeed--;
				mav(left.port , clspeed);
				mav(right.port , crspeed);
			}
			if (avg_dist > dist)
			{
				clspeed-=A_DEL;
				crspeed+=A_DEL;
				mav(left.port , clspeed);
				mav(right.port , crspeed);
			}
			if (dist > avg_dist)
			{
				clspeed+=A_DEL;
				crspeed-=A_DEL;
				mav(left.port , clspeed);
				mav(right.port , crspeed);
			}
		}
	}
	return 0;
}

#endif // CBCclass_H_INCLUDED

// theoretical furthest distance 80cm = 0
// theoretical closest distance 5cm = 1023
// (0 , 80)
// (1023 , 5)
// (80 , 0)
// ()
#ifndef CBCclass_H_INCLUDED
#define CBCclass_H_INCLUDED
#define PI 3.14159
#define DEG_TO_RAD (PI / 180.0)
/*
class cbc: contains the structures for different motor, servos , and (TODO) sensors
	struct drive_motors: contains information on the two main drive motors: left and right
	struct gen_motor: contains information on motors not associated with driving
	struct servo: contains information on servos
methods: // unfinished
	build_left/right_motor // p = port
					 	   // r = 1/2 of the distance from one motor to the other (meters)
					   	   // ticks per rev (1000)
					       // diameter of the wheel (meters)
	build_generic_motor // n = index of motor (10 are initialized)
						// p = port 
						// t = number of ticks
						// d = diameter of the wheel (meters)
	build_servo // n = index of servo (10 are initialized)
				// p = port 
				// t = number of ticks
				// mi = minimum ticks
				// ma = maximum ticks
	drive_straight // s = speed (ticks / second)
				   // d = distance in meters
	drive_arc // s = speed (ticks / second)
			  // r = radius (meters)
			  // a = angle (degrees)
			  // d = direction (1 = CCW , -1 = CW , neither = error)
	drive_spin // s = speed (ticks / second) 
			   // t = theta (degrees)
			   // d = direction (1 = CCW , -1 = CW , neither = error)
	motor_spin_for // n = index of motor
				   // s = speed (ticks / second)
				   // t = time (seconds)
				   // c = concurrency (if 1 then other things can run with the motor of 0 then no)
	motor_spin_ticks // n = index of motor
					 // s = speed
					 // t = ticks (ticks)
					 // c = concurrency (if 1 then other things can run with the motor of 0 then no)
	move_servo_to // n = index of servo
				  // f = final position
	double_servo_move // n = index of servo
					  // m = index of servo
					  // f = final position

*/
/*
USING SENSORS

*/

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
	} light[5] , l_top_hat[5] , s_top_hat[5] , ET[5];
	struct s_digital {
		int port;
		int value;
	} slot[5] , l_touch[5] , s_touch[5] , lever[5];
	public:
		void build_left_motor(int , float , float , float);
		void build_right_motor(int , float , float , float);
		void build_generic_motor(int , int , int , float);
		void build_servo(int , int , int , int , int);
		void build_light(int, int); // analog
		void build_l_top(int, int); // analog
		void build_s_top(int, int); // analog
		void build_ET(int, int); // analog
		void build_slot(int ,int); // digital
		void build_l_touch(int ,int); // digital
		void build_s_touch(int ,int); // digital
		void build_lever(int ,int); // digital
		int drive_straight(int , float);
		int drive_arc(int , float , float , float);
		int drive_spin(int , float , int); // DO THIS
		int motor_spin_for(int , float , float , int); // 
		int motor_spin_ticks(int , int , int , int);
		int move_servo_to(int , int);
		int double_servo_move(int , int , int);
		int average(int , int);
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

void cbc::build_light(int n , int p)
{
	light[n].port = p;
}

void cbc::build_l_top(int n , int p)
{
	l_top_hat[n].port = p;
}

void cbc::build_s_top(int n , int p)
{
	s_top_hat[n].port = p;
}

void cbc::build_ET(int n , int p)
{
	ET[n].port = p;
}

void cbc::build_slot(int n , int p)
{
	slot[n].port = p;
}

void cbc::build_l_touch(int n , int p)
{
	l_touch[n].port = p;
}

void cbc::build_s_touch(int n , int p)
{
	s_touch[n].port = p;
}

void cbc::build_lever(int n , int p)
{
	lever[n].port = p;
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

int cbc::drive_spin(int s , float t , int d)
// if direction == 1 then left wheel is inner
// if direction == -1 then right wheel is inner
// if direction != -1 && direction != 1 then error
{
	float lticks;
	float rticks;
	if (d == 1) // CCW
	{
	
	}
	if (d == -1) // CW
	{

	}
	if (d != 1 && d != -1)
	{

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
	}
	for (int m = 0; m < 4; m++)
	{
		sum += nums[m];
	}
	average = (float)sum / 5.0;
	return ((int)average);
}

#endif // CBCclass_H_INCLUDED
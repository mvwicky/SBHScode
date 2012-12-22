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
	drive_straight 	// not done
					// s = speed (ticks / second)
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
	average // port = port (analog sensor)
			// samples = amount of samples taken (more samples = more time)
*/
/*
USING SENSORS
// TODO

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
	};
	struct s_digital {
		int port;
		int value;
	};
	public:
		void build_left_motor(int , float , float , float);
		void build_right_motor(int , float , float , float);
		void build_generic_motor(int , int , int , float);
		void build_servo(int , int , int , int , int);
		int drive_straight(int , float);
		int drive_arc(int , float , float , float);
		int drive_spin(int , float , int); // DO THIS
		int motor_spin_for(int , float , float , int); // 
		int motor_spin_ticks(int , int , int , int);
		int move_servo_to(int , int);
		int double_servo_move(int , int , int);
		int average(int , int);
		int ramp_up(float , float);
		float mm_to_ticks(float);
		float ticks_to_mm(float);
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
	average = ((float)sum / (float)samples);
	return ((int)average);
}

__inline int cbc::ramp_up(float speed , float distance)
{
	float ticks = mm_to_ticks(distance);
	float time_f = (ticks / speed);
	float time_s = 0;
	float seg1 = (time_f / 5);
	float acc = ((5 * speed) / time_f);
	float v_subn = 0; 
	while (time_s < seg1) // ramp up
	{
		v_subn = v_subn + (acc * time_s);
		time_s += (time_f / 25);
		mav(left.port , (int)v_subn);
		mav(right.port , (int)v_subn);
		sleep((time_f / 25));
	}
	mav(left.port , speed); // top speed
	mav(right.port , speed);
	time_s += ((3 * time_s) / 5);
	while (time_s < time_f) // ramp down
	{
		v_subn = v_subn + ((-1.0 * acc) * time_s);
		time_s += (time_f / 25);
		mav(left.port , (int)v_subn);
		mav(right.port , (int)v_subn);
		sleep((time_f / 25));
	}
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

#endif // CBCclass_H_INCLUDED
#define PI 3.14159
#define DEG_TO_RAD (PI / 180.0)

class cbc {
	struct drive_motors {
		int port; // port that the motor is in
		float radius_to_middle; // 1/2 of the distance from one motor to the other m
		float ticks; // ticks per revolution (1000)
		float diameter; // diameter of the wheel in meters
	} left , right;
	struct gen_motor {
		int port; // port that the motor is in
		int ticks; // ticks per revolution (1000)
		float diameter; // diameter of the wheel in m
	} gen[10];
	struct servo {
		int port;
		int ticks;
		int min;
		int max;
	}servo[10];
	public:
		void build_left_motor(int , float , float ,float);
		void build_right_motor(int , float , float , float);
		void build_generic_motor(int , int , int , float);
		void build_servo(int , int , int , int , int);
		int drive_straight(int , float);
		int drive_arc(int , float , float , float);
		int drive_spin(int , float , int);
		int motor_spin_for(int , float , float);
		int move_servo_to(int , int);
		int double_servo_move(int , int , int);
}calvin;

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
// pretty straight forward
// s = speed = ticks/second , d = distance = 
	float lticks = ((d * left.ticks) / (left.diameter * PI));
	float rticks = ((d * right.ticks) / (right.diameter * PI));
	mrp(left.port , s , ((int)lticks) );
	mrp(right.port , s , ((int)rticks) );
	bmd(left.port);
	bmd(right.port);
	return 1;
}

int cbc::drive_arc(int s , float r , float a , float d)
{
// if direction == 1 then left wheel is inner
// if direction == -1 then right wheel is inner
// if direction != -1 && direction != 1 then error	
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
{


}
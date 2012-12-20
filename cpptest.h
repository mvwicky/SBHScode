#define PI 3.14159
#define DEG_TO_RAD (PI / 180.0)

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
}lego;

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

int drive_arc(int s , float r , float a , float d)
{
	
}
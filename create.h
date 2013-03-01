//CREATE FUNCTIONS
#ifndef create_H_INCLUDED

#define create_H_INCLUDED
#define DIST100(x) ((.952847 * x) - 26.9)
#define DIST200(x) ((.971316 * x) - 5.06716)
#define SDIST set_create_distance(LAG)
#define GDIST get_create_distance(LAG)
#define LAG .001
#define LFCLIFF get_create_lfcliff_amt(LAG)
#define RFCLIFF get_create_rfcliff_amt(LAG)
#define WHITE 275
#define BLACK 450
#define CD(x) create_drive_direct(x , x)

bool left_cliff_black;
bool right_cliff_black;

void move_while(int speed , int togo)
{
	SDIST;
	int dist = GDIST;
	while (dist < togo)
	{
		create_drive_direct(speed , speed);
		dist = GDIST;
		msleep(1);
	}
}

void set_cliff()
{
	if (LFCLIFF <= WHITE)
	{
		left_cliff_black = false;
	}
	if (LFCLIFF >= BLACK)
	{
		left_cliff_black = true;
	}
	if (RFCLIFF <= WHITE)
	{
		right_cliff_black = false;
	}
	if (RFCLIFF >= BLACK)
	{
		right_cliff_black = true;
	}
}

void line_track()
{
	int rspeed = 500;
	int lspeed = 500;
	int dist_g = GDIST;
	set_cliff();
	if (left_cliff_black == false || right_cliff_black == true)
	{
		rspeed += 5;
		lspeed -= 5;
	}
	if (left_cliff_black == true || right_cliff_black == false)
	{
		rspeed -= 5;
		lspeed += 5;
	}
	create_drive_direct(rspeed , lspeed);
	dist_g = GDIST;
	msleep(10);
}

__inline int create_ramp_up(float speed , float distance)
{
	SDIST;
	float dist_seg = (distance / 5); // distance to go per segment
	float t_time = (distance / speed); // total time
	float seg_time = t_time / 5;
	float time_s = seconds(); // starting (current) time
	float s_subn = 0;
	float speed_to_add = (speed / (20 * seg_time));
 	while (s_subn < speed)
 	{
 		s_subn += speed_to_add;
 		CD((int)s_subn);
 		msleep(10);
 		std::cout << s_subn << std::endl;
 	}
 	float t = (1000 * (3 * seg_time));
 	CD(speed);
 	msleep(t);
 	std::cout << s_subn << std::endl;
 	while (s_subn > 0)
 	{
 		s_subn -= speed_to_add;
 		CD((int)s_subn);
 		msleep(10);
 		std::cout << s_subn << std::endl;
 	}
 	return 0;

}



#endif // create_H_INCLUDED

// v = d/t
// d = vt
//d/v = t
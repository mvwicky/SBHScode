//CREATE FUNCTIONS
#ifndef create_H_INCLUDED

#define create_H_INCLUDED
#define DIST100(x) ((.952847 * x) - 26.9)
#define DIST200(x) ((.971316 * x) - 5.06716)
#define SDIST set_create_distance
#define GDIST get_create_distance(LAG)
#define LAG .001
#define LFCLIFF get_create_lfcliff_amt(LAG)
#define RFCLIFF get_create_rfcliff_amt(LAG)
#define WHITE 275
#define BLACK 450

bool left_cliff_black;
bool right_cliff_black;

void move_while(int speed , int togo)
{
	SDIST(LAG);
	int dist = GDIST;
	while (dist < togo)
	{
		create_drive_direct(speed , speed);
		dist = GDIST;
		msleep(.001);
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

void line_track(int dist)
{
	int rspeed = 500;
	int lspeed = 500;
	int dist_g = GDIST;
	while (dist_g <= dist)
	{
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
}

void ramp_up(float speed , float distance)
{

}

#endif // create_H_INCLUDED
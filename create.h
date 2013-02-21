//CREATE FUNCTIONS
#ifndef create_H_INCLUDED

#define create_H_INCLUDED
#define DIST100(x) ((.952847 * x) - 26.9)
#define DIST200(x) ((.971316 * x) - 5.06716)
#define SDIST set_create_distance
#define GDIST get_create_distance(LAG)
#define LAG .001
#define LFWALL get_create_lfwall_amt(LAG)
#define RFWALL get_create_rfwall_amt(LAG)
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

void set_cliffs()
{
	
}

void ramp_up(float speed , float distance)
{

}

#endif // create_H_INCLUDED
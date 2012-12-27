//CREATE FUNCTIONS
#ifndef create_H_INCLUDED
#define create_H_INCLUDED

#define DIST100(x) ((.952847 * x) - 26.9)
#define DIST200(x) ((.971316 * x) - 5.06716)
// #define DIST300
// #define DIST500
// #define DIST600
// #define DIST700
// #define DIST800
// #define DIST900

void move100(float dist)
{
	float movedist = DIST100(dist);
	float movetime = (movedist / 100);
	create_drive_direct(100 , 100);
	sleep(movetime);
	create_stop();

}

void move200(int dist)
{
	float movedist = DIST200(dist);
	float movetime = (movedist / 200);
	create_drive_direct(200 , 200);
	sleep(movetime);
	create_stop();

}
/*
void move300(int dist)
{
	float movedist = DIST300(dist);
	float movetime = (movedist / 300);
	create_drive_direct(300 , 300);
	sleep(movetime);
	create_stop();
}

void move400(int dist)
{
	float movedist = DIST400(dist);
	float movetime = (movedist / 400);
	create_drive_direct(400 , 400);
	sleep(movetime);
	create_stop();
}

void move500(int dist)
{
	float movedist = DIST500(dist);
	float movetime = (movedist / 500);
	create_drive_direct(500 , 500);
	sleep(movetime);
	create_stop();
}

void move600(int dist)
{
	float movedist = DIST600(dist);
	float movetime = (movedist / 600);
	create_drive_direct(600 , 600);
	sleep(movetime);
	create_stop();
}

void move700(int dist)
{
	float movedist = DIST700(dist);
	float movetime = (movedist / 700);
	create_drive_direct(700 , 700);
	sleep(movetime);
	create_stop();
}

void move800(int dist)
{
	float movedist = DIST800(dist);
	float movetime = (movedist / 800);
	create_drive_direct(800 , 800);
	sleep(movetime);
	create_stop();
}

void move900(int dist)
{
	float movedist = DIST900(dist);
	float movetime = (movedist / 900);
	create_drive_direct(900 , 900);
	sleep(movetime);
	create_stop();
}

void ramp_up(float speed , float distance)

*/
#endif // create_H_INCLUDED
// Created on Wed Feb 13 2013
// 1 if black, 0 if white


#include <iostream>
#define GETL get_create_lfcliff_amt(.01)
#define GETR get_create_rfcliff_amt(.01)
#define BLACK 275
#define WHITE 450

using namespace std;

bool lc_bl = false;
bool rc_bl = false;

void set_cliffs();

int main() 
{
	int rspeed = 500;
	int lspeed = 500;
	create_connect();
	while (1 == 1)
	{
		set_cliffs();
		if (lc_bl == true)
		{
			rspeed += 5;
			lspeed -= 5;
		}
		if (lc_bl == false)
		{
			rspeed -= 5;
			lspeed += 5;
		}
		if (rc_bl == true)
		{
			rspeed -= 5;
			lspeed += 5;
		}
		if (rc_bl == false)
		{
			rspeed += 5;
			lspeed -= 5;
		}
		create_drive_direct(rspeed , lspeed);
		cout << "Left Sensor = " << GETL << "Right Sensor = " << GETR << endl;
		msleep(1);
	}
	create_disconnect();
	return 0;
}

void set_cliffs()
{
	int diffr;
	int diffl;
	if (GETR <= BLACK)
	{
		rc_bl = true;
	}
	if (GETR >= WHITE)
	{
		rc_bl = false;
	}
	if (GETL <= BLACK)
	{
		lc_bl = true;
	}
	if (GETL >= WHITE)
	{
		lc_bl = false;
	}
}

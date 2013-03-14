// Created on Wed March 13 2013
// left white  = 1092 , left black  = 236
// right white = 1692 , right white = 300


#define LAG .001
#define T 600
#define LFCLIFF get_create_lfcliff_amt(LAG)
#define RFCLIFF get_create_rfcliff_amt(LAG)
#define LCLIFF get_create_lcliff_amt(LAG)
#define RCLIFF get_create_rcliff_amt(LAG)

void line_follow();

int main()
{
	create_connect();
	while (side_button() == 0)
	{
		line_follow();
		msleep(10);
	}
	create_disconnect();
	return 0;
}

void line_follow()
{
	int ldrive = -100;
	int hdrive = 250;
	if (LFCLIFF >= T && RFCLIFF <= T)
	{
		create_drive_direct(hdrive , hdrive);
	}
	else if (LFCLIFF <= T && RFCLIFF >= T)
	{
		create_drive_direct(ldrive , hdrive);
	}
	else if  (LFCLIFF >= T && RFCLIFF >= T)
	{
		while (LCLIFF <= T || RCLIFF <= T)
		{
			create_drive_direct(hdrive , hdrive);
			msleep(10);
			if (LCLIFF > T)
			{
				
			}
			if (RCLIFF > T)
			{
				
			}
		}
	}
	else if (LFCLIFF <= T && RFCLIFF <= T)
	{
		create_drive_direct(ldrive , hdrive);
	}
}

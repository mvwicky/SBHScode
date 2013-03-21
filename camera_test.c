// Created on Wed March 20 2013
// 1 = left
// 3 = right
int main()
{
	int lspeed = 10;
	int hspeed = 80;
	camera_open(LOW_RES);
	while (side_button() == 0)
	{
		camera_update();
		if (get_object_center(0 , 0).x < 60)
		{
			motor(1 , hspeed);
			motor(3 , lspeed);
			printf("LEFT\n");
		}
		if (get_object_center(0 , 0).x > 100)
		{
			motor(1 , lspeed);
			motor(3 , hspeed);
			printf("RIGHT\n");
		}
		if (get_object_center(0 , 0).x >= 60 && get_object_center(0 , 0).x <= 100)
		{
			motor(1 , hspeed);
			motor(3 , hspeed);
			printf("CENTER\n");
		}
	}
	return 0;
}

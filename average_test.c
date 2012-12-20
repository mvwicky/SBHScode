float average(int port , int samples)
{
	int nums[samples];
	int sum = 0; 
	float average;
	for (int n = 0; , n < 4 , n+=1)
	{
		nums[n] = analog10(port);
	}
	for (int m = 0;, m < 4 , m+=1
	{
		sum += nums[m];
	}
	average = (float)sum / 5.0;
	return average;
}
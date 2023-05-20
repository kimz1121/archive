#include<stdio.h>
#include<math.h>

int find_radius(int x, int y)
{
	printf("(%d,%d)", x, y);
	printf("%f ", atan2(y,x));
}

int main()
{
	int i = 0;
	int j = 0;
	for(i = -5; i < 6;i = i+ 1)
	{
		for(j = -5;j < 6;j = j + 1)
		{
			find_radius(j, -i);
		}
		printf("\n\n");
	}
}

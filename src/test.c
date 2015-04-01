#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "timeDifference.h"

int main(int argc, char** argv)
{
	double delta;
	int day;

	printf("SunDeclination,\tDay\n");
	for( day = 0; day < 365; day++)
	{
		sunDeclination(&delta, day);
		delta = (delta*180)/(4*atan(1));
		printf("%f\t%d\n", delta, day);
	}

	return 0;
} 

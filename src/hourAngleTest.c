#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "localHourAngle.h"

int main()
{
	int i = 0;
	double t =0.0;
	
	for( i = 0; i < 86400; i += 3600)
	{
		localHourAngle(&t, (double)i, 0.0, 0.0, 0);
		printf("%d   %f\n", i, (t*180)/(4*atan(1)));
	}
}

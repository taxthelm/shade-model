#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "solarAltitude.h"
#include "localHourAngle.h"
#include "sunDeclination.h"
#include "timeDifference.h"

int main()
{
	double lat = 40.100178;
	double longi = -105.99758;
	double stdMer = -106.0;
	int day = 172;
	double time = 43200;	
	double sunDec = 0.0;
	double timeDiff = 0.0;
	double localHrAng = 0.0;
	double solarAlt = 0.0;

	sunDeclination(&sunDec, day);
	timeDifference(&timeDiff, stdMer, longi);
	localHourAngle(&localHrAng, time, timeDiff, 0.0, 0);
	
	printf("vals are: %f, %f\n", sunDec, localHrAng); 
	
	solarAltitude(&solarAlt, sunDec, lat*(4*atan(1)/180.0), localHrAng);

	printf("solar alt is: %f\n", solarAlt*180/(4*atan(1)));

	return 0;
}

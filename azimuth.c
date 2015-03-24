#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tilt.h"

void azimuth(double *azimuth, double solarAlt, double latitude, double declination, int time){	

	*azimuth = ((sin(solarAlt) * sin((latitude/180.0)*(4*atan(1)))) - sin(declination))/(cos(solarAlt) * cos((latitude/180.0)*(4*atan(1))));

	*azimuth = acos(*azimuth);

	if (time <= 43200)
		*azimuth = -*azimuth;
}


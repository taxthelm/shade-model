/*
File:	azimuth.c

ToDo:
  rename azimuth argument since C is a flat namespace
  inline comments

ChangeLog
2015.04.04	JDB	Restructure & simple optimizations
2015.04.04  JDB Removed stdio.h and stdlib.h; they are not needed.

*/

#include <math.h>
#include "tilt.h"

void azimuth( 
		double *azimuth, 
		double solarAlt, 
		double latitude, 
		double declination, 
		int time
		) {	

	double lat_tmp = M_PI*latitude/180.0;
	*azimuth = (sin(solarAlt)*sin(lat_tmp) - sin(declination)) / 
		(cos(solarAlt) * cos(lat_tmp));

	*azimuth = acos(*azimuth);

	if (time <= 43200)
		*azimuth = -(*azimuth);
}


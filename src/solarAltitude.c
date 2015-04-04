#include <math.h>

#include "solarAltitude.h"

void solarAltitude(
		double *solarAlt,
		double const sunDeclination, 
		double const latitude, 
		double const localHourAngle)
{
	//Equation for solar altitude as found on page 25 of reading (Planetary Motions and the Distribution of Radiation)
	*solarAlt = asin( 
			sin(sunDeclination)*sin(latitude) + 
			cos(sunDeclination)*cos(latitude)*cos(localHourAngle)
			);
		
}

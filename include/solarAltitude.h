/*
File: solarAltitude.h


Original:
Author Troy Axthelm

solarAltitude() This function will calculate the solar altitude as defined
in "Planetary Motions and the Distribution of Radiation" equation 2.4. The
equations can be found on page 25.

*/

#ifndef SOLARALTITUDE_H
#define SOLARALTITUDE_H

//pre:  sunDeclination (delta), latitude (phi) and localHourAngle (tau) are radians
//post: The solar altitude is calculated and stored as solarAltitude
void solarAltitude(
		double *solarAltitude, 
		double const sunDeclination, 
		double const latitude, 
		double const localHourAngle
		);

#endif 

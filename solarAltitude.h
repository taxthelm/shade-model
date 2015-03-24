#ifndef SOLARALTITUDE_H
#define SOLARALTITUDE_H

/*
 * Troy Axthelm
*/

/*
 * This function will calculate the solar altitude as defined in "Planetary Motions and the Distribution of Radiation"
 * equation 2.4 which can be found on page 25.
*/ 

//pre:  sunDeclination (delta), latitude (phi) and localHourAngle (tau) are radians
//post: The solar altitude is calculated and stored as solarAltitude
void solarAltitude(double * solarAltitude, double sunDeclination, double latitude, double localHourAngle);

#endif 

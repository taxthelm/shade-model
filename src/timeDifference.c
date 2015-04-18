/*
File: timeDifference.c

ChangeLog

2015.04.04 JDB	swapped the order of thetaS and thetaL and calculated 1.0/15.0 
				to be a constant. Remember, divisions can be expensive :)

Original: 
//Bill Matonte
//6.27.13
//REU SUMMER 2013 
//timedifference.c
//This function finds the offset for the time of day for the solar cumilative 
//radiation formula.
*/

#include "timeDifference.h"
//The Variables ThetaS and ThetaL with the pointer to the deltaT
//see timedifference.h
//thetaS = standard time meridian longitde of time zone
//thetaL = local longitude

void timeDifference(
		double* deltaT1, 
		double const thetaS,
		double const thetaL
		) {
	
	*deltaT1 = 0.06666666666666666*(thetaL-thetaS);

}


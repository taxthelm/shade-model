//Bill Matonte
//6.27.13
//REU SUMMER 2013 
//timedifference.c
//This function finds the offset for the time of day for the solar cumilative 
//radiation formula. 
#include "timeDifference.h"
#include <stdio.h>
//The Variables ThetaS and ThetaL with the pointer to the deltaT
//see timedifference.h
//thetaS = standard time meridian longitde of time zone
//thetaL = local longitude

void timeDifference(double* deltaT1, double thetaS,double thetaL)
{
	*deltaT1=-( thetaS-thetaL)/15.0;
	return;
}




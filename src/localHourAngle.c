/*
REU HPC Summer 2013 
Program Purpose: Linear algebra Library.
Created by: Noll Roberts
On: 2013.06.20
Modified by:
On:
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "shadem.h"

#include "localHourAngle.h"


/************ FUNCTIONS ***************************/
// see project.h
void localHourAngle(
		double * restrict const tau, 
		double const Ts, 
		double const deltaT1, 
		double const deltaT2, 
		int const timeStep
		) {
	
	if (Ts<43200.0) *tau = ( (Ts/3600) + 12 - deltaT1 + deltaT2 ) * DegToRad(15.0);
	else            *tau = ( (Ts/3600) - 12 - deltaT1 + deltaT2 ) * DegToRad(15.0); // * (4*atan(1)/180);   
	
	//Ts+=timeStep;

 	return;
}

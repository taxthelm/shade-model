/*
File: localHourAngle.h

ChangeLog:

2015.04.04 JDB	Added const keywords


Original:  
  REU HPC Summer 2013 
  Program Purpose: 
  Created by: Noll Roberts
  On: 2013.06.20
  Modified by:
  On:

*/

#ifndef LAL_H 
#define LAL_H 

// Returns tao, the local hour angle. (in degrees) 
void localHourAngle( 
		double * restrict const tau, 
		double const Ts, 
		double const deltaT1, 
		double const deltaT2, 
		int const timeStep
		);

#endif

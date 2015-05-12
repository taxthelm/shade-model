/*
File: timeDifference

ChangeLog
2015.04.04 JDB	added const keywords

Original:
Bill Matonte
6.27.13
REU SUMMER 2013 
timedifference.h

*/

#ifndef TIMEDIFFERENCE_H
#define TIMEDIFFERENCE_H

//pre: takes in pointer to deltaT, and the thetaS and thetaL.
//post: Gives the DeltaT
void timeDifference(
		double* deltaT1, 
		double const thetaS,
		double const thetaL
		); 


#endif // TIMEDIFFERENCE_H


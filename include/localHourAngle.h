/* REU HPC Summer 2013 
 * Program Purpose: 
 * Created by: Noll Roberts
 * On: 2013.06.20
 * Modified by:
 * On:
 * */

#ifndef LAL_H 
#define LAL_H 

/**************** Linear Algebra Functions ************************/

// Returns tao, the local hour angle. (in degrees) 
void localHourAngle( double *tau, double Ts, double deltaT1, double deltaT2, int timeStep);


#endif

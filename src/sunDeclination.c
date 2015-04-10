/*
File: sunDeclination.c

ChangeLog
2015.04.04 JDB Restructured code, used M_PI from POSIX
2015.04.09 JDB Replaced some constants
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "sunDeclination.h"

void sunDeclination(
		double *delta, 
		int const day
		) {

//	*delta = (23.45*M_PI/180.0)*cos((2*M_PI/365.0)*(172-day));
	*delta = 0.40927970959267024*cos(0.017214206321039961*(172-day));


}

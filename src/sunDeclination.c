/*
File: sunDeclination.c

ChangeLog
2015.04.04 JDB Restructured code, used M_PI from POSIX
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

	*delta = (23.45*M_PI/180.0)*cos((2*M_PI/365.0)*(172-day));

}

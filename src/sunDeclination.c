/*
File: sunDeclination.c

ChangeLog
2015.04.04 JDB Restructured code, used M_PI from POSIX
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "shadem.h"

#include "sunDeclination.h"

void sunDeclination(
		double * const delta, 
		int const day
		) {

	//*delta = (23.45*M_PI/180.0)*cos((2*M_PI/365.0)*(172-day));
	*delta = DegToRad(23.45)*cos( (2.0*M_PI/365.0) * (172-day));

}

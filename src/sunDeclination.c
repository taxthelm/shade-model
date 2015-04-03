#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "sunDeclination.h"



void sunDeclination(double *delta, int day){
	*delta = ((23.45*4*atan(1))/180)*cos(((2*4*atan(1))/365)*(172-day));

	return;

}

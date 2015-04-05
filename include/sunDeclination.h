/*
File: sunDeclination.c

ChangeLog
2015.04.04 JDB	Added const keywords


Contents:

sunDeclination()	Calculates the angle of the sun above or below
					the equator on a given day of the year.

*/


#ifndef MAT_H
#define MAT_H

void sunDeclination(
		double *delta, 
		int const D
		);

#endif

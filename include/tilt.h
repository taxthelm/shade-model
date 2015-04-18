/*
File: tilt.h

ChangeLog
2015.04.04 JDB	Added const keywords

*/

#include "landStruct.h"
#ifndef TILT_H
#define TILT_H

void tilt(
		LandData * restrict mainPlot,	// plot of land tilt angles are calculated for
		LandData const * const restrict rightNeighbor, 	// plot of land directly to the right of the main plot
		LandData const * const restrict lowerNeighbor,	// plot of land directly below the main plot
		int const upperLower			// tag for upper or lower trianglular plot. 1 denotes upper
		);
		
#endif // TILT_H

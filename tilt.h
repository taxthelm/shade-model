#include "landStruct.h"
#ifndef TILT_H
#define TILT_H

void tilt(LandData *mainPlot, 				// plot of land tilt angles are calculated for
		  LandData *rightNeighbor, 			// plot of land directly to the right of the main plot
		  LandData *lowerNeighbor,			// plot of land directly below the main plot
		  int upperLower);					// tag for upper or lower trianglular plot. 1 denotes upper
#endif

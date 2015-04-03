#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "landStruct.h"
#include "tilt.h"

void tilt(LandData *mainPlot, LandData *xNeighbor, LandData *yNeighbor, int upperLower){
	if (upperLower == 1){
		mainPlot->angleX = atan((xNeighbor->elevation - mainPlot->elevation)/mainPlot->sizeX);
		mainPlot->angleY = atan((mainPlot->elevation - yNeighbor->elevation)/mainPlot->sizeY);
	} else {
		mainPlot->angleX = atan((mainPlot->elevation - xNeighbor->elevation)/mainPlot->sizeX);
		mainPlot->angleY = atan((yNeighbor->elevation - mainPlot->elevation)/mainPlot->sizeY); 
	}
}


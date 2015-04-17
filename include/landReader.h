/*
 *landReader.h
 *
 *Extracts and calculates necessary data to fill a landStruct.
 *Data is read from a .asc file, see include DEM.asc as format guide
 *
*/

#include "landStruct.h"

#ifndef LANDREADER_H
#define LANDREADER_H

LandData* extractData(char *infileName, int* numCols, int* numRows);

#endif

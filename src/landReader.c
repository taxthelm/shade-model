#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "landStruct.h"
#include "tilt.h"

LandData* extractData(char *inFileName, int* num_Cols, int* num_Rows)
{
	FILE *inFile;
	int numCols = 0;
	int numRows = 0;
	float xllcorner = 0.0;
	float yllcorner = 0.0;
 	float cellsize = 0.0;
	float elevation = 0.0;

	
	//potential variables
	double thetaS = -106.0;	//standard meridian longitude

	int i,j;

	//open the file to be read
	inFile = fopen(inFileName, "r");
	
	//make sure the file was opened
	if(inFile == NULL)
	{
		printf("Error, Cannot open file: %s\n", inFileName);
		exit(1);
	}

	//Read in variables from file
	fscanf(inFile, "%*s %d", &numCols);
	fscanf(inFile, "%*s %d", &numRows);
	fscanf(inFile, "%*s %f", &xllcorner);	
	fscanf(inFile, "%*s %f", &yllcorner);
	fscanf(inFile, "%*s %f", &cellsize);
	
    //LandData** structMat = (LandData**)malloc(sizeof(LandData*)*(numRows)); 
    //Lineraize this 2D access using strided access of contiguous memory        NJF 2015-04-14
    LandData* structMat = (LandData*)(malloc(sizeof(LandData)*numRows*numCols));	
    for(i = 0; i < numRows; i++)
	{
		//structMat[i] = (LandData*)malloc(sizeof(LandData)*(numCols));
		for(j = 0; j < numCols; j++)
		{
			fscanf(inFile, "%f", &elevation);
				
			LandData temp;
			temp.sizeX = cellsize;
			temp.sizeY = cellsize;
			temp.elevation = elevation;
	
			//calculation of latitude
			double latitude = (yllcorner + (numRows-(i+1))*cellsize)/6378100.0;
			//calculation of longitude
			temp.latitude = latitude*180.0/(4*atan(1));	//convert the latitude to degrees	
			temp.thetaL = -106 + (xllcorner - ((j/2)-1))/(6378100.0 * cos(latitude));
			temp.thetaS = thetaS;
            //Strided access i*row size gives current row, +j give column in that row
			structMat[i*numRows + j] = temp;
		}
	}
	
	//Fill the secondary cells of matrix, used for triangular mesh
    const int rows = numRows - 1;
    const int cols = numCols - 1;
	for(i = 0; i < (rows); i++)
	{
		for(j = 0; j < (cols); j++)
		{
			//LandData temp;
			LandData temp1;
			LandData temp2;
			LandData temp3;
			
			//temp.elevation = structMat[i][j].elevation;
			//temp.latitude = structMat[i][j].latitude;
			//temp.thetaL = structMat[i][j].thetaL;
			//temp.thetaS = thetaS;
			//temp.sizeX = cellsize;
			//temp.sizeY = cellsize;
			
			temp1 = structMat[i*rows + j+1];
			temp2 = structMat[(i+1)*rows + j];			
			temp3 = structMat[i*rows + j];
			
			//calculate the angles of each struct			
			//tilt(&temp, &temp1, &temp2, 0);
			tilt(&temp3, &temp1, &temp2, 1);			
			
			
			//if(i == 0 && j-1 == 0)
			structMat[i*rows + j] = temp3;
			//structMat[i][j] = temp;
		}
	}	

	fclose(inFile);
	
	*num_Rows = numRows;
	*num_Cols = numCols;
	
	return structMat;
}

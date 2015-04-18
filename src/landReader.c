/*
File: LandReader.c

ChangeLog:
2015-04-15 NJF Linearization of memory for LandData structure
2015-04-17 JDB Evaluate file and determine some math optmizations and general C optimizations


*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

#include "landStruct.h"
#include "tilt.h"

LandData* extractData(
		char const *inFileName, 
		int *num_Cols, 
		int *num_Rows
		)
{

/*
int extractData(
		char const *elevation_file,
		LandData *elevation_data,
		unsigned int *num_columns,
		unsigned int *num_rows
		){
*/
	/*
	   *inFileName - Null terminated character string of file for reading
	   *num_Cols   - Integer value for number of columns in the grid. Consider making unsigned, value should never be negative.
	   *num_Rows   - Integer value for number of rows in the grid. Consider making unsigned, value should never be negative.

		RETURN:
		LandData*  - A pointer to an linearized array of structured data. After looking at this more, I think it would be wise
		             to remove the unsigned character array from the structure. Below there is a use of quite a bit of temporary
					 variables, each of which allocations a good segment of memory. We want to keep that memory usage low, especially
					 with temporary variables.
		NOTES:
					 Consider moving the return value (LandData*) into the function arguments and letting the function return
					 an integer valued routine for error checking. A list of bad return values can be specified designating the
					 error. This may make debugging much easier in the future. Again, just a suggestion.
	*/

	FILE * input_file;
	int numCols = 0;
	int numRows = 0;

	// Are these really floats, or should they just be doubles? JDB
	float xllcorner = 0.0;
	float yllcorner = 0.0;
 	float cellsize  = 0.0;
	float elevation = 0.0;

	
	// Potential variables
	double thetaS = -106.0;	// Standard meridian longitude

	int i,j;

	// Open the file to be read
	input_file = fopen(inFileName, "r");
	
	// Make sure the file was opened
	if(input_file == NULL)
	{
		printf("Error, Cannot open file: %s\n", inFileName);
		// This should return an error from the function, therefore we can apply
		// an MPI abort instead of an general standard exit()
		exit(1);
	}

	//Read in variables from file
	fscanf(input_file, "%*s %d", &numCols);
	fscanf(input_file, "%*s %d", &numRows);
	fscanf(input_file, "%*s %f", &xllcorner);	
	fscanf(input_file, "%*s %f", &yllcorner);
	fscanf(input_file, "%*s %f", &cellsize);
	
    //LandData** structMat = (LandData**)malloc(sizeof(LandData*)*(numRows)); 
    //Lineraize this 2D access using strided access of contiguous memory        NJF 2015-04-14
    LandData* structMat = (LandData*)(malloc(sizeof(LandData)*numRows*numCols));
	unsigned int index;
	

	/*
	   I've moved the file reading portion outside the assignment loop to engage
	   a threading model using OpenMP. This is because the file is not binary and
	   is text based.
	*/
	double t1,t2;
	double *elevation_arr;
	elevation_arr = (double*)malloc(sizeof(double)*numRows*numCols);
	t1 = MPI_Wtime();
	for (i = 0; i < numRows*numCols; i++){
		fscanf(input_file, "%lf ",&(elevation_arr[0]));
	}
	t2 = MPI_Wtime();
	printf("Time to read elevation data: %lf seconds\n",t2-t1);

	// Close the geographic elevation file
	fclose(input_file);

t1 = MPI_Wtime();	
#pragma omp parallel for
	{

    for(i = 0; i < numRows; i++)
	{
		//structMat[i] = (LandData*)malloc(sizeof(LandData)*(numCols));
		for(j = 0; j < numCols; j++)
		{
			index = i*numRows + j;
			// Read the elevation, perhaps we can read in an elevation array, one read of the file
			// is generally a slow process. Perhaps we can read in a very long stride, do some quick
			// calculations, and read in again.
			//fscanf(input_file, "%f", &elevation);
			
			// Do we really need a temporary variable here? JDB	
			//LandData temp;
			//temp.sizeX = cellsize;
			//temp.sizeY = cellsize;
			//temp.elevation = elevation_arr[index];

			structMat[index].sizeX = cellsize;
			structMat[index].sizeY = cellsize;
			structMat[index].elevation = elevation_arr[index];
	
			//calculation of latitude -- Remove the division
			double latitude = (yllcorner + (numRows-(i+1))*cellsize)/6378100.0;
			
			//calculation of longitude
			//temp.latitude = latitude*180.0/(4*atan(1));	//convert the latitude to degrees
			structMat[index].latitude = latitude * 180.0/M_PI;
			
			//            V-- is this thetaS?
			//temp.thetaL = -106 + (xllcorner - ((j/2)-1))/(6378100.0 * cos(latitude));
			structMat[index].thetaL = -106.0 + (xllcorner - ((j/2)-1))/(6378100.0 * cos(latitude));


			//temp.thetaS = thetaS; // Why do we really need thetaS inside the struct? JDB
			structMat[index].thetaS = thetaS;

            //Strided access i*row size gives current row, +j give column in that row
			//structMat[index] = temp; // copies entire structure... JDB
		}
	}

	}
	t2 = MPI_Wtime();
	printf("Time for first pass of calculations on grid: %lf seconds \n",t2-t1);


	//Fill the secondary cells of matrix, used for triangular mesh
    int const rows = numRows - 1;
    int const cols = numCols - 1;

	t1 = MPI_Wtime();
	for(i = 0; i < (rows); i++)
	{
		for(j = 0; j < (cols); j++)
		{
			//LandData temp;
			// Lots of extra non-used memory here, potentially allocated each loop. Not needed.
			//LandData temp1;
			//LandData temp2;
			//LandData temp3;
			
			//temp.elevation = structMat[i][j].elevation;
			//temp.latitude = structMat[i][j].latitude;
			//temp.thetaL = structMat[i][j].thetaL;
			//temp.thetaS = thetaS;
			//temp.sizeX = cellsize;
			//temp.sizeY = cellsize;
			
			//temp1 = structMat[i*rows + j+1];
			//temp2 = structMat[(i+1)*rows + j];			
			//temp3 = structMat[i*rows + j];
			
			//calculate the angles of each struct			
			//tilt(&temp, &temp1, &temp2, 0);
			//tilt(&temp3, &temp1, &temp2, 1);	
			
			tilt(
					&structMat[i*rows + j],
					&structMat[i*rows + (j+1)],
					&structMat[(i+1)*rows + j],
					1
				);
			
			//if(i == 0 && j-1 == 0)
			//structMat[i*rows + j] = temp3;
			//structMat[i][j] = temp;
		}
	}
	t2 = MPI_Wtime();
	printf("Time for second pass of calculations on grid: %lf seconds \n",t2-t1);

	//fclose(input_file);
	
	*num_Rows = numRows;
	*num_Cols = numCols;
	
	// Free the elevation_arr data
	free(elevation_arr);

	return structMat;
}

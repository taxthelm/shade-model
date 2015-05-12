/*
/ Troy Axthelm and Jingyu Li
/ Land shade model program
/ 12 December 2013
/
/ ChangeLog:
/
/ 2015.04.02 JDB rm parlib for std MPI
/
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#include <mpi.h>

#include "shadem.h"

#include "landStruct.h"
#include "landReader.h"
#include "sunDeclination.h"
#include "timeDifference.h"
#include "localHourAngle.h"
#include "solarAltitude.h"
#include "azimuth.h"

int main(int argc, char* argv[])
{

	// Initialize MPI
	int ierr;
	int thd_safety;
	ierr = MPI_Init_thread(&argc,&argv,MPI_THREAD_SERIALIZED,&thd_safety);
	int psize;
	int rank;
	MPI_Comm_size(MPI_COMM_WORLD,&psize);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    double start_t, end_t;
    start_t = MPI_Wtime();
	//Set to the day of shading calculation
	//January 1  = 0; December 31 = 364
	// Do we need to consider leap years?
	int Day = 20;

	//Interval of time to have shading information for
	//Must change landStruct shade array size to 86400/timeInterval
	int timeInterval = 300; // This is a delta time for the day i.e., 300 seconds

	//Set to the file name to read elevation data from
	char *fileName = "DEM.asc";
	
	//Variables will be set at runtime
	LandData* ourData;
	int numRows         = 0;
	int numCols         = 0;
	double sunDeclin    = 0.0;
	double localHrAngle = 0.0;
	double azi          = 0.0;	
	double solarAlt     = 0.0;
	double timeDif      = 0.0;
	
	// Pi - POSIX defines M_PI in the math library header. Therefore,
    //      change to using the definition and if the implementation 
    //      does not include M_PI, define our own using a simple def.

	const double PI = M_PI;
	//const double PI = 4*atan(1);  //#define for const??? NJF 2015-03-31

	//index variables
	int i,j,k;

	double temp;

	//ourData is now populated with the necessary information
	//from the inpt file.

	double time0  = 0.0;
	double time1  = 0.0;
	double deltat = 0.0;

	// When running in MPI mode, this needs to be read by rank 0, then broadcasted to
	// all other processes in the communicator. Currently read by every process. Fixing
	// on 2015-05-02 by JDB

	// unsigned char shading[288];
    // double sizeX;
    // double sizeY;
    // double angleX;
    // double angleY;
    // double latitude;
    // double thetaL;
    // double thetaS;
    // double elevation;

	// Required to send a structure
	MPI_Datatype mpi_land_data;
	int land_data_block_lengths[9] = 
		{288,1,1,1,1,1,1,1,1};

	MPI_Datatype land_data_types[9] = 
		{MPI_UNSIGNED_CHAR,MPI_DOUBLE,MPI_DOUBLE,
		 MPI_DOUBLE,MPI_DOUBLE,MPI_DOUBLE,
		 MPI_DOUBLE,MPI_DOUBLE,MPI_DOUBLE};

	MPI_Aint land_data_offset[9] = 
		{offsetof(LandData,shading),
		 offsetof(LandData,sizeX),
		 offsetof(LandData,sizeY),
		 offsetof(LandData,angleX),
		 offsetof(LandData,angleY),
		 offsetof(LandData,latitude),
		 offsetof(LandData,thetaL),
         offsetof(LandData,thetaS),
		 offsetof(LandData,elevation) };

	ierr = MPI_Type_create_struct(
			9,land_data_block_lengths,land_data_offset,
			land_data_types,&mpi_land_data);

	if ( MPI_SUCCESS != ierr) MPI_Abort(MPI_COMM_WORLD,ierr);

	ierr = MPI_Type_commit(&mpi_land_data);

	if (MPI_SUCCESS != ierr) MPI_Abort(MPI_COMM_WORLD,ierr);

	MPI_Barrier(MPI_COMM_WORLD);	

	if ( rank == 0 ) {

	printf("Reading file: %s\n",fileName);
	time0 = MPI_Wtime(); // Technically undefined until an MPI_Init routine is called.
	ourData = extractData(fileName, &numCols, &numRows);
	time1 = MPI_Wtime();
	printf("Time to read and initialize data: %lf seconds\n",time1-time0);
	}

	// Basic data

	if ( rank == 0 ) {
		printf("Sending dimensions to all processors...");
	}
	MPI_Bcast(&numCols,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&numRows,1,MPI_INT,0,MPI_COMM_WORLD);

	if ( rank == 0 ) {
		printf("done\n");
	} else { printf("%d\n",numCols); }

	
	// Allocate data on other processors
	if ( rank != 0 ) {
		ourData = (LandData*)(malloc(sizeof(LandData)*numRows*numCols));
		if ( NULL == ourData ) MPI_Abort(MPI_COMM_WORLD,2);
	}

	if ( rank == 0 ) {
		printf("Elevation at point %d in linear memory: %15.7lf\n",93,ourData[93].elevation);
		printf("Sending geographic data to all processors...");
	}

	//MPI_Barrier(MPI_COMM_WORLD);
	//MPI_Abort(MPI_COMM_WORLD,0);
	// Structured Data (i.e., ourData), more difficult
	MPI_Bcast(ourData,numCols*numRows,mpi_land_data,0,MPI_COMM_WORLD);

	if ( rank == 0 ) {
		printf("done\n");
	}

	printf("Elevation at point %d in linear memory: %15.7lf\n",93,ourData[93].elevation);

	//Calculate the sun declination for the given day.	
	sunDeclination(&sunDeclin,Day);

	// May need to move most of this inside the loop for C99 to help with
	// loop threading. These need to be private.
	double stepX;
	double stepY;
	double lenX;
	double lenY;
	double tempX;
	double tempY;
	int roundTempX;
	int roundTempY;
	double stepTempX;
	double stepTempY;
	double tempSlope;
	double ourStep;
	double darkAngle;



	//Not including lower portions of triangular mesh
	for(k = rank; k < 288; k += psize)
	{
		//Set the name of the current file to write to
		char fileName[50];
		sprintf(fileName, "shadingPlot_%d.m", k);
		FILE *thisFile;
		thisFile = fopen(fileName, "w");
		fprintf(thisFile, "a = [ ");

		for( i = 0; i < numRows; i++)
		{
			for (j = 0; j < numCols; j++)
			{
				unsigned int index = i*numRows + j;

				double currentSizeX = ourData[index].sizeX;
				double currentSizeY = ourData[index].sizeY;
				ourStep = currentSizeX;

			
				//Calculate the dark angle
				darkAngle = acos( -tan( ourData[index].latitude ) * tan(sunDeclin) );
	
				//For each thirty minute azimuth, calculate the horizon(in this day)			
		
				//-----			
				timeDifference(
						&timeDif, 
						ourData[index].thetaS, 
						ourData[index].thetaL
						);

				localHourAngle( 
						&localHrAngle,
						(double)(k * timeInterval),
						timeDif,
						0.0, 
						timeInterval 
						);

                //Can't 15.0*PI/180 be macroed? same with 30??? NJF
				if ( 
						//((localHrAngle < (darkAngle + M_PI - (15.0*M_PI/180))) && k < (86400/timeInterval/2))  || 
						//((localHrAngle > (M_PI-darkAngle + (30.0*M_PI/180.0)) && k >= (86400/timeInterval/2)))

						( (localHrAngle < (darkAngle + M_PI - DegToRad(15.0)) && k <  (86400/timeInterval/2)) )  || 
						( (localHrAngle > (M_PI - darkAngle + DegToRad(30.0)) && k >= (86400/timeInterval/2)) )
				   ){
					ourData[index].shading[k] = 1;
				}

				else {

					// LATITUDE NEEDS TO BE IN RADIANS!!!!!!!!				
					solarAltitude(
							&solarAlt, 
							sunDeclin, 
							DegToRad(ourData[index].latitude),
							localHrAngle );

					azimuth(
							&azi,
							solarAlt, 
							ourData[index].latitude,
							sunDeclin,
							k*timeInterval );

					stepX = -ourStep*sin(azi);			
					stepY =  ourStep*cos(azi);		
			
					// Why set to zero then increment? JDB
					//lenX = 0.0;
					//lenY = 0.0;

				 	stepTempX = stepX / currentSizeX;
				   	stepTempY = stepY / currentSizeY;

					lenX = stepX;
					lenY = stepY;

					tempX = lenX/currentSizeX + (double)j;
					tempY = lenY/currentSizeY + (double)i;

					ourData[index].shading[k] = 0;
	
					solarAlt = tan(solarAlt);				
	
					//Step along azimuth until off of grid or the observer point is determind to be shaded
					while (
							tempX >= 0              && 
							tempX <= (numCols-1)    && 
							tempY >= 0              && 
							tempY <= (numRows - 1) ) 
					{

						roundTempX = (int)round(tempX);
						roundTempY = (int)round(tempY);			

						if(
							roundTempX != i ||  
							roundTempY != j )
						{
							
							tempSlope = (ourData[roundTempY*numRows + roundTempX].elevation - ourData[index].elevation) / sqrt(lenX*lenX + lenY*lenY);	
				
							if(solarAlt <= tempSlope) 
							{	
								ourData[index].shading[k] = 1;
								break;
							}
						}			

						lenX += stepX;		
						lenY += stepY;

						tempX += stepTempX;
						tempY += stepTempY;
				
					}

				}

				// Print all of shade data to files in .m format
				fprintf(
						thisFile, 
						"%d ",
						ourData[index].shading[k]
						);

			}

			fprintf(thisFile, "; ");
		}

/*
		int timeHours = (int)floor(k*timeInterval/3600);
		int timeMinutes = (int)((k*timeInterval)/60);
		timeMinutes = timeMinutes%60;		
		char time[5];			

		sprintf(time,"%2d:%02d", timeHours, timeMinutes);					
*/

		fprintf(thisFile, "];");

		fclose(thisFile);

		printf("%d remaining\n", 288-k);
	}

	//fprintf(thisFile,"\nh = surf(a)\n m = [0:9.17:2053*9.17];\nn = [1965*9.17:-9.17:0];\nh = surf(m, n, b, a);\nset(h,'LineStyle','none')\ncolormap([0, 0.75, 0.75; 0, 0, 0]);\nview(-35, 25);\nxlabel('West - East');\nylabel('North - South');\nzlabel('Elevation (m)');\ntitle('Land Shading: %d minute interval |  Time: %s');\naxis equal\naxis vis3d\nsaveas(h, 'shadePlot_%02d.jpeg', 'jpeg' );", timeInterval/60, time, k);



	//Release all memory allocation
	free(ourData);
    MPI_Barrier(MPI_COMM_WORLD);
    end_t = MPI_Wtime();
    if(rank == 0)
    {
        printf("Total Wall Time from parallel INIT to FINSIH: %f\n", end_t - start_t);
    }

	MPI_Finalize();

	return 0;
}



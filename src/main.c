/*
/ Troy Axthelm and Jingyu Li
/ Land shade model program
/ 12 December 2013
/
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

#include "landStruct.h"
#include "landReader.h"
#include "sunDeclination.h"
#include "timeDifference.h"
#include "localHourAngle.h"
#include "solarAltitude.h"
#include "azimuth.h"

int main(int argc, char** argv)
{	
	//Set to the day of shading calculation
	//January 1  = 0; December 31 = 364
	int Day = 20;

	//Interval of time to have shading information for
	//Must change landStruct shade array size to 86400/timeInterval
	int timeInterval = 300;

	//Set to the file name to read elevation data from
	char* fileName = "DEM.asc";
	
	//Variables will be set at runtime
	LandData** ourData;
	int numRows=0;
	int numCols=0;
	double sunDeclin  = 0.0;
	double localHrAngle = 0.0;
	double azi = 0.0;	
	double solarAlt = 0.0;
	double timeDif = 0.0;
	
	//pi	
	const double PI = 4*atan(1);  //#define for const??? NJF 2015-03-31

	//index variables
	int i,j,k;

	double temp;

	//ourData is now populated with the necessary information
	//from the inpt file.
	ourData = extractData(fileName, &numCols, &numRows);
	
	//Calculate the sun declination for the given day.	
	sunDeclination(&sunDeclin,Day);

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

	int rank;
	int psize;

	int ierr;
	int thread_safety;
	ierr = MPI_Init_thread(&argc, &argv,MPI_THREAD_SERIALIZED,&thread_safety);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&psize);

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

				double currentSizeX = ourData[i][j].sizeX;
				double currentSizeY = ourData[i][j].sizeY;
				ourStep = currentSizeX;

			
				//Calculate the dark angle
				darkAngle = acos(-tan(ourData[i][j].latitude) * tan(sunDeclin));
	
				//For each thirty minute azimuth, calculate the horizon(in this day)			
		
				//-----			
				timeDifference(&timeDif, ourData[i][j].thetaS, ourData[i][j].thetaL);

				localHourAngle(&localHrAngle,(double)(k * timeInterval), timeDif, 0.0, timeInterval);

				if (((localHrAngle < (darkAngle + PI - (15.0*PI/180))) && k < (86400/timeInterval/2))  || ((localHrAngle > (PI-darkAngle + (30.0*PI/180.0)) && k >= (86400/timeInterval/2))))
				{	
					ourData[i][j].shading[k] = 1.0;
				}
				else
				{

					//LATITUDE NEEDS TO BE IN RADIANS!!!!!!!!				
					solarAltitude(&solarAlt, sunDeclin, ((ourData[i][j].latitude)*PI/180), localHrAngle);

					azimuth(&azi, solarAlt, ourData[i][j].latitude, sunDeclin, k * timeInterval);

					stepX = ourStep*-sin(azi);			
					stepY = ourStep*cos(azi);		
			
					lenX = 0.0;
					lenY = 0.0;

				 	stepTempX = stepX / currentSizeX;
				   	stepTempY = stepY / currentSizeY;

					lenX += stepX;
					lenY += stepY;

					tempX = lenX/currentSizeX + (double)j;
					tempY = lenY/currentSizeY + (double)i;

					ourData[i][j].shading[k] = 0;
	
					solarAlt = tan(solarAlt);				
	
					//Step along azimuth until off of grid or the observer point is determind to be shaded
					while (tempX >= 0 && tempX <= (numCols-1) && tempY >= 0 && tempY <= (numRows - 1))
					{	
						roundTempX = (int)round(tempX);
						roundTempY = (int)round(tempY);			

						if(roundTempX != i ||  roundTempY != j) 
						{
							tempSlope = (ourData[roundTempY][roundTempX].elevation - ourData[i][j].elevation)/sqrt(lenX*lenX + lenY*lenY);	
				
							if(solarAlt <= tempSlope)
							{	
								ourData[i][j].shading[k] = 1;
								break;
							}
						}												
			
						lenX += stepX;		
						lenY += stepY;

						tempX += stepTempX;
						tempY += stepTempY;
				
					}

				}
				//Print all of shade data to files in .m format
				fprintf(thisFile, "%d ", ourData[i][j].shading[k]);

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
	for(i = 0; i < numRows; i++)
	{
		free(ourData[i]);
	}
	free(ourData);	
	MPI_Finalize();
	return 0;
}










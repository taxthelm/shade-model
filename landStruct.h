#ifndef LANDSTRUCT_H
#define LANDSTRUCT_H

typedef struct{
	unsigned char shading[288];
	double sizeX;
	double sizeY;
	double angleX;
	double angleY;
	double latitude;
	double thetaL;
	double thetaS;
	double elevation;
} LandData;

#endif

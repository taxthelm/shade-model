/*
File: landStruct.h

Defines the structure which holds the physical characteristics
for the geographical cell in the mesh.

QUESTIONS:
Does the shading[] array really need to be in the structure below?
I feel that id should actually be decoupled from the structure, but
I would like more opinions.

Is thetaS mutable? I didn't really see that it was within the code?
Perhaps I'm missing it somewhere

*/

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

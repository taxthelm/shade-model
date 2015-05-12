/*
   Solar azimuth angle calculated from North. Azimuth angle will
   be negative before noon.
*/

#ifndef AZIMUTH_H
#define AZIMUTH_H

// All input angles should be in radians
void azimuth(
		double * restrict const azimuth,	// solar azimuth angle (output)
		double const solarAlt,				// solar altitude
		double const latitude,				// latitude in radians, negative latitude implies south of equator
		double const declination,		 	// solar declination angle
		int const time);					// denotes AM/PM with 0 for AM and 1 for PM
#endif

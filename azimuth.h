#ifndef AZIMUTH_H
#define AZIMUTH_H

// Solar azimuth angle calculated from North
// Azimuth angle will be negative before noon
// All input angles should be in radians
void azimuth(double *azimuth, 			// solar azimuth angle (output)
			 double solarAlt,			// solar altitude
			 double latitude,			// latitude in radians, negative latitude implies south of equator
			 double declination, 		// solar declination angle
			 int time);					// denotes AM/PM with 0 for AM and 1 for PM
#endif

// ****************************************************************************
// 
// This is the header file for all of the files in parlib.c. This is an
// interface to a parallel communications middleware, e.g., MPI.
//
// The basic library was written as part of Mathematics 5490 at the
// University of Wyoming during the Spring semester, 2009 by Derrick
// Cerwinsky. It was generalized by Craig C. Douglas for the 2010 Winter
// Enhancement Program (WE 244) at the King Abdul University of Science
// and Technology (http://www.kaust.edu.sa).
//
// The code is open source. It may be redistributed freely as long as the
// initial comments are maintained intact.
//
// Currently, there is/are version(s) for
//
//     MPI          parlib_mpi.h and parlib_mpi.c
//
// Derrick Cerwinsky and Craig C. Douglas
// January, 2010
//
// ****************************************************************************

#ifndef H_PARLIB_MPI
#define H_PARLIB_MPI

#include <mpi.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef MPI_Request par_request;
typedef MPI_Status  par_status;


#define MY_MPI_INT 0
#define MY_MPI_DOUBLE 1
#define MY_MPI_CHAR 2


#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif


#endif      /* H_PARLIB_MPI */

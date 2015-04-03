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


#ifndef H_PARLIB
#define H_PARLIB

#ifdef __cplusplus
extern "C" {
#endif


// ****************************************************************************
//
// Definitions that are independent of the middleware
//
//     Data types
//     Reduce operations
//
// ****************************************************************************

#define PAR_INT     0
#define PAR_FLOAT   1
#define PAR_DOUBLE  2
#define PAR_CHAR    3

#define PAR_TYPE_SIZE       PAR_CHAR+1

#define PAR_NOOP            0
#define PAR_MAX             1
#define PAR_MIN             2
#define PAR_SUM             3
#define PAR_PROD            4
#define PAR_LAND            5
#define PAR_BAND            6
#define PAR_LOR             7
#define PAR_BOR             8
#define PAR_LXOR            9
#define PAR_BXOR           10
#define PAR_MINLOC         11

#define PAR_REDUCE_SIZE     PAR_MINLOC+1


// ****************************************************************************
//
// Include definitions that are specific to the middleware, which currently
// is/are for the following:
//
//     MPI
//
// See Makefile for definitions, which can be overriden on the make command
// line.
//
// ****************************************************************************

// The MPI version

#if PAR_MPI == 1
#include "parlib_mpi.h"
#endif


// ****************************************************************************
//
// The length (in bytes) of the two stuctures that are defined for requests
// and status. These can be used in calling programs that have to provide
// space.
//
// ****************************************************************************

#define par_request_size    sizeof( par_request )
#define par_status_size     sizeof( par_status )


// ****************************************************************************
//
// The program header definitions that are independent of the middleware
//
// ****************************************************************************

// par_start initializes the middleware using the program arguments and
// returns the number of processors and the processor number in the last
// two arguments

void par_start( int, char**, int*, int* );


// par_end ends the middleware and does not return

void par_end( );


// par_size returns the number of processes

int par_size( );


// par_rank returns the process id, which is [0, par_size()-1]

int par_rank( );


// par_walltime returns the wall clock time as a double

double par_walltime( );


// par_send sends a certain number of elements of a data type and blocks.
// The inputs expected are: buffer address, length, data type, destination,
// and tag.


int par_send( void*, int, int, int, int );


// par_recv receives a certain number of elements of a data type and blocks.
// The inputs expected are: buffer address, length, data type, destination,
// and tag.

int par_recv( void*, int, int , int, int );


// par_isend sends a certain number of elements of a data type and does not
// block.
// The inputs expected are: buffer address, length, data type, destination,
// tag, and an address for a completion status variable for later queries.

//int par_isend( void*, int, int, int, int, par_request* );
int par_isend( void*, int, int, int, int, par_request* );


// par_irecv receives a certain number of elements of a data type and does not
// block.
// The inputs expected are: buffer address, length, data type, destination,
// tag, and an address for a completion status variable for later queries.

int par_irecv( void*, int, int, int, int, par_request* );


// par_wait waits until a request completes.
// The input is the request (returned by par_isend/irecv) and the status
// is returned in the second argument.

int par_wait( par_request*, par_status* );


// par_waitall waits until a set of requests all complete.
// The input is the set of requests (returned by par_isend/irecv) and the
// status is returned in the second argument.

int par_waitall( int, par_request*, par_status* );


// par_waitany waits until one from a set of requests completes.
// The inputs are the set of requests (returned by par_isend/irecv) and the
// number of requests and the status is returned in the third argument.

int par_waitany( int, par_request*, int*, par_status* );


// par_bast broadcasts data.
// The inputs expected are: buffer address, length, the data type, and source.

int par_bcast( void*, int, int, int );


// par_reduce does a reduce operation on a set of parallel data.
// The inputs expected are: input buffer address, output buffer address,
// length of input buffer, the data type, and the processor id of the root
// process.

int par_reduce(void *, void *, int, int, int, int);


// par_barrier stops all processes until they all reach the barrier.

void par_barrier( );



#ifdef __cplusplus
} /* closing brace for extern "C" */
#endif


#endif      /* H_PARLIB */

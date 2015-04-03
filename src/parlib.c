// ****************************************************************************
//
//
//
//
//
// ****************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "parlib.h"

// ****************************************************************************
//
// Include the implementation that is specific to the middleware, which
// currently is/are for the following:
//
//     MPI
//
// See Makefile for definitions, which can be overriden on the make command
// line.
//
// ****************************************************************************

// The MPI version

#if PAR_MPI == 1
#include "parlib_mpi.c"
#endif

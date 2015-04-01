// ****************************************************************************
//
// Define common data for MPI
//
//     Data types
//     Reduce operations
//
// ****************************************************************************

MPI_Datatype
     par_datatypes[] =  { MPI_INT, MPI_FLOAT,
                          MPI_DOUBLE, MPI_CHAR };

MPI_Op
     par_reduce_ops[] = { MPI_OP_NULL, MPI_MAX, MPI_MIN, MPI_SUM, MPI_PROD,
                          MPI_LAND, MPI_BAND, MPI_LOR, MPI_BOR, MPI_LXOR,
                          MPI_BXOR, MPI_MINLOC };



// ****************************************************************************

void par_start( int argc, char **argv, int* psize, int* prank ) {

    MPI_Init( &argc, &argv );
    *psize = par_size( );
    *prank = par_rank( );

}


// ****************************************************************************

void par_end( ) {

    MPI_Finalize();
    exit(0);

}


// ****************************************************************************

int par_rank( ) {

    int my_rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    return my_rank;
}


// ****************************************************************************

int par_size( ) {

    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return size;

}


// ****************************************************************************

double par_walltime( ) {

    return MPI_Wtime();

}


// ****************************************************************************

int par_send( void *buff, int size, int type, int dest, int tag ) {

    return MPI_Send( buff, size, par_datatypes[type], dest,
                     tag, MPI_COMM_WORLD );

}


// ****************************************************************************

int par_recv( void* buff, int size, int type, int source, int tag ) {

    MPI_Status status;

    return MPI_Recv( buff, size, par_datatypes[type], source,
                     tag, MPI_COMM_WORLD, &status);

}


// ****************************************************************************

int par_isend( void* buff, int size, int type, int dest, int tag,
               par_request *request ) {

    return MPI_Isend( buff, size, par_datatypes[type], dest,
                      tag, MPI_COMM_WORLD, request);

}


// ****************************************************************************

int par_irecv( void* buff, int size, int type, int source, int tag,
               par_request *request ) {

    return MPI_Irecv( buff, size, par_datatypes[type], source,
                      tag, MPI_COMM_WORLD, request);

}


// ****************************************************************************

int par_wait( par_request* request, par_status* status ) {

    return MPI_Wait( request, status );

}


// ****************************************************************************

int par_waitall( int count, par_request* request, par_status* status ) {


    return MPI_Waitall( count, request, status );

}


// ****************************************************************************

int par_waitany( int count, par_request* requests, int* index,
                 par_status* status ) {

    return MPI_Waitany( count, requests, index, status );

}


// ****************************************************************************

int par_bcast( void* buff, int size, int type, int source ) {

    return MPI_Bcast( buff, size, par_datatypes[type], source,
                      MPI_COMM_WORLD );

}


// ****************************************************************************

int par_reduce( void* inbuff, void* outbuff, int count, int datatype,
                int reduce_op, int root ) {

    return MPI_Reduce( inbuff, outbuff, count,
                       par_datatypes[datatype],
                       par_reduce_ops[reduce_op], root,
                       MPI_COMM_WORLD);

}


// ****************************************************************************

void par_barrier( ) {

    MPI_Barrier( MPI_COMM_WORLD );

}

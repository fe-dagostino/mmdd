
#include "../include/Mmdd.h"

USING_NAMESPACE_FED

int main( int argc, char* argv[] )
{
   /////////////////////////
   // Create a new service instance
   Mmdd  _srvMmdd( argc, argv );

   // Start the service.
   _srvMmdd.Start();

   return 0;
};


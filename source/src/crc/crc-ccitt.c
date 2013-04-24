
#include <stdio.h>
#include <stdlib.h>

#include "crc/lib_crc.h"


unsigned short crc_ccitt( unsigned char* buffer, unsigned short len )
{
  unsigned short _crc   = 0x0000;
  register int   _count = 0;
  for (_count = 0; _count < len; ++_count)
  {
    _crc = update_crc_kermit( _crc, *buffer++ );
  }
 
  return _crc;
}













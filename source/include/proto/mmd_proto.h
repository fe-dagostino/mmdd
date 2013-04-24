/***************************************************************************
                         mmd_proto.h -  description
                             -------------------

    copyright            : (C) 2000 by Francesco Emanuele D'Agostino
    email                : fedagostino@gmail.com
 ***************************************************************************/


#ifndef __MMD_PROTO_H__
#define __MMD_PROTO_H__

#include "FObject.h"

extern "C"
{
#include "../crc/crc-ccitt.h"
}

USING_NAMESPACE_FED

#pragma pack(push,1)

// Version 

#define   MMDP_VERSION  0x01

// INFO 

#define   MMDP_SERIAL_SIZE             200
#define   MMDP_DESCRIPTION_SIZE        200
#define   MMDP_MAX_CMD_LEN             200   // null terminated string

// Commands

#define   MMDP_CMD_UNKNOWN              0x00
#define   MMDP_CMD_DISCOVERY_REQUEST    0x01
#define   MMDP_CMD_DISCOVERY_REPLY      0x02
#define   MMDP_CMD_EXEC_REQUEST         0x03
#define   MMDP_CMD_EXEC_REPLY           0x04

// End Point Type

#define   MMDP_EPT_UNKNOWN              0x00
#define   MMDP_EPT_DAEMON               0x01
#define   MMDP_EPT_CLIENT               0x02
#define   MMDP_EPT_ANYONE               0xFF




struct MMDP_Header
{
  MMDP_Header( BYTE version, BYTE command, BYTE source, BYTE destination )
    : VERSION(version), COMMAND( command ), SOURCE(source), DESTINATION(destination)
  {
  }
  
  BYTE VERSION;
  BYTE COMMAND;
  BYTE SOURCE;
  BYTE DESTINATION; 
};


struct MMDP_DiscoveryRequest : MMDP_Header
{
  MMDP_DiscoveryRequest()
    : MMDP_Header( MMDP_VERSION, MMDP_CMD_DISCOVERY_REQUEST, MMDP_EPT_CLIENT, MMDP_EPT_DAEMON )
  {
    PADDING = 0x0000;
    CRC     = crc_ccitt( (unsigned char *)this, 6 );
  }
  
  WORD PADDING;
  WORD CRC;
};

struct MMDP_DiscoveryReply : MMDP_Header
{
  MMDP_DiscoveryReply( WORD devType, const BYTE* serial, WORD slen, const BYTE* description, WORD dlen )
    : MMDP_Header( MMDP_VERSION, MMDP_CMD_DISCOVERY_REPLY, MMDP_EPT_DAEMON, MMDP_EPT_CLIENT )
  {
    
    DEVICE_TYPE = devType;
    SER_LEN     = slen;
    DSC_LEN     = dlen;
    
    memset( SERIAL, '\0', MMDP_SERIAL_SIZE );
    if ( SER_LEN > 0 )
      memcpy( SERIAL, serial, (SER_LEN<=MMDP_SERIAL_SIZE)?SER_LEN:MMDP_SERIAL_SIZE );

    memset( DESCRIPTION, '\0', MMDP_DESCRIPTION_SIZE );
    if ( DSC_LEN > 0 )
      memcpy( DESCRIPTION, description, (DSC_LEN<=MMDP_DESCRIPTION_SIZE)?DSC_LEN:MMDP_DESCRIPTION_SIZE );
    
    CRC         = crc_ccitt( (unsigned char *)this, 14+MMDP_SERIAL_SIZE+MMDP_DESCRIPTION_SIZE );
  }
  
  BYTE SERIAL[MMDP_SERIAL_SIZE];
  BYTE DESCRIPTION[MMDP_DESCRIPTION_SIZE];
  WORD SER_LEN;
  WORD DSC_LEN;
  WORD DEVICE_TYPE;
  WORD PADDING;
  WORD CRC;
};

struct MMDP_ExecRequest : MMDP_Header
{
  MMDP_ExecRequest( const CHAR* cmd, WORD clen, const BYTE* match, WORD mlen )
    : MMDP_Header( MMDP_VERSION, MMDP_CMD_EXEC_REQUEST, MMDP_EPT_CLIENT, MMDP_EPT_DAEMON )
  {
    CMD_LEN      = clen;
    SER_LEN      = mlen;

    memset( CMD, '\0', MMDP_MAX_CMD_LEN );
    if ( clen > 0 )
       memcpy( CMD, cmd, (CMD_LEN<=MMDP_MAX_CMD_LEN)?CMD_LEN:MMDP_MAX_CMD_LEN );

    memset( SERIAL, '\0', MMDP_SERIAL_SIZE );
    if ( SER_LEN > 0 )
      memcpy( SERIAL, match, (SER_LEN<=MMDP_SERIAL_SIZE)?SER_LEN:MMDP_SERIAL_SIZE );

    CRC     = crc_ccitt( (unsigned char *)this, 10+MMDP_MAX_CMD_LEN+MMDP_SERIAL_SIZE );
  }

  BYTE CMD[MMDP_MAX_CMD_LEN];
  BYTE SERIAL[MMDP_SERIAL_SIZE];
  WORD CMD_LEN;
  WORD SER_LEN;
  WORD PADDING;
  WORD CRC;
};

struct MMDP_ExecReply : MMDP_Header
{
  MMDP_ExecReply( const BYTE* serial, WORD slen, WORD ret )
    : MMDP_Header( MMDP_VERSION, MMDP_CMD_EXEC_REPLY, MMDP_EPT_DAEMON, MMDP_EPT_CLIENT )
  {
    SER_LEN      = slen;

    memset( SERIAL, '\0', MMDP_SERIAL_SIZE );
    if ( SER_LEN > 0 )
      memcpy( SERIAL, serial, (SER_LEN<=MMDP_SERIAL_SIZE)?SER_LEN:MMDP_SERIAL_SIZE );

    RETVALUE = ret;
    PADDING  = 0x0000;

    CRC      = crc_ccitt( (unsigned char *)this, 10+MMDP_SERIAL_SIZE );
  }

  BYTE SERIAL[MMDP_SERIAL_SIZE];
  WORD SER_LEN;
  WORD RETVALUE;
  WORD PADDING;
  WORD CRC;
};

#pragma pack(pop)

#endif

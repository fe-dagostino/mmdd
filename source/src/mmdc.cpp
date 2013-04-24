
#include "FSocket.h"
#include "../include/proto/mmd_proto.h"

USING_NAMESPACE_FED

int main( int argc, char* argv[] )
{
  int                    _iRetVal = 0;
  MMDP_DiscoveryRequest  _req;
  size_t                 _iBufferSize    = 1024;
  VOID*                  _pReadingBuffer = malloc(_iBufferSize);
  BOOL                   _bExit = FALSE;
  FSocket*               _pSocket = new FSocket(
						AF_INET,
						SOCK_DGRAM,
						IPPROTO_UDP
					      );

  if ( _pSocket == NULL )
  {
    return -1;
  }
  
  FTRY
  {
    // Avoid bind failures.
    _pSocket->SetReuseAddress( TRUE );
    // 
    _pSocket->BindOnBroadcast( 0 );
  
    _pSocket->SendBroadcast( &_req, sizeof(_req), 11111 );
  
    
    while ( _bExit == FALSE )
    {
      struct timeval  _timeout     = { 10, 0 };

      WORD _wFlags = _pSocket->Select( FSocket::READY_FOR_READ|FSocket::TIME_OUT_OCCUR, &_timeout );

      if ( _wFlags & FSocket::READY_FOR_READ )
      {
	sockaddr_in   _source;
	size_t        _iMsgSize =  _pSocket->Peek();
	  
	if ( _iMsgSize > _iBufferSize )
	{
	  _pReadingBuffer = realloc( _pReadingBuffer, _iMsgSize );
	  _iBufferSize = _iMsgSize;
	}
	  
	_pSocket->ReceiveFrom( _pReadingBuffer, _iMsgSize, _source );
	if ( _iMsgSize >= sizeof(MMDP_Header) )
	{
	  MMDP_Header* _pHeader = (MMDP_Header*)_pReadingBuffer;
	  
	  if (
		( _pHeader->VERSION == MMDP_VERSION             ) && 
		( _pHeader->COMMAND == MMDP_CMD_DISCOVERY_REPLY )
	     )
	  {
	    MMDP_DiscoveryReply*  _pReply = (MMDP_DiscoveryReply*)_pHeader;
	    FString               _sSerial( (const char*)_pReply->SERIAL, _pReply->SER_LEN );
	    FString               _sDescription( (const char*)_pReply->DESCRIPTION, _pReply->DSC_LEN );
	    FString               _sIp = FSocket::Inet2String( _source.sin_addr );
	    
	    printf( "DEVICE TYPE [%#X] IP [%s]\n", _pReply->DEVICE_TYPE, (const char*)_sIp );
	    printf( "   |- SERIAL       [%s]\n", (const char*)_sSerial );
	    printf( "   |- DESCRIPTION  [%s]\n", (const char*)_sDescription );
	  }
	}
      }

      if ( _wFlags & FSocket::TIME_OUT_OCCUR )
      {
	_bExit = TRUE;
      }
    }

    _pSocket->Close();
  }
  FCATCH( FSocketException, ex )
  {
  }
  
  delete _pSocket;
  _pSocket = NULL;
  
  return _iRetVal;
};


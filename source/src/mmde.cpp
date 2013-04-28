
#include "FSocket.h"
#include "../include/proto/mmd_proto.h"

USING_NAMESPACE_FED

int main( int argc, char* argv[] )
{
  int                    _iRetVal = 0;
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
    FString           _sCmd   ( argv[1] );
    FString           _sSerial( argv[2] );
    MMDP_ExecRequest  _req(
                             (const CHAR *)_sCmd,
                             _sCmd.GetLen(),
                             (const BYTE *)(const CHAR *)_sSerial,
                             _sSerial.GetLen()
                          );

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
		( _pHeader->VERSION == MMDP_VERSION        ) && 
		( _pHeader->COMMAND == MMDP_CMD_EXEC_REPLY )
	     )
	  {
	    MMDP_ExecReply*  _pReply = (MMDP_ExecReply*)_pHeader;
	    FString          _sSerial( (const char*)_pReply->SERIAL, _pReply->SER_LEN );
	    FString          _sIp = FSocket::Inet2String( _source.sin_addr );
	    
	    printf( "DEVICE IP [%s]\n", (const char*)_sIp );
	    printf( "   |- SERIAL        [%s]\n", (const char*)_sSerial );
	    printf( "   |- RETURN VALUE  [%d]\n", _pReply->RETVALUE );
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



#include "../include/MmddEvents.h"
#include "../include/Mmdd.h"

#include "FSocket.h"
#include "LOGGING/FLogger.h"
#include "LOGGING/FLogThread.h"
#include "LOGGING/FLogDeviceCollector.h"
#include "LOGGING/FLogMessageColorization.h"
#include "LOGGING/FTcpLogDevice.h"
#include "LOGGING/FDiskLogDevice.h"
#include "LOGGING/FConsoleLogDevice.h"

GENERATE_CLASSINFO( MmddEvents, FServiceEventInterface )

USING_NAMESPACE_FED
USING_NAMESPACE_LOGGING


BOOL MmddEvents::OnStart( const FService* pService )
ENTER( OnStart() )
  const Mmdd* _pMmddSrv = (const Mmdd*)pService;

  //////////////
  //  Initialize Smcd configuration
  FTRY
  {
    MmddConfig::Initialize();
  }
  FCATCH( FException, ex )
  {
    TRACE_EXCEPTION_CATCH( ex, OnStart() )
  }

  ///////////////////
  // Initialize the log device collector
  FLogDeviceCollector::Initialize();

  ///////////////////
  // Create message colorozation class instance used to colorize
  // streamed messages on the consolle and on the tcp.
  const ILogMessageColorization* pLogMessageColorization  = new FLogMessageColorization();

  ///////////////////
  // Create an instace for a Log device over TCP.
  // The device will accept row tcp connection on port 10000 on each active interface.
  ILogDevice* _pLogDevice0 = new FTcpLogDevice( "TCP", 
                                                MmddConfig::GetInstance().GetLogServerAddress( NULL ), // Default "127.0.0.1" 
                                                MmddConfig::GetInstance().GetLogServerPort( NULL ),    // Default 55930 
                                                pLogMessageColorization 
                                              );
  ILogDevice* _pLogDevice1 = new FDiskLogDevice( "DLD",
                                                 0xFFFFFFFF,
                                                 MmddConfig::GetInstance().GetLogPath( NULL ),
                                                 MmddConfig::GetInstance().GetLogPrefix( NULL ),
                                                 MmddConfig::GetInstance().GetLogExtension( NULL ),
                                                 MmddConfig::GetInstance().GetLogMaxFiles( NULL ),
                                                 MmddConfig::GetInstance().GetLogSizeLimit( NULL )
                                               );

#ifdef _DEBUG 
  ILogDevice* _pLogDevice2 = new FConsoleLogDevice( "CON", pLogMessageColorization );
#endif
  ///////////////////
  // Add the new device to the list of available devices.
  FLogDeviceCollector::GetInstance().AddDevice( _pLogDevice0 );
  FLogDeviceCollector::GetInstance().AddDevice( _pLogDevice1 );
#ifdef _DEBUG 
  FLogDeviceCollector::GetInstance().AddDevice( _pLogDevice2 );
#endif
  ///////////////////
  // Initialize the Logger
  FLogger::Initialize();

  ///////////////////
  // Register the device for message dispatching.
  FLogger::GetInstance().RegisterDevice( "TCP" );
  FLogger::GetInstance().RegisterDevice( "DLD" );
#ifdef _DEBUG
  FLogger::GetInstance().RegisterDevice( "CON" );
#endif
  (GET_LOG_MAILBOX())->SetLogMessageFlags( _pMmddSrv->GetLogMessageFlags()     );
  (GET_LOG_MAILBOX())->SetVerbosityFlags ( _pMmddSrv->GetVerbosityLevelFlags() );

  //Initialize member variable that control the program exit
  m_bExit   = FALSE;
  m_pSocket = NULL;

  return TRUE;
EXIT

VOID MmddEvents::OnRun(const FService* pService)
ENTER( OnRun() )

  Mmdd*    _pService        = (Mmdd*)pService;
  size_t   _iBufferSize     = 1024;
  VOID*    _pReadingBuffer  = malloc( _iBufferSize );

  while ( !m_bExit )
  {
    if ( m_pSocket == NULL )
    { 
      m_pSocket = new FSocket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

      if ( m_pSocket == NULL )
      {
        ERROR_INFO( "Not Enough Memory : failed allocating Socket", OnRun() )

        FThread::Sleep( 1000 );

        continue;
      }
      
      FTRY
      {
        m_pSocket->SetReuseAddress( TRUE );

        m_pSocket->BindOnBroadcast( MmddConfig::GetInstance().GetBroadcastPort( NULL ) );
      }
      FCATCH( FSocketException, ex )
      {
        delete m_pSocket;
        m_pSocket = NULL;

        TRACE_EXCEPTION_CATCH( ex, OnRun() )
      }
    }
    
    if ( m_pSocket != NULL )
    {
      struct timeval  _timeout     = { 1, 0 };

      FTRY
      {
        WORD _wFlags = m_pSocket->Select( FSocket::READY_FOR_READ|FSocket::TIME_OUT_OCCUR, &_timeout );

        if ( _wFlags & FSocket::READY_FOR_READ )
        {
          sockaddr_in   _source;
          size_t        _iMsgSize =  m_pSocket->Peek();
  
          if ( _iMsgSize > _iBufferSize )
          {
            _pReadingBuffer = realloc( _pReadingBuffer, _iMsgSize );
            _iBufferSize = _iMsgSize;
          }

          m_pSocket->ReceiveFrom( _pReadingBuffer, _iMsgSize, _source );
          if ( _iMsgSize == 0 )
          {
            VERBOSE_INFO( ILogMessage::VL_HIGH_PERIODIC_MESSAGE, "", OnRun() )
          }
          else
          if ( _iMsgSize <  sizeof(MMDP_Header) )
          {
            ERROR_INFO( "Received Message with wrong size", OnRun() )
          }
          else
          {
            HandleMMDProtoMessage( (const MMDP_Header*)_pReadingBuffer, _source );
          }
        }
      }
      FCATCH( FSocketException, ex )
      {
        delete m_pSocket;
        m_pSocket = NULL;

        TRACE_EXCEPTION_CATCH( ex, OnRun() )
      }
    }
    
    if ( m_pSocket == NULL )
    {
      FThread::Sleep( 1000 );
    }
  }//while ( !m_bExit )

EXIT

VOID MmddEvents::OnStop(const FService* pService)
ENTER( OnStop() )

  //
  m_bExit = TRUE;
EXIT

VOID MmddEvents::OnInterrogate(const FService* pService)
ENTER( OnInterrogate() )

EXIT

VOID MmddEvents::OnPause(const FService* pService)
ENTER( OnPause() )

EXIT

VOID MmddEvents::OnContinue(const FService* pService)
ENTER( OnContinue() )

EXIT

VOID MmddEvents::OnShutdown(const FService* pService)
ENTER( OnShutdown() )

EXIT

VOID MmddEvents::OnNetBindAdd(const FService* pService)
ENTER( OnNetBindAdd() )

EXIT

VOID MmddEvents::OnNetBindDisable(const FService* pService)
ENTER( OnNetBindDisable() )

EXIT

VOID MmddEvents::OnNetBindEnable(const FService* pService)
ENTER( OnNetBindEnable() )

EXIT

VOID MmddEvents::OnNetBindRemove(const FService* pService)
ENTER( OnNetBindRemove() )

EXIT

VOID MmddEvents::OnParamChange(const FService* pService)
ENTER( OnParamChange() )

EXIT

BOOL MmddEvents::OnUserControl(const FService* pService, DWORD dwOpcode)
ENTER( OnUserControl() )
  return TRUE;
EXIT

BOOL MmddEvents::HandleMMDProtoMessage( const MMDP_Header* pHeader, struct sockaddr_in& source )
ENTER( HandleMMDProtoMessage() )

  if ( pHeader->VERSION != MMDP_VERSION )
  {
    return FALSE;
  }

  switch ( pHeader->COMMAND )
  {
    case MMDP_CMD_DISCOVERY_REQUEST :
    {
      WORD    _wDeviceType        = MmddConfig::GetInstance().GetDeviceType( NULL );
      FString _sDeviceSerial      = MmddConfig::GetInstance().GetDeviceSerial( NULL );
      FString _sDeviceDescription = MmddConfig::GetInstance().GetDeviceDescription( NULL );
      FString _sSourceIp          = FSocket::Inet2String( source.sin_addr );
      WORD    _wSourcePort        = ntohs( source.sin_port );
      
      MMDP_DiscoveryReply  _reply(  
                                    _wDeviceType,
                                    (const BYTE*)(const char *)_sDeviceSerial,
                                    _sDeviceSerial.GetLen(),
                                    (const BYTE*)(const char *)_sDeviceDescription,
                                    _sDeviceDescription.GetLen()
                                 );

      LOG_INFO( FString( 0, "SENDING: VER[%#X] CMD[%#X] TO DST[%s] ON PORT[%d]", _reply.VERSION, _reply.COMMAND, (const char*)_sSourceIp, _wSourcePort ), HandleMMDProtoMessage() ) 
      if ( SendMMDPMessage( &_reply, sizeof(_reply), source ) == FALSE )
      {
        ERROR_INFO( "ERROR SENDING DISCOVERY REPLY",  HandleMMDProtoMessage() )
      }
    }; break;

#if defined(_MMDD_EXEC_ENABLED)
    case MMDP_CMD_EXEC_REQUEST :
    {
      const MMDP_ExecRequest*  _cmd = (const MMDP_ExecRequest*)pHeader; 

      FString _sDeviceSerial        = MmddConfig::GetInstance().GetDeviceSerial( NULL );
      FString _sSourceIp            = FSocket::Inet2String( source.sin_addr );
      WORD    _wSourcePort          = ntohs( source.sin_port );

      FString _sCmd   ( (const CHAR*)_cmd->CMD   , _cmd->CMD_LEN );
      FString _sSerial( (const CHAR*)_cmd->SERIAL, _cmd->SER_LEN );


      // Move to upcase both serials
      _sDeviceSerial.UpCase(); // Serial coming from configuration file
      _sSerial.UpCase();       // Serial coming from request


      LOG_INFO( FString( 0, "GOT CMD REQ: REQ SERIAL[%s] SERIAL[%s]", (const CHAR*)_sSerial, (const CHAR*)_sDeviceSerial ), HandleMMDProtoMessage() ) 
      if ( memcmp( _sSerial.GetBuffer(), _sDeviceSerial.GetBuffer(), _sSerial.GetLen() ) == 0 )
      {
         LOG_INFO( FString( 0, "EXECUTING: CMD[%s]", (const CHAR*)_sCmd ), HandleMMDProtoMessage() ) 
         int _iRetVal = system( (const CHAR*)_sCmd );

         MMDP_ExecReply           _reply( 
                                           (const BYTE*)(const char *)_sDeviceSerial,
                                           _sDeviceSerial.GetLen(),
                                           _iRetVal
                                        );

         LOG_INFO( FString( 0, "SENDING: VER[%#X] CMD[%#X] TO DST[%s] ON PORT[%d] - REVAL[%d]", _reply.VERSION, _reply.COMMAND, (const char*)_sSourceIp, _wSourcePort, _iRetVal ), HandleMMDProtoMessage() ) 
         if ( SendMMDPMessage( &_reply, sizeof(_reply), source ) == FALSE )
         {
            ERROR_INFO( "ERROR SENDING EXEC REPLY",  HandleMMDProtoMessage() )
         }
      }
    }; break;
#endif  //#if defined(_MMDD_EXEC_ENABLED)
    
    default:
    {
      VERBOSE_INFO( ILogMessage::VL_HIGH_PERIODIC_MESSAGE, FString( 0, "Unmanaged Command [%#X]", pHeader->COMMAND ), HandleMMDProtoMessage() ) 
    }; break;
  }

EXIT

      
BOOL      MmddEvents::SendMMDPMessage( const MMDP_Header* pHeader, size_t iMsgLen, struct sockaddr_in& source )
ENTER( SendMMDPMessage() )

  BOOL _bRetVal = TRUE;
  
  if ( m_pSocket == NULL )
    return FALSE;
  
  FTRY
  {
    m_pSocket->SendBroadcast( pHeader, iMsgLen, ntohs(source.sin_port) );
  }
  FCATCH( FSocketException, ex )
  {
    delete m_pSocket;
    m_pSocket = NULL;

    _bRetVal = FALSE;
    
    TRACE_EXCEPTION_CATCH( ex, OnRun() )
  }
  
  return _bRetVal;
EXIT

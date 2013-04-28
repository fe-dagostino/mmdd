
#include "../include/Mmdd.h"
#include "../include/MmddEvents.h"

#include "LOGGING/FLogger.h"

USING_NAMESPACE_LOGGING

GENERATE_CLASSINFO( Mmdd, FService )

Mmdd::Mmdd( int argc, char* argv[] )
 : FService( 
                 FSP_NORMAL, 
                 TRUE, 
                 new MmddEvents(), 
                 "Make Me Discoverable Daemon",
                 MAKEVERSION( 1,1,0,0 ),
                 argc, argv
              )
ENTER(Mmdd())

EXIT

Mmdd::~Mmdd()
ENTER(~Mmdd())
EXIT

DWORD	Mmdd::GetLogMessageFlags() const
{
	DWORD _dwRetVal = 0;

	_dwRetVal |= MmddConfig::GetInstance().IsTraceExceptionEnabled(NULL)?FLogMessage::MT_TRACE_EXCEPTION:0;
	_dwRetVal |= MmddConfig::GetInstance().IsCatchExceptionEnabled(NULL)?FLogMessage::MT_CATCH_EXCEPTION:0;
	_dwRetVal |= MmddConfig::GetInstance().IsAssertionFailureEnabled(NULL)?FLogMessage::MT_ASSERTION_FAILURE:0;
	_dwRetVal |= MmddConfig::GetInstance().IsErrorInfoEnabled(NULL)?FLogMessage::MT_ERROR_INFO:0;
	_dwRetVal |= MmddConfig::GetInstance().IsLoggingInfoEnabled(NULL)?FLogMessage::MT_LOGGING_INFO:0;
	_dwRetVal |= MmddConfig::GetInstance().IsVerboseInfoEnabled(NULL)?FLogMessage::MT_VERBOSE_INFO:0;
	_dwRetVal |= MmddConfig::GetInstance().IsRawInfoEnabled(NULL)?FLogMessage::MT_RAW_INFO:0;
	_dwRetVal |= MmddConfig::GetInstance().IsEnterMethodEnabled(NULL)?FLogMessage::MT_ENTER_METHOD:0;
	_dwRetVal |= MmddConfig::GetInstance().IsExitMethodEnabled(NULL)?FLogMessage::MT_EXIT_METHOD:0;
		
	return _dwRetVal;
}


DWORD	Mmdd::GetVerbosityLevelFlags() const
{
	DWORD _dwRetVal = 0;
	
	_dwRetVal |= MmddConfig::GetInstance().IsStartUpMessageEnabled(NULL)?FLogMessage::VL_START_UP_MESSAGE:0;
	_dwRetVal |= MmddConfig::GetInstance().IsShutDownMessageEnabled(NULL)?FLogMessage::VL_SHUT_DOWN_MESSAGE:0;
	_dwRetVal |= MmddConfig::GetInstance().IsLowPeriodicMessageEnabled(NULL)?FLogMessage::VL_LOW_PERIODIC_MESSAGE:0;
	_dwRetVal |= MmddConfig::GetInstance().IsMediumPeriodicMessageEnabled(NULL)?FLogMessage::VL_MEDIUM_PERIODIC_MESSAGE:0;
	_dwRetVal |= MmddConfig::GetInstance().IsHighPeriodicMessageEnabled(NULL)?FLogMessage::VL_HIGH_PERIODIC_MESSAGE:0;

	return _dwRetVal;
}


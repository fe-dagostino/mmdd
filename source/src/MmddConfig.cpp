
#include "../include/MmddConfig.h"
#include "LOGGING/FLogger.h"

USING_NAMESPACE_LOGGING

GENERATE_CLASSINFO( MmddConfig, FSingleton )
IMPLEMENT_SINGLETON( MmddConfig )

#ifdef _WIN32
# define CFG_FILENAME  "mmdd.cfg"
#else
# define CFG_FILENAME  "/etc/mmdd/mmdd.cfg"
#endif


/////////////////////////////////////
  //
   //
    ////////////////////////////////////////////////////////////////////////////////////////////
   //
  //		Disable warning 4715 : "The specified function can potentially not return a value."
 //
//////////
#pragma warning( disable : 4715 )

/////////////////
////	MMDP
//////

WORD        MmddConfig::GetDeviceType       ( BOOL* pbStored ) const
{
  FTRY
  {
    return m_cfg.GetValue( "MMDP", "DEVICE_TYPE", 0, pbStored );
  }
  FCATCH( FConfigFileException, fexception  )
  {
    TRACE_EXCEPTION_CATCH( fexception, GetDeviceType() );
  }

  //Return default value
  return 0xFFFF;
}

FString     MmddConfig::GetDeviceSerial       ( BOOL* pbStored ) const
{
  FTRY
  {
    return m_cfg.GetValue( "MMDP", "DEVICE_SERIAL", 0, pbStored );
  }
  FCATCH( FConfigFileException, fexception  )
  {
    TRACE_EXCEPTION_CATCH( fexception, GetDeviceSerial() );
  }

  //Return default value
  return "not configured.";
}

FString     MmddConfig::GetDeviceDescription   ( BOOL* pbStored ) const
{
  FTRY
  {
    return m_cfg.GetValue( "MMDP", "DEVICE_DESCRIPTION", 0, pbStored );
  }
  FCATCH( FConfigFileException, fexception  )
  {
    TRACE_EXCEPTION_CATCH( fexception, GetDeviceDescription() );
  }

  //Return default value
  return "not configured.";
}

/////////////////
////	GENERAL
//////

WORD		MmddConfig::GetBroadcastPort   ( BOOL* pbStored ) const
{
  FTRY
  {
    return m_cfg.GetValue( "GENERAL", "BROADCAST_PORT", 0, pbStored );
  }
  FCATCH( FConfigFileException, fexception  )
  {
    TRACE_EXCEPTION_CATCH( fexception, GetBroadcastPort() );
  }

  //Return default value
  return 11111;
}


/////////////////
////	GENERAL
//////

FString		MmddConfig::GetLogServerAddress( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "GENERAL", "LOG_SERVER", 0, pbStored );
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, GetLogServerAddress() );
	}

	//Return default value
	return "127.0.0.1";
}

WORD		MmddConfig::GetLogServerPort( BOOL* pbStored ) const
{
	FTRY
	{
		return (WORD)m_cfg.GetValue( "GENERAL", "LOG_SERVER", 1, pbStored );
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, GetLogServerPort() );
	}

	//Return default value
	return 55930;
}


FString  MmddConfig::GetLogPath( BOOL* pbStored )
{
	FTRY
	{
		return m_cfg.GetValue( "GENERAL", "LOG_DISK", 0, pbStored );
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, GetLogPath() );
	}

	//Return default value
	return "/var/log/mmdd/";
}

FString  MmddConfig::GetLogPrefix( BOOL* pbStored )
{
	FTRY
	{
		return m_cfg.GetValue( "GENERAL", "LOG_DISK", 1, pbStored );
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, GetLogPrefix() );
	}

	//Return default value
	return "mmdd_";
}

FString  MmddConfig::GetLogExtension( BOOL* pbStored )
{
	FTRY
	{
		return m_cfg.GetValue( "GENERAL", "LOG_DISK", 2, pbStored );
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, GetLogExtension() );
	}

	//Return default value
	return "log";
}

DWORD    MmddConfig::GetLogMaxFiles( BOOL* pbStored )
{
	FTRY
	{
		return m_cfg.GetValue( "GENERAL", "LOG_DISK", 3, pbStored );
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, GetLogMaxFiles() );
	}

	//Return default value 1
	return 1;
}

DWORD    MmddConfig::GetLogSizeLimit( BOOL* pbStored )
{
	FTRY
	{
		return m_cfg.GetValue( "GENERAL", "LOG_DISK", 4, pbStored );
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, GetLogSizeLimit() );
	}

	//Return default value 5 MB
	return (5*1024*1024);
}

////////////////////////////////////////

VOID     MmddConfig::OnInitialize()
{
   m_cfg.SetFileName( CFG_FILENAME );

   FTRY
   {
      m_cfg.LoadFile();
   }
   FCATCH( FConfigFileException, ex )
   {
      switch( ex.GetErrorCode() )
      {
         case FConfigFileException::CFG_FILE_NOT_FOUND :
         {
            THROW_MSG_EXCEPTION( MmddConfigException, FString( 0,"  Filed to open [%s]", (const char*)m_cfg.GetFileName() ), MmddConfigException::CFG_FILE_NOT_FOUND_OR_INACCESSIBLE
, OnInitialize()  )
         }; break;
      }
   }
}

VOID     MmddConfig::OnFinalize()
{

}



BOOL		MmddConfig::IsTraceExceptionEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Log Message","MT_TRACE_EXCEPTION", 0, pbStored ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsTraceExceptionEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetTraceExceptionStatus( BOOL bStatus )
ENTER( SetTraceExceptionStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Log Message","MT_TRACE_EXCEPTION", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddLogMessageFlag( FLogMessage::MT_TRACE_EXCEPTION );
			else
				(GET_LOG_MAILBOX())->SubLogMessageFlag( FLogMessage::MT_TRACE_EXCEPTION );
		}
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetTraceExceptionStatus() );
	}
EXIT

BOOL		MmddConfig::IsCatchExceptionEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Log Message","MT_CATCH_EXCEPTION", 0, pbStored  ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsCatchExceptionEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetCatchExceptionStatus( BOOL bStatus )
ENTER( SetCatchExceptionStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Log Message","MT_CATCH_EXCEPTION", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddLogMessageFlag( FLogMessage::MT_CATCH_EXCEPTION );
			else
				(GET_LOG_MAILBOX())->SubLogMessageFlag( FLogMessage::MT_CATCH_EXCEPTION );
		}
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetCatchExceptionStatus() );
	}
EXIT

BOOL		MmddConfig::IsAssertionFailureEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Log Message","MT_ASSERTION_FAILURE", 0, pbStored  ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsAssertionFailureEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetAssertionFailureStatus( BOOL bStatus ) 
ENTER( SetAssertionFailureStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Log Message","MT_ASSERTION_FAILURE", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddLogMessageFlag( FLogMessage::MT_ASSERTION_FAILURE );
			else
				(GET_LOG_MAILBOX())->SubLogMessageFlag( FLogMessage::MT_ASSERTION_FAILURE );
		}
	}
	FCATCH( FConfigFileException, fexception )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetAssertionFailureStatus() );
	}
EXIT

BOOL		MmddConfig::IsErrorInfoEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Log Message","MT_ERROR_INFO", 0, pbStored ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsErrorInfoEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetErrorInfoStatus( BOOL bStatus )
ENTER( SetErrorInfoStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Log Message","MT_ERROR_INFO", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddLogMessageFlag( FLogMessage::MT_ERROR_INFO );
			else
				(GET_LOG_MAILBOX())->SubLogMessageFlag( FLogMessage::MT_ERROR_INFO );
		}
	}
	FCATCH( FConfigFileException, fexception )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetErrorInfoStatus() );
	}
EXIT

BOOL		MmddConfig::IsLoggingInfoEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Log Message","MT_LOGGING_INFO", 0, pbStored ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsLoggingInfoEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetLoggingInfoStatus( BOOL bStatus )
ENTER( SetLoggingInfoStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Log Message","MT_LOGGING_INFO", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddLogMessageFlag( FLogMessage::MT_LOGGING_INFO );
			else
				(GET_LOG_MAILBOX())->SubLogMessageFlag( FLogMessage::MT_LOGGING_INFO );
		}
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetLoggingInfoStatus() );
	}
EXIT

BOOL		MmddConfig::IsVerboseInfoEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Log Message","MT_VERBOSE_INFO", 0, pbStored ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsVerboseInfoEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetVerboseInfoStatus( BOOL bStatus )
ENTER( SetVerboseInfoStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Log Message","MT_VERBOSE_INFO", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddLogMessageFlag( FLogMessage::MT_VERBOSE_INFO );
			else
				(GET_LOG_MAILBOX())->SubLogMessageFlag( FLogMessage::MT_VERBOSE_INFO );
		}
	}
	FCATCH( FConfigFileException, fexception )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetVerboseInfoStatus() );
	}
EXIT

BOOL		MmddConfig::IsRawInfoEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Log Message","MT_RAW_INFO", 0, pbStored ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsRawInfoEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetRawInfoStatus( BOOL bStatus )
ENTER( SetRawInfoStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Log Message","MT_RAW_INFO", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddLogMessageFlag( FLogMessage::MT_RAW_INFO );
			else
				(GET_LOG_MAILBOX())->SubLogMessageFlag( FLogMessage::MT_RAW_INFO );
		}
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetRawInfoStatus() );
	}
EXIT

BOOL		MmddConfig::IsEnterMethodEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Log Message","MT_ENTER_METHOD", 0, pbStored ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsEnterMethodEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetEnterMethodStatus( BOOL bStatus )
ENTER( SetEnterMethodStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Log Message","MT_ENTER_METHOD", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddLogMessageFlag( FLogMessage::MT_ENTER_METHOD );
			else
				(GET_LOG_MAILBOX())->SubLogMessageFlag( FLogMessage::MT_ENTER_METHOD );
		}
	}
	FCATCH( FConfigFileException, fexception )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetEnterMethodStatus() );
	}
EXIT

BOOL		MmddConfig::IsExitMethodEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Log Message","MT_EXIT_METHOD", 0, pbStored ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsExitMethodEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetExitMethodStatus( BOOL bStatus )
ENTER( SetExitMethodStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Log Message","MT_EXIT_METHOD", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddLogMessageFlag( FLogMessage::MT_EXIT_METHOD );
			else
				(GET_LOG_MAILBOX())->SubLogMessageFlag( FLogMessage::MT_EXIT_METHOD );
		}
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetExitMethodStatus() );
	}
EXIT

BOOL		MmddConfig::IsStartUpMessageEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Verbosity Level","VL_START_UP_MESSAGE", 0, pbStored ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsStartUpMessageEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetStartUpMessageStatus( BOOL bStatus )
ENTER( SetStartUpMessageStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Verbosity Level","VL_START_UP_MESSAGE", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddVerbosityFlag( FLogMessage::VL_START_UP_MESSAGE );
			else
				(GET_LOG_MAILBOX())->SubVerbosityFlag( FLogMessage::VL_START_UP_MESSAGE );
		}
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetStartUpMessageStatus() );
	}
EXIT

BOOL		MmddConfig::IsShutDownMessageEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Verbosity Level","VL_SHUT_DOWN_MESSAGE", 0, pbStored ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsShutDownMessageEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetShutDownMessageStatus( BOOL bStatus )
ENTER( SetShutDownMessageStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Verbosity Level","VL_SHUT_DOWN_MESSAGE", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddVerbosityFlag( FLogMessage::VL_SHUT_DOWN_MESSAGE );
			else
				(GET_LOG_MAILBOX())->SubVerbosityFlag( FLogMessage::VL_SHUT_DOWN_MESSAGE );
		}
	}
	FCATCH( FConfigFileException, fexception )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetShutDownMessageStatus() );
	}
EXIT

BOOL		MmddConfig::IsLowPeriodicMessageEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Verbosity Level","VL_LOW_PERIODIC_MESSAGE", 0, pbStored ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsLowPeriodicMessageEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetLowPeriodicMessageStatus( BOOL bStatus )
ENTER( SetLowPeriodicMessageStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Verbosity Level","VL_LOW_PERIODIC_MESSAGE", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddVerbosityFlag( FLogMessage::VL_LOW_PERIODIC_MESSAGE );
			else
				(GET_LOG_MAILBOX())->SubVerbosityFlag( FLogMessage::VL_LOW_PERIODIC_MESSAGE );
		}
	}
	FCATCH( FConfigFileException, fexception )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetLowPeriodicMessageStatus() );
	}
EXIT

BOOL		MmddConfig::IsMediumPeriodicMessageEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Verbosity Level","VL_MEDIUM_PERIODIC_MESSAGE", 0, pbStored ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsMediumPeriodicMessageEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetMediumPeriodicMessageStatus( BOOL bStatus )
ENTER( SetMediumPeriodicMessageStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Verbosity Level","VL_MEDIUM_PERIODIC_MESSAGE", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddVerbosityFlag( FLogMessage::VL_MEDIUM_PERIODIC_MESSAGE );
			else
				(GET_LOG_MAILBOX())->SubVerbosityFlag( FLogMessage::VL_MEDIUM_PERIODIC_MESSAGE );
		}
	}
	FCATCH( FConfigFileException, fexception )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetMediumPeriodicMessageStatus() );
	}
EXIT

BOOL		MmddConfig::IsHighPeriodicMessageEnabled( BOOL* pbStored ) const
{
	FTRY
	{
		return m_cfg.GetValue( "Verbosity Level","VL_HIGH_PERIODIC_MESSAGE", 0, pbStored ) == "ENABLED";
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, IsHighPeriodicMessageEnabled() );
	}
	return TRUE;
}

VOID		MmddConfig::SetHighPeriodicMessageStatus( BOOL bStatus )
ENTER( SetHighPeriodicMessageStatus() )
	FTRY
	{
		m_cfg.SetValue( (bStatus)?"ENABLED":"DISABLED", "Verbosity Level","VL_HIGH_PERIODIC_MESSAGE", 0 );
		if ( GET_LOG_MAILBOX() )
		{
			if ( bStatus )
				(GET_LOG_MAILBOX())->AddVerbosityFlag( FLogMessage::VL_HIGH_PERIODIC_MESSAGE );
			else
				(GET_LOG_MAILBOX())->SubVerbosityFlag( FLogMessage::VL_HIGH_PERIODIC_MESSAGE );
		}
	}
	FCATCH( FConfigFileException, fexception  )
	{
		TRACE_EXCEPTION_CATCH( fexception, SetHighPeriodicMessageStatus() );
	}
EXIT


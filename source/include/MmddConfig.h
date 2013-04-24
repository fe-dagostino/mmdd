/***************************************************************************
                         MmddConfig.h -  description
                             -------------------

    copyright            : (C) 2000 by Francesco Emanuele D'Agostino
    email                : fedagostino@gmail.com
 ***************************************************************************/


#ifndef __MMDDCONFIG_H__
#define __MMDDCONFIG_H__

#include "FSingleton.h"
#include "FConfigFileEx.h"
#include "MmddConfigException.h"

USING_NAMESPACE_FED

/**
 * This is a singleton object used to facilitates access 
 * to configuration parameters.
 */
class MmddConfig : public FSingleton
{
   ENABLE_FRTTI( MmddConfig )
   DECLARE_SINGLETON( MmddConfig )
protected:

   /***/
   VOID         OnInitialize();
   /***/
   VOID         OnFinalize();

public:
   /////////////////////////////
   // MMDP
   ///////////////////////

   /**
    * Return device type. This value must be defined at user level assuming
    * to have a different device type for each hardware product. 
    */
   WORD        GetDeviceType       ( BOOL* pbStored ) const;
   /**
    * User defined serial number sent together with discovery replay message.
    */
   FString     GetDeviceSerial     ( BOOL* pbStored ) const;
   /**
    * User defined description to be sent together with discovery replay message.
    */
   FString     GetDeviceDescription( BOOL* pbStored ) const;
   
   /////////////////////////////
   // 
   ///////////////////////
   /**
    */
   WORD		GetBroadcastPort   ( BOOL* pbStored ) const;
   
   /**
    * Interface to be used for binding operation.
    * Defaul value is localohost.
    */
   FString	GetLogServerAddress( BOOL* pbStored ) const;
   /**
    * Return tcp port to be used from MMDD for waiting local or remote connections. 
    */
   WORD		GetLogServerPort   ( BOOL* pbStored ) const;
   
   /**
    */
   FString      GetLogPath( BOOL* pbStored );
   /**
    */
   FString      GetLogPrefix( BOOL* pbStored );
   /**
    */
   FString      GetLogExtension( BOOL* pbStored );
   /**
    */
   DWORD        GetLogMaxFiles( BOOL* pbStored );
   /**
    */
   DWORD        GetLogSizeLimit( BOOL* pbStored );
   
   
   /////////////////////////////
   // LOG INFO
   ///////////////////////
   /***/
   BOOL		IsTraceExceptionEnabled( BOOL* pbStored ) const;
   VOID		SetTraceExceptionStatus( BOOL bStatus );
   /***/
   BOOL		IsCatchExceptionEnabled( BOOL* pbStored ) const;
   VOID		SetCatchExceptionStatus( BOOL bStatus );
   /***/
   BOOL		IsAssertionFailureEnabled( BOOL* pbStored ) const;
   VOID		SetAssertionFailureStatus( BOOL bStatus );
   /***/
   BOOL		IsErrorInfoEnabled( BOOL* pbStored ) const;
   VOID		SetErrorInfoStatus( BOOL bStatus );
   /***/
   BOOL		IsLoggingInfoEnabled( BOOL* pbStored ) const;
   VOID		SetLoggingInfoStatus( BOOL bStatus );
   /***/
   BOOL		IsVerboseInfoEnabled( BOOL* pbStored ) const;
   VOID		SetVerboseInfoStatus( BOOL bStatus );
   /***/
   BOOL		IsRawInfoEnabled( BOOL* pbStored ) const;
   VOID		SetRawInfoStatus( BOOL bStatus );
   /***/
   BOOL		IsEnterMethodEnabled( BOOL* pbStored ) const;
   VOID		SetEnterMethodStatus( BOOL bStatus );
   /***/
   BOOL		IsExitMethodEnabled( BOOL* pbStored ) const;
   VOID		SetExitMethodStatus( BOOL bStatus );


   /////////////////////////////
   // LOG VERBOSITY
   ///////////////////////

   /***/
   BOOL		IsStartUpMessageEnabled( BOOL* pbStored ) const;
   VOID		SetStartUpMessageStatus( BOOL bStatus );
   /***/
   BOOL		IsShutDownMessageEnabled( BOOL* pbStored ) const;
   VOID		SetShutDownMessageStatus( BOOL bStatus );
   /***/
   BOOL		IsLowPeriodicMessageEnabled( BOOL* pbStored ) const;
   VOID		SetLowPeriodicMessageStatus( BOOL bStatus );
   /***/
   BOOL		IsMediumPeriodicMessageEnabled( BOOL* pbStored ) const;
   VOID		SetMediumPeriodicMessageStatus( BOOL bStatus );
   /***/
   BOOL		IsHighPeriodicMessageEnabled( BOOL* pbStored ) const;
   VOID		SetHighPeriodicMessageStatus( BOOL bStatus );


private:
   FConfigFileEx        m_cfg;

};

#endif


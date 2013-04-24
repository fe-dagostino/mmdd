
#include "../include/MmddConfigException.h"


GENERATE_CLASSINFO( MmddConfigException, FException )


MmddConfigException::MmddConfigException( DWORD nErrorCode )
	: FException( FString(), nErrorCode )
{
	m_sErroMsg = FString( MmddConfigException::GetClassName() ) + FString(" -:- ");
	AddMessageCodeToErrorMsg( nErrorCode );
}

MmddConfigException::MmddConfigException( const FString& sErrorMsg, DWORD nErrorCode )
	: FException( FString(), nErrorCode )
{
	m_sErroMsg = FString( MmddConfigException::GetClassName() ) + FString(" -:- ");
	AddMessageCodeToErrorMsg( nErrorCode );
	m_sErroMsg += FString("-:- MSG=") + sErrorMsg;
}

MmddConfigException::~MmddConfigException()
{

}

VOID		MmddConfigException::AddMessageCodeToErrorMsg( DWORD nErrorCode )
{
   switch ( nErrorCode )
   {
      case OBJECT_NOT_INITIALIZED :
            m_sErroMsg += FMAKE_STRING( OBJECT_NOT_INITIALIZED );
      break;
      case OBJECT_ALREADY_INITIALIZED :
            m_sErroMsg += FMAKE_STRING( OBJECT_ALREADY_INITIALIZED );
      break;
      case NOT_ENOUGH_MEMORY :
            m_sErroMsg += FMAKE_STRING( NOT_ENOUGH_MEMORY );
      break;
      case CFG_FILE_NOT_FOUND_OR_INACCESSIBLE :
            m_sErroMsg += FMAKE_STRING( CFG_FILE_NOT_FOUND_OR_INACCESSIBLE );
      break;
   }
}



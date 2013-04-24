/***************************************************************************
                     MmddConfigException.h -  description
                             -------------------

    copyright            : (C) 2000 by Francesco Emanuele D'Agostino
    email                : fedagostino@gmail.com
 ***************************************************************************/

#ifndef __MMDDCONFIGEXCEPTION_H__
#define __MMDDCONFIGEXCEPTION_H__

#include "FException.h"

USING_NAMESPACE_FED

/***/
class MmddConfigException : public FException
{
	ENABLE_FRTTI( MmddConfigException )
public:
	/**
	 * Exceptions types.
	 */
	enum EXCEPTION_CODES {
                                   OBJECT_NOT_INITIALIZED,
                                   OBJECT_ALREADY_INITIALIZED,
                                   NOT_ENOUGH_MEMORY,
                                   CFG_FILE_NOT_FOUND_OR_INACCESSIBLE
                           };

	/**
	 * Constructor.
	 * Require only a single parameter.
	 * @param nErrorCode is and exception type code.
	 */
	MmddConfigException( DWORD nErrorCode );

	/**
	 * Constructor.
	 * Require two parameters.
	 * @sErrorMsg error message
	 * @param nErrorCode is and exception type code.
	 */
	MmddConfigException( const FString& sErrorMsg, DWORD nErrorCode );

	/** Destructor */
	~MmddConfigException();

protected:

private:
	/***/
	VOID		AddMessageCodeToErrorMsg( DWORD nErrorCode );

};


#endif


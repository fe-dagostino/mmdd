/***************************************************************************
                          Mmdd.h -  description
                             -------------------

    copyright            : (C) 2000 by Francesco Emanuele D'Agostino
    email                : fedagostino@gmail.com
 ***************************************************************************/

#ifndef __MMDD_H__
#define __MMDD_H__

#include "FService.h"
#include "MmddConfig.h"

USING_NAMESPACE_FED

/**
 * Make Me Discoverable Daemon/Service
 */
class Mmdd : public FService
{
   ENABLE_FRTTI( Mmdd )
public:

   /***/
   Mmdd( int argc, char* argv[] );
   /***/
   virtual ~Mmdd();

   /***/
   DWORD        GetLogMessageFlags() const;
   /***/
   DWORD        GetVerbosityLevelFlags() const;

private:

};

#endif


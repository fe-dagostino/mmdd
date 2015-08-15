/***************************************************************************
                          MmddEvents.h -  description
                             -------------------

    copyright            : (C) 2000 by Francesco Emanuele D'Agostino
    email                : fedagostino@gmail.com
 ***************************************************************************/

#ifndef __MMDDEVENTS_H__
#define __MMDDEVENTS_H__

#include "FServiceEventInterface.h"

#include "proto/mmd_proto.h"

USING_NAMESPACE_FED

class FSocket;

/**
 * Interface to be implement to handle service events.
 */
class MmddEvents : public FServiceEventInterface
{
   ENABLE_FRTTI( MmddEvents )
protected:
      /**
       * Called before starting service.
       */
      virtual BOOL OnStart( const FService* pService );

      /**
       * This is service thread body.
       */
      virtual VOID OnRun(const FService* pService);

      /**
       * Called before stopping service.
       */
      virtual VOID OnStop(const FService* pService);

      /***/
      virtual VOID OnInterrogate(const FService* pService);

      /***/
      virtual VOID OnPause(const FService* pService);

      /***/
      virtual VOID OnContinue(const FService* pService);

      /***/
      virtual VOID OnShutdown(const FService* pService);

      /***/
      virtual VOID OnNetBindAdd(const FService* pService);

      /***/	
      virtual VOID OnNetBindDisable(const FService* pService);

      /***/
      virtual VOID OnNetBindEnable(const FService* pService);

      /***/
      virtual VOID OnNetBindRemove(const FService* pService);

      /***/
      virtual VOID OnParamChange(const FService* pService);

      /***/
      virtual BOOL OnUserControl(const FService* pService, DWORD dwOpcode);
private:
      /***/
      BOOL	HandleMMDProtoMessage( const MMDP_Header* pHeader, struct sockaddr_in& source );
      /***/
      BOOL      SendMMDPMessage( const MMDP_Header* pHeader, size_t iMsgLen, struct sockaddr_in& source );
private:
      volatile BOOL  m_bExit;
      FSocket*       m_pSocket;

};

#endif


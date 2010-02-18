/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/mincmdproc.cpp,v $
 *
 *  $Revision: 1.1.1.1 $
 *
 *	$Log: not supported by cvs2svn $
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2002 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "mincmdproc.h"

#include "osdep2.h"
#include "rolleicom.h"

#ifdef __linux__
pthread_mutex_t g_aRecursiveMutex;
#endif

// *******************************************************************
// *******************************************************************
// *******************************************************************

extern "C" void _CALLING_CONV _MinCmdProcThreadStarter( void * pData )
{
	minCmdProcessor * pCmdProcessor = (minCmdProcessor *)pData;

	if( pCmdProcessor )
	{
		pCmdProcessor->Run();
	}
}

// *******************************************************************

minCmdProcessor::minCmdProcessor( RolleiCom * pProjector )
: m_bIdle( true ),
  m_bStop( false ),
  m_ulThreadId( (unsigned long )-1 ),
  m_pProjector( pProjector )
{
//    m_pSyncObj = 0; // TODO: new minSyncObject();
}

minCmdProcessor::~minCmdProcessor()
{
	Stop();
	minSleep( 100 );

//	delete m_pSyncObj;
}

bool minCmdProcessor::IsBusy() const
{
	return !IsIdle();
}

bool minCmdProcessor::IsIdle() const
{
	return m_bIdle;
}

bool minCmdProcessor::IsRunning() const
{
	return ( m_ulThreadId != (unsigned long )-1 );
}

bool minCmdProcessor::IsStoped() const
{
	return m_bStop;
}

bool minCmdProcessor::AppendBatchCmd( const string & sCmd )
{
//	minLock aLock( m_pSyncObj );
    m_aSyncObj.lock();

	m_aCmdQueue.push_back( sCmd );

    m_aSyncObj.unlock();
	return true;
}

bool minCmdProcessor::ExecuteRealtimeCmd( const string & sCmd )
{
//	minLock aLock( m_pSyncObj );
    m_aSyncObj.lock();

	m_aCmdQueue.push_front( sCmd );

    m_aSyncObj.unlock();
    return true;
}

bool minCmdProcessor::Start()
{
	if( !IsRunning() )
	{
		m_ulThreadId = minBeginThread( _MinCmdProcThreadStarter, _DEFAULT_STACK_SIZE, this );
	}
	return IsRunning();
}

void minCmdProcessor::Stop()
{
	m_bStop = true;
}

void minCmdProcessor::Run()
{
	m_bStop = false;

	while( !m_bStop	)
	{
		string	sCmd;
		bool	bCmdExecute = false;

		// ** protect the container in multithreading environment
		{
//			minLock aLock( m_pSyncObj );
            m_aSyncObj.lock();

			// ** something to do ?
			if( !m_aCmdQueue.empty() )
			{
				// ** yes --> process the command
				sCmd = m_aCmdQueue.front();
				m_aCmdQueue.pop_front();
				bCmdExecute = true;
			}

            m_aSyncObj.unlock();
		}

		// ** execute the cmd
		if( bCmdExecute )
		{
			if( m_pProjector )
			{
				m_bIdle = false;
				/*bool bOk =*/ m_pProjector->SendCmd( sCmd );
				m_bIdle = true;
			}
		}
		else
		{
			minSleep( 10 );
		}
	}

	// ** if thread has finished, reset thread-id
	m_ulThreadId = (unsigned long)-1;
}


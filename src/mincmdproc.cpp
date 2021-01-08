/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2013 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "mincmdproc.h"

#include "misctools.h"
#include "rolleicom.h"

// *******************************************************************
// *******************************************************************
// *******************************************************************

minCmdProcessor::minCmdProcessor( RolleiCom * pProjector )
: m_bIdle( true ),
  m_bStop( false ),
  m_ulThreadId( (unsigned long )-1 ),
  m_pProjector( pProjector )
{
}

minCmdProcessor::~minCmdProcessor()
{
	Stop();
    msSleepMindia( 100 );
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
    m_aSyncObj.lock();

	m_aCmdQueue.push_back( sCmd );

    m_aSyncObj.unlock();
	return true;
}

bool minCmdProcessor::ExecuteRealtimeCmd( const string & sCmd )
{
    m_aSyncObj.lock();

	m_aCmdQueue.push_front( sCmd );

    m_aSyncObj.unlock();
    return true;
}

bool minCmdProcessor::Start()
{
	if( !IsRunning() )
	{
        start();
        m_ulThreadId = 1;   // dummy id
	}
	return IsRunning();
}

void minCmdProcessor::Stop()
{
	m_bStop = true;
}

void minCmdProcessor::run()
{
	m_bStop = false;

	while( !m_bStop	)
	{
		string	sCmd;
		bool	bCmdExecute = false;

		// ** protect the container in multithreading environment
		{
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
            msSleepMindia( 10 );
		}
	}

	// ** if thread has finished, reset thread-id
	m_ulThreadId = (unsigned long)-1;
}


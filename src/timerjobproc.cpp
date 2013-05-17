/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002-2003 by Michael Neuroth
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 20013 by Michael Neuroth.							       *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "timerjobproc.h"

#include <QTimer>

#include <algorithm>
#include <iostream>

#include <stdio.h>

// *******************************************************************

Job::Job( int iAbsStartTime )
: m_iAbsStartTime( iAbsStartTime )
{
}

Job::~Job()
{
}

bool Job::operator<( const Job & aOther ) const
{
	return m_iAbsStartTime < aOther.m_iAbsStartTime;
}

bool Job::Run()
{
	return false;
}

void Job::Cancel()
{
}

string Job::GetString() const
{
	char sBuffer[256];

	sprintf( sBuffer, "%d", m_iAbsStartTime );

	return sBuffer;
}

int Job::GetStartTime() const
{
	return m_iAbsStartTime;
}

void Job::SetStartTime( int iStartTime )
{
	m_iAbsStartTime = iStartTime;
}

// *******************************************************************

//TimerJobProcessor::TimerJobProcessor( JobContainerT & aJobContainer )
//: m_pTimer( 0 ),
//  m_iRealPlayedTime( 0 ),
//  m_bPaused( false ),
//  m_aJobContainer( aJobContainer )
//{
//	m_aActPosIter = m_aJobContainer.end();

//	m_pTimer = new QTimer();
//	connect( m_pTimer, SIGNAL( timeout() ), this, SLOT( sltTimerEvent() ) );
//}

//TimerJobProcessor::~TimerJobProcessor()
//{
//	delete m_pTimer;
//}
///*
//void TimerJobProcessor::Clear()
//{
//	m_aJobContainer.erase( m_aJobContainer.begin(), m_aJobContainer.end() );
//}

//template <class Type>
//class HandleCompare
//{
//public:
//	bool operator()( minHandle<Type> hVal1, minHandle<Type> hVal2 )
//	{
//		return hVal1->GetStartTime() < hVal2->GetStartTime();
//	}
//};

//void TimerJobProcessor::AddJob( JobHandle hJob )
//{
//	// sort all the jobs after the start time !

//	JobContainerT::iterator aFound = upper_bound( m_aJobContainer.begin(), m_aJobContainer.end(), hJob, HandleCompare<Job>() );
	
//	m_aJobContainer.insert( aFound, hJob );
//}
//*/
//void TimerJobProcessor::Start( int iAbsoluteStartTime )
//{
//	int iNextTimeout = 0;

//	if( m_bPaused )
//	{
//		// restart after pause

//		// to calculate the time where to restart
//		iAbsoluteStartTime = m_iRealPlayedTime;

//		// the iterator has still the correct position !
//	}
//	else
//	{
//		// new start

//		m_aActPosIter = FindPosForStartTime( iAbsoluteStartTime );
//		// reset the real time counter
//		m_iRealPlayedTime = 0;
//	}

//	m_bPaused = false;

//	// is there any offset needed ?
//	if( m_aActPosIter != m_aJobContainer.end() )
//	{
//		iNextTimeout = (*m_aActPosIter)->GetStartTime() - iAbsoluteStartTime;
//	}

//	// to measure the real time
//	m_aRealTime = QTime();			// .start();
//	m_aRealTime.restart();

//	// do we have to start ?
//	//if( iNextTimeout>0 )
//	{
//		iNextTimeout = iNextTimeout>= 0 ? iNextTimeout : 0;
//		m_pTimer->start( iNextTimeout );
//	}
//}

//void TimerJobProcessor::Continue()
//{
//	// continue at that position, where the processor was paused
//	if( m_bPaused )
//	{
//		Start( m_iRealPlayedTime );
//	}
//}

//void TimerJobProcessor::Stop()
//{
//	m_pTimer->stop();

//	m_iRealPlayedTime += m_aRealTime.elapsed();
//}

//void TimerJobProcessor::Pause()
//{
//	m_pTimer->stop();

//	m_bPaused = true;

//	m_iRealPlayedTime += m_aRealTime.elapsed();
//}

//int TimerJobProcessor::GetNextTimeout() /*const*/
//{
//	if( m_aActPosIter != m_aJobContainer.end() )
//	{
//		//JobHandle hActJob = *m_aActPosIter;

//		JobContainerT::const_iterator aNextPosIter = m_aActPosIter + 1;
//		if( aNextPosIter != m_aJobContainer.end() )
//		{
//			// get the real time, maybe the last job took some time !
//			int iRealTime = m_aRealTime.elapsed();	// NaActJob.GetStartTime();
//			return (*aNextPosIter)->GetStartTime() - iRealTime;
//		}
//	}
//	return 0;
//}

//JobContainerT::iterator TimerJobProcessor::FindPosForStartTime( int iAbsoluteStartTime )
//{
//	if( iAbsoluteStartTime>0 )
//	{
//		JobContainerT::iterator aIter = m_aJobContainer.begin();

//		while( (aIter != m_aJobContainer.end()) && ((*aIter)->GetStartTime()<iAbsoluteStartTime) )
//		{
//			++aIter;
//		}

//		if( aIter != m_aJobContainer.end() )
//		{
//			return aIter;
//		}
//	}
//	return m_aJobContainer.begin();
//}

//void TimerJobProcessor::sltTimerEvent()
//{
//	m_pTimer->stop();

//	if( /*IsJobAvailable?*/m_aActPosIter != m_aJobContainer.end() )
//	{
//		JobHandle hJob = *m_aActPosIter;

//		hJob->Run();

//		int iNextTimeout = GetNextTimeout();

//		++m_aActPosIter;

//		if( iNextTimeout > 0 )
//		{
//			iNextTimeout = iNextTimeout>= 0 ? iNextTimeout : 0;
//			m_pTimer->start( iNextTimeout );
//		}
//	}
//	else
//	{
//		// done !
//	}
//}



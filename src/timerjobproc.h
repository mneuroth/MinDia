/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002-2003 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/timerjobproc.h,v $
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
 * Copyright (C) 2002-2003 by Michael Neuroth.							   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#ifndef _TIMERJOBPROCPROC_H
#define _TIMERJOBPROCPROC_H

#include <string>
#include <vector>

using namespace std;

#include <qobject.h>
#include <qdatetime.h>		// for QTime

#include "minhandle.h"

class QTimer;

#define TIME_STAMP(x)	{ cout << x << "actTime: " << (const char *)QTime::currentTime().toString() << endl; }

// *******************************************************************
class Job
{
public:
	Job( int iAbsStartTime );
	virtual ~Job();

	bool operator<( const Job & aOther ) const;

	int  GetStartTime() const;
	void SetStartTime( int iStartTime );

	string GetString() const;

	virtual bool Run();
	virtual void Cancel();

	virtual bool Read( istream & aStream ) = 0;
	virtual bool Write( ostream & aStream ) const = 0;

private:
	int		m_iAbsStartTime;
};

// *******************************************************************
typedef minHandle<Job>		JobHandle;
typedef vector<JobHandle>	JobContainerT;

// *******************************************************************
class TimerJobProcessor : public QObject
{
	Q_OBJECT

public:
	TimerJobProcessor( JobContainerT & aJobContainer );
	~TimerJobProcessor();

//	void Clear();

//	void AddJob( JobHandle hJob );

	void Start( int iAbsoluteStartTime = 0 );
	void Continue();
	void Pause();
	void Stop();

public slots:

	void sltTimerEvent();

private:
	JobContainerT::iterator FindPosForStartTime( int iAbsoluteStartTime );
	int						GetNextTimeout() /*const*/;
	
	QTimer *					m_pTimer;
	QTime						m_aRealTime;
	int							m_iRealPlayedTime/*SinceLastStopOrPause*/;	// allready played time without pause times
	bool						m_bPaused;
	JobContainerT &				m_aJobContainer;
	JobContainerT::iterator		m_aActPosIter;
};

#endif

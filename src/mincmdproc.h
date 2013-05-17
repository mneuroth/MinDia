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

#ifndef _MINCMDPROC_H
#define _MINCMDPROC_H

#include <string>
#include <list>

#include <QMutex>
#include <QThread>

using namespace std;

class RolleiCom;
class minSyncObject;

// *******************************************************************
/** Implements a command processor. 
    Executes commands in an own thread.
  */
class minCmdProcessor : public QThread
{
public:
	minCmdProcessor( RolleiCom * pProjector );
	~minCmdProcessor();

	bool IsBusy() const;
	bool IsIdle() const;
	bool IsRunning() const;
	bool IsStoped() const;

	bool AppendBatchCmd( const string & sCmd );
	bool ExecuteRealtimeCmd( const string & sCmd );

	bool Start();
	void Stop();

	void run();

private:
	// ** helper methods **

	// ** data **
	bool				m_bIdle;
	bool				m_bStop;
	unsigned long		m_ulThreadId;
	list<string>		m_aCmdQueue;
	RolleiCom *			m_pProjector;	// no owner !
    QMutex              m_aSyncObj;
};

#endif

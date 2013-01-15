/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/minisound.h,v $
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

#ifndef _MINISOUND_H
#define _MINISOUND_H

#include "soundinfo.h"

#include <qdatetime.h>		// for QTime

#include <QThread>

// *******************************************************************
/** a very simple interface for playing sounds.
  */
class miniSound : public QThread
{
	enum MciCmdType { _MCI_NONE, _MCI_PLAY, _MCI_STOP, _MCI_PAUSE, _MCI_CONTINUE };

    Q_OBJECT

public:
    miniSound( const char * sWavFileName = "" );
	~miniSound();

	bool SetWavFile( const char * sWavFileName );

	void SetSoundInfo( SoundInfoContainer *	pSoundInfoContainer );

	bool IsOk() const;
	bool IsSilent() const;		// means 'dummy' wav file silent.wav is 'playing'

	bool IsPlaying() const;
	bool IsPause() const;

	int  GetPositionInMS() const;
	int  GetTotalLengthInMS() const;

	bool Start( int iAbsStartTimeInMS );
	bool Pause();
	bool Continue();
	bool Stop();

	void run();

public slots:
    void sltTotalTimeChanged(qint64 val);

private:
	// ** help methods **
	int  GetPositionInMSImpl() const;
	bool StartPlayImpl( int iStartPosInMs = -1, int iStopPosInMs = -1,
						int iFadeInStartPosInMS = -1, int iFadeInLengthInMS = 0,
						int iFadeOutStartPosInMS = -1, int iFadeOutLengthInMS = 0 );
	bool CloseSound();

	bool IsCallingThreadMciThread() const;
	bool IsThreadRunning() const;
	void StartThread();
	void StopThread();
	bool DoStopThread();

	bool IsFileChangeNeeded( int iNextRelStopPos, int iSilentOffset ) const;

	bool CheckFile( const char * sFileName );
	bool CheckIfIsSilentFile( const char * sFileName );

	bool CheckSoundError( int iErrorNo ) const;


	// ** temp-data **
	bool							m_bIsOk;
	bool							m_bReadError;
	bool							m_bIsSilent;
	bool							m_bSilentPause;
	int								m_iOpenCount;
	int								m_iSilentTimer;
	QTime							m_aSilentStartTime;
	bool							m_bStop;
	int								m_iTotalTimeInMS;
	int								m_iAbsStartTimeInMS;
	MciCmdType						m_iMciCmdId;
	unsigned long					m_ulMciThreadID;
	unsigned long					m_ulThreadID;
	SoundInfoContainer *			m_pSoundInfoContainer;	// no owner !
	SoundInfoContainer::iterator	m_aIterator;
};

#endif

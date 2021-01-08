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

#ifndef _MINISOUND_H
#define _MINISOUND_H

#include "soundinfo.h"

#include <QElapsedTimer>
#include <QThread>
#include <QString>

#if defined(Q_OS_ANDROID)
#undef _WITH_PHONON
#else
#if QT_VERSION < 0x050000
#define _WITH_PHONON
#else
#undef _WITH_PHONON
#define _WITH_MULTIMEDIA
#endif
#endif

#if defined(Q_OS_WASM)
#undef _WITH_MULTIMEDIA
#endif

#ifdef _WITH_PHONON
#include <phonon/AudioOutput>   // use Multimedia for Qt5
#include <phonon/MediaObject>
#endif

#ifdef _WITH_MULTIMEDIA
#include <QMediaPlayer>
#endif

// *******************************************************************
/** a very simple interface for playing sounds.
  * inspired by mci inteface under windows and
  * introduced when Qt::phonon was not available...
  */
class miniSound : public QThread
{
	enum MciCmdType { _MCI_NONE, _MCI_PLAY, _MCI_STOP, _MCI_PAUSE, _MCI_CONTINUE };

    Q_OBJECT

public:
    miniSound( const QString & sWavFileName = "" );
	~miniSound();

    bool SetWavFile( const QString & sWavFileName );

	void SetSoundInfo( SoundInfoContainer *	pSoundInfoContainer );

	bool IsOk() const;
	bool IsSilent() const;		// means 'dummy' wav file silent.wav is 'playing'

	bool IsPlaying() const;
	bool IsPause() const;

	int  GetPositionInMS() const;
	int  GetTotalLengthInMS() const;
    void AsyncGetTotalLengthForFile( const QString & sWavFileName, QWidget * pRequester );

	bool Start( int iAbsStartTimeInMS );
	bool Pause();
	bool Continue();
	bool Stop();

    bool CloseSound();
    bool StartPlayImpl( int iStartPosInMs = -1, int iStopPosInMs = -1,
                        int iFadeInStartPosInMS = -1, int iFadeInLengthInMS = 0,
                        int iFadeOutStartPosInMS = -1, int iFadeOutLengthInMS = 0 );

    void run();

public slots:
    void sltTotalTimeChanged(qint64 val);

private:
	// ** help methods **
	int  GetPositionInMSImpl() const;

	bool IsCallingThreadMciThread() const;
	bool IsThreadRunning() const;
	void StartThread();
	void StopThread();
	bool DoStopThread();

    void PostCloseSound();
    void PostSetWavFile( const QString & sFileName );
    void PostStartPlayImpl( int iStartPosInMs = -1 );

	bool IsFileChangeNeeded( int iNextRelStopPos, int iSilentOffset ) const;

    bool CheckFile( const QString & sFileName );
    bool CheckIfIsSilentFile( const QString & sFileName );

	// ** temp-data **
	bool							m_bIsOk;
	bool							m_bReadError;
	bool							m_bIsSilent;
	bool							m_bSilentPause;
	int								m_iOpenCount;
	int								m_iSilentTimer;
    QElapsedTimer   				m_aSilentStartTime;
	bool							m_bStop;
	int								m_iTotalTimeInMS;
	int								m_iAbsStartTimeInMS;
	MciCmdType						m_iMciCmdId;
    Qt::HANDLE  					m_ulMciThreadID;
	SoundInfoContainer *			m_pSoundInfoContainer;	// no owner !
	SoundInfoContainer::iterator	m_aIterator;
    QWidget *                       m_pRequester;
    QString                         m_sSoundFile;
#ifdef _WITH_PHONON
    Phonon::MediaObject *           m_pPlayer;
#endif
#ifdef _WITH_MULTIMEDIA
    QMediaPlayer *                  m_pPlayer;
#endif
};

#endif

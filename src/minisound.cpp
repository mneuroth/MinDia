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

#include "minisound.h"

#include "misctools.h"

#include "timelineview.h"       //for: GetSoundLengthEvent

#include <QFile>
#include <QEvent>
#include <QApplication>

// *******************************************************************
// *******************************************************************
// *******************************************************************

#define CHANGE_NEEDED -2

miniSound::miniSound( const QString & sWavFileName )
: m_bIsOk( false ),
  m_bReadError( true ),
  m_bIsSilent( false ),
  m_bSilentPause( false ),
  m_iOpenCount( 0 ),
  m_iSilentTimer( 0 ),
  m_bStop( false ),
  m_iTotalTimeInMS( 0 ),
  m_iAbsStartTimeInMS( 0 ),
  m_iMciCmdId( _MCI_NONE ),
  m_ulMciThreadID( 0 ),
  m_pSoundInfoContainer( 0 ),
  m_pRequester( 0 )
#ifdef _WITH_PHONON
  , m_pPlayer( 0 )
#endif
{
#ifdef _WITH_PHONON
    m_pPlayer = Phonon::createPlayer(Phonon::MusicCategory);

    connect(m_pPlayer,SIGNAL(totalTimeChanged(qint64)),this,SLOT(sltTotalTimeChanged(qint64)));
#endif
    SetWavFile( sWavFileName );
}

miniSound::~miniSound()
{    
	StopThread();

	CloseSound();

#ifdef _WITH_PHONON
    delete m_pPlayer;
#endif
}

void miniSound::sltTotalTimeChanged(qint64 val)
{
    m_iTotalTimeInMS = (int)val;

    if( m_pRequester!=0 && m_iTotalTimeInMS>=0 )
    {
        Stop();

        GetSoundLengthEvent * pEvent = new GetSoundLengthEvent(m_sSoundFile,0);
        pEvent->SetSoundLength(m_iTotalTimeInMS);
        QApplication::postEvent(m_pRequester,pEvent);

        m_pRequester = 0;
    }
}

bool miniSound::SetWavFile( const QString & sWavFileName )
{
	if( CheckFile( sWavFileName ) && !IsSilent() )
	{
        m_sSoundFile = sWavFileName;

#ifdef _WITH_PHONON
        m_pPlayer->setCurrentSource(Phonon::MediaSource(sWavFileName));
#endif

        m_iOpenCount++;

        m_bReadError = false;

        return m_bReadError;
	}
	else
	{
		m_bReadError = true;
	}

	if( IsSilent() )
	{
		m_aSilentStartTime = QTime();
		m_iSilentTimer = 0;
		m_bReadError = false;
	}

    return false;
}

void miniSound::AsyncGetTotalLengthForFile( const QString & sWavFileName, QWidget * pRequester )
{
    if( m_pRequester==0 )
    {
        m_pRequester = pRequester;
        SetWavFile( sWavFileName );
        // ggf. sound ausschalten
        Start(999999999);     // get the length for the last sound file (new files will always appended !)
    }
}

void miniSound::SetSoundInfo( SoundInfoContainer *	pSoundInfoContainer )
{
	m_pSoundInfoContainer = pSoundInfoContainer;
}

bool miniSound::IsOk() const
{
	return m_bIsOk && (!m_bReadError);
}

bool miniSound::IsSilent() const
{
	return m_bIsSilent;
}

bool miniSound::IsPlaying() const
{
#ifdef _WITH_PHONON
    return m_pPlayer->state()==Phonon::PlayingState;
#else
    return false;
#endif
}

bool miniSound::IsPause() const
{
#ifdef _WITH_PHONON
    return m_pPlayer->state()==Phonon::PausedState;
#else
    return false;
#endif
}

int  miniSound::GetPositionInMS() const
{
	return m_iTotalTimeInMS + GetPositionInMSImpl();
}

int  miniSound::GetTotalLengthInMS() const
{
	if( IsSilent() )
	{
		return 1000*60*60;		// return 1 h
	}
	else
	{
#ifdef _WITH_PHONON
        return m_pPlayer->totalTime();
#else
        return 0;
#endif
    }
}

bool miniSound::Start( int iAbsStartTimeInMS )
{
    // ** reset total play time **
	m_iTotalTimeInMS = 0;

	m_iAbsStartTimeInMS = iAbsStartTimeInMS;

	// ** if there is more than one file, use thread for sound-playing
	if( m_pSoundInfoContainer )
	{
		StartThread();

		return true;
	}
	else
	{
		return StartPlayImpl();
	}
}

bool miniSound::CheckFile( const QString & sFileName )
{
	m_bIsSilent = CheckIfIsSilentFile( sFileName );

    m_bIsOk = (m_bIsSilent || QFile::exists( sFileName ));

	return m_bIsOk;
}

int  miniSound::GetPositionInMSImpl() const
{
	if( IsSilent() )
	{
		int iRet = m_iSilentTimer;
		if( !m_bSilentPause )
		{
			iRet += ((miniSound *)this)->m_aSilentStartTime.elapsed();
		}
		return iRet;
	}
	else
	{
#ifdef _WITH_PHONON
        return m_pPlayer->currentTime();
#else
        return 0;
#endif
    }
}

bool miniSound::StartPlayImpl( int /*iStartPosInMs*/, int /*iStopPosInMs*/,
                               int /*iFadeInStartPosInMS*/, int /*iFadeInLengthInMS*/,
                               int /*iFadeOutStartPosInMS*/, int /*iFadeOutLengthInMS*/ )
{
	if( IsOk() )
	{
        // ** fade in / fade out is not supported any more !

        if( IsSilent() )
		{
			// ** for silent there is nothing to do, except starting the timer...
			m_aSilentStartTime.restart();
		}
		else
		{
#ifdef _WITH_PHONON
            m_pPlayer->play();
#endif
		}

		return true;
	}

	return false;
}

bool miniSound::Pause()
{
	if( IsOk() )
	{
		if( IsSilent() )
		{
			m_bSilentPause = true;
			m_iSilentTimer += m_aSilentStartTime.elapsed();
		}
		else
		{
			if( IsCallingThreadMciThread() )
			{
#ifdef _WITH_PHONON
                m_pPlayer->pause();
#endif
			}
			else
			{
				m_iMciCmdId = _MCI_PAUSE;
			}
		}

		return true;
	}

	return false;
}

bool miniSound::Continue()
{
	if( IsOk() )
	{
		if( IsSilent() )
		{
			m_bSilentPause = false;
			m_aSilentStartTime.restart();
		}
		else
		{
			if( IsCallingThreadMciThread() )
			{
#ifdef _WITH_PHONON
                m_pPlayer->play();
#endif
			}
			else
			{
				m_iMciCmdId = _MCI_CONTINUE;
			}
		}

		return true;
	}

	return false;
}

bool miniSound::Stop()
{
	if( IsOk() )
	{
		if( IsSilent() )
		{
			// ** there is nothing to do...
			m_bSilentPause = false;
			m_iSilentTimer = 0;
		}
		else
		{
			// Attention: mci-commands can only be send from the 
			//   thread which opens/initiates the mci-stream
			if( IsCallingThreadMciThread() )
			{
#ifdef _WITH_PHONON
                m_pPlayer->stop();
#endif
			}
			else
			{
				m_iMciCmdId = _MCI_STOP;
			}
		}

		// important: stop the play-thread !!!
		if( IsThreadRunning() )
		{
			StopThread();
		}

		return true;
	}

	return false;
}

bool miniSound::CloseSound()
{
	if( IsSilent() )
	{
		// ** there is nothing to do...
		m_bSilentPause = false;
		m_iSilentTimer = 0;
	}
	else
	{
		if( m_iOpenCount>0 )
		{
#ifdef _WITH_PHONON
            m_pPlayer->stop();
#endif
			m_iOpenCount--;
		}
		else
		{
            // dieses sollte nie auftreten !
			cerr << "BUG: closing sound !" << endl;
		}
	}

	return true;
}

bool miniSound::IsCallingThreadMciThread() const
{
    return ( QThread::currentThreadId() == m_ulMciThreadID );
}

bool miniSound::IsThreadRunning() const
{
    return m_ulMciThreadID != 0;
}

void miniSound::StopThread()
{
	if( IsThreadRunning() )
	{
		m_bStop = true;

		// ** give the thread some time to stop ...
        QThread::msleep( 100 );
	}
}

void miniSound::StartThread()
{
	// ** stop the old thread...
	StopThread();

	// wait for the stoped thread 
    while( m_ulMciThreadID != 0 )
	{
        QThread::msleep( 1 );
	}

    start();
}

void miniSound::run()
{
#ifdef _WITH_PHONON
    qRegisterMetaType<Phonon::MediaSource>( "MediaSource" );
#endif

    bool bStopedInThread = false;

	m_bStop = false;
	m_iMciCmdId = _MCI_NONE;		// bugfix: 25.1.2003

	// to identify if the calling thread is the mci-thread
    m_ulMciThreadID = QThread::currentThreadId();

	if( m_pSoundInfoContainer )
	{
		int iNextRelStopPos = CHANGE_NEEDED;	// ** force file change for first item !
		int iDeltaTime = 0;

		bool bIsFirstDia = true;
		int iOffsetTime = 0;
		m_aIterator = m_pSoundInfoContainer->GetItemForAbsPlayPos( m_iAbsStartTimeInMS, iOffsetTime );

        while( !m_bStop )
		{
			if( IsFileChangeNeeded( iNextRelStopPos, iOffsetTime ) )
			{
				// ** we need the offset time for the first dia,
				// ** because of the silent-mode handling
				// ** (to calculate real play position of the silent simulation)
				if( !bIsFirstDia )
				{
					iOffsetTime = 0;
				}
				// ** are we finished ?
				if( iNextRelStopPos >= 0 /*!= -1*/ )		// since 25.1.2003
				{
					// ** stop and close old wav-file
					//Stop();
					CloseSound();
					bStopedInThread = true;
				}

				// ** get the next sound item and start playing
                if( m_aIterator!=m_pSoundInfoContainer->end() )
				{
					// ** open new wav-file
					minHandle<SoundInfo> hItem = *m_aIterator;
                    SetWavFile( ToQString(hItem->GetFileName()) );

					// ** calculate time values
					m_iTotalTimeInMS += iDeltaTime;
					iNextRelStopPos = hItem->GetStopPos();

					if( iNextRelStopPos == -1 )
					{
                        iNextRelStopPos = hItem->GetTotalLength(); //GetTotalLengthInMS();
                    }

					iDeltaTime = iNextRelStopPos - hItem->GetStartPos();

					// ** and start playing
					StartPlayImpl( hItem->GetStartPos()+iOffsetTime,
								   hItem->GetStopPos(),
								   hItem->GetFadeInStartPos(),
								   hItem->GetFadeInLength(),
								   hItem->GetFadeOutStartPos(),
								   hItem->GetFadeOutLength() );
					bStopedInThread = false;
					bIsFirstDia = false;

					++m_aIterator;
				}
                else if( m_sSoundFile.length()>0 )
                {
                    // hier zur Bestimmung der sound file length abspielen starten
                    SetWavFile( m_sSoundFile );
                    StartPlayImpl();
                    bStopedInThread = false;
                    bIsFirstDia = false;
                    m_bStop = true;
                }
				else
				{
					m_bStop = true;
				}
			}

            QThread::msleep( 10 );

			switch( m_iMciCmdId )
			{
				case _MCI_NONE:
					break;

				case _MCI_PLAY :
					Start( 0 );		// dummy
					m_iMciCmdId = _MCI_NONE;
					break;
				case _MCI_STOP :
					Stop();
					m_iMciCmdId = _MCI_NONE;
					break;
				case _MCI_PAUSE :
					Pause();
					m_iMciCmdId = _MCI_NONE;
					break;
				case _MCI_CONTINUE :
					Continue();
					m_iMciCmdId = _MCI_NONE;
					break;

				default:
					m_iMciCmdId = _MCI_NONE;
			}
		}
	}

	if( !bStopedInThread )
	{
		CloseSound();		// since 25.1.2003
	}

	// ** if thread has finished, reset thread-id
    m_ulMciThreadID = 0;
}

bool miniSound::IsFileChangeNeeded( int iNextRelStopPos, int iSilentOffset ) const
{
	if( iNextRelStopPos == CHANGE_NEEDED )
	{
		return true;
	}

	int iRelActPos = GetPositionInMSImpl();

	// ** in silent-mode you have to adjust the act pos with the offset
	if( IsSilent() )
	{
		iRelActPos += iSilentOffset;
	}

	//cout << "IsFileChangeNeeded " << iRelActPos << " >= " << iNextRelStopPos << endl;
	return iRelActPos >= iNextRelStopPos;
}

bool miniSound::CheckIfIsSilentFile( const QString & sFileName )
{
	// ** silent modus: empty string or silent.wav
    return (sFileName.length()==0) || (sFileName.compare("silent.wav")==0);
}

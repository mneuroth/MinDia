/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/minisound.cpp,v $
 *
 *  $Revision: 1.4 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.3  2004/02/16 19:47:25  min
 *	Fixes for Borland C++
 *	
 *	Revision 1.2  2004/01/18 23:38:18  min
 *	Disabling debug outputs.
 *	
 *	Revision 1.1.1.1  2003/08/15 16:38:21  min
 *	Initial checkin of MinDia Ver. 0.97.1
 *	
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

#define _async


#include "osdep2.h"

#include "minisound.h"

#include <stdio.h>
#include <stdlib.h>

#include <qfile.h>

#include <phonon/AudioOutput>
#include <phonon/MediaObject>

#if defined( _MSC_VER ) || defined( __BORLANDC__ ) || defined( __MINGW32__ )
//#define _WINDOWS_SOUND
#define _UNIX_SOUND		// for debugging TESTS
#else
//#ifdef __linux__
#define _UNIX_SOUND
//#endif
#endif

#ifdef _WINDOWS_SOUND
#include <windows.h>
#include <mmsystem.h>

int mciSendStringX( const miniSound * pSound, const char * lpszCommand, char * lpszReturnString, unsigned int cchReturn, void * hwndCallback )
{
    return ::mciSendStringA( lpszCommand, lpszReturnString, cchReturn, (HWND)hwndCallback );
}

int mciGetErrorStringX( const miniSound * pSound, int iErrorNo, char * sBuffer, int iBufferLength )
{
    return ::mciGetErrorStringA( iErrorNo, sBuffer, iBufferLength );
}

#endif //_WINDOWS_SOUND

#ifdef _UNIX_SOUND

#define DEBUG_OUT(x)
//#define DEBUG_OUT(x)	x

//#include "wavfile.h"
//#include "mp3file.h"

#define _PLAY		"play"
#define _RESUME		"resume"
#define _PAUSE		"pause"
#define _STOP 		"stop"
#define _OPEN		"open"
#define _CLOSE		"close"
#define _STATUS		"status"	// sub-cmd: mode-->playing,paused
								// sub-cmd: sound length
								// sub-cmd: sound position
#define _SUB_POSITION	"position"
#define _SUB_LENGTH		"length"
#define _SUB_MODE		"mode"
#define _SUB_FROM		" from "
#define _SUB_TO			" to "

// ** new command, only for linux
#define _FADE_IN		"fade_in"
#define _FADE_OUT		"fade_out"

#if defined(__linux__) || defined(__APPLE__)
typedef int MCIERROR;
#endif

// ** some dummy implementions for linux **
#include <QtGlobal>

//Wave 	aWave;
//Mp3File aMp3;
Phonon::MediaObject * g_pPlayer = 0;

bool g_bIsWavFile = 1;
int  g_isLoaded = 0;

int mciSendStringX( const miniSound * pSound, const char * lpszCommand, char * lpszReturnString, unsigned int cchReturn, void * /*hwndCallback*/ )
{
    DEBUG_OUT( cout << "SOUND: " << lpszCommand << endl; )
	if( strncmp( lpszCommand, _PLAY, strlen( _PLAY ) ) == 0 )
	{
		DEBUG_OUT( cout << _PLAY << endl; )

        if( g_pPlayer )
        {
//            g_pPlayer->seek(35000);
            g_pPlayer->play();
        }
//		int iFromPos = 0;
//		int iToPos = -1;

//		if( strstr( lpszCommand, _SUB_FROM ) != 0 )
//		{
//			if( strstr( lpszCommand, _SUB_TO ) != 0 )
//			{
//				sscanf( lpszCommand, "play sound from %d to %d", &iFromPos, &iToPos );
//			}
//			else
//			{
//				sscanf( lpszCommand, "play sound from %d", &iFromPos );
//			}

//			DEBUG_OUT( cout << "play pos " << iFromPos << " " << iToPos << endl; )
//		}

//		if( g_bIsWavFile )
//		{
//			/*int iOk =*/ aWave.initPlay();
//			aWave.setCurrPlayPosInSeconds( ((double)iFromPos)*0.001 );
//			aWave.setMaxPlayPosInSeconds( ((double)iToPos)*0.001 );
//			aWave.startAsyncReadBuffer();
//			aWave.playInThread();
//		}
//		else
//		{
//			// mp3 handling
//			//aMp3.startPlay( ((double)iFromPos)*0.001, ((double)iToPos)*0.001 );
//            aMp3.playInThread( ((double)iFromPos)*0.001, ((double)iToPos)*0.001 );
//		}
	}
	else if( strncmp( lpszCommand, _FADE_IN, strlen( _FADE_IN ) ) == 0 )
	{
		DEBUG_OUT( cout << _FADE_IN << endl; )

		int iFromPosInMS = 0;
		int iLengthInMS = -1;

		sscanf( lpszCommand, "fade_in from %d length %d", &iFromPosInMS, &iLengthInMS );

		double dFrom = ((double)iFromPosInMS)*0.001;
		double dLength = ((double)iLengthInMS)*0.001;

// TODO --> AudioOutput.Volume + g_pPlayer->seek(time)
/*
        if( g_bIsWavFile )
		{
			aWave.setFadeInInfos( dFrom, dLength );
		}
		else
		{
			// mp3 handling
			// not supported yet !
		}
*/
	}
	else if( strncmp( lpszCommand, _FADE_OUT, strlen( _FADE_OUT ) ) == 0 )
	{
		DEBUG_OUT( cout << _FADE_OUT << endl; )

		int iFromPosInMS = 0;
		int iLengthInMS = -1;

		sscanf( lpszCommand, "fade_out from %d length %d", &iFromPosInMS, &iLengthInMS );

		double dFrom = ((double)iFromPosInMS)*0.001;
		double dLength = ((double)iLengthInMS)*0.001;
/*
		if( g_bIsWavFile )
		{
			aWave.setFadeOutInfos( dFrom, dLength );
		}
		else
		{
			// mp3 handling
			// not supported yet !
		}
*/
	}
	else if( strncmp( lpszCommand, _RESUME, strlen( _RESUME ) ) == 0 )
	{
		DEBUG_OUT( cout << _RESUME << endl; )

        if( g_pPlayer )
        {
            g_pPlayer->play();
        }
//		if( g_bIsWavFile )
//		{
//			aWave.resumePlay();
//			aWave.playInThread();
//		}
//		else
//		{
//			// mp3 handling
//			aMp3.resumePlay();
//		}
	}
	else if( strncmp( lpszCommand, _PAUSE, strlen( _PAUSE ) ) == 0 )
	{
		DEBUG_OUT( cout << _PAUSE << endl; )

        if( g_pPlayer )
        {
            g_pPlayer->pause();
        }
//		if( g_bIsWavFile )
//		{
//			aWave.pausePlay();
//		}
//		else
//		{
//			// mp3 handling
//			aMp3.pausePlay();
//		}
	}
	else if( strncmp( lpszCommand, _STOP, strlen( _STOP ) ) == 0 )
	{
		DEBUG_OUT( cout << _STOP << endl; )

        if( g_pPlayer )
        {
            //int iTime = g_pPlayer->totalTime();     // TODO gulp
            g_pPlayer->stop();
        }
//		if( g_bIsWavFile )
//		{
//			aWave.stopPlay();
//		}
//		else
//		{
//			// mp3 handling
//			aMp3.stopPlay();
//		}
	}
	else if( strncmp( lpszCommand, _OPEN, strlen( _OPEN ) ) == 0 )
	{
		DEBUG_OUT( cout << _OPEN << endl; )

		char sBuf[512];
		sscanf( lpszCommand, "open %s alias sound", sBuf );
		DEBUG_OUT( cout << "file: " << sBuf << endl; )

        //if( g_pPlayer )
        //{
        //    delete g_pPlayer;
        //    g_pPlayer = 0;
        //}
        if( g_pPlayer==0 )
        {
//            g_pPlayer = Phonon::createPlayer(Phonon::MusicCategory,
//                                             Phonon::MediaSource(QString(sBuf)));
            g_pPlayer = Phonon::createPlayer(Phonon::MusicCategory);
            bool ok = pSound->connect(g_pPlayer,SIGNAL(totalTimeChanged(qint64)),pSound,SLOT(sltTotalTimeChanged(qint64)));
            //cout << "CONNECT: "<< ok << endl;
        }
        else
        {
//            g_pPlayer->setCurrentSource(Phonon::MediaSource(QString(sBuf)));
        }
        g_pPlayer->setCurrentSource(Phonon::MediaSource(QString(sBuf)));

//see:        http://de.softuses.com/145419

//        // is the file a wav or mp3 file?
//		// look for the extension (*.wav or *.mp3)
//		if( strstr( sBuf, ".wav" ) != 0 )
//		{
//			g_bIsWavFile = true;
//		}
//		else if( strstr( sBuf, ".mp3" ) != 0 )
//		{
//			g_bIsWavFile = false;
//		}
//		else
//		{
//			// every other file formats will be treated as wav file...
//			g_bIsWavFile = true;
//		}

//// min todo --> nur fuer tests ...
//#ifdef _async
//		int iRet = 0;
//		if( g_bIsWavFile )
//		{
//			iRet = aWave.openWaveAsync( sBuf );
//		}
//		else
//		{
//			// mp3 handling
//			iRet = aMp3.openFile( sBuf );
//		}
//#else
//		if( !g_isLoaded )
//		{
//			g_isLoaded = 1;

//			if( g_bIsWavFile )
//			{
//				iRet = aWave.openWave( sBuf );
//			}
//			else
//			{
//				// mp3 handling
//				iRet = aMp3.openFile( sBuf );
//			}
//		}
//#endif
//		if( iRet != 0 )
//		{
//			return -5;		// error opening file !
//		}
	}
	else if( strncmp( lpszCommand, _CLOSE, strlen( _CLOSE ) ) == 0 )
	{
		DEBUG_OUT( cout << _CLOSE << endl; )

        if( g_pPlayer )
        {
            g_pPlayer->stop();
        }
//		if( g_bIsWavFile )
//		{
//			aWave.stopPlay();
//		}
//		else
//		{
//			// mp3 handling
//			aMp3.stopPlay();
//		}
	}
	else if( strncmp( lpszCommand, _STATUS, strlen( _STATUS ) ) == 0 )
	{
		char sBuf[512];

//		DEBUG_OUT( cout << _STATUS << endl; )
		if( strstr( lpszCommand, _SUB_POSITION ) != 0 )
		{
//			DEBUG_OUT( cout << "SUB: " << _SUB_POSITION << endl; )

            if( g_pPlayer )
            {
                qint32 iTime = g_pPlayer->currentTime();
                sprintf( sBuf, "%d", (int)iTime );
            }
//			// ** + 10 for rounding errors ... (double --> int)
//			if( g_bIsWavFile )
//			{
//				sprintf( sBuf, "%d", (int)(aWave.getCurrPlayPosInSeconds()*1000.0) );
//			}
//			else
//			{
//				// mp3 handling
//				sprintf( sBuf, "%d", (int)(aMp3.getCurrPlayPosInSeconds()*1000.0) );
//			}
            strncpy( lpszReturnString, sBuf, cchReturn );

            DEBUG_OUT( cout << "return: " << lpszReturnString << endl; )
		}
		else if( strstr( lpszCommand, _SUB_LENGTH ) != 0 )
		{
			DEBUG_OUT( cout << "SUB: " << _SUB_LENGTH << endl; )

            if( g_pPlayer )
            {
                int i = 0;
                qint64 iTime = 0;
                //while( iTime==0 )
                {
                    iTime = g_pPlayer->totalTime();
                    //if( iTime==0 )
                    //{
                    //    minSleep(1);
                    //    i++;
                    //}
                }
                //cout << "*** LENGTH i=" << i << endl;
//                qint64 iTime = g_pPlayer->totalTime();
//                int iTimeSec = iTime/1000;
                sprintf( sBuf, "%d", (int)iTime );
            }
//			if( g_bIsWavFile )
//			{
//				sprintf( sBuf, "%d", (int)(aWave.getTotalPlayTimeInSeconds()*1000.0) );
//			}
//			else
//			{
//				// mp3 handling
//				sprintf( sBuf, "%d", (int)(aMp3.getTotalPlayTimeInSeconds()*1000.0) );
//			}
			strncpy( lpszReturnString, sBuf, cchReturn );

			DEBUG_OUT( cout << "return: " << lpszReturnString << endl; )
		}
		else if( strstr( lpszCommand, _SUB_MODE ) != 0 )
		{
			DEBUG_OUT( cout << "SUB: " << _SUB_MODE << endl; )
/*
			if( g_bIsWavFile )
			{                
				if( aWave.getActivePlay() )
				{
					strncpy( lpszReturnString, "playing", cchReturn );
				}
				else if( aWave.getCurrPlayPos()>0 )
				{
					strncpy( lpszReturnString, "paused", cchReturn );
				}
				else
				{
					strncpy( lpszReturnString, "", cchReturn );
				}                
			}
			else
			{
				// mp3 handling
				if( aMp3.getActivePlay() )
				{
					strncpy( lpszReturnString, "playing", cchReturn );
				}
				else if( aMp3.getCurrPlayPos()>0 )
				{
					strncpy( lpszReturnString, "paused", cchReturn );
				}
				else
				{
					strncpy( lpszReturnString, "", cchReturn );
				}
			}
*/
			DEBUG_OUT( cout << "return: " << lpszReturnString << endl; )
		}
		else
		{
			DEBUG_OUT( cout << "WARNING: sub command not handled ! " << lpszCommand << endl; )
			return -2;
		}
	}
	else
	{
		DEBUG_OUT( cout << "WARNING: command not handled ! --> " << lpszCommand << endl; )
		return -1;
	}

	return 0;
}

int mciGetErrorStringX( int /*iErrorNo*/, char * /*sBuffer*/, int /*iBufferLength*/ )
{
	return 0;
}

#endif //_UNIX_SOUND

// *******************************************************************
// *******************************************************************
// *******************************************************************

#ifdef _with_min_threads
extern "C" void _CALLING_CONV _miniSoundThreadStarter( void * pData )
{
	miniSound * pSound = (miniSound *)pData;

	if( pSound )
	{
		pSound->run();
	}
}
#endif

// *******************************************************************

#define _MAXLENGTH 512

#define CHANGE_NEEDED -2

miniSound::miniSound( const char * sWavFileName )
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
  m_ulMciThreadID( (unsigned long)-1 ),
  m_ulThreadID( (unsigned long)-1 ),
  m_pSoundInfoContainer( 0 ),
  m_pRequester( 0 )
{
	SetWavFile( sWavFileName );
    // gulp
//    connect(g_pPlayer,SIGNAL(totalTimeChanged(qint64)),this,SLOT(sltTotalTimeChanged(qint64)));
}

miniSound::~miniSound()
{
	StopThread();

	CloseSound();
}

// TODO
#include <QEvent>
#include <QApplication>
#include "mindiawindow.h"
#include "timelineview.h"

void miniSound::sltTotalTimeChanged(qint64 val)
{
    cout << "TOTAL TIME " << val << endl;
/*
    QEvent * pUserEvent = new QEvent((QEvent::Type)((int)QEvent::User+1)); //new MyCheckReloadEvent();
    QWidget * pWidget = GetMainWindow();
    QApplication::postEvent(pWidget,pUserEvent);
*/
    m_iTotalTimeInMS = (int)val;

    if( m_pRequester!=0 && m_iTotalTimeInMS>=0 )
    {
        Stop();

        GetSoundLengthEvent * pEvent = new GetSoundLengthEvent((const char *)m_sSoundFile.toAscii(),0);
        pEvent->SetSoundLength(m_iTotalTimeInMS);
        QApplication::postEvent(m_pRequester,pEvent);

        m_pRequester = 0;
    }
}

bool miniSound::SetWavFile( const char * sWavFileName )
{
	char lpszReturnString[_MAXLENGTH];
	char sBuffer[_MAXLENGTH];

	if( CheckFile( sWavFileName ) && !IsSilent() )
	{
        m_sSoundFile = sWavFileName;

		//MCIERROR iRet = mciSendStringX( "close sound", lpszReturnString, _MAXLENGTH, NULL );
		//CheckSoundError( (int)iRet );

#ifdef Q_OS_WIN32
        sprintf( sBuffer, "open \"%s\" alias sound", sWavFileName );    // type waveaudio
#else
        sprintf( sBuffer, "open %s alias sound", sWavFileName );    // type waveaudio
#endif
        MCIERROR iRet = mciSendStringX( this, sBuffer, lpszReturnString, _MAXLENGTH, NULL );
		m_iOpenCount++;

		m_bReadError = !CheckSoundError( (int)iRet );

//        sprintf( sBuffer, "play sound" );
//        sprintf( sBuffer, "status sound length" );
//        iRet = mciSendStringX( sBuffer, lpszReturnString, _MAXLENGTH, NULL );
//        CheckSoundError( (int)iRet );

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

void miniSound::AsyncGetTotalLengthForFile( const char * sWavFileName, QWidget * pRequester )
{
    if( m_pRequester==0 )
    {
        m_pRequester = pRequester;
        SetWavFile( sWavFileName );
        // ggf. sound ausschalten
        Start(0);
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
	char lpszReturnString[_MAXLENGTH];

	if( IsSilent() )
	{
		// ** silent can be played endless
		if( !m_bSilentPause )
		{
			strcpy( lpszReturnString, "playing" );
		}
	}
	else
	{
        MCIERROR iRet = mciSendStringX( this, "status mode", lpszReturnString, _MAXLENGTH, NULL );
		CheckSoundError( (int)iRet );
	}

	return (strcmp( lpszReturnString, "playing" ) == 0);
}

bool miniSound::IsPause() const
{
	char lpszReturnString[_MAXLENGTH];

	if( IsSilent() )
	{
		if( m_bSilentPause )
		{
			strcpy( lpszReturnString, "paused" );
		}
	}
	else
	{
        MCIERROR iRet = mciSendStringX( this, "status mode", lpszReturnString, _MAXLENGTH, NULL );
		CheckSoundError( (int)iRet );
	}

	return (strcmp( lpszReturnString, "paused" ) == 0);
}

int  miniSound::GetPositionInMS() const
{
	return m_iTotalTimeInMS + GetPositionInMSImpl();
}

int  miniSound::GetTotalLengthInMS() const
{
	char lpszReturnString[_MAXLENGTH];

	if( IsSilent() )
	{
		return 1000*60*60;		// return 1 h
	}
	else
	{
        /*
        char sBuffer[_MAXLENGTH];

        bool bPlaying = IsPlaying();

        if( !bPlaying )
        {
            // TODO --> 15.1.2013: starte sound erst bevor length abgefragt werden kann, ansonsten funktioniert es unter linux nicht mehr !
            sprintf( sBuffer, "play sound" );
            MCIERROR iRet1 = mciSendStringX( this, sBuffer, lpszReturnString, _MAXLENGTH, NULL );
        }
        */
        MCIERROR iRet = mciSendStringX( this, "status sound length", lpszReturnString, _MAXLENGTH, NULL );
		CheckSoundError( (int)iRet );
        /*
        if( !bPlaying )
        {
            sprintf( sBuffer, "stop sound" );   // gulp
            MCIERROR iRet2 = mciSendStringX( this, sBuffer, lpszReturnString, _MAXLENGTH, NULL );
        }
        */
    }

	return atoi( lpszReturnString );
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

bool miniSound::CheckFile( const char * sFileName )
{
	m_bIsSilent = CheckIfIsSilentFile( sFileName );

// min todo
	m_bIsOk = (m_bIsSilent || FileUtilityObj::ExistsFile( sFileName ) /*QFile::exists( QString( sFileName ) )*/);

	return m_bIsOk;
}

int  miniSound::GetPositionInMSImpl() const
{
	char lpszReturnString[_MAXLENGTH];

	if( IsSilent() )
	{
		int iRet = m_iSilentTimer;
		if( !m_bSilentPause )
		{
			iRet += ((miniSound *)this)->m_aSilentStartTime.elapsed();
		}
		//cout << "GetActPos: " << iRet << endl;
		return iRet;
	}
	else
	{
        MCIERROR iRet = mciSendStringX( this, "status sound position", lpszReturnString, _MAXLENGTH, NULL );
		CheckSoundError( (int)iRet );
    }

	return atoi( lpszReturnString );
}

bool miniSound::StartPlayImpl( int iStartPosInMs, int iStopPosInMs,
							   int iFadeInStartPosInMS, int iFadeInLengthInMS,
							   int iFadeOutStartPosInMS, int iFadeOutLengthInMS )
{
	if( IsOk() )
	{
		char lpszReturnString[_MAXLENGTH];
		char sBuffer[_MAXLENGTH];

		strcpy( sBuffer, "" );

		// ** fade in / fade out is only supported under linux !
#if defined(__linux__) || defined(__APPLE__)
        sprintf( sBuffer, "fade_in from %d length %d", iFadeInStartPosInMS, iFadeInLengthInMS );
		{
            MCIERROR iRet = mciSendStringX( this, sBuffer, lpszReturnString, _MAXLENGTH, NULL );
			CheckSoundError( (int)iRet );
		}
		sprintf( sBuffer, "fade_out from %d length %d", iFadeOutStartPosInMS, iFadeOutLengthInMS );
		{
            MCIERROR iRet = mciSendStringX( this, sBuffer, lpszReturnString, _MAXLENGTH, NULL );
			CheckSoundError( (int)iRet );
		}
#endif

		if( (iStartPosInMs<0) && (iStopPosInMs<0) )
		{
			sprintf( sBuffer, "play sound" );
		}
		else
		{
			if( iStartPosInMs<0 )
			{
				iStartPosInMs = 0;
			}

			if( (iStopPosInMs<0) || (iStopPosInMs<iStartPosInMs) )
			{
				sprintf( sBuffer, "play sound from %d", iStartPosInMs );
			}
			else
			{
				sprintf( sBuffer, "play sound from %d to %d", iStartPosInMs, iStopPosInMs );
			}
		}

		if( IsSilent() )
		{
			// ** for silent there is nothing to do, except starting the timer...
			m_aSilentStartTime.restart();
		}
		else
		{
            MCIERROR iRet = mciSendStringX( this, sBuffer, lpszReturnString, _MAXLENGTH, NULL );
			CheckSoundError( (int)iRet );
		}

		return true;
	}

	return false;
}

bool miniSound::Pause()
{
	if( IsOk() )
	{
		char lpszReturnString[_MAXLENGTH];

		if( IsSilent() )
		{
			m_bSilentPause = true;
			m_iSilentTimer += m_aSilentStartTime.elapsed();
		}
		else
		{
			if( IsCallingThreadMciThread() )
			{
                MCIERROR iRet = mciSendStringX( this, "pause sound", lpszReturnString, _MAXLENGTH, NULL );
				CheckSoundError( (int)iRet );
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
		char lpszReturnString[_MAXLENGTH];

		if( IsSilent() )
		{
			m_bSilentPause = false;
			m_aSilentStartTime.restart();
		}
		else
		{
			if( IsCallingThreadMciThread() )
			{
                MCIERROR iRet = mciSendStringX( this, "resume sound", lpszReturnString, _MAXLENGTH, NULL );
				CheckSoundError( (int)iRet );
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
		char lpszReturnString[_MAXLENGTH];

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
                MCIERROR iRet = mciSendStringX( this, "stop sound", lpszReturnString, _MAXLENGTH, NULL );
				CheckSoundError( (int)iRet );
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
	char lpszReturnString[_MAXLENGTH];

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
            MCIERROR iRet = mciSendStringX( this, "close sound", lpszReturnString, _MAXLENGTH, NULL );
			CheckSoundError( (int)iRet );
			m_iOpenCount--;
		}
		else
		{
			// min todo gulp sollte nie auftreten !
			cerr << "BUG: closing sound !" << endl;
		}
	}

	return true;
}

bool miniSound::IsCallingThreadMciThread() const
{
	return ( minGetThreadId() == m_ulMciThreadID );
}

bool miniSound::IsThreadRunning() const
{
	return m_ulThreadID != (unsigned long)-1;
}

void miniSound::StopThread()
{
	if( IsThreadRunning() )
	{
		m_bStop = true;

		// ** give the thread some time to stop ...
		minSleep( 100 );
	}
}

void miniSound::StartThread()
{
	// ** stop the old thread...
	StopThread();

	// wait for the stoped thread 
	while( m_ulThreadID != (unsigned long)-1 )
	{
		minSleep( 5 );
	}

#ifdef _with_min_threads
	m_ulThreadID = minBeginThread( _miniSoundThreadStarter, _DEFAULT_STACK_SIZE, this );
#else
    start();
    m_ulThreadID = 1;   // dummy id    
#endif    
}

void miniSound::run()
{
	bool bStopedInThread = false;

	m_bStop = false;
	m_iMciCmdId = _MCI_NONE;		// bugfix: 25.1.2003

	// to identify if the calling thread is the mci-thread
	m_ulMciThreadID = minGetThreadId();

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
					SetWavFile( hItem->GetFileName().c_str() );

					// ** calculate time values
					m_iTotalTimeInMS += iDeltaTime;
					iNextRelStopPos = hItem->GetStopPos();

					if( iNextRelStopPos == -1 )
					{
						iNextRelStopPos = GetTotalLengthInMS();
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
// TODO --> hier zur bestimmung der sound file length abspielen starten
                    SetWavFile( m_sSoundFile.toAscii() );
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

			minSleep( 10 );

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
	m_ulMciThreadID = (unsigned long)-1; 
	m_ulThreadID = (unsigned long)-1;
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

bool miniSound::CheckSoundError( int iErrorNo ) const
{
	if( iErrorNo != 0 )
	{
		char sBuffer[512];

		sprintf( sBuffer, "unknown error id=%d", iErrorNo );

        mciGetErrorStringX( iErrorNo, sBuffer, 512 );

		cerr << "Sound error: " << sBuffer << endl;

		return false;
	}
	return true;
}

bool miniSound::CheckIfIsSilentFile( const char * sFileName )
{
	// ** silent modus: empty string or silent.wav
	return (strlen( sFileName )==0) || (strcmp( sFileName, "silent.wav" )==0);
}


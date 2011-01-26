/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/mp3file.cpp,v $
 *
 *  $Revision: 1.7 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.6  2005/12/26 16:16:43  Michael
 *	added support to compile file under windows and bugfix for playing multiple mp3 files under linux
 *	
 *	Revision 1.5  2004/02/16 19:45:58  min
 *	Fixes for Borland C++
 *	
 *	Revision 1.4  2004/01/18 23:37:05  min
 *	Unused variable commented
 *	
 *	Revision 1.3  2003/10/26 22:37:20  min
 *	Bugfix
 *	
 *	Revision 1.2  2003/10/26 17:28:55  min
 *	Implemented function to calculate mp3-file length for linux.
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

#include "mp3file.h"

#include "osdep2.h"

#include "read_mp3.cpp"

#include <QProcess>
#include <QSettings>


#ifdef _with_min_threads
extern "C" void _CALLING_CONV _mp3SoundThreadStarter( void * pData )
{
	Mp3File * pMp3 = (Mp3File *)pData;

	if( pMp3 )
	{
        pMp3->run();
	}
}
#endif

#define _MAX_LEN  512

inline int hours( double dSeconds )
{
	return (int)(dSeconds / (60.0*60.0));
}

inline int minutes( double dSeconds )
{
	return (int)((dSeconds-60.0*hours( dSeconds )) / (60.0));
}

inline int seconds( double dSeconds )
{
	return (int)(dSeconds-60.0*hours( dSeconds )-60.0*60.0*minutes( dSeconds ));
}

//static bool IsWhiteSpace( char ch )
//{
//	if( (ch == ' ') || (ch == '\t') )
//	{
//		return true;
//	}
//	return false;
//}
//
//static int ParseOptions( const string & sOptions, vector<string> & aOptionsVec )
//{
//	aOptionsVec.erase( aOptionsVec.begin(), aOptionsVec.end() );
//
//	string sTemp;
//	unsigned long iCount = 0;
//
//	while( iCount < sOptions.length() )
//	{
//		if( IsWhiteSpace( sOptions[iCount] ) )
//		{
//			if( sTemp.length()>0 )
//			{
//				aOptionsVec.push_back( sTemp );
//			}
//			sTemp = "";
//		}
//		else
//		{
//			sTemp += sOptions[iCount];
//		}
//		iCount++;
//	}
//	if( sTemp.length()>0 )
//	{
//		aOptionsVec.push_back( sTemp );
//	}
//
//	return aOptionsVec.size();
//}

// *******************************************************************
// *******************************************************************
// *******************************************************************

Mp3File::Mp3File()
	: m_pProcess( 0 ),
      m_iLengthInSeconds( -1 ),
	  m_iPlayModus( 0 ),
      m_dStartPosInSeconds( 0 ),
      m_dStopPosInSeconds( 0 )
{
#if defined(__linux__)
    m_sMp3Player = "madplay";               	// "mpg123" or "afplay" (Mac)
	m_sMp3Options = "--no-tty-control -v";
#elif defined(__APPLE__)
    m_sMp3Player = "afplay";
    m_sMp3Options = "";
#endif
}

Mp3File::~Mp3File()
{
    delete m_pProcess;
}

double Mp3File::GetStartTime() const
{
    return m_dStartPosInSeconds;
}

double Mp3File::GetStopTime() const
{
    return m_dStopPosInSeconds;
}

int Mp3File::openFile( char * sFileName )
{
	m_sFileName = sFileName;

	// reset the length information of the file
    m_iLengthInSeconds = -1;

	return 0;
}

void Mp3File::stopPlay()
{
    DoKill();

	m_iPlayModus = 0;
}

void Mp3File::pausePlay()
{
    DoKill();
}

void Mp3File::resumePlay()
{
// TODO --> einfach neu starten an der zuletzt gespeicherten position/frame no    
    DoKill();

	m_iPlayModus = 1;
}

void Mp3File::DoKill()
{
    if( m_pProcess )
    {
        m_pProcess->kill();
        m_pProcess->waitForFinished();
        delete m_pProcess;
        m_pProcess = 0;
    }
}

void Mp3File::startPlay( double dStartPosInSeconds, double /*dStopPosInSeconds*/ )
{
	// start the timer for the play-time... (important: start timer before fork() !!!)
	m_aPlayTime.start();

    DoKill();
    
    QStringList aArgs;
//    QString sProgramName(/*m_sMp3Player.c_str()*/"afplay");
//    QString sProgramName(/*m_sMp3Player.c_str()*/"mpg123");
//    QString sProgramName(/*m_sMp3Player.c_str()*/"/usr/bin/afplay");
//    QString sProgramName(/*m_sMp3Player.c_str()*/"/opt/local/bin/mpg123");
    QString sProgramName(m_sMp3Player);
    if( dStartPosInSeconds>0 && sProgramName.indexOf("mpg123")!=-1 )
    {
        MP3Header aMP3Header;

        bool boolIsMP3 = aMP3Header.ReadMP3Information( (const char *)m_sFileName );
        if(boolIsMP3)
        {
           QString sFrames;
           sFrames = sFrames.setNum((int)(aMP3Header.getNumberOfFrames()*(int)dStartPosInSeconds/aMP3Header.getLengthInSeconds()));
           aArgs << "-k" << sFrames;
        }
    }
    if( m_sMp3Options!="<none>" )
    {
        aArgs << m_sMp3Options;
    }

    aArgs << m_sFileName;

// TODO test:
//    QString sProgramName("/bin/sh");
//    string s = string("mpg123 ")+m_sFileName;
//    aArgs << "-c" << s.c_str();

    m_pProcess = new QProcess(0);

// TODO test:
//    QProcessEnvironment aEnv = QProcessEnvironment::systemEnvironment();
//    aEnv.insert("PATH", "/opt/local/bin:"+aEnv.value("PATH"));
//    m_pProcess->setProcessEnvironment(aEnv);

//    QStringList aLst = m_pProcess->systemEnvironment();
//    aLst.replaceInStrings(QRegExp("^PATH=(.*)", Qt::CaseInsensitive), "PATH=\\1:/opt/local/bin");
//    m_pProcess->setEnvironment(aLst);

//    QProcessEnvironment aPE = m_pProcess->processEnvironment();
//    m_pProcess->setEnvironment(QProcessEnvironment::systemEnvironment().toStringList());

//    QStringList aLst2 = aPE.toStringList();

//    for(int j=0; j<aLst2.length(); j++ )
//    {
//        cout << "j=" << j << " " << (const char *)aLst2[j] << endl;
//    }


    m_pProcess->start(sProgramName,aArgs);

//    int iState = m_pProcess->state();
//    cout << "STARTED: " << iState << endl;

    if( m_pProcess->state()!=QProcess::NotRunning )
    {
        m_iPlayModus = 1;
    }
    else
    {
        m_iPlayModus = 0;
    }

}

double Mp3File::getCurrPlayPosInSeconds()
{
    return m_aPlayTime.elapsed()/1000;
}

double Mp3File::getTotalPlayTimeInSeconds()
{
    if( m_iLengthInSeconds<0 )
	{
		MP3Header aMP3Header;

		bool boolIsMP3 = aMP3Header.ReadMP3Information( (const char *)m_sFileName );
		if(boolIsMP3)
		{
            m_iLengthInSeconds = aMP3Header.intLength;
		}
		else
		{
            m_iLengthInSeconds = 0;
		}
	}
    return (double)m_iLengthInSeconds;
}

int Mp3File::getActivePlay()
{
	return m_iPlayModus;
}

int Mp3File::getCurrPlayPos()
{
	return m_aPlayTime.elapsed();
}

void Mp3File::playInThread( double dStartPosInSeconds, double dStopPosInSeconds )
{
	UpdateSettingsFromIniFile();

    m_dStartPosInSeconds = dStartPosInSeconds;
    m_dStopPosInSeconds = dStopPosInSeconds;

#ifdef _with_min_threads
	/*unsigned long ulThreadID =*/ minBeginThread( _mp3SoundThreadStarter, _DEFAULT_STACK_SIZE, this );
#else 
    start();
#endif    
}

void Mp3File::UpdateSettingsFromIniFile()
{
    QSettings aSettings;
   
    m_sMp3Player = aSettings.value("Mp3/PlayerName",QString()).toString();
    m_sMp3Options = aSettings.value("Mp3/PlayerOptions",QString()).toString();    
}

void Mp3File::run()
{
    startPlay(GetStartTime(),GetStopTime());
}

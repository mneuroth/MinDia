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
 *  $Revision: 1.3 $
 *
 *	$Log: not supported by cvs2svn $
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

#ifndef _MSC_VER

#include "mp3file.h"

#include "osdep2.h"

#include "iniconst.h"
#include "miniini.h"

#include "read_mp3.cpp"

#include <vector>

#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>

extern "C" void _CALLING_CONV _mp3SoundThreadStarter( void * pData )
{
	Mp3File * pMp3 = (Mp3File *)pData;

	if( pMp3 )
	{
		pMp3->startPlay();
	}
}

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

static bool IsWhiteSpace( char ch )
{
	if( (ch == ' ') || (ch == '\t') )
	{
		return true;
	}
	return false;
}

static int ParseOptions( const string & sOptions, vector<string> & aOptionsVec )
{
	aOptionsVec.erase( aOptionsVec.begin(), aOptionsVec.end() );

	string sTemp;
	unsigned long iCount = 0;

	while( iCount < sOptions.length() )
	{
		if( IsWhiteSpace( sOptions[iCount] ) )
		{
			if( sTemp.length()>0 )
			{
				aOptionsVec.push_back( sTemp );
			}
			sTemp = "";
		}
		else
		{
			sTemp += sOptions[iCount];
		}
		iCount++;
	}
	if( sTemp.length()>0 )
	{
		aOptionsVec.push_back( sTemp );
	}

	return aOptionsVec.size();
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

Mp3File::Mp3File()
	: m_iPID( 0 ),
	  m_pIniDB( 0 ),
	  m_iLengthInSeconds( -1 )
{
	m_iPlayModus = 0;
	m_sMp3Player = "madplay";
	m_sMp3Options = "--no-tty-control -v";
}

Mp3File::~Mp3File()
{
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
	if( m_iPID>0 )
	{
		int iOk = kill( m_iPID, SIGKILL );
	}

	m_iPlayModus = 0;
}

void Mp3File::pausePlay()
{
	if( m_iPID>0 )
	{
		kill( m_iPID, SIGSTOP );
	}
}

void Mp3File::resumePlay()
{
	if( m_iPID>0 )
	{
		kill( m_iPID, SIGCONT );
	}

	m_iPlayModus = 1;
}

void Mp3File::startPlay( double dStartPosInSeconds, double dStopPosInSeconds )
{
	m_iPID = fork();

	if( m_iPID==0 )
	{
		signal( SIGTERM, SIG_DFL );
		setpgrp();

		char sProgName[_MAX_LEN];
		char sName[_MAX_LEN];
		char sStart[_MAX_LEN];
		char sLength[_MAX_LEN];
		char sFade[_MAX_LEN];
		char sOption1[_MAX_LEN];
		char sOption2[_MAX_LEN];
		int iCount = 0;
		unsigned long i = 0;
		char * sArgs[7];

		sprintf( sProgName, "%s", m_sMp3Player.c_str() );	// "mpg123"
		sArgs[iCount] = sProgName;
		iCount++;

		vector<string> aOptionsVec;
		ParseOptions( m_sMp3Options, aOptionsVec );
		for( i=0; i<aOptionsVec.size(); i++ )
		{
			sArgs[iCount] = (char *)aOptionsVec[i].c_str();
			iCount++;
		}

/*
		sprintf( sProgName, "%s", "madplay" );	// "mpg123"
		sArgs[iCount] = sProgName;
		iCount++;

		sprintf( sOption1, "%s", "--no-tty-control" );
		sArgs[iCount] = sOption1;
		iCount++;

		sprintf( sOption2, "%s", "-v" );
		sArgs[iCount] = sOption2;
		iCount++;
*/
		sprintf( sName, "%s", m_sFileName.c_str() );
		sArgs[iCount] = sName;
		iCount++;

		sArgs[iCount] = 0;
		iCount++;

		int iOk = execvp( sArgs[0], sArgs );

		printf( "child iRet=%d!\n", iOk );
	}
	else
	{
		//int iStatus = 0;
		//waitpid( m_iPID, &iStatus, WNOHANG );

		printf( "child PID=%d! this=%x\n", m_iPID, (void *)this );

		if( m_iPID != -1 )
		{
			m_iPlayModus = 1;
		}
		else
		{
			m_iPlayModus = 0;
		}
	}
}

double Mp3File::getCurrPlayPosInSeconds()
{
	// min todo
	return 0;
}

double Mp3File::getTotalPlayTimeInSeconds()
{
	if( m_iLengthInSeconds<0 )
	{
		MP3Header aMP3Header;

		bool boolIsMP3 = aMP3Header.ReadMP3Information( m_sFileName.c_str() );
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
	//return m_pPlayer->control;
	return m_iPlayModus;
}

int Mp3File::getCurrPlayPos()
{
	// min todo
	return 0;
}

void Mp3File::playInThread()
{
	UpdateSettingsFromIniFile();

	unsigned long ulThreadID = minBeginThread( _mp3SoundThreadStarter, _DEFAULT_STACK_SIZE, this );
}

void Mp3File::SetIniDB( MiniIniDB * pIniDB )
{
	m_pIniDB = pIniDB;
}

void Mp3File::UpdateSettingsFromIniFile()
{
	if( m_pIniDB )
	{
		m_sMp3Player = m_pIniDB->GetValue( c_sActPlayerKey );
		m_sMp3Options = m_pIniDB->GetValue( c_sActPlayerOptionsKey );
	}
}

#endif

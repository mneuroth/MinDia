/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/mp3file.h,v $
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

#ifndef _MP3FILE_H
#define _MP3FILE_H

#ifndef _MSC_VER

#include <string>

using namespace std;

#include <stdio.h>

struct player;

class MiniIniDB;

class Mp3File
{
public:
	Mp3File();
	~Mp3File();

	int openFile( char * sFileName );
	void startPlay( double dStartPosInSeconds = 0, double dStopPosInSeconds = 0 );
	void stopPlay();
	void pausePlay();
	void resumePlay();
	double getCurrPlayPosInSeconds();
	double getTotalPlayTimeInSeconds();
	int getActivePlay();
	int getCurrPlayPos();
	void playInThread();

	void SetIniDB( MiniIniDB * pIniDB );

private:
	void UpdateSettingsFromIniFile();

	string 			m_sFileName;
	string			m_sMp3Player;
	string			m_sMp3Options;
	int				m_iPlayModus;
	int				m_iPID;
	MiniIniDB *		m_pIniDB;			// no owner !
};

#endif

#endif

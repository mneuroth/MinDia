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
 *  $Revision: 1.4 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.3  2004/02/16 19:45:42  min
 *	Fixes for Borland C++
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

#ifndef _MP3FILE_H
#define _MP3FILE_H

#include <QTime>
#include <QString>
#include <QThread>
    
class QProcess;

class Mp3File : public QThread
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
    void playInThread( double dStartPosInSeconds = 0, double dStopPosInSeconds = 0 );

    double GetStartTime() const;
    double GetStopTime() const;

    virtual void run();
    
private:
	void UpdateSettingsFromIniFile();
    void DoKill();

    QProcess *      m_pProcess;
	QTime			m_aPlayTime;
	QString 		m_sFileName;
    QString			m_sMp3Player;
    QString			m_sMp3Options;
    int				m_iLengthInSeconds;
	int				m_iPlayModus;
    double          m_dStartPosInSeconds;
    double          m_dStopPosInSeconds;
};

#endif

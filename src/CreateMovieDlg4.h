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

#ifndef _CREATEMOVIEDLG_H
#define _CREATEMOVIEDLG_H

#include "ui_CreateMovieDlg4.h"

#include <QDialog>
#include <QProcess>

class DocumentAndControler;

// *************************************************************************

class CreateMovieDlg4 : public QDialog
{
	Q_OBJECT

public:
    CreateMovieDlg4(DocumentAndControler * pDocControler, double dTotalTimeMS, QWidget *parent = 0);
    virtual ~CreateMovieDlg4();

    void saveSettings();
    void restoreSettings();

    void setOutputSizeIfPossible( unsigned long ulWidth, unsigned long ulHeight );

public slots:
    int  sltImageRatioSelected( const QString & );
    void sltSetFfmpegDefaultPath();
    void sltSelectOutputDirectory();
    void sltCreateImages();
    void sltCreateAVI();
    void sltAddSound();
    void sltMakeShow();
    void sltKillProcess();
    void sltDeleteTempFiles( bool bAsk = true );
    void sltChangeMjpegToolsDirectory();
    void sltSetToTemp();
    void sltImagesPerSecondsChanged( const QString & );
    void sltImageExtensionChanged( const QString & );
    void sltImageOutputChanged( const QString & );
    void sltMovieOutputChanged( const QString & );
    void sltMovieExtensionChanged( const QString & );
    void sltOutputDirectoryChanged( const QString & );
    void sltReadyReadStdOutput();
    void sltReadyReadStdError();
    void sltProcessFinished( int exitCode, QProcess::ExitStatus exitStatus );
    void sltEnable();
    void sltDisable();

private:
    void RunCommands( const QString & sCmds );
    void ProcessCommands( const QString & sCmdsIn );
    void CreateProcess( bool bAutoDelete );
    void KillProcess();
    void DeleteProcess();
    void UpdateCmds();
    void EnableDialog( bool value );
    QString GetFfmpegDefaultPath() const;

    Ui::CreateMovieDlg4     ui;

    double					m_dTotalTimeMS;
    DocumentAndControler *	m_pDocControler;	// not an owner !
    QProcess *              m_pProcess;
    bool                    m_bAutoDeleteProcess;
};

#endif //_CREATEMOVIEDLG_H

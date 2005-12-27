/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/zaurus/zmindia.h,v $
 *
 *  $Revision: 1.6 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.5  2005/12/26 16:23:01  Michael
 *	added save support to zaurus platform
 *	
 *	Revision 1.4  2004/04/09 15:49:29  min
 *	PlayInfo dialog for the Zaurus implemented, Optimizations for c860.
 *	
 *	Revision 1.2  2004/02/26 22:15:24  min
 *	New slot sltPlayFinished() added.
 *	
 *	Revision 1.1.1.1  2003/08/15 16:38:22  min
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

#ifndef ZMINDIA_H
#define ZMINDIA_H

#include <qpe/filemanager.h>
#include <qpe/applnk.h>

#include <qmainwindow.h>

#include "doccontroler.h"
#include "minlog.h"

class FileSelector;
class QToolBar;
class QPEMenuBar;
class QWidgetStack;
class QVBox;
class QHBox;
class QMultiLineEdit;
class QTable;
class QLabel;
class QTimer;

class DiaInfoDlgImpl;
class PlayInfoDlgImpl;
class SoundInfoDlgImpl;

class ZMinDia : public QMainWindow, public minLoggingInterface
{
    Q_OBJECT

public:
    ZMinDia( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ZMinDia();

    // *** implements the minLoggingInterface ***
    virtual void LogMsg( const char * sMsg );

public slots:
    void sltFileOpen();
    void sltFileSave();
    void sltFileSaveAs();
    void sltPlayStart();
    void sltPlayStop();
    void sltPlayPause();
    void sltPlayStartFromSelected();
    void sltPlayStartAtSelected();
    void sltShowAbout();
    void sltPlayerConfiguration();
    void sltShowStatusBarMessage( const QString & sText );
    void sltShowErrorMessage( const QString & sText );
    void sltShowWarningMessage( const QString & sText );
    void sltSelectItem( int iIndex, int iDissolveTimeInMS );
    void sltUpdateOutput();
    //void sltFastRead();
    void sltModusIsSwitched();
	void sltPlayFinished();

    void sltSelectPrev();
    void sltSelectNext();
    void sltNewItemAfterSelected();
	void sltDeleteSelectedItem();
	void sltDoDataChanged();

private slots:
    void sltOpenFile( const DocLnk & );
    void sltConfiguration();
    void sltClearLogging();
    void sltProjectorControl();
    void sltDiaInfo();
    void sltSoundInfo();
    void sltPlayInfo();
    void sltShowWorkspace();
    void sltStatusUpdateTimerEvent();

	void sltSelectionChanged( int row, int col );

protected:
    virtual void closeEvent( QCloseEvent * pCloseEvent );
	virtual void customEvent( QCustomEvent * pEvent );

private:
    void updateCaption( const QString & sName=QString::null );
    void updateFileSelector();
    int  GetSelectedRow() const;
	void updateDiaInfoDialog( int iRow );

	// ** data **
	DocumentAndControler	m_aDocContrl;

	DiaInfoDlgImpl * 	m_pDiaInfo;
	PlayInfoDlgImpl * 	m_pPlayInfo;
    SoundInfoDlgImpl *  m_pSoundInfo;

#ifdef WITH_ORGINAL_FILE_DIALOG
	FileSelector *		m_pFileSelector;
#endif
	QToolBar *			m_pMenu;
	QToolBar *			m_pButtonBar;
	QPEMenuBar *		m_pMenuBar;
	QWidgetStack *		m_pStack;
	QVBox * 			m_pVBox;
	QTable *			m_pOutput;
	QMultiLineEdit * 	m_pLogging;
	QLabel *			m_pStatusLabel1;
	QLabel *			m_pStatusLabel2;

	QTimer *			m_pStatusUpdateTimer;

    DocLnk 				m_aDoc;

	QString				m_sScriptExtention;

	int					m_iActRow;
};

#endif // ZMINDIA

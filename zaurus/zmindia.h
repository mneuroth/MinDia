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
    void sltTestRead();
    void sltModusIsSwitched();

    void sltSelectPrev();
    void sltSelectNext();
    void sltNewItemAfterSelected();

private slots:
    void sltOpenFile( const DocLnk & );
    void sltConfiguration();
    void sltClearLogging();
    void sltProjectorControl();
    void sltDiaInfo();
    void sltShowWorkspace();
    void sltStatusUpdateTimerEvent();

protected:
    virtual void closeEvent( QCloseEvent * pCloseEvent );
	virtual void customEvent( QCustomEvent * pEvent );

private:
    void updateCaption( const QString & sName=QString::null );
    void updateFileSelector();
    int  GetSelectedRow() const;

	// ** data **
	DocumentAndControler	m_aDocContrl;

	FileSelector *		m_pFileSelector;
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
};

#endif // ZMINDIA

/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/zaurus/zmindia.cpp,v $
 *
 *  $Revision: 1.7 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.6  2005/12/27 16:55:57  Michael
 *	using better file-dialog and addes support for sound-info-dialog for zaurus platform
 *	
 *	Revision 1.5  2005/12/26 16:23:01  Michael
 *	added save support to zaurus platform
 *	
 *	Revision 1.4  2004/04/09 15:49:29  min
 *	PlayInfo dialog for the Zaurus implemented, Optimizations for c860.
 *	
 *	Revision 1.2  2004/02/26 22:16:04  min
 *	Disable screen saver while playing.
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
/*
	virtual frame buffer starten: qvfb

	einzelapplikation starten: zmindia -qws

	qtopia desktop starten: qpe

	ACHTUNG: zum Verkleinern der Programmdatei die Option -s beim Linken angeben (strip) !
	         LFLAGS = -s

4) During the boot process you will see a 'Wait' message displayed followed by a number
   that decrements from 5. Before the number decrements to 0, press the '/?' key on the 
   keypad. This will display the following menu

q (x): Qtopia
a (e): Linux Console
e (e): init 3 (ttyS0: -free-)
r (e): init 4 (ttyS0: terminal)
t (e): init 5 (ttyS0: pppd) 

5) Press the 'E' key on the keypad. The device will boot in INIT 3 mode with the 
   serial port configured for SerialPort (free from the getty daemon). 
   Note: this must be done each time the device is soft reset.

*/

#include "zmindia.h"

#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;

#include <qpe/qpeapplication.h>
#include <qpe/qpemenubar.h>
#include <qpe/qpetoolbar.h>
#include <qpe/resource.h>
#include <qpe/fileselector.h>
#include <qpe/config.h>

#include <qpe/qcopenvelope_qws.h>

#include <qaction.h>
#include <qpushbutton.h>
#include <qmultilineedit.h>
#include <qlineedit.h>
#include <qtable.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qwidgetstack.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qtimer.h>

#ifndef ZAURUS
#include "qtmtlock.h"
#else
class QtMTLock {};
#endif

#include "configdlgimpl.h"
#include "configplayerdlgimpl.h"
#include "pcdlgimpl.h"
#include "diainfodlgimpl.h"
#include "playinfodlgimpl.h"
#include "sndinfodlgimpl.h"
#include "SaveAsDlg.h"
#include "AboutDlg.h"
#include "filebrowser.h"

const unsigned long c_ulStatusTimeout = 2000;
const unsigned long c_ulStatusBarTimer = 100;

const int c_iCustomEvent_Logging = 12345;

#define _ZMinDia_NAME		"ZMinDia"

static int u_id = 1;
static int get_unique_id()
{
    return u_id++;
}

static void SetScreenSaverInterval( int iTimeInSeconds )
{
	QCopEnvelope e( "QPE/System", "setScreenSaverInterval(int)" );
	e << iTimeInSeconds;
}

static void copyDocLnk(const DocLnk &source, DocLnk &target)
{
  target.setName(source.name());
  target.setFile(source.file());
  target.setLinkFile(source.linkFile());
  target.setComment(source.comment());
  target.setType(source.type());
  target.setCategories(source.categories());
}

static int g_iIDCount = 0;

static string GetNextFreeID()
{
	char sBuffer[12];

	g_iIDCount++;

	sprintf( sBuffer, "id%d", g_iIDCount );

	return sBuffer;
}

static int GetPercentageWidth( int iMaxWidth, double dPercent )
{
	int iRet = (int)(dPercent*((double)iMaxWidth)/100.0);
    //cout << "Percentage: " << iRet << " " << iMaxWidth << " " << dPercent << endl;
	return iRet;
}


/*
 *  Constructs a ZMinDia which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
ZMinDia::ZMinDia( QWidget* parent,  const char* name, WFlags fl )
    : QMainWindow( parent, name, fl ),
	  m_aDocContrl( /*bIgnoreComSettings*/false, /*bSimulation*/true, RolleiCom::TWIN_DIGITAL_P, this, 0, this ),
	  m_pDiaInfo( 0 ),
	  m_pPlayInfo( 0 ),
#ifdef WITH_ORGINAL_FILE_DIALOG
	  m_pFileSelector( 0 ),
#endif
	  m_sScriptExtention( "text/plain" ),	// "text/mindia"
	  m_iActRow( 0 )
{
//	bool bok = m_aDocContrl.LoadPresentation( "example_nz.dia", false );
//cout << "load doc: " << bok << " " << m_aDocContrl.GetDiaCount() << endl;

    setToolBarsMovable( FALSE );

//    setIcon( Resource::loadPixmap( "TextEditor" ) );

	// *************************************************************
	// *************************************************************

    m_pMenu = new QPEToolBar( this );
    m_pMenu->setHorizontalStretchable( TRUE );
	//addToolBar( m_pMenu, "tool", QMainWindow::Top, TRUE );

    m_pMenuBar = new QPEMenuBar( m_pMenu );
    QPopupMenu * pFile = new QPopupMenu( this );
    QPopupMenu * pPlay = new QPopupMenu( this );
    //QPopupMenu * pOperations = new QPopupMenu( this );

    m_pButtonBar = new QPEToolBar( this );

    m_pMenuBar->insertItem( tr( "File" ), pFile );
    m_pMenuBar->insertItem( tr( "Play" ), pPlay );
    //m_pMenuBar->insertItem( tr( "Actions" ), pOperations );

	// ++++++++++++++++++++++++++++++++++++++++++

    QAction * aAction = new QAction( tr( "New" ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltAskNewDoc() ) );
    aAction->addTo( pFile );

    aAction = new QAction( tr( "Open..." ), Resource::loadPixmap( "fileopen" ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltFileOpen() ) );
    aAction->addTo( m_pButtonBar );
    aAction->addTo( pFile );

    aAction = new QAction( tr( "Save" ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltFileSave() ) );
    aAction->addTo( pFile );

    aAction = new QAction( tr( "Save as..." ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltFileSaveAs() ) );
    aAction->addTo( pFile );

	pFile->insertSeparator();

	// "settings"
    aAction = new QAction( tr( "Configuration..." ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltConfiguration() ) );
    aAction->addTo( pFile );

    aAction = new QAction( tr( "Player configuration..." ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltPlayerConfiguration() ) );
    aAction->addTo( pFile );

    aAction = new QAction( tr( "Projector control..." ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltProjectorControl() ) );
    aAction->addTo( pFile );

    aAction = new QAction( tr( "Dia info..." ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltDiaInfo() ) );
    aAction->addTo( pFile );

    aAction = new QAction( tr( "Sound info..." ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltSoundInfo() ) );
    aAction->addTo( pFile );

    aAction = new QAction( tr( "Play info..." ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltPlayInfo() ) );
    aAction->addTo( pFile );

	pFile->insertSeparator();

    aAction = new QAction( tr( "Clear logging" ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltClearLogging() ) );
    aAction->addTo( pFile );
/*
    aAction = new QAction( tr( "Fast read" ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltFastRead() ) );
    aAction->addTo( pFile );
*/
	pFile->insertSeparator();

	// "settings"
    aAction = new QAction( tr( "About..." ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltShowAbout() ) );
    aAction->addTo( pFile );

	// ++++++++++++++++++++++++++++++++++++++++++

    aAction = new QAction( tr( "Start from seleted" ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltPlayStartFromSelected() ) );
    aAction->addTo( pPlay );

    aAction = new QAction( tr( "Start at selected" ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltPlayStartAtSelected() ) );
    aAction->addTo( pPlay );

    pPlay->insertSeparator();

	aAction = new QAction( tr( "Start" ), Resource::loadPixmap( "play" ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltPlayStart() ) );
    aAction->addTo( m_pButtonBar );
    aAction->addTo( pPlay );

    aAction = new QAction( tr( "Pause" ), Resource::loadPixmap( "wait" ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltPlayPause() ) );
    aAction->addTo( m_pButtonBar );
    aAction->addTo( pPlay );

    aAction = new QAction( tr( "Stop" ), Resource::loadPixmap( "stop" ), QString::null, 0, this, 0 );
    connect( aAction, SIGNAL( activated() ), this, SLOT( sltPlayStop() ) );
    aAction->addTo( m_pButtonBar );
    aAction->addTo( pPlay );

	// *************************************************************
	// *************************************************************

	m_pStack = new QWidgetStack( this );
    setCentralWidget( m_pStack );

    m_pVBox = new QVBox( m_pStack );
	m_pVBox->setSpacing( 4 );
	m_pVBox->setMargin( 2 );

    m_pOutput = new QTable( 0, 5, /*m_pSplitter*/m_pVBox, "m_pOutput" ); //new QMultiLineEdit( m_pVBox, "m_pOutput" );
    //m_pOutput->setReadOnly( TRUE );
    //m_pOutput->setEnabled( FALSE );
	m_pOutput->setSelectionMode( QTable::Single );
//	m_pOutput->setColumnWidth( 0, GetPercentageWidth(m_pOutput->width(),30.0) );
//	m_pOutput->setColumnWidth( 1, GetPercentageWidth(m_pOutput->width(),15.0) );
//	m_pOutput->setColumnWidth( 2, GetPercentageWidth(m_pOutput->width(),15.0) );
//	m_pOutput->setColumnWidth( 3, GetPercentageWidth(m_pOutput->width(),20.0) );
//	m_pOutput->setColumnWidth( 4, GetPercentageWidth(m_pOutput->width(),20.0) );
//	m_pOutput->setColumnWidth( 0, 75 );
//	m_pOutput->setColumnWidth( 1, 30 );
//	m_pOutput->setColumnWidth( 2, 30 );
//	m_pOutput->setColumnWidth( 3, 60 );
//	m_pOutput->setColumnWidth( 4, 60 );
	m_pOutput->horizontalHeader()->setLabel( 0, tr( "Comment" ) );
	m_pOutput->horizontalHeader()->setLabel( 1, tr( "Dissolve time [s]" ) );
	m_pOutput->horizontalHeader()->setLabel( 2, tr( "Show time [s]" ) );
	m_pOutput->horizontalHeader()->setLabel( 3, tr( "Id" ) );
	m_pOutput->horizontalHeader()->setLabel( 4, tr( "Image" ) );

    m_pLogging = new QMultiLineEdit( /*m_pSplitter*/m_pVBox, "m_pLogging" );
    m_pLogging->setReadOnly( TRUE );
	// set the size of the logging field...
//	m_pLogging->setMaximumSize( QSize( 240, 65 ) );
	m_pLogging->setMaximumHeight( 65 );
	m_pLogging->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );

	m_pStatusLabel1 = new QLabel( m_pVBox, "m_pStatusLabel1" );
	m_pStatusLabel2 = new QLabel( m_pVBox, "m_pStatusLabel2" );

    // ** script edit field should be larger than the output edit field
    //m_pVBox->setStretchFactor( m_pOutput, /*3*/2 );
    //m_pVBox->setStretchFactor( m_pLogging, /*3*/2 );
    //m_pVBox->setStretchFactor( m_pStatusLabel1, 1 );
    //m_pVBox->setStretchFactor( m_pStatusLabel2, 1 );

    m_pStack->addWidget( m_pVBox, get_unique_id() );

	// ** start status-update timer **
	m_pStatusUpdateTimer = new QTimer( this );
	connect( m_pStatusUpdateTimer, SIGNAL( timeout() ), this, SLOT( sltStatusUpdateTimerEvent() ) );
	m_pStatusUpdateTimer->start( c_ulStatusBarTimer );

	connect( m_pOutput, SIGNAL( currentChanged(int,int) ), this, SLOT( sltSelectionChanged(int,int) ) );

	m_aDocContrl.GetDiaCom().SetLoggingChannel( this );
	m_aDocContrl.GetDiaCom().SetLogging( true );
	m_aDocContrl.GetDiaCom().SetProjectorType( RolleiCom::TWIN_DIGITAL_P );

	updateFileSelector();

    //m_pStack->raiseWidget( m_pVBox );

//  sltFileOpen();
	sltShowWorkspace();
//	updateCaption( "example_nz.dia" );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ZMinDia::~ZMinDia()
{
    // no need to delete child widgets, Qt does it all for us

	if( m_pDiaInfo )
	{
		delete m_pDiaInfo;
	}
	if( m_pPlayInfo )
	{
		delete m_pPlayInfo;
	}
}

void ZMinDia::updateFileSelector()
{
#ifdef WITH_ORGINAL_FILE_DIALOG
    delete m_pFileSelector;

    m_pFileSelector = new FileSelector( /*"text/plain"*/m_sScriptExtention, m_pStack, "fileselector" , FALSE, TRUE );
    connect( m_pFileSelector, SIGNAL( closeMe() ), this, SLOT( sltShowWorkspace() ) );
    connect( m_pFileSelector, SIGNAL( fileSelected( const DocLnk &) ), this, SLOT( sltOpenFile( const DocLnk & ) ) );
#endif
}

void ZMinDia::sltFileOpen()
{
#ifdef WITH_ORGINAL_FILE_DIALOG
    m_pMenu->hide();
    m_pButtonBar->hide();
    clearWState (WState_Reserved1 );
    updateFileSelector();
    m_pStack->raiseWidget( m_pFileSelector );
    m_pFileSelector->reread();
    updateCaption();
#else
	fileBrowser dlg(this, true, "*.dia");

	if( dlg.exec() && !dlg.selectedFileName.isEmpty() )
	{
    	bool bOk = m_aDocContrl.LoadPresentation( (const char *)dlg.selectedFileName, false );
    	//cout << "load doc: " << bok << " " << m_aDocContrl.GetDiaCount() << endl;
    
    	if( bOk )
    	{
    		sltUpdateOutput();
            updateCaption( dlg.selectedFileName );
    	}
	}
    sltShowWorkspace();
#endif
}

void ZMinDia::sltFileSave()
{
	m_aDocContrl.sltSaveDoc();
}

void ZMinDia::sltFileSaveAs()
{
	fileBrowser dlg(this, false, "*.dia");

	if( dlg.exec() && !dlg.selectedFileName.isEmpty() )
	{
        bool bOk = false;
        m_aDocContrl.sltSaveAsDoc( dlg.selectedFileName, bOk );
    
    	if( bOk )
    	{
            updateCaption( dlg.selectedFileName );
    	}
	}	
}

void ZMinDia::sltConfiguration()
{
	ConfigurationDlgImpl * pDlg = new ConfigurationDlgImpl( &m_aDocContrl, this, "configuredlg", /*bModal*/TRUE );

	pDlg->adjustSize();

	int iRet = pDlg->exec();

	if( iRet == 1 )
	{
		// ok, pressed
		pDlg->sltCloseDialog();
	}

	delete pDlg;
}

void ZMinDia::sltPlayerConfiguration()
{
	ConfigPlayerDlgImpl * pDlg = new ConfigPlayerDlgImpl( m_aDocContrl.GetIniDB(), this, "playerconfiguredlg", /*bModal*/TRUE );

	pDlg->adjustSize();

	int iRet = pDlg->exec();

	if( iRet == 1 )
	{
		// ok, pressed
		pDlg->sltOk();
	}

	delete pDlg;
}

void ZMinDia::sltShowAbout()
{
	AboutDlg * pDlg = new AboutDlg( this, "aboutdlg", /*bModal*/TRUE );

	/*int iRet =*/ pDlg->exec();

	delete pDlg;
}

void ZMinDia::sltPlayStart()
{
	if( m_aDocContrl.IsEditModus() || m_aDocContrl.IsPauseModus() )
	{
		m_aDocContrl.Play();

		// disable the screen saver for the length of the slide show
		SetScreenSaverInterval((int)(m_aDocContrl.GetPresentation().GetTotalTime())+15);
	}
}

void ZMinDia::sltPlayStop()
{
	if( !m_aDocContrl.IsEditModus() )
	{
		m_aDocContrl.Stop();
	}
}

void ZMinDia::sltPlayPause()
{
	if( m_aDocContrl.IsPlayModus() )
	{
		m_aDocContrl.Pause();
	}
}

void ZMinDia::sltPlayStartFromSelected()
{
	int iSelectedIndex = GetSelectedRow();

	if( iSelectedIndex>=0 )
	{
		m_aDocContrl.sltGotoPosition( iSelectedIndex );
		m_aDocContrl.sltPlayPresentationAt( iSelectedIndex );
	}
}

void ZMinDia::sltPlayStartAtSelected()
{
	int iSelectedIndex = GetSelectedRow();

	if( iSelectedIndex>=0 )
	{
		m_aDocContrl.sltPlayPresentationAt( iSelectedIndex );
	}
}

void ZMinDia::sltAskNewDoc()
{
	// ask user before deleting the actual document...
	int iRet = QMessageBox::warning( 0, tr( "ZMinDia - Question" ), 
										tr( "Create new document and\nloose all changes?" ), 1, 2 );
	if( iRet == 1 )
	{
		m_aDocContrl.sltNewDoc();
		sltUpdateOutput();
        updateCaption( "newfile.dia" );
	}
}

void ZMinDia::sltOpenFile( const DocLnk & f )
{
#ifdef WITH_ORGINAL_FILE_DIALOG
	// ACHTUNG: man MUSS eine deep-copy durchfuehren !!!
	//error:	m_aDoc = f;
	copyDocLnk( f, m_aDoc );

	bool bOk = m_aDocContrl.LoadPresentation( f.file(), false );
	//cout << "load doc: " << bok << " " << m_aDocContrl.GetDiaCount() << endl;

	if( bOk )
	{
		sltUpdateOutput();
	}

    FileManager fm;
    QString txt;
    if ( !fm.loadFile( f, txt ) ) {
	// ####### could be a new file
	//qDebug( "Cannot open file" );
	//return;
    }

    sltShowWorkspace();

    updateCaption( m_aDoc.name() );
#else
	fileBrowser dlg(this, true);

	if( dlg.exec() && !dlg.selectedFileName.isEmpty() )
	{
    	bool bOk = m_aDocContrl.LoadPresentation( (const char *)dlg.selectedFileName, false );
    	//cout << "load doc: " << bok << " " << m_aDocContrl.GetDiaCount() << endl;
    
    	if( bOk )
    	{
    		sltUpdateOutput();
            updateCaption( dlg.selectedFileName );
    	}
	}

    sltShowWorkspace();
#endif
}

void ZMinDia::sltShowWorkspace()
{
#ifdef WITH_ORGINAL_FILE_DIALOG
    m_pFileSelector->hide();
#endif
    m_pMenu->show();
    m_pButtonBar->show();
    m_pStack->raiseWidget( m_pVBox );
    m_pOutput->setFocus();
	m_pOutput->setColumnWidth( 0, GetPercentageWidth(m_pOutput->width(),30.0) );
	m_pOutput->setColumnWidth( 1, GetPercentageWidth(m_pOutput->width(),15.0) );
	m_pOutput->setColumnWidth( 2, GetPercentageWidth(m_pOutput->width(),15.0) );
	m_pOutput->setColumnWidth( 3, GetPercentageWidth(m_pOutput->width(),20.0) );
	m_pOutput->setColumnWidth( 4, GetPercentageWidth(m_pOutput->width(),20.0) );
}

void ZMinDia::updateCaption( const QString & sName=QString::null )
{
	if( sName.isNull() )
	{
		setCaption( tr( "ZMinDia" ) );
	}
	else
	{
		setCaption( sName + " - " + tr( "ZMinDia" ) );
	}
}

void ZMinDia::closeEvent( QCloseEvent * pCloseEvent )
{
	if( m_aDocContrl.IsEditModus() )
	{
		pCloseEvent->accept();
	}
	else
	{
		/*int iRet =*/ QMessageBox::warning( this, tr( "MinDia - Warning" ), tr( "Can't close the application, because\nthe presentation is still playing!" ) );

		pCloseEvent->ignore();
	}
}

void ZMinDia::customEvent( QCustomEvent * pEvent )
{
    switch( pEvent->type() )
	{
		case c_iCustomEvent_Logging:
		{
			if( m_pOutput )
			{
				//QtMTLock aMTLock;

			    QString * psMsg = (QString *)pEvent->data();

				m_pLogging->insertLine( *psMsg );
				// ** update the visible area, so that the last output is visible
				m_pLogging->setCursorPosition( m_pLogging->numLines(), 1 );

				delete psMsg;
			}
		} break;

		default:
		{
		}
	}
}

void ZMinDia::sltShowStatusBarMessage( const QString & sText )
{
	m_pStatusLabel1->setText( sText );
}

void ZMinDia::sltStatusUpdateTimerEvent()
{
	m_pStatusUpdateTimer->stop();

	//debug: cout << "*** UPDATE act=" << m_pControler->GetPresentation().GetActPlayTime() << " total=" << m_pControler->GetPresentation().GetTotalTime() << endl;

	double dActTime = m_aDocContrl.GetPresentation().GetActPlayTime();

	QString sPlay;
	QString sTotal;
	QString sCountDown;

	QTime aPlayTime;
	aPlayTime = aPlayTime.addMSecs( (int)(dActTime*1000) );
	QTime aTotalTime;
	aTotalTime = aTotalTime.addMSecs( (int)(m_aDocContrl.GetPresentation().GetTotalTime()*1000) );

	double dCountDown = m_aDocContrl.GetPresentation().GetCountDownTime();
	sCountDown.sprintf( "%.1f", dCountDown );

	sPlay.sprintf( "%d:%02d", aPlayTime.minute(), aPlayTime.second() );
	sTotal.sprintf( "%d:%02d", aTotalTime.minute(), aTotalTime.second() );

	bool bIsPlaying = m_aDocContrl.IsPlayModus();
	bool bIsPause = m_aDocContrl.IsPauseModus();
	bool bIsEdit = m_aDocContrl.IsEditModus();
	QString sMsg;
	if( !bIsEdit )
	{
		sMsg = tr( "play" ) + " / ";
	}
	sMsg += tr( "total time: " );
	if( !bIsEdit )
	{
		sMsg += sPlay + " / ";
	}
	sMsg += sTotal + tr( " min" );

	if( bIsPlaying || bIsPause )
	{
		sMsg += tr( " *** next: " ) + sCountDown + tr( " s" );
	}
	else
	{
		QString sCount;
		sCount.setNum( m_aDocContrl.GetPresentation().GetDiaCount() );
		sMsg += tr( " *** count: " ) + sCount;
	}

	{
		//QtMTLock aMTLock;

		m_pStatusLabel2->setText( sMsg );
		//m_pStatusBarModus->setText( m_aDocContrl.GetPlayModusStrg() );
	}

	// ** restart the timer again
	m_pStatusUpdateTimer->start( c_ulStatusBarTimer );
}

void ZMinDia::LogMsg( const char * sMsg )
{
	QCustomEvent * pEvent = new QCustomEvent( c_iCustomEvent_Logging );
    pEvent->setData( new QString( sMsg ) );
	QApplication::postEvent( this, pEvent);

	/*
	if( m_pLogging )
	{
		QtMTLock aMTLock;

		m_pLogging->insertLine( QString( sMsg ) );
		// ** update the visible area, so that the last output is visible
		m_pLogging->setCursorPosition( m_pLogging->numLines(), 1 );
	}
	*/
}

void ZMinDia::sltClearLogging()
{
	if( m_pLogging )
	{
		m_pLogging->clear();
	}
}

void ZMinDia::sltProjectorControl()
{
	ProjectorControlDlgImpl * pDlg = new ProjectorControlDlgImpl( &(m_aDocContrl.GetDiaCom()), &(m_aDocContrl.GetPresentation()), this, "projector_control", /*bTrue*/true );

	if( width()<300 )
	{
		pDlg->setFixedWidth(220);
		pDlg->setFixedHeight(280);
		pDlg->updateGeometry();
	}
	else
	{
		pDlg->adjustSize();
	}

	/*int iRet =*/ pDlg->exec();
/*
	if( iRet == 1 )
	{
		// ok, pressed
	}
*/
	delete pDlg;
}

void ZMinDia::sltDiaInfo()
{
	if( m_pDiaInfo==0 )
	{
		m_pDiaInfo = new DiaInfoDlgImpl( this, this, "dia_info", /*bModal*/FALSE );

		if( width()<300 )
		{
			m_pDiaInfo->setFixedWidth(220);
			//m_pDiaInfo->setFixedHeight(280);
			m_pDiaInfo->updateGeometry();
		}
		else
		{
			m_pDiaInfo->adjustSize();
		}
	}

	/*int iRet =*/ m_pDiaInfo->show();
	m_pDiaInfo->raise();
/*
	if( iRet == 1 )
	{
		// ok, pressed
	}
*/
//	delete m_pDiaInfo;
}

void ZMinDia::sltSoundInfo()
{
	SoundInfoDlgImpl * pSoundInfo = new SoundInfoDlgImpl( &m_aDocContrl.GetPresentation().GetSoundInfoData(), this, "sound_info", /*bModal*/TRUE );

	if( width()<300 )
	{
		pSoundInfo->setFixedWidth(240);
		pSoundInfo->setFixedHeight(290);
		pSoundInfo->updateGeometry();
	}
	else
	{
		pSoundInfo->adjustSize();
	}

    /*int iRet =*/ pSoundInfo->exec();

    delete pSoundInfo;
}

void ZMinDia::sltPlayInfo()
{
	if( m_pPlayInfo==0 )
	{
		m_pPlayInfo = new PlayInfoDlgImpl( &m_aDocContrl, this, "play_info", /*bModal*/FALSE );

		if( width()<300 )
		{
			m_pPlayInfo->setFixedWidth(220);
			//m_pPlayInfo->setFixedHeight(280);
			m_pPlayInfo->updateGeometry();
		}
		else
		{
			//m_pPlayInfo->setFixedWidth(460);
			//m_pPlayInfo->setFixedHeight(320);
			m_pPlayInfo->updateGeometry();
			//m_pPlayInfo->adjustSize();
		}

	}

	/*int iRet =*/ m_pPlayInfo->show();
	m_pPlayInfo->raise();
/*
	if( iRet == 1 )
	{
		// ok, pressed
	}
*/
//	delete m_pPlayInfo;
}

void ZMinDia::sltUpdateOutput()
{
	char sBuffer[ 512 ];

	DiaPresentation & aPresentation = m_aDocContrl.GetPresentation();

	m_pOutput->clearSelection();
	m_pOutput->setNumRows( aPresentation.GetDiaCount() );
	for( int i=0; i<aPresentation.GetDiaCount(); i++ )
	{
		minHandle<DiaInfo> hDia = aPresentation.GetDiaAt( i );

		m_pOutput->setText( i, 0, hDia->GetComment() );
		sprintf( sBuffer, "%3.1f", hDia->GetDissolveTime() );
		m_pOutput->setText( i, 1, sBuffer );
		sprintf( sBuffer, "%3.1f", hDia->GetShowTime() );
		m_pOutput->setText( i, 2, sBuffer );
		m_pOutput->setText( i, 3, hDia->GetId() );
		m_pOutput->setText( i, 4, hDia->GetImageFile() );
	}
}
/*
void ZMinDia::sltFastRead()
{
	m_aDocContrl.LoadPresentation( "/mnt/cf/diapresentation.dia", false );

	sltShowWorkspace();
	updateCaption( "diapresentation.dia" );
}
*/
void ZMinDia::sltSelectPrev()
{
	if( m_iActRow>0 )
	{
		m_iActRow--;

		updateDiaInfoDialog( m_iActRow );
	}
}

void ZMinDia::sltSelectNext()
{
	if( m_iActRow+1<m_aDocContrl.GetPresentation().GetDiaCount() )
	{
		m_iActRow++;

		updateDiaInfoDialog( m_iActRow );
	}
}

void ZMinDia::sltNewItemAfterSelected()
{
	m_iActRow++;

	m_aDocContrl.GetPresentation().AddDiaAt( m_iActRow, minHandle<DiaInfo>( new DiaInfo( GetNextFreeID() ) ) );

	sltUpdateOutput();

	updateDiaInfoDialog( m_iActRow );
}

void ZMinDia::sltDeleteSelectedItem()
{
	m_aDocContrl.GetPresentation().RemoveDiaAt( m_iActRow );

	sltUpdateOutput();

	updateDiaInfoDialog( m_iActRow );
}

void ZMinDia::sltShowErrorMessage( const QString & sMessage )
{
	QMessageBox::warning( this, tr( "ZMinDia - Error" ), sMessage );
}

void ZMinDia::sltShowWarningMessage( const QString & sMessage )
{
	QMessageBox::warning( this, tr( "ZMinDia - Warning" ), sMessage );
}

void ZMinDia::sltModusIsSwitched()
{
}

void ZMinDia::sltPlayFinished()
{
	// reset the screen saver interval to a reasonable value (60s)
	SetScreenSaverInterval( 60 );
}

void ZMinDia::sltSelectItem( int iIndex, int iDissolveTimeInMS )
{
	QTableSelection aSelect;
	aSelect.init( iIndex, 0 );
	aSelect.expandTo( iIndex, 5 );

	m_pOutput->clearSelection();
	m_pOutput->addSelection( aSelect );
	m_pOutput->ensureCellVisible( iIndex, 0 );

	if( m_pPlayInfo && m_pPlayInfo->isVisible() )
	{
		minHandle<DiaInfo> hSelected = m_aDocContrl.GetPresentation().GetDiaAt( iIndex );
		m_pPlayInfo->sltSetImage( hSelected->GetImageFile(), m_aDocContrl.IsPlayModus(), iDissolveTimeInMS );
	}
}

int ZMinDia::GetSelectedRow() const
{
	for( int i=0; i<m_pOutput->numRows(); i++ )
	{
		if( m_pOutput->isRowSelected( i, TRUE ) )
		{
			return i;
		}
	}

	return -1;		// no row selected !
}

void ZMinDia::sltSelectionChanged( int iRow, int iCol )
{
	updateDiaInfoDialog( iRow );
}

void ZMinDia::updateDiaInfoDialog( int iRow )
{
	m_iActRow = iRow;
//cout << "updateDiaInfo " << iRow << endl;
	minHandle<DiaInfo> hSelected = m_aDocContrl.GetPresentation().GetDiaAt( m_iActRow );

    if( m_pDiaInfo )
	{
//cout << "doit " << (void *)hSelected.GetPtr() << endl;
		m_pDiaInfo->sltUpdateData( hSelected, true );
	}

	if( m_pPlayInfo && m_pPlayInfo->isVisible() )
	{
		m_pPlayInfo->sltSetImage( QImage( hSelected->GetImageFile() ) );
	}
}

void ZMinDia::sltDoDataChanged()
{
	sltUpdateOutput();
}



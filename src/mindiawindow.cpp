/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/mindiawindow.cpp,v $
 *
 *  $Revision: 1.12 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.11  2004/02/22 11:00:10  min
 *	QTranslator better handled.
 *	
 *	Revision 1.10  2004/02/21 14:58:21  min
 *	Help dialog improved
 *	
 *	Revision 1.9  2004/02/21 00:54:52  min
 *	Automatic language detection implemented.
 *	
 *	Revision 1.8  2004/02/20 23:06:37  min
 *	Qt About via MessageBox handled.
 *	
 *	Revision 1.7  2004/02/16 19:48:01  min
 *	About Qt added.
 *	
 *	Revision 1.6  2004/01/29 21:29:06  min
 *	Bugfix: clear tooltip if a new message for the statusbar arrives
 *	
 *	Revision 1.5  2004/01/28 19:11:37  min
 *	Plugins menuitem changed
 *
 *	Revision 1.4  2004/01/18 23:52:36  min
 *	Updated.
 *	
 *	Revision 1.3  2003/10/26 17:34:05  min
 *	MakeRelativePaths() added. Directories for images, sounds, data, scirpts added.
 *	
 *	Revision 1.2  2003/08/15 19:39:06  min
 *	size of sub-widget changed, debug comments deleted
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

#include "mindiawindow.h"

#include "minbase.h"
#include "iscript.h"
#include "qtmtlock.h"

#include "appconfig.h"
#include "configdlgimpl.h"
#include "configplayerdlgimpl.h"
#include "comlogimpl.h"
#include "pcdlgimpl.h"
#include "eventmapdlgimpl.h"
#include "diainfodlgimpl.h"
#include "playinfodlgimpl.h"
#include "pddlgimpl.h"
#include "sndinfodlgimpl.h"
#include "commentdlgimpl.h"
#include "createmoviedlgimpl.h"
#include "helpdlgimpl.h"
#include "EnterValueDlg.h"
#include "LicenseDlg.h"
#include "AboutExtDlg.h"

#include <qapplication.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qprinter.h>
#include <qclipboard.h>
#include <qtimer.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qtranslator.h>
#include <qtextbrowser.h>
#include <qlineedit.h>
#include <qtextview.h>
#include <qtextcodec.h>

//#include <qsound.h>

#include "icons.xpm"

#include <stdlib.h>

/**
	@mainpage MinDia Documentation


	The program MinDia is usefull to edit and run a slide show.
	It supports also the possibility to control a slide show
	with the computer controlable projectors from Rollei:
	this are the Rolleivision 35 twin digital P dia projector
	and the Rollei twin MSC 3x0 P dia projector family.

	@author		Michael Neuroth
	@version	1.0
	@date		2001-2004

	./COPYING

*/

// *******************************************************************

const unsigned long c_ulStatusTimeout		= 2000;
const unsigned long c_ulStatusBarTimer		= 100;
const unsigned long c_ulAutoStartTimeout	= 100;

// ***********************************************************************

string GetMinDiaSharedDirectory();

// ** this function is defined in main.cpp */
QApplication * GetApplication();
QString ProcessLanguage( QTranslator * pTranslator, const QString & sLanguage, QApplication * qApp );

// ***********************************************************************

MinDiaWindow::MinDiaWindow( const QString & sLanguage, bool bIgnoreComSettings, bool bSimulation, int iProjectorType, QWidget* parent, const char* name, WFlags f )
	: QMainWindow( parent, name, f ),
	  m_pConfigurationDialog( 0 ),
	  m_pConfigPlayerDialog( 0 ),
	  m_pLoggingDialog( 0 ),
	  m_pProjectorControlDialog( 0 ),
	  m_pPresentationDataDialog( 0 ),
	  m_pPresentationEventsDialog( 0 ),
	  m_pDiaInfoDialog( 0 ),
	  m_pPlayInfoDialog( 0 ),
	  m_pSoundInfoDialog( 0 ),
	  m_pSoundCommentDialog( 0 ),
	  m_pPlotCommentDialog( 0 ),
	  m_pDynGraphicOpDialog( 0 ),
	  m_pHelpDialog( 0 ),
	  m_pStatusUpdateTimer( 0 ),
	  m_pAutoStartTimer( 0 ),
	  m_iCount( 0 ),
	  m_pFirstSelectedItem( 0 ),
	  m_iFirstSelectedItemNo( -1 ),
	  m_sLanguage( sLanguage ),
	  m_dDissolveTime( 2.5 ),
	  m_bExitOnFinished( false ),
      m_iScreenX( 0 ),
      m_iScreenY( 0 ),
	  m_iPosX( 10 ),
	  m_iPosY( 350 )
{
	// ** prepare application for different languages...**
	m_pTranslator = new QTranslator( this );
	m_sLanguage = ProcessLanguage( m_pTranslator, sLanguage, qApp );

	setCaption( tr( "MinDia" ) );

	// set application icon
	QPixmap aIcon( mindia_icon );
	setIcon( aIcon );

	// Add your code

	// *** create some needed sub-dialogs ***
	m_pLoggingDialog = new ComLoggingDialogImpl( this );
	//m_pControler->GetDiaCom().SetLoggingChannel( m_pLoggingDialog );

	m_pHelpDialog = new HelpDlgImpl( this, "help" );
	m_pHelpDialog->move( 470, 5 );

	m_pControler = new DocumentAndControler( bIgnoreComSettings, bSimulation, iProjectorType, this, this, m_pLoggingDialog );

	CreateChildWidgets();
	CreateMenus();

    resize( 800, 600 );
	move( 5, 5 );

	sltModusIsSwitched();
	sltDoDocumentStateUpdate();
}

MinDiaWindow::~MinDiaWindow()
{
	delete m_pControler;

	delete m_pDiaStateView;
	delete m_pAudioPultView;
	delete m_pDiaPultView;

	delete m_pTimeLineView;
	delete m_pSlideView;

	delete m_pSourceBox;
	delete m_pTargetBox;

	delete m_pHBox;

	delete m_pStatusUpdateTimer;
	delete m_pAutoStartTimer;

	delete m_pTranslator;
}

DocumentAndControler * MinDiaWindow::GetDocument()
{
	return m_pControler;
}

QPopupMenu * MinDiaWindow::GetPluginsMenuPtr()
{
	return m_pPlugins;
}

void MinDiaWindow::CreateMenus()
{
    // popuplate a menu with all actions

	QPixmap aOpenIcon( fileopen );
	QPixmap aSaveIcon( filesave );
	QPixmap aRunIcon( runscript );
	QPixmap aPauseIcon( pausescript );
	QPixmap aStopIcon( stopscript );

    QToolBar * pTools = new QToolBar( this, "file operations" );
    pTools->setLabel( tr("File Operations") );

    //QPixmap openIcon = QPixmap( fileopen );
    m_pFileOpen  = new QToolButton( aOpenIcon, tr("Open File"), QString::null, this, SLOT(sltAskLoadDoc()), pTools, "open file" );
    m_pFileSave	 = new QToolButton( aSaveIcon, tr("Save File"), QString::null, m_pControler, SLOT( sltSaveDoc() ), pTools, "save file" );
	pTools->addSeparator();
    m_pPlayRun   = new QToolButton( aRunIcon, tr("Start play"), QString::null, m_pControler, SLOT( sltPlayPresentation() ), pTools, "start play" );
    m_pPlayPause = new QToolButton( aPauseIcon, tr("Pause play"), QString::null, m_pControler, SLOT( sltPausePresentation() ), pTools, "pause play" );
    m_pPlayStop  = new QToolButton( aStopIcon, tr("Stop play"), QString::null, m_pControler, SLOT( sltStopPresentation() ), pTools, "stop play" );

    // *** create toplevel menu items ***
    m_pFile		= new QPopupMenu( this );
    m_pEdit		= new QPopupMenu( this );
    m_pPlay		= new QPopupMenu( this );
    m_pExtras	= new QPopupMenu( this );
	m_pPlugins	= new QPopupMenu( this );
    m_pHelp		= new QPopupMenu( this );

    menuBar()->insertItem( tr( "&File" ), m_pFile );
    menuBar()->insertItem( tr( "&Edit" ), m_pEdit );
    menuBar()->insertItem( tr( "&Play" ), m_pPlay );
    menuBar()->insertItem( tr( "E&xtras" ), m_pExtras );
    menuBar()->insertItem( tr( "Plu&gins" ), m_pPlugins );
    menuBar()->insertItem( tr( "&Help" ), m_pHelp );

	// *** submenu: File ***
    m_pFileNewAction = new QAction( tr( "New" ), tr( "&New" ), CTRL+Key_N, this, "new" );
    connect( m_pFileNewAction, SIGNAL( activated() ), this, SLOT( sltAskNewDoc() ) );
    m_pFileLoadAction = new QAction( tr( "Open an existing file" ), aOpenIcon, tr( "&Open..." ), CTRL+Key_O, this, "open" );
    connect( m_pFileLoadAction, SIGNAL( activated() ), this, SLOT( sltAskLoadDoc() ) );
	m_pFileLoadForEditAction = new QAction( tr( "Open an existing file for edit (script-events are not executed)" ), tr( "Open for &edit..." ), ALT+Key_O, this, "open_for_edit" );
    connect( m_pFileLoadForEditAction, SIGNAL( activated() ), this, SLOT( sltAskLoadForEditDoc() ) );
    m_pFileSaveAction = new QAction( tr( "Save data" ), aSaveIcon, tr( "&Save" ), CTRL+Key_S, this, "save" );
    connect( m_pFileSaveAction, SIGNAL( activated() ), m_pControler, SLOT( sltSaveDoc() ) );
    m_pFileSaveAsAction = new QAction( tr( "Save data as" ), tr( "Save &as..." ), 0, this, "saveas" );
    connect( m_pFileSaveAsAction, SIGNAL( activated() ), this, SLOT( sltAskSaveAsDoc() ) );
	m_pFileMakeRelPathsAction = new QAction( tr( "Make absolute paths to reative paths" ), tr( "Make &relative paths" ), 0, this, "makerelpaths" );
    connect( m_pFileMakeRelPathsAction, SIGNAL( activated() ), this, SLOT( sltMakeRelPaths() ) );
	m_pFileImportXMLAction = new QAction( tr( "Import data from a XML file" ), tr( "&Import XML..." ), 0, this, "import_xml" );
	connect( m_pFileImportXMLAction, SIGNAL( activated() ), this, SLOT( sltImportXMLDoc() ) );
	m_pFileExportXMLAction = new QAction( tr( "Export data as XML file" ), tr( "E&xport XML..." ), 0, this, "export_xml" );
	connect( m_pFileExportXMLAction, SIGNAL( activated() ), this, SLOT( sltExportXMLDoc() ) );
	m_pFileExportAction = new QAction( tr( "Export data" ), tr( "Expo&rt..." ), 0, this, "export" );
	connect( m_pFileExportAction, SIGNAL( activated() ), this, SLOT( sltExportDoc() ) );
	m_pFileExportAVIAction = new QAction( tr( "Export as AVI video" ), tr( "Export as &AVI..." ), 0, this, "export_avi" );
	connect( m_pFileExportAVIAction, SIGNAL( activated() ), this, SLOT( sltExportAVI() ) );
	m_pFileExportDynGraphAction = new QAction( tr( "Export dynamic graphic data" ), tr( "Export dyn. &graphics..." ), 0, this, "export_dyn_graph" );
	connect( m_pFileExportDynGraphAction, SIGNAL( activated() ), this, SLOT( sltExportDynGraphData() ) );
	m_pFileImportDynGraphAction = new QAction( tr( "Import dynamic graphic data" ), tr( "Import d&yn. graphics..." ), 0, this, "import_dyn_graph" );
	connect( m_pFileImportDynGraphAction, SIGNAL( activated() ), this, SLOT( sltImportDynGraphData() ) );
    m_pFilePrintAction = new QAction( tr( "Print data" ), tr( "&Print..." ), 0, this, "print" );
    connect( m_pFilePrintAction, SIGNAL( activated() ), this, SLOT( sltPrintDoc() ) );
    m_pFileExitAction = new QAction( tr( "Exit" ), tr( "E&xit" ), ALT+Key_F4, this, "exit" );
    connect( m_pFileExitAction, SIGNAL( activated() ), this, SLOT( close() ) );
    //connect( m_pFileExitAction, SIGNAL( activated() ), qApp, SLOT( quit() ) );

	// ** create menu item for last opend files
	m_pLastFilesSubMenu = new QPopupMenu( m_pFile );
	connect( m_pLastFilesSubMenu, SIGNAL( aboutToShow() ), this, SLOT( sltUpdateLastFilesMenu() ) );
	connect( m_pLastFilesSubMenu, SIGNAL( activated(int) ), this, SLOT( sltLastFilesMenuActivated(int) ) );

	m_pImportExportFilesSubMenu = new QPopupMenu( m_pFile );
    m_pFileImportXMLAction->addTo( m_pImportExportFilesSubMenu );
    m_pFileExportXMLAction->addTo( m_pImportExportFilesSubMenu );
	m_pImportExportFilesSubMenu->insertSeparator();
    m_pFileImportDynGraphAction->addTo( m_pImportExportFilesSubMenu );
    m_pFileExportDynGraphAction->addTo( m_pImportExportFilesSubMenu );
	m_pImportExportFilesSubMenu->insertSeparator();
    m_pFileExportAction->addTo( m_pImportExportFilesSubMenu );
	m_pFileExportAVIAction->addTo( m_pImportExportFilesSubMenu );

    m_pFileNewAction->addTo( m_pFile );
	m_pFile->insertSeparator();
    m_pFileLoadAction->addTo( m_pFile );
	m_pFileLoadForEditAction->addTo( m_pFile );
    m_pFileSaveAction->addTo( m_pFile );
    m_pFileSaveAsAction->addTo( m_pFile );
	m_pFileMakeRelPathsAction->addTo( m_pFile );
	m_pFile->insertSeparator();
    m_pFilePrintAction->addTo( m_pFile );
	m_pFile->insertSeparator();
	m_pFile->insertItem( tr( "&Import/Export" ), m_pImportExportFilesSubMenu );
	m_pFile->insertItem( tr( "Las&t files" ), m_pLastFilesSubMenu );
	m_pFile->insertSeparator();
    m_pFileExitAction->addTo( m_pFile );

	// *** submenu: edit ***
    m_pEditUndoAction = new QAction( tr( "Undo last operation" ), tr( "&Undo" ), ALT+Key_Backspace, this, "undo" );
    connect( m_pEditUndoAction, SIGNAL( activated() ), m_pControler, SLOT( sltUndoOperation() ) );
	m_pEditUndoAction->setEnabled( false );
    m_pEditRedoAction = new QAction( tr( "Redo last operation" ), tr( "&Redo" ), CTRL+Key_Backspace, this, "redo" );
    connect( m_pEditRedoAction, SIGNAL( activated() ), m_pControler, SLOT( sltRedoOperation() ) );
	m_pEditRedoAction->setEnabled( false );

    m_pEditCutAction = new QAction( tr( "Cut" ), tr( "C&ut" ), CTRL+Key_X, this, "cut" );
    connect( m_pEditCutAction, SIGNAL( activated() ), m_pControler, SLOT( sltCutClipboard() ) );
    m_pEditCopyAction = new QAction( tr( "Copy" ), tr( "&Copy" ), CTRL+Key_C, this, "copy" );
    connect( m_pEditCopyAction, SIGNAL( activated() ), m_pControler, SLOT( sltCopyClipboard() ) );
    m_pEditPasteAction = new QAction( tr( "Paste" ), tr( "&Paste" ), CTRL+Key_V, this, "paste" );
    connect( m_pEditPasteAction, SIGNAL( activated() ), m_pControler, SLOT( sltPasteClipboard() ) );
    m_pEditSelectAllAction = new QAction( tr( "Select all" ), tr( "Select &all" ), CTRL+Key_A, this, "selectall" );
    connect( m_pEditSelectAllAction, SIGNAL( activated() ), m_pControler, SLOT( sltSelectAllClipboard() ) );
    m_pEditNewDiaAction = new QAction( tr( "Append dia" ), tr( "Append d&ia" ), CTRL+Key_I, this, "append_dia" );
    connect( m_pEditNewDiaAction, SIGNAL( activated() ), this, SLOT( sltNewItem() ) );
    m_pEditAddDiaAction = new QAction( tr( "Add dia at current position" ), tr( "Add &dia" ), ALT+Key_I, this, "add_dia" );
    connect( m_pEditAddDiaAction, SIGNAL( activated() ), this, SLOT( sltAddItem() ) );
    
	m_pEditDeleteAction = new QAction( tr( "Delete" ), tr( "&Delete" ), Key_Delete, this, "delete" );
    connect( m_pEditDeleteAction, SIGNAL( activated() ), m_pControler, SLOT( sltDeleteSelectedItems() ) );

    m_pEditFindAction = new QAction( tr( "Find" ), tr( "&Find..." ), CTRL+Key_F, this, "find" );
    connect( m_pEditFindAction, SIGNAL( activated() ), this, SLOT( sltFindItem() ) );
    m_pEditFindNextAction = new QAction( tr( "Find next" ), tr( "Find &next" ), Key_F3, this, "find_next" );
    connect( m_pEditFindNextAction, SIGNAL( activated() ), this, SLOT( sltFindNextItem() ) );

    m_pEditUndoAction->addTo( m_pEdit );
    m_pEditRedoAction->addTo( m_pEdit );
	m_pEdit->insertSeparator();
    m_pEditCutAction->addTo( m_pEdit );
    m_pEditCopyAction->addTo( m_pEdit );
    m_pEditPasteAction->addTo( m_pEdit );
    m_pEditSelectAllAction->addTo( m_pEdit );
	m_pEdit->insertSeparator();
	m_pEditFindAction->addTo( m_pEdit );
	m_pEditFindNextAction->addTo( m_pEdit );
	m_pEdit->insertSeparator();
    m_pEditNewDiaAction->addTo( m_pEdit );
    m_pEditAddDiaAction->addTo( m_pEdit );
    m_pEditDeleteAction->addTo( m_pEdit );

	// *** submenu: play ***
    m_pPlayStartAction = new QAction( tr( "Start" ), aRunIcon, tr( "Sta&rt" ), CTRL+Key_R, this, "start" );
    connect( m_pPlayStartAction, SIGNAL( activated() ), m_pControler, SLOT( sltPlayPresentation() ) );
	m_pPlayPauseAction = new QAction( tr( "Pause" ), aPauseIcon, tr( "&Pause" ), CTRL+Key_P, this, "start" );
    connect( m_pPlayPauseAction, SIGNAL( activated() ), m_pControler, SLOT( sltPausePresentation() ) );
    m_pPlayStopAction = new QAction( tr( "Stop" ), aStopIcon, tr( "S&top" ), CTRL+Key_T, this, "stop" );
    connect( m_pPlayStopAction, SIGNAL( activated() ), m_pControler, SLOT( sltStopPresentation() ) );
	// ** go to position and start from that position:
    m_pPlayStartFromAction = new QAction( tr( "Start from selected position (goto position and start)" ), tr( "Start &from selected" ), ALT+Key_R, this, "start_from" );
    connect( m_pPlayStartFromAction, SIGNAL( activated() ), this, SLOT( sltPlayFromSelected() ) );
	// ** start from already selected position (dia is already positioned correctly):
    m_pPlayStartAtAction = new QAction( tr( "Start at selected position (just start at position)" ), tr( "Start &at selected" ), CTRL+ALT+Key_R, this, "start_at" );
    connect( m_pPlayStartAtAction, SIGNAL( activated() ), this, SLOT( sltPlayAtSelected() ) );
	// ** set a comment mark at actual time-position
    m_pPlayAddSoundCommentAction = new QAction( tr( "Add sound comment" ), tr( "A&dd sound comment" ), CTRL+Key_B, this, "addsoundcomment" );
    connect( m_pPlayAddSoundCommentAction, SIGNAL( activated() ), m_pControler, SLOT( sltAddSoundComment() ) );
	// ** set a dynamic graphic operation mark at actual time-position
    m_pPlayAddGraphicOpAction = new QAction( tr( "Add graphic operation" ), tr( "Add &graphic operation" ), CTRL+Key_G, this, "addgraphicop" );
    connect( m_pPlayAddGraphicOpAction, SIGNAL( activated() ), m_pControler, SLOT( sltAddGraphicOperation() ) );

    m_pPlayEditFadeTimeAction = new QAction( tr( "Edit dissolve time" ), tr( "&Dissolve time..." ), 0, this, "edit_fade_time" );
    connect( m_pPlayEditFadeTimeAction, SIGNAL( activated() ), this, SLOT( sltEditFadeInTime() ) );
    m_pPlayFadeInAction = new QAction( tr( "Fade in test" ), tr( "&Fade in test" ), ALT+Key_F, this, "fade_in" );
    connect( m_pPlayFadeInAction, SIGNAL( activated() ), this, SLOT( sltFadeInTest() ) );
    m_pPlayFadeOutAction = new QAction( tr( "Fade out test" ), tr( "Fade &out test" ), ALT+Key_O, this, "fade_out" );
    connect( m_pPlayFadeOutAction, SIGNAL( activated() ), this, SLOT( sltFadeOutTest() ) );

	m_pPlayStartFromAction->addTo( m_pPlay );
	m_pPlayStartAtAction->addTo( m_pPlay );
	m_pPlay->insertSeparator();
    m_pPlayStartAction->addTo( m_pPlay );
    m_pPlayPauseAction->addTo( m_pPlay );
    m_pPlayStopAction->addTo( m_pPlay );
	m_pPlay->insertSeparator();
	m_pPlayAddSoundCommentAction->addTo( m_pPlay );
	m_pPlayAddGraphicOpAction->addTo( m_pPlay );
	m_pPlay->insertSeparator();
	m_pPlayEditFadeTimeAction->addTo( m_pPlay );
    m_pPlayFadeInAction->addTo( m_pPlay );
    m_pPlayFadeOutAction->addTo( m_pPlay );

	// *** submenu: extras ***
    m_pExtrasConfigAction = new QAction( tr( "Configuration" ), tr( "&Configuration..." ), ALT+Key_C, this, "configuration"/*, TRUE*/ );
    connect( m_pExtrasConfigAction, SIGNAL( activated() ), this, SLOT( sltDoConfiguration() ) );
    m_pExtrasConfigPlayerAction = new QAction( tr( "mp3 player configuration" ), tr( "Pla&yer configuration..." ), ALT+Key_Y, this, "playerconfiguration"/*, TRUE*/ );
    connect( m_pExtrasConfigPlayerAction, SIGNAL( activated() ), this, SLOT( sltDoPlayerConfiguration() ) );
#ifndef __linux__
	m_pExtrasConfigPlayerAction->setEnabled( false );
#endif
	m_pExtrasLoggingAction = new QAction( tr( "Logging" ), tr( "&Logging..." ), ALT+Key_L, this, "logging", TRUE );
    connect( m_pExtrasLoggingAction, SIGNAL( activated() ), this, SLOT( sltDoLogging() ) );
	m_pExtrasControlProjectorAction = new QAction( tr( "Projector control dialog" ), tr( "Pro&jector control..." ), ALT+Key_J, this, "controlprojector", TRUE );
    connect( m_pExtrasControlProjectorAction, SIGNAL( activated() ), this, SLOT( sltDoControlProjector() ) );
	m_pExtrasPresentationDataAction = new QAction( tr( "Presentation data" ), tr( "Presentation &data..." ), CTRL+Key_D, this, "presentationdata"/*, TRUE*/ );
    connect( m_pExtrasPresentationDataAction, SIGNAL( activated() ), this, SLOT( sltDoPresentationData() ) );
	m_pExtrasPresentationEventsAction = new QAction( tr( "Presentation events" ), tr( "Presentation &events..." ), CTRL+Key_E, this, "presentationevents"/*, TRUE*/ );
    connect( m_pExtrasPresentationEventsAction, SIGNAL( activated() ), this, SLOT( sltDoPresentationEvents() ) );	
	m_pExtrasSoundDataAction = new QAction( tr( "Sound data" ), tr( "So&und data..." ), CTRL+Key_U, this, "sounddata"/*, TRUE*/ );
    connect( m_pExtrasSoundDataAction, SIGNAL( activated() ), this, SLOT( sltDoSoundData() ) );
	m_pExtrasSoundCommentAction = new QAction( tr( "Sound comments" ), tr( "Sou&nd comments..." ), CTRL+Key_Z, this, "soundcomment"/*, TRUE*/ );
    connect( m_pExtrasSoundCommentAction, SIGNAL( activated() ), this, SLOT( sltDoSoundComment() ) );
	m_pExtrasPlotCommentAction = new QAction( tr( "Plot comments" ), tr( "&Plot comments..." ), ALT+Key_Z, this, "plotcomment"/*, TRUE*/ );
    connect( m_pExtrasPlotCommentAction, SIGNAL( activated() ), this, SLOT( sltDoPlotComment() ) );
	m_pExtrasDynGraphOpAction = 0; //new QAction( tr( "Dynamic graphic operations" ), tr( "D&yn. graphic operations..." ), ALT+Key_G, this, "dyngraphops"/*, TRUE*/ );
    //connect( m_pExtrasDynGraphOpAction, SIGNAL( activated() ), this, SLOT( sltDoDynGraphicOp() ) );
	m_pExtrasModifyItemAction = new QAction( tr( "Modify item" ), tr( "&Modify item..." ), CTRL+Key_M, this, "modifyitem", TRUE );
    connect( m_pExtrasModifyItemAction, SIGNAL( activated() ), this, SLOT( sltDoModifyItem() ) );
	m_pExtrasPlayStatusAction = new QAction( tr( "Play infos" ), tr( "Pla&y infos..." ), CTRL+Key_Y, this, "playinfos", TRUE );
    connect( m_pExtrasPlayStatusAction, SIGNAL( activated() ), this, SLOT( sltDoPlayInfos() ) );

	m_pExtrasConfigAction->addTo( m_pExtras );
	m_pExtrasConfigPlayerAction->addTo( m_pExtras );
	m_pExtrasLoggingAction->addTo( m_pExtras );
	m_pExtrasControlProjectorAction->addTo( m_pExtras );
	m_pExtras->insertSeparator();
	m_pExtrasPresentationDataAction->addTo( m_pExtras );
	m_pExtrasPresentationEventsAction->addTo( m_pExtras );
	m_pExtrasSoundDataAction->addTo( m_pExtras );
	m_pExtrasSoundCommentAction->addTo( m_pExtras );
	m_pExtrasPlotCommentAction->addTo( m_pExtras );
	//m_pExtrasDynGraphOpAction->addTo( m_pExtras );
	m_pExtrasModifyItemAction->addTo( m_pExtras );
	m_pExtras->insertSeparator();
	m_pExtrasPlayStatusAction->addTo( m_pExtras );
	
	// *** submenu: scripts ***
	// will be filled in the dll-module !

	// *** submenu: help ***
    QAction * helpAboutAction = new QAction( tr( "About" ), tr( "&About..." ), CTRL+Key_F1, this, "about" );
    connect( helpAboutAction, SIGNAL( activated() ), this, SLOT( sltShowAbout() ) );
    QAction * helpAboutQtAction = new QAction( tr( "About Qt" ), tr( "About &Qt..." ), 0, this, "about_qt" );
    connect( helpAboutQtAction, SIGNAL( activated() ), this, SLOT( sltShowQtAbout() ) );
    //QAction * helpLicenseAction = new QAction( tr( "License" ), tr( "&License..." ), 0, this, "license" );
    //connect( helpLicenseAction, SIGNAL( activated() ), this, SLOT( sltShowLicense() ) );
    QAction * helpAction = new QAction( tr( "Help for mindia" ), tr( "&Help..." ), Key_F1, this, "help" );
    connect( helpAction, SIGNAL( activated() ), this, SLOT( sltShowHelpForMe() ) );

    helpAction->addTo( m_pHelp );
	m_pHelp->insertSeparator();
	//helpLicenseAction->addTo( m_pHelp );
    helpAboutAction->addTo( m_pHelp );
	helpAboutQtAction->addTo( m_pHelp );


	// *** connect signals to slots ***
    connect( this, SIGNAL( sigLoadDocFromFile(const QString &,bool) ), m_pControler, SLOT( sltLoadDoc(const QString &,bool) ) );
    connect( this, SIGNAL( sigSaveDocToFile(const QString &) ), m_pControler, SLOT( sltSaveAsDoc(const QString &) ) );
    connect( this, SIGNAL( sigSaveDocAsXML(const QString &) ), m_pControler, SLOT( sltSaveDocAsXML(const QString &) ) );
    connect( this, SIGNAL( sigQuit() ), qApp, SLOT( quit() ) );
	connect( this, SIGNAL( sigStartPresentation() ), m_pControler, SLOT( sltPlayPresentation() ) );
	connect( this, SIGNAL( sigShowPlayInfos() ), this, SLOT( sltDoPlayInfos() ) );

	connect( m_pEdit, SIGNAL( aboutToShow() ), this, SLOT( sltUpdateEditMenu() ) );
	connect( m_pFile, SIGNAL( aboutToShow() ), this, SLOT( sltUpdateFileMenu() ) );
	connect( m_pPlay, SIGNAL( aboutToShow() ), this, SLOT( sltUpdatePlayMenu() ) );
	connect( m_pExtras, SIGNAL( aboutToShow() ), this, SLOT( sltUpdateExtrasMenu() ) );
	// ** script-menu will be handled in dll
	//connect( m_pPlugins, SIGNAL( aboutToShow() ), this, SLOT( sltUpdateScriptsMenu() ) );

    connect( this, SIGNAL( sigDialogHelp(const QString &) ), this, SLOT( sltShowHelp(const QString &) ) );
}

void MinDiaWindow::CreateChildWidgets()
{
	const int iInitWidth = 795;
	const int iInitHeight1 = 260;
	const int iInitHeight2 = 260;

	// *** create now all the widgets
	m_pHBox			= new QHBox( this );

	m_pTargetBox	= new QVBox( m_pHBox );
	m_pSourceBox	= new QVBox( m_pHBox );

	m_pSlideView	= new HItemView( m_pTargetBox, iInitWidth, iInitHeight1, this, m_pControler, &m_pControler->GetPresentation() );
	m_pTimeLineView	= new TimeLineView( m_pTargetBox, iInitWidth, iInitHeight2, this, m_pControler, &m_pControler->GetPresentation() );

	// TODO --> ggf. optionale Anzeige als Text-Tabelle --> QTableView

/*
	m_pDiaPultView	= new IconItemView( m_pSourceBox, 200, iInitHeight1 );
	m_pDiaPultView->setSizeHint( QSize( 100, 100 ) );
	m_pAudioPultView = new IconItemView( m_pSourceBox, 200, iInitHeight2 );
	m_pAudioPultView->setSizeHint( QSize( 100, 100 ) );
*/
	m_pDiaStateView = 0; //new DiaStateView( m_pSourceBox, 200, iInitHeight );
	//m_pDiaStateView->setSizeHint( QSize( 100, 100 ) );

	m_pDiaPultView = 0;
	m_pAudioPultView = 0;

	setCentralWidget( m_pHBox );

	// ** create statusbar **
    /*QStatusBar * pStatusBar */ statusBar();

	m_pStatusBarMsg = new QLabel( statusBar() );
	m_pStatusBarModus = new QLabel( statusBar() );
	m_pStatusBarTime = new QLabel( statusBar() );
	statusBar()->addWidget( m_pStatusBarMsg, 5, FALSE );
	statusBar()->addWidget( m_pStatusBarModus, 1, FALSE );
	statusBar()->addWidget( m_pStatusBarTime, 3, FALSE );
	//m_pStatusBarMsg->setText( tr( "message" ) );
	//m_pStatusBarModus->setText( tr( "modus" ) );
	//m_pStatusBarTime->setText( tr( "time" ) );
	
	// ** start status-update timer **
	m_pStatusUpdateTimer = new QTimer( this );
	connect( m_pStatusUpdateTimer, SIGNAL( timeout() ), this, SLOT( sltStatusUpdateTimerEvent() ) );
	m_pStatusUpdateTimer->start( c_ulStatusBarTimer );
}

void MinDiaWindow::sltDoPlayerConfiguration()
{
	if( !m_pConfigPlayerDialog )
	{
		m_pConfigPlayerDialog = new ConfigPlayerDlgImpl( m_pControler->GetIniDB(), this, "configplayerdlg", /*modal*/TRUE );
		//m_pConfigPlayerDialog->move( 450, 10 );
	}

	int iRet = m_pConfigPlayerDialog->exec();

	if( iRet == 1 )
	{
		// ** ok 
		// ** nothing to do, all things will be done in the dialog !
	}

	// ** destroy modal dialog ! **
	delete m_pConfigPlayerDialog;
	m_pConfigPlayerDialog = 0;
}

void MinDiaWindow::sltDoConfiguration()
{
/*	if( !m_pExtrasConfigAction->isOn() )
	{
		if( m_pConfigurationDialog )
		{
			m_pConfigurationDialog->hide();
		}
	}
	else
*/	{
		if( !m_pConfigurationDialog )
		{
			m_pConfigurationDialog = new ConfigurationDlgImpl( m_pControler, this, "configdlg", /*modal*/TRUE );
			//m_pConfigurationDialog->move( 450, 10 );
		}

		int iRet = m_pConfigurationDialog->exec();

		if( iRet == 1 )
		{
			// ** ok 
			// ** nothing to do, all things will be done in the dialog !
		}

		// ** destroy modal dialog ! **
		delete m_pConfigurationDialog;
		m_pConfigurationDialog = 0;
	}
}

void MinDiaWindow::sltDoModifyItem()
{
	// ** at this point the state of the action is allready toggled !
	// ** so we need the 'negative' logic !
	if( !m_pExtrasModifyItemAction->isOn() )
	{
		if( m_pDiaInfoDialog )
		{
			m_pDiaInfoDialog->hide();

			//delete m_pDiaInfoDialog;
			//m_pDiaInfoDialog = 0;
		}
	}
	else
	{
		if( !m_pDiaInfoDialog )
		{
			m_pDiaInfoDialog = new DiaInfoDlgImpl( m_pSlideView, this );
			m_pDiaInfoDialog->move( 450, 330 );
		}

		m_pDiaInfoDialog->show();

		// ** update the data for the selected item
		sltItemSelected( m_iCount, m_pFirstSelectedItem, m_iFirstSelectedItemNo, 0 );
	}
}

void MinDiaWindow::sltShowModifyItem()
{
	if( !m_pExtrasModifyItemAction->isOn() )
	{
		m_pExtrasModifyItemAction->setOn( true );
		sltDoModifyItem();
	}
	m_pDiaInfoDialog->show();
}

void MinDiaWindow::sltDoSoundData()
{
/*	if( !m_pExtrasSoundDataAction->isOn() )
	{
		if( m_pSoundInfoDialog )
		{
			m_pSoundInfoDialog->hide();
		}
	}
	else
*/	{
		if( !m_pSoundInfoDialog )
		{
			m_pSoundInfoDialog = new SoundInfoDlgImpl( &m_pControler->GetPresentation().GetSoundInfoData(), this, "sounddata", /*modal*/TRUE );
			//m_pSoundInfoDialog->move( 10, 350 );
		}

		m_pSoundInfoDialog->exec();

		// ** destroy modal dialog ! **
		delete m_pSoundInfoDialog;
		m_pSoundInfoDialog = 0;
	}
}

void MinDiaWindow::sltShowSoundData()
{
	sltDoSoundData();
}

void MinDiaWindow::sltDoSoundComment()
{
/*	if( !m_pExtrasSoundCommentAction->isOn() )
	{
		if( m_pSoundCommentDialog )
		{
			m_pSoundCommentDialog->hide();
		}
	}
	else
*/	{
		if( !m_pSoundCommentDialog )
		{
			m_pSoundCommentDialog = new CommentDlgImpl( &m_pControler->GetPresentation().GetSoundCommentData(), this, "soundcomment", /*modal*/TRUE );
			//m_pSoundCommentDialog->move( 10, 350 );
		}

		m_pSoundCommentDialog->exec();

		// ** destroy modal dialog ! **
		delete m_pSoundCommentDialog;
		m_pSoundCommentDialog = 0;
	}
}

void MinDiaWindow::sltShowSoundComment()
{
	sltDoSoundComment();
}

void MinDiaWindow::sltDoPlotComment()
{
/*	if( !m_pExtrasPlotCommentAction->isOn() )
	{
		if( m_pPlotCommentDialog )
		{
			m_pPlotCommentDialog->hide();
		}
	}
	else
*/	{
		if( !m_pPlotCommentDialog )
		{
			m_pPlotCommentDialog = new CommentDlgImpl( &m_pControler->GetPresentation().GetPlotCommentData(), this, "plotcomment", /*modal*/TRUE );
			//m_pPlotCommentDialog->move( 10, 350 );
		}

		m_pPlotCommentDialog->exec();

		// ** destroy modal dialog ! **
		delete m_pPlotCommentDialog;
		m_pPlotCommentDialog = 0;
	}
}

void MinDiaWindow::sltDoDynGraphicOp()
{
/*	if( !m_pExtrasDynGraphOpAction->isOn() )
	{
		if( m_pDynGraphicOpDialog )
		{
			m_pDynGraphicOpDialog->hide();
		}
	}
	else
*/	{
		if( !m_pDynGraphicOpDialog )
		{
			m_pDynGraphicOpDialog = new CommentDlgImpl( /*&m_pControler->GetPresentation().GetPlotCommentData()*/0, this, "dyngraphicsop", /*modal*/TRUE );
			//m_pDynGraphicOpDialog->move( 10, 350 );
		}

		m_pDynGraphicOpDialog->exec();

		// ** destroy modal dialog ! **
		delete m_pDynGraphicOpDialog;
		m_pDynGraphicOpDialog = 0;
	}
}

void MinDiaWindow::sltShowPlotComment()
{
	sltDoPlotComment();
}

void MinDiaWindow::sltDoPlayInfos()
{
	if( m_pPlayInfoDialog && !m_pExtrasPlayStatusAction->isOn() )
	{
		if( m_pPlayInfoDialog )
		{
			m_pPlayInfoDialog->hide();
		}
	}
	else
	{
		if( !m_pPlayInfoDialog )
		{
			m_pPlayInfoDialog = new PlayInfoDlgImpl( m_pControler, this );
			m_pPlayInfoDialog->move( 10, 350 );

			if( m_iPosX>0 && m_iPosY>0 )
			{
				m_pPlayInfoDialog->SetPos( m_iPosX, m_iPosY );
			}

			if( m_iScreenX>0 && m_iScreenY>0 )
			{
				m_pPlayInfoDialog->SetSize( m_iScreenX, m_iScreenY );
			}

			// update the states of the run, pause and stop buttons in this dialog
			sltUpdatePlayMenu();
		}

		m_pPlayInfoDialog->show();
	}
}

void MinDiaWindow::sltShowPlayStatus()
{
	if( !m_pExtrasPlayStatusAction->isOn() )
	{
		m_pExtrasPlayStatusAction->setOn( true );
		sltDoPlayInfos();
	}
	m_pPlayInfoDialog->show();
}

void MinDiaWindow::sltDoPresentationData()
{
/*
	if( !m_pExtrasPresentationDataAction->isOn() )
	{
		if( m_pPresentationDataDialog )
		{
			m_pPresentationDataDialog->hide();
		}
	}
	else
*/	{
		if( !m_pPresentationDataDialog )
		{
			m_pPresentationDataDialog = new PresentationDataDlgImpl( &(m_pControler->GetPresentation()), this, "presentationdata", /*modal*/TRUE );
			//m_pPresentationDataDialog->move( 450, 10 );
		}

		m_pPresentationDataDialog->exec();

		// ** destroy modal dialog ! **
		delete m_pPresentationDataDialog;
		m_pPresentationDataDialog = 0;
	}
}

void MinDiaWindow::sltDoPresentationEvents()
{
/*
	if( !m_pExtrasPresentationEventsAction->isOn() )
	{
		if( m_pPresentationEventsDialog )
		{
			m_pPresentationEventsDialog->hide();
		}
	}
	else
*/	{
		if( !m_pPresentationEventsDialog )
		{
			m_pPresentationEventsDialog = new EventMapDlgImpl( &(m_pControler->GetPresentation().GetScriptEnvironment()), this, "presentationevents", /*modal*/TRUE );
			//m_pPresentationEventsDialog->move( 450, 10 );
		}

		m_pPresentationEventsDialog->exec();

		// ** destroy modal dialog ! **
		delete m_pPresentationEventsDialog;
		m_pPresentationEventsDialog = 0;

		// ** update the document state, scripts maybe changed
		sltDoDocumentStateUpdate();
	}
}

void MinDiaWindow::sltDoControlProjector()
{
	if( !m_pExtrasControlProjectorAction->isOn() )
	{
		if( m_pProjectorControlDialog )
		{
			m_pProjectorControlDialog->hide();
		}
	}
	else
	{
		if( !m_pProjectorControlDialog )
		{
			m_pProjectorControlDialog = new ProjectorControlDlgImpl( &(m_pControler->GetDiaCom()), &(m_pControler->GetPresentation()), this );
			m_pProjectorControlDialog->move( 450, 10 );
		}

		m_pProjectorControlDialog->show();
	}
}

void MinDiaWindow::sltDoLogging()
{
	if( !m_pExtrasLoggingAction->isOn() )
	{
		m_pLoggingDialog->hide();
	}
	else
	{
		m_pLoggingDialog->show();
	}
}

void MinDiaWindow::sltShowAbout()
{
	AboutExtDlg * pAboutDlg = new AboutExtDlg( this, "about", /*modal*/TRUE );

	string sText;
	bool bOk = ReadTextFile( "COPYING", sText );

	QString sLicense = tr( "This program is released under the GPL Version 2:\n\n" );

	if( bOk )
	{
		sLicense += sText.c_str();
	}
	else
	{
		sLicense += "http://www.fsf.org/licenses/gpl.html";
	}

	pAboutDlg->m_pLicenseEdit->setText( sLicense );	
	
	pAboutDlg->exec();

	delete pAboutDlg;
}

void MinDiaWindow::sltShowQtAbout()
{
	QMessageBox::aboutQt( this, "MinDia Qt Version" );
}

void MinDiaWindow::sltShowLicense()
{
	LicenseDlg * pLicenseDlg = new LicenseDlg( this, "license", /*modal*/TRUE );

	string sText;
	bool bOk = ReadTextFile( "COPYING", sText );

	if( bOk )
	{
		QString sLicense = tr( "This program is released under the GPL Version 2:\n\n" );
		sLicense += sText.c_str();

		pLicenseDlg->m_pEdit->setText( sLicense );

		pLicenseDlg->exec();
	}

	delete pLicenseDlg;
}

void MinDiaWindow::sltModifyItemDialogClosed()
{
	m_pExtrasModifyItemAction->setOn( false );
}

void MinDiaWindow::sltPlayInfoDialogClosed()
{
	m_pExtrasPlayStatusAction->setOn( false );
}

void MinDiaWindow::sltProjectorControlDialogClosed()
{
	m_pExtrasControlProjectorAction->setOn( false );
}

void MinDiaWindow::sltLoggingDialogClosed()
{
	m_pExtrasLoggingAction->setOn( false );
}

void MinDiaWindow::sltSoundDataDialogClosed()
{
	m_pExtrasSoundDataAction->setOn( false );
}

void MinDiaWindow::sltSoundCommentDialogClosed()
{
	m_pExtrasSoundCommentAction->setOn( false );
}

void MinDiaWindow::sltPlotCommentDialogClosed()
{
	m_pExtrasPlotCommentAction->setOn( false );
}

void MinDiaWindow::sltPresentationDataDialogClosed()
{
	m_pExtrasPresentationDataAction->setOn( false );
}

void MinDiaWindow::sltPresentationEventsDialogClosed()
{
	m_pExtrasPresentationEventsAction->setOn( false );
}

void MinDiaWindow::sltConfigurationDialogClosed()
{
	m_pExtrasConfigAction->setOn( false );
}

void MinDiaWindow::sltAskNewDoc()
{
	// ask user before deleting the actual document...
	int iRet = QMessageBox::warning( 0, tr( "MinDia - Question" ), 
										tr( "Really create a new document and loose all changes ?" ), 1, 2 );
	if( iRet == 1 )
	{
		m_pControler->sltNewDoc();
	}
}

void MinDiaWindow::sltLoadDoc( const QString & sFileName, bool bExecuteEvent )
{
    if( !sFileName.isEmpty() )
	{
    	sltCheckCloseApplication();

		emit sigLoadDocFromFile( sFileName, bExecuteEvent );

		// ** select the first slide after reading the file
		sltSelectItem( 0, 0 );
	}
    else
	{
		statusBar()->message( tr( "Loading aborted" ), c_ulStatusTimeout );
	}
}

void MinDiaWindow::sltAskLoadDoc()
{
    QString sFileName = QFileDialog::getOpenFileName( /*QString::null*/GetDataPath().c_str(), /*QString::null*/"*.dia", this, "load", tr( "Open" ) );

	sltLoadDoc( sFileName, /*bExecuteEvent*/true );
}

void MinDiaWindow::sltAskLoadForEditDoc()
{
    QString sFileName = QFileDialog::getOpenFileName( /*QString::null*/GetDataPath().c_str(), /*QString::null*/"*.dia", this, "load_for_dit", tr( "Open for edit" ) );

	sltLoadDoc( sFileName, /*bExecuteEvent*/false );
}

void MinDiaWindow::sltAskSaveAsDoc()
{
	QString sFileName = QFileDialog::getSaveFileName( /*QString::null*/GetDataPath().c_str(), /*QString::null*/"*.dia", this, "save", tr( "Save as" ) );

    if( !sFileName.isEmpty() )
	{
		emit sigSaveDocToFile( sFileName );
	}
    else
	{
		statusBar()->message( tr( "Save was aborted" ), c_ulStatusTimeout );
	}
}

void MinDiaWindow::sltMakeRelPaths()
{
	m_pControler->GetPresentation().MakeRelativePaths();
	
	sltDoUpdateAllViews();
}

void MinDiaWindow::sltPrintDoc()
{
	QPrinter aPrinter;

	bool bOk = aPrinter.setup();

	if( bOk )
	{
		sltShowErrorMessage( tr( "Sorry, printing not implemented yet !" ) );
	}
}

void MinDiaWindow::sltImportXMLDoc()
{
	// min todo gulp
	sltShowErrorMessage( tr( "Not implemented yet !" ) );
}

void MinDiaWindow::sltExportXMLDoc()
{
    QString sFileName = QFileDialog::getSaveFileName( QString::null, /*QString::null*/"*.xml", this, "save", tr( "Export as XML" ) );

    if( !sFileName.isEmpty() )
	{
		emit sigSaveDocAsXML( sFileName );
	}
    else
	{
		statusBar()->message( tr( "Export to XML was aborted" ), c_ulStatusTimeout );
	}
}

void MinDiaWindow::sltExportDoc()
{
	// use script interface for exporting data in other formats (i.e. csv)
	sltShowErrorMessage( tr( "Use scripts to export the presentation-data for other applications." ) );
}

void MinDiaWindow::sltExportAVI()
{
	CreateMovieDlgImpl * pDlg = new CreateMovieDlgImpl( m_pControler->GetPresentation().GetTotalTime()*1000, this, "create_move", /*modal*/TRUE );
		
	int iRet = pDlg->exec();

	if( iRet == 1 )
	{
		QString sName = pDlg->m_pImageNameOffset->text();
		QString sDir = pDlg->m_pDirectoryName->text();
		QString sCmd = pDlg->m_pGeneratorCmd->text();
		int iImagesPerSecond = pDlg->m_pImagesPerSecond->text().toInt();
		int iWidth = pDlg->m_pImageWidth->text().toInt();
		int iHeight = pDlg->m_pImageHeight->text().toInt();

		double dDeltaInMS = 1/((double)iImagesPerSecond)*1000.0;

		int iCount = m_pControler->CreateImagesForMovie( 
			    (const char *)sDir, (const char *)sName, 
				iWidth, iHeight,
				0, m_pControler->GetPresentation().GetTotalTime()*1000, dDeltaInMS );

		if( iCount>0 )
		{
//			int iRet = system( (const char *)sCmd );
//			cerr << "system(\"" << (const char *)sCmd << "\") = " << iRet << endl;
		}
	}

	delete pDlg;

}

void MinDiaWindow::sltExportDynGraphData()
{
    QString sFileName = QFileDialog::getSaveFileName( QString::null, /*QString::null*/"*.dat", this, "save", tr( "Export dynamic graphic data" ) );

    if( !sFileName.isEmpty() )
	{
		m_pControler->sltExportDynGraphicData( sFileName );
	}
}

void MinDiaWindow::sltImportDynGraphData()
{
    QString sFileName = QFileDialog::getOpenFileName( QString::null, /*QString::null*/"*.dat", this, "read", tr( "Import dynamic graphic data" ) );

    if( !sFileName.isEmpty() )
	{
		m_pControler->sltImportDynGraphicData( sFileName );
	}
}

void MinDiaWindow::sltCheckCloseApplication()
{
	if( m_pControler->IsChanged() )
	{
		int iRet = QMessageBox::warning( this, tr( "MinDia - Warning" ), tr( "Write changed data ?" ), tr( "Yes" ), tr( "No" ) );

		if( iRet == 0 )
		{
			bool bOk;
			m_pControler->sltSaveDoc( bOk );
		}
	}
}

void MinDiaWindow::sltUpdateEditMenu()
{
	bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	bool bEdit = !(bIsPlaying || bIsPause);
	bool bValue = bEdit;

	m_pEditUndoAction->setEnabled( false );
	m_pEditRedoAction->setEnabled( false );
	m_pEditCutAction->setEnabled( bValue );
	m_pEditCopyAction->setEnabled( bValue );
	m_pEditPasteAction->setEnabled( bValue );
	m_pEditDeleteAction->setEnabled( bValue );
	m_pEditSelectAllAction->setEnabled( bValue );
	m_pEditNewDiaAction->setEnabled( bValue );
	m_pEditAddDiaAction->setEnabled( bValue );
	m_pEditFindAction->setEnabled( bValue );
	m_pEditFindNextAction->setEnabled( bValue );

	if( bEdit )
	{
		if( IsValidClipboardData() )
		{
			m_pEditPasteAction->setEnabled( true );
		}
		else
		{
			m_pEditPasteAction->setEnabled( false );
		}

		if( m_pSlideView->GetSelectedCount()>0 )
		{
			m_pEditCutAction->setEnabled( true );
			m_pEditCopyAction->setEnabled( true );
			m_pEditDeleteAction->setEnabled( true );
		}
		else
		{
			m_pEditCutAction->setEnabled( false );
			m_pEditCopyAction->setEnabled( false );
			m_pEditDeleteAction->setEnabled( false );
		}
	}
}

void MinDiaWindow::sltUpdateFileMenu()
{
	bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	bool bEdit = !(bIsPlaying || bIsPause);
	bool bValue = bEdit;

	m_pFileNewAction->setEnabled( bValue );
	m_pFileLoadAction->setEnabled( bValue );
	m_pFileLoadForEditAction->setEnabled( bValue );
	m_pFileOpen->setEnabled( bValue );
	m_pFileSaveAction->setEnabled( bValue );
	m_pFileSave->setEnabled( bValue );
	m_pFileSaveAsAction->setEnabled( bValue );
	m_pFileExportAction->setEnabled( bValue );
	m_pFilePrintAction->setEnabled( bValue );

	m_pLastFilesSubMenu->setEnabled( bValue );
	m_pImportExportFilesSubMenu->setEnabled( bValue );
}

void MinDiaWindow::sltUpdatePlayMenu()
{
	bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	bool bEdit = !(bIsPlaying || bIsPause);
	bool bDiaSelected = (m_pSlideView->GetSelectedCount() == 1);

	m_pPlayStartFromAction->setEnabled( bEdit && bDiaSelected );
	m_pPlayStartAtAction->setEnabled( bEdit && bDiaSelected );
	m_pPlayStartAction->setEnabled( !bIsPlaying );
	m_pPlayPauseAction->setEnabled( bIsPlaying && !bIsPause );
	m_pPlayStopAction->setEnabled( bIsPlaying || bIsPause );
	m_pPlayAddSoundCommentAction->setEnabled( !bEdit );
	m_pPlayAddGraphicOpAction->setEnabled( !bEdit );

	m_pPlayRun->setEnabled( !bIsPlaying );
	m_pPlayPause->setEnabled( bIsPlaying && !bIsPause );
	m_pPlayStop->setEnabled( bIsPlaying || bIsPause );

	if( m_pPlayInfoDialog )
	{
		m_pPlayInfoDialog->UpdateStatus( bIsPlaying, bIsPause );
	}

	bool bTwoDiasSelected = (m_pSlideView->GetSelectedCount() == 2);
	m_pPlayFadeInAction->setEnabled( bTwoDiasSelected );
	m_pPlayFadeOutAction->setEnabled( bTwoDiasSelected );
}

void MinDiaWindow::sltUpdateExtrasMenu()
{
	bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	bool bEdit = !(bIsPlaying || bIsPause);

	m_pExtrasConfigAction->setEnabled( bEdit );
#ifndef __linux__
	m_pExtrasConfigPlayerAction->setEnabled( false );
#else
	m_pExtrasConfigPlayerAction->setEnabled( bEdit );
#endif
	m_pExtrasPresentationDataAction->setEnabled( bEdit );
	m_pExtrasPresentationEventsAction->setEnabled( bEdit );
	m_pExtrasSoundDataAction->setEnabled( bEdit );
	m_pExtrasSoundCommentAction->setEnabled( bEdit );
	m_pExtrasPlotCommentAction->setEnabled( bEdit );
}

void MinDiaWindow::sltUpdateLastFilesMenu()
{
	m_pLastFilesSubMenu->clear();

	MiniIniDB & aIniDB = m_pControler->GetIniDB();

	int iMax = aIniDB.GetHistoryQueueCount( g_sLastFilesKey );

	for( int i=iMax-1; i>=0; i-- )
	{
		string sValue;

		if( aIniDB.GetHistoryQueueValue( g_sLastFilesKey, i, sValue ) )
		{
			// ** show filename with number 

			QString s;

			s.sprintf( "&%d ", iMax-i );
			s += sValue.c_str();

			m_pLastFilesSubMenu->insertItem( s, i );
		}
	}
}

void MinDiaWindow::sltLastFilesMenuActivated( int iIndex )
{
	MiniIniDB & aIniDB = m_pControler->GetIniDB();

	string sFileName;

	if( aIniDB.GetHistoryQueueValue( g_sLastFilesKey, iIndex, sFileName ) )
	{
		sltLoadDoc( sFileName.c_str(), /*bExecuteEvent*/true );
	}
}

/*
void MinDiaWindow::sltUpdateScriptsMenu()
{
	bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	bool bEdit = !(bIsPlaying || bIsPause);

	// ** enable script dialog only in edit modus and if there is a script-engine available
//	minClientHandle<IGeneralScriptEnvironment> hScriptEnv( g_IGeneralScriptEnvironmentID );
//	bool bEnabled = bEdit && hScriptEnv.IsValid() && hScriptEnv->IsOk();
//	m_pPlugins->setEnabled( bEnabled );
}
*/

void MinDiaWindow::sltShowErrorMessage( const QString & sMessage )
{
	QMessageBox::warning( this, tr( "MinDia - Error" ), sMessage );
}

void MinDiaWindow::sltShowWarningMessage( const QString & sMessage )
{
	QMessageBox::warning( this, tr( "MinDia - Warning" ), sMessage );
}

void MinDiaWindow::sltShowStatusBarMessage( const QString & sMsg )
{
	if( m_pStatusBarMsg )
	{
		m_pStatusBarMsg->setText( sMsg );

		// clear tooltips showed over the status bar
		statusBar()->clear();
	}
}

void MinDiaWindow::SetHelpFile( HelpDlgImpl * pHelpDialog, const QString & sHelpTag ) const
{
	// ** help-file is language sensitive
	QString sHelp = GetMinDiaSharedDirectory().c_str();
	sHelp += "mindia_";
	sHelp += m_sLanguage;
	sHelp += ".html";

	// ** and add the tag for the jump in the document
	sHelp += "#" + sHelpTag;

	// ** possible Help tags are (see code: emit sigDialogHelp) **
	/*
		mindia
		LoggingDialog
		ConfigurationDialog
		DiaInfoDialog
		HItemView
		MinDiaWindow
		ProjectorControlDialog
		PresentationDataDialog
		PlayInfoDialog
		CommentDialog	(for Sound and Plot)
		SoundInfoDialog
		EventMapDialog

		ScriptModifyDialog
	*/

    pHelpDialog->m_pTextBrowser->mimeSourceFactory()->setFilePath( "." );
	pHelpDialog->m_pTextBrowser->mimeSourceFactory()->setExtensionType("html", "text/html;charset=iso8859-1");

	//pHelpDialog->m_pTextBrowser->setText( sHelp );
	pHelpDialog->m_pTextBrowser->setSource( sHelp );
}

void MinDiaWindow::sltShowModalHelp( QWidget * pParent, const QString & sHelpTag )
{
	HelpDlgImpl * pHelpDialog = new HelpDlgImpl( pParent, "help_temp", TRUE );

	pHelpDialog->move( 470, 5 );

	SetHelpFile( pHelpDialog, sHelpTag );

	pHelpDialog->exec();

	delete pHelpDialog;
}

void MinDiaWindow::sltShowHelp( const QString & sHelpTag )
{
	SetHelpFile( m_pHelpDialog, sHelpTag );

	m_pHelpDialog->show();
}

void MinDiaWindow::sltShowHelpForMe()
{
	sltShowHelp( /*html-tag*/"mindia" );
}

void MinDiaWindow::sltShowImageFile( const QString & sFileName )
{
	if( m_pPlayInfoDialog && m_pPlayInfoDialog->isVisible() )
	{
		m_pPlayInfoDialog->sltSetImage( sFileName );
	}
}

void MinDiaWindow::sltDoDocumentStateUpdate()
{
	// *** update caption of the window, with file name ! ***
	QString sCaption = "MinDia - [";
	sCaption += m_pControler->GetName().c_str();
	if( m_pControler->IsChanged() )
	{
		sCaption += " *";
	}
	sCaption += "] ";
	sCaption += m_pControler->GetPlayModusStrg();
	setCaption( sCaption );
}

void MinDiaWindow::sltDoSyncAllViews()
{
	m_pSlideView->SyncViewWithData();
	m_pTimeLineView->SyncViewWithData();
}

void MinDiaWindow::sltStartAutoStartTimer( bool bAutoRun, bool bShowScreen, bool bExitOnFinished, int iScreenX, int iScreenY, int iPosX, int iPosY )
{
	m_pAutoStartTimer = new QTimer( this );

	m_bAutoRun = bAutoRun;
	m_bShowScreen = bShowScreen;
	m_bExitOnFinished = bExitOnFinished;
	m_iScreenX = iScreenX;
	m_iScreenY = iScreenY;
	m_iPosX = iPosX;
	m_iPosY = iPosY;

	connect( m_pAutoStartTimer, SIGNAL( timeout() ), this, SLOT( sltAutoStartTimerEvent() ) );

	m_pAutoStartTimer->start( c_ulAutoStartTimeout );
}

void MinDiaWindow::sltAutoStartTimerEvent()
{
	m_pAutoStartTimer->stop();

	if( m_bShowScreen )
	{
		emit sigShowPlayInfos();
	}
	if( m_bAutoRun )
	{
		emit sigStartPresentation();
	}

	delete m_pAutoStartTimer;
	m_pAutoStartTimer = 0;
}

void MinDiaWindow::sltStatusUpdateTimerEvent()
{
	m_pStatusUpdateTimer->stop();

	//debug: cout << "*** UPDATE act=" << m_pControler->GetPresentation().GetActPlayTime() << " total=" << m_pControler->GetPresentation().GetTotalTime() << endl;

	double dActTime = m_pControler->GetPresentation().GetActPlayTime();

	QString sPlay;
	QString sTotal;
	QString sCountDown;

	QTime aPlayTime;
	aPlayTime = aPlayTime.addMSecs( (int)(dActTime*1000) );
	QTime aTotalTime;
	aTotalTime = aTotalTime.addMSecs( (int)(m_pControler->GetPresentation().GetTotalTime()*1000) );

	double dCountDown = m_pControler->GetPresentation().GetCountDownTime();
	sCountDown.sprintf( "%.1f", dCountDown );

	sPlay.sprintf( "%d:%02d", aPlayTime.minute(), aPlayTime.second() );
	sTotal.sprintf( "%d:%02d", aTotalTime.minute(), aTotalTime.second() );

	bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	bool bIsEdit = m_pControler->IsEditModus();
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
		sCount.setNum( m_pControler->GetPresentation().GetDiaCount() );
		sMsg += tr( " *** count: " ) + sCount;
	}

	{
		QtMTLock aMTLock;

		m_pStatusBarTime->setText( sMsg );
		m_pStatusBarModus->setText( m_pControler->GetPlayModusStrg() );

		// clear tooltips showed over the status bar
		// --> not here because than tooltips are never seen
		// because the status-bar is updated every few ms
		//statusBar()->clear();

		if( !bIsEdit )
		{
			// ** update play mark in TimeLineView **
			m_pTimeLineView->SetPlayMark( dActTime );
		}
		else
		{
			m_pTimeLineView->SetPlayMark( -1 );
		}
	}

	// ** restart the timer again
	m_pStatusUpdateTimer->start( c_ulStatusBarTimer );
}

void MinDiaWindow::sltDoUpdateAllViews()
{
	m_pSlideView->sltUpdateView();
	m_pTimeLineView->sltUpdateView();
}

void MinDiaWindow::sltDoDataChanged()
{
	sltDoUpdateAllViews();
	sltDoDocumentStateUpdate();
	//m_pControler->sltDataChanged();
}

void MinDiaWindow::sltPlayFromSelected()
{
	int iSelectedIndex = m_pSlideView->GetLastSelectedItemIndex();

	m_pControler->sltGotoPosition( iSelectedIndex );
	m_pControler->sltPlayPresentationAt( iSelectedIndex );
}

void MinDiaWindow::sltPlayAtSelected()
{
	int iSelectedIndex = m_pSlideView->GetLastSelectedItemIndex();

	m_pControler->sltPlayPresentationAt( iSelectedIndex );
}

void MinDiaWindow::sltItemSelected( int iCount, HItem * pFirstSelectedItem, int iFirstSelectedItemNo, int iDissolveTimeInMS )
{
	m_iCount = iCount;
	m_pFirstSelectedItem = pFirstSelectedItem;
	m_iFirstSelectedItemNo = iFirstSelectedItemNo;

	if( m_pDiaInfoDialog )
	{
		if( iCount==1 )
		{
			m_pDiaInfoDialog->sltUpdateData( pFirstSelectedItem, m_pControler->IsEditModus() );
		}
		else
		{
			m_pDiaInfoDialog->sltDisableDialog( true );
		}
	}

	if( m_pPlayInfoDialog )
	{
		bool bIsPlaying = m_pControler->IsPlayModus();

		if( iCount==1 )
		{
			QString sFileName = pFirstSelectedItem->GetInfoData()->GetImageFile();
			m_pPlayInfoDialog->sltSetImage( sFileName, bIsPlaying, iDissolveTimeInMS );
		}
		else
		{
			m_pPlayInfoDialog->sltSetImage( "", bIsPlaying, iDissolveTimeInMS );
		}
	}

	// ** pass info of selected item to sub-views
	m_pSlideView->sltItemSelected( iCount, iFirstSelectedItemNo );
	m_pTimeLineView->sltItemSelected( iCount, iFirstSelectedItemNo );

	// ** update the edit menu, because here are some accelerator-keys used...
	sltUpdateEditMenu();
	sltUpdatePlayMenu();
}

void MinDiaWindow::sltSelectItem( int iIndex, int iDissolveTimeInMS )
{
	m_pSlideView->sltSelectItem( iIndex, iDissolveTimeInMS );
	m_pTimeLineView->sltSelectItem( iIndex, iDissolveTimeInMS );
}

void MinDiaWindow::sltNewItem()
{
	m_pSlideView->sltNewItem();
}

void MinDiaWindow::sltAddItem()
{
	m_pSlideView->sltNewItemAfterSelected();
}

void MinDiaWindow::sltFindItem()
{
	EnterValueDlg aEnterDlg( this, "enter_text", /*modal*/TRUE );

	aEnterDlg.setCaption( tr( "Find" ) );
	aEnterDlg.m_pTextLabel->setText( tr( "search for:" ) );
	aEnterDlg.m_pEnterValue->setFocus();
	aEnterDlg.m_pEnterValue->setText( m_sFindText );

	int iRet = aEnterDlg.exec();

	if( iRet == 1 )
	{
		m_sFindText = aEnterDlg.m_pEnterValue->text();

		m_pControler->sltSelectItemWithText( m_sFindText );
	}
}

void MinDiaWindow::sltFindNextItem()
{
	if( m_sFindText.isEmpty() )
	{
		sltFindItem();
	}
	else
	{
		m_pControler->sltSelectNextItemWithText( m_sFindText );
	}
}

void MinDiaWindow::sltEditFadeInTime()
{
	EnterValueDlg aEnterDlg( this, "enter_dissolve_time", /*modal*/TRUE );

	aEnterDlg.setCaption( tr( "Enter new dissolve time" ) );
	aEnterDlg.m_pTextLabel->setText( tr( "dissolve time [s]:" ) );
	aEnterDlg.m_pEnterValue->setFocus();

	char sBuffer[255];
	sprintf( sBuffer, "%4.1f", m_dDissolveTime );
	aEnterDlg.m_pEnterValue->setText( sBuffer );

	int iRet = aEnterDlg.exec();

	if( iRet == 1 )
	{
		QString s = aEnterDlg.m_pEnterValue->text();

		m_dDissolveTime = s.toDouble();
	}
}

void MinDiaWindow::sltFadeInTest()
{
	if( m_pPlayInfoDialog )
	{
		HItem * pItem1 = 0;
		HItem * pItem2 = 0;

		if( m_pSlideView->GetTwoSelectedItems( pItem1, pItem2 ) )
		{
			QString sFileName1 = pItem1->GetInfoData()->GetImageFile();
			m_pPlayInfoDialog->sltSetImage( sFileName1, /*bIsPlaying*/false, 0 );

			int iDissolveTimeInMS = (int)(m_dDissolveTime * 1000.0);
			QString sFileName2 = pItem2->GetInfoData()->GetImageFile();
			m_pPlayInfoDialog->sltSetImage( sFileName2, /*bIsPlaying*/true, iDissolveTimeInMS );
		}
	}
}

void MinDiaWindow::sltFadeOutTest()
{
	if( m_pPlayInfoDialog )
	{
		HItem * pItem1 = 0;
		HItem * pItem2 = 0;

		if( m_pSlideView->GetTwoSelectedItems( pItem2, pItem1 ) )
		{
			QString sFileName1 = pItem1->GetInfoData()->GetImageFile();
			m_pPlayInfoDialog->sltSetImage( sFileName1, /*bIsPlaying*/false, 0 );

			int iDissolveTimeInMS = (int)(m_dDissolveTime * 1000.0);
			QString sFileName2 = pItem2->GetInfoData()->GetImageFile();
			m_pPlayInfoDialog->sltSetImage( sFileName2, /*bIsPlaying*/true, iDissolveTimeInMS );
		}
	}
}

void MinDiaWindow::sltModusIsSwitched()
{
	sltUpdateFileMenu();
	sltUpdateEditMenu();
	sltUpdatePlayMenu();
	sltUpdateExtrasMenu();
	//sltUpdateScriptsMenu();

	sltDoDocumentStateUpdate();

	bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	//bool bIsEdit = m_pControler->IsEditModus();

	if( m_pDiaInfoDialog )
	{
		if( bIsPlaying || bIsPause )
		{
			m_pDiaInfoDialog->sltDisableDialog( true );
		}
		else
		{
			m_pDiaInfoDialog->sltUpdateData( m_pFirstSelectedItem, m_pControler->IsEditModus() );
		}
	}

	// ** disable Dialog in play modus !
	if( m_pProjectorControlDialog && m_pProjectorControlDialog->isVisible() )
	{
		m_pProjectorControlDialog->sltCheckMode();
	}
}

void MinDiaWindow::sltPlayFinished()
{
	if( m_bExitOnFinished )
	{
		close();
	}
}

void MinDiaWindow::sltCopyClipboard()
{
	QClipboard * pClip = QApplication::clipboard();

	QString sData = "";
	if( pClip && m_pSlideView->GetActClipboardData( sData ) )
	{
 		pClip->setText( sData );
	}

	// ** update the state of the edit-menu, because now there is data in the clipboard
	sltUpdateEditMenu();
}

void MinDiaWindow::sltCutClipboard()
{
	sltCopyClipboard();
	sltDeleteSelectedItems();
}

void MinDiaWindow::sltPasteClipboard()
{
	QClipboard * pClip = QApplication::clipboard();

	if( pClip )
	{
		QString sData = pClip->text();
		if( !sData.isNull() )
		{
			m_pSlideView->SetFromClipboardData( sData );
		}
	}

	// ** update the state of the edit-menu 
	sltUpdateEditMenu();
}

void MinDiaWindow::sltSelectAllClipboard()
{
	m_pSlideView->sltSelectAllItems( true, true );
}

void MinDiaWindow::sltDeleteSelectedItems()
{
	m_pSlideView->sltDeleteAllSlectedItems();
}

bool MinDiaWindow::IsValidClipboardData()
{
	QClipboard * pClip = QApplication::clipboard();

	if( pClip )
	{
		QString sData = pClip->text();

		if( !sData.isNull() )
		{
			// ** you need at leas one valid item, to enable paste
			return m_pSlideView->GetCountValidClipboardData( sData ) > 0;
		}
	}

	return false;
}


void MinDiaWindow::closeEvent( QCloseEvent * pCloseEvent )
{
	sltCheckCloseApplication();

	if( m_pControler->IsEditModus() )
	{
		pCloseEvent->accept();
	}
	else
	{
		/*int iRet =*/ QMessageBox::warning( this, tr( "MinDia - Warning" ), tr( "Can't close the application, because the presentation is still playing !" ) );

		pCloseEvent->ignore();
	}
}

void MinDiaWindow::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Key_F1) )
	{
		emit sigDialogHelp( "MinDiaWindow" );
	}
	else
	{
		QMainWindow::keyPressEvent( pEvent );
	}
}

bool MinDiaWindow::IsValid() const
{
	return ( m_pPlayInfoDialog != 0 );
}

bool MinDiaWindow::Show()
{
	sltShowPlayStatus();
	return m_pPlayInfoDialog->isVisible(); 
}

bool MinDiaWindow::Hide()
{
	if( m_pPlayInfoDialog )
	{
		m_pPlayInfoDialog->hide();
		return m_pPlayInfoDialog->isVisible(); 
	}
	return true;
}

bool MinDiaWindow::Maximize()
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->Maximize();
	}
	return false;
}

bool MinDiaWindow::Restore()
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->Restore();
	}
	return false;
}

int MinDiaWindow::GetWidth() const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetWidth();
	}
	return 0;
}

int MinDiaWindow::GetHeight() const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetHeight();
	}
	return 0;
}

bool MinDiaWindow::SetSize( int iWidth, int iHeight )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetSize( iWidth, iHeight );
	}
	return false;
}

bool MinDiaWindow::SetPos( int iXPos, int iYPos )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetPos( iXPos, iYPos );
	}
	return false;
}

int MinDiaWindow::GetDrawWidth() const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetDrawWidth();
	}
	return 0;
}

int MinDiaWindow::GetDrawHeight() const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetDrawHeight();
	}
	return 0;
}

bool MinDiaWindow::Clear()
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->Clear();
	}
	return false;
}

bool MinDiaWindow::SetColor( int iRed, int iGreen, int iBlue )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetColor( iRed, iGreen, iBlue );
	}
	return false;
}

bool MinDiaWindow::SetFont( const char * sFontName )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetFont( sFontName );
	}
	return false;
}

bool MinDiaWindow::SetFontSize( int iSizeInPixel, bool bBold, bool bItalic )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetFontSize( iSizeInPixel, bBold, bItalic );
	}
	return false;
}

int MinDiaWindow::SetTextXY( int x, int y, const char * sText )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetTextXY( x, y, sText );
	}
	return -1;
}

int MinDiaWindow::GetTextCount() const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetTextCount();
	}
	return 0;
}

bool MinDiaWindow::MoveText( int iTextID, int x, int y )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->MoveText( iTextID, x, y );
	}
	return false;
}

int	MinDiaWindow::GetTextX( int iTextID ) const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetTextX( iTextID );
	}
	return 0;
}

int	MinDiaWindow::GetTextY( int iTextID ) const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetTextY( iTextID );
	}
	return 0;
}

int MinDiaWindow::GetTextWidth( int iTextID ) const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetTextWidth( iTextID );
	}
	return 0;
}

int	MinDiaWindow::GetTextHeight( int iTextID ) const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetTextHeight( iTextID );
	}
	return 0;
}

bool MinDiaWindow::SetTextColor( int iTextID, int iRed, int iGreen, int iBlue )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetTextColor( iTextID, iRed, iGreen, iBlue );
	}
	return false;
}

IColor MinDiaWindow::GetTextColor( int iTextID ) const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetTextColor( iTextID );
	}

	return IColor();
}

bool MinDiaWindow::DeleteText( int iTextID )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->DeleteText( iTextID );
	}
	return false;
}

QCanvas * MinDiaWindow::GetCanvas()
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetCanvas();
	}
	return 0;
}

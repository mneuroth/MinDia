/*
	Program name:	MinDia

		Start: 27. 8.2001
		Now:   19. 2.2003

	Program to create a slide show and run the show
	with dia-projectors from rollei:
	  - Rolleivision 35 twin digital P
	  - Rolleivision twin MSC 3x0 P

	(c) by Michael Neuroth, 2001-2003


	Contributions:
		minicom		com-port support for linux
			--> sysdep.h, sysdep1.c
		kwav		wav-file import, wav-file playing for linux
			--> wavfile.h, wavfile.cpp (modified)
		Python		the script-language for this application
		SWIG		for python (and other script languages) bindings
		cdparanoia	for wav-file generation from audio cd's (linux)
		LAME		for mp3-file encoding (smaller sound files for the Zaurus!)
		CDex		for wav-file and mp3-file generation from audio cd's (windows)

	Create Makefile for Linux:

		tmake mindia.pro -o Makefile
		tmake mindia.pro "CONFIG+=debug" -o Makefile.dbg

        doku for tmake: file:/usr/share/doc/packages/qt-freebies/tmake/doc/tmake.html

    Create an other Language (creating *.po and *.qm files):

		findtr *.h *.cpp >mindia.po
		copy mindia.po mindia_de.po
		msg2qm mindia_de.po mindia_de.qm

		// ** to update an existing language-file (mindia_de.po) after code changes
		findtr *.h *.cpp >mindia.po
		mergetr mindia_de.po mindia.po

    Create autoconf support:

        change configure.in and lmindia.mak.in, aclocal.m4, acinclude.m4
		run autoconf
		execute ./configure

    Create documentation with doxygen:

		doxygen mindoc.ini

    ----------------------------------------------------------------------

    Some comments about the implementation:

    There are several threads:
	  * Command Processor Thread (to send commands to the projector via RS232)
	  * Sound Thread (to control the playing of the sound (wav- or mp3-files))
	  * Linux only: mp3 Player Thread
	  * Linux only: wav play Thread
	  * Linux only: wav-file buffer reader Thread

    The time line behaviour is realized with Timer-Events (QTimer).

*/
/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/mindiawindow.h,v $
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

#ifndef _MINDIAWINDOW_h
#define _MINDIAWINDOW_h

#include <qmainwindow.h>
#include <qhbox.h> 
#include <qvbox.h>
#include <qcanvas.h> 
#include <qmultilineedit.h>

#include "iscript.h"
#include "doccontroler.h"
#include "hitemview.h"
#include "timelineview.h"


class QAction;
class QTimer;
class QLabel;
class QToolButton;

class ConfigurationDlgImpl;
class ConfigPlayerDlgImpl;
class ProjectorControlDlgImpl;
class ComLoggingDialogImpl;
class PlayInfoDlgImpl;
class PresentationDataDlgImpl;
class EventMapDlgImpl;
class DiaInfoDlgImpl;
class SoundInfoDlgImpl;
class CommentDlgImpl;
class HelpDialog;

// min todo --> richtige Implementation angeben
typedef HItemView	IconItemView;
typedef HItemView	DiaStateView;


// *******************************************************************
/** The main window for the slide show program.
  */
class MinDiaWindow : public QMainWindow, public IDiaOutputWindowInternal
{
	Q_OBJECT

public:
	MinDiaWindow( const QString & sLanguage, bool bIgnoreComSettings, bool bSimulation, int iProjectorType, QWidget* parent = 0, const char* name = 0, WFlags f = WType_TopLevel );
	virtual ~MinDiaWindow();

	// ** access for script engine
	DocumentAndControler * 	GetDocument();

	QPopupMenu *			GetPluginsMenuPtr();

	// ** implements the IDiaOutputWindow-(Script)Interface **
	// ** use this object as proxy for the output window, because the
	// ** output window may not exist all the time !
	virtual bool			IsValid() const;
	virtual bool			Show();
	virtual bool			Hide();
	virtual bool			Maximize();
	virtual bool			Restore();
	virtual int				GetWidth() const;
	virtual int				GetHeight() const;
	virtual bool			SetSize( int iWidth, int iHeight );
	virtual bool			SetPos( int iXPos, int iYPos );
	virtual int				GetDrawWidth() const;
	virtual int				GetDrawHeight() const;
	virtual bool			Clear();
	virtual bool			SetColor( int iRed, int iGreen, int iBlue );
	virtual bool			SetFont( const char * sFontName );
	virtual bool			SetFontSize( int iSizeInPixel, bool bBold, bool bItalic );
	virtual int 			SetTextXY( int x, int y, const char * sText );
	virtual int				GetTextCount() const;
	virtual bool			MoveText( int iTextID, int x, int y );
	virtual int				GetTextX( int iTextID ) const;
	virtual int				GetTextY( int iTextID ) const;
	virtual int				GetTextWidth( int iTextID ) const;
	virtual int				GetTextHeight( int iTextID ) const;
	virtual bool			SetTextColor( int iTextID, int iRed, int iGreen, int iBlue );
	virtual IColor			GetTextColor( int iTextID ) const;
	virtual bool			DeleteText( int iTextID );

	virtual QCanvas *		GetCanvas();

public slots:
	void sltModifyItemDialogClosed();
	void sltPlayInfoDialogClosed();
	void sltProjectorControlDialogClosed();
	void sltLoggingDialogClosed();
	void sltSoundDataDialogClosed();
	void sltSoundCommentDialogClosed();
	void sltPlotCommentDialogClosed();
	void sltPresentationDataDialogClosed();
	void sltPresentationEventsDialogClosed();
	void sltConfigurationDialogClosed();
	void sltDoModifyItem();
	void sltShowModifyItem();
	void sltDoConfiguration();
	void sltDoPlayerConfiguration();
	void sltDoPlayInfos();
	void sltShowPlayStatus();
	void sltDoSoundData();
	void sltShowSoundData();
	void sltDoSoundComment();
	void sltShowSoundComment();
	void sltDoPlotComment();
	void sltDoDynGraphicOp();
	void sltShowPlotComment();
	void sltDoPresentationData();
	void sltDoPresentationEvents();
	void sltDoControlProjector();
	void sltDoLogging();
	void sltShowAbout();
	void sltShowLicense();

	// handle gui request in this class and pass file-name to document
	void sltLoadDoc( const QString & sFileName, bool bExecuteEvent );
	void sltAskNewDoc();
	void sltAskLoadDoc();
	void sltAskLoadForEditDoc();
	void sltAskSaveAsDoc();
	void sltPrintDoc();
	void sltExportDoc();
	void sltExportDynGraphData();
	void sltImportDynGraphData();
	void sltExportXMLDoc();
	void sltImportXMLDoc();
	void sltCheckCloseApplication();

	// ** synchronize menu state with application state
	void sltUpdateEditMenu();
	void sltUpdateFileMenu();
	void sltUpdatePlayMenu();
	void sltUpdateExtrasMenu();
	void sltUpdateLastFilesMenu();
	//void sltUpdateScriptsMenu();

	void sltLastFilesMenuActivated( int iIndex );

	// *** other helper slots ***
	void sltShowErrorMessage( const QString & sMessage );
	void sltShowWarningMessage( const QString & sMessage );
	void sltShowStatusBarMessage( const QString & sMsg );
	void sltShowHelp( const QString & sHelpTag );
	void sltShowModalHelp( QWidget * pParent, const QString & sHelpTag );
	void sltShowHelpForMe();

	void sltShowImageFile( const QString & sFileName );

	void sltDoDocumentStateUpdate();
	void sltDoDataChanged();
	void sltDoUpdateAllViews();
	void sltDoSyncAllViews();

	void sltStartAutoStartTimer( bool bAutoRun, bool bShowScreen, bool bExitOnFinished, int iScreenX, int iScreenY, int iPosX, int iPosY );
	void sltAutoStartTimerEvent();
	void sltStatusUpdateTimerEvent();

	void sltPlayFromSelected();
	void sltPlayAtSelected();

	void sltItemSelected( int iCount, HItem * pFirstSelectedItem, int iFirstSelectedItemNo, int iDissolveTimeInMS );
	void sltSelectItem( int iIndex, int iDissolveTimeInMS );

	void sltNewItem();
	void sltAddItem();

	void sltFindItem();
	void sltFindNextItem();
	
	void sltEditFadeInTime();
	void sltFadeInTest();
	void sltFadeOutTest();

	void sltModusIsSwitched();
	void sltPlayFinished();

	void sltCopyClipboard();
	void sltCutClipboard();
	void sltPasteClipboard();
	void sltSelectAllClipboard();
	void sltDeleteSelectedItems();

signals:
	void sigLoadDocFromFile( const QString & sFileName, bool bExecuteEvent );
	void sigSaveDocToFile( const QString & sFileName );
	void sigSaveDocAsXML( const QString & sFileName );
	void sigQuit();
	void sigDialogHelp( const QString & sHelpTag);
	void sigStartPresentation();
	void sigShowPlayInfos();

protected:
	// ** if document has changed, ask the user for saving the data
	virtual void closeEvent( QCloseEvent * pCloseEvent );
	virtual void keyPressEvent( QKeyEvent * pEvent );

private:
	// *** some helper methods ***
	void CreateMenus();
	void CreateChildWidgets();
	bool IsValidClipboardData();
	void SetHelpFile( HelpDialog * pHelpDialog, const QString & sHelpTag ) const;

	// *** private data ***

	DocumentAndControler *		m_pControler;

	// ** gui elements **
	QHBox *						m_pHBox;
	QVBox *						m_pTargetBox;
	QVBox *						m_pSourceBox;

	HItemView *					m_pSlideView;		// dia pult view
	TimeLineView *				m_pTimeLineView;	// time line view

	IconItemView *				m_pDiaPultView;
	IconItemView *				m_pAudioPultView;
	DiaStateView *				m_pDiaStateView;

	QLabel *					m_pStatusBarTime;
	QLabel *					m_pStatusBarModus;
	QLabel *					m_pStatusBarMsg;

	ConfigurationDlgImpl *		m_pConfigurationDialog;		// modal
	ConfigPlayerDlgImpl *		m_pConfigPlayerDialog;		// modal
	ComLoggingDialogImpl *		m_pLoggingDialog;
	ProjectorControlDlgImpl *	m_pProjectorControlDialog;
	PresentationDataDlgImpl *	m_pPresentationDataDialog;	// modal
	EventMapDlgImpl *			m_pPresentationEventsDialog;// modal
	DiaInfoDlgImpl *			m_pDiaInfoDialog;
	PlayInfoDlgImpl *			m_pPlayInfoDialog;
	SoundInfoDlgImpl *			m_pSoundInfoDialog;			// modal
	CommentDlgImpl *			m_pSoundCommentDialog;		// modal
	CommentDlgImpl *			m_pPlotCommentDialog;		// modal
	CommentDlgImpl *			m_pDynGraphicOpDialog;		// modal
	HelpDialog *				m_pHelpDialog;

	// ****************************************

	QTimer *		m_pStatusUpdateTimer;
	QTimer *		m_pAutoStartTimer;

    QPopupMenu *	m_pFile;
    QPopupMenu *	m_pEdit;
    QPopupMenu *	m_pPlay;
    QPopupMenu *	m_pExtras;
    QPopupMenu *	m_pPlugins;
    QPopupMenu *	m_pHelp;
	QPopupMenu *	m_pLastFilesSubMenu;
	QPopupMenu *	m_pImportExportFilesSubMenu;

	QAction *		m_pPlayStartAction;
	QAction *		m_pPlayPauseAction;
	QAction *		m_pPlayStopAction;
	QAction *		m_pPlayStartFromAction;
	QAction *		m_pPlayStartAtAction;
	QAction *		m_pPlayAddSoundCommentAction;
	QAction *		m_pPlayAddGraphicOpAction;
	QAction *		m_pPlayEditFadeTimeAction;
	QAction *		m_pPlayFadeInAction;
	QAction *		m_pPlayFadeOutAction;

	QAction *		m_pExtrasConfigAction;
	QAction *		m_pExtrasConfigPlayerAction;
	QAction *		m_pExtrasLoggingAction;
	QAction *		m_pExtrasControlProjectorAction;
	QAction *		m_pExtrasPresentationDataAction;
	QAction *		m_pExtrasPresentationEventsAction;
	QAction *		m_pExtrasSoundDataAction;
	QAction *		m_pExtrasSoundCommentAction;
	QAction *		m_pExtrasPlotCommentAction;
	QAction *		m_pExtrasDynGraphOpAction;
	QAction *		m_pExtrasPlayStatusAction;
	QAction *		m_pExtrasModifyItemAction;

	QAction *		m_pEditUndoAction;
	QAction *		m_pEditRedoAction;
	QAction *		m_pEditCutAction;
	QAction *		m_pEditCopyAction;
	QAction *		m_pEditPasteAction;
	QAction *		m_pEditDeleteAction;
	QAction *		m_pEditSelectAllAction;
	QAction *		m_pEditNewDiaAction;
	QAction *		m_pEditAddDiaAction;
	QAction *		m_pEditFindAction;
	QAction *		m_pEditFindNextAction;

	QAction *		m_pFileNewAction;
	QAction *		m_pFileLoadAction;
	QAction *		m_pFileLoadForEditAction;
	QAction *		m_pFileSaveAction;
	QAction *		m_pFileSaveAsAction;
	QAction *		m_pFileExportXMLAction;
	QAction *		m_pFileImportXMLAction;
	QAction *		m_pFileExportAction;
	QAction *		m_pFileExportDynGraphAction;
	QAction *		m_pFileImportDynGraphAction;
	QAction *		m_pFilePrintAction;
	QAction *		m_pFileExitAction;

	QToolButton *	m_pFileOpen;
	QToolButton *	m_pFileSave;
	QToolButton *	m_pPlayRun;
	QToolButton *	m_pPlayPause;
	QToolButton *	m_pPlayStop;

	// ** temp data **
	int				m_iCount;
	HItem *			m_pFirstSelectedItem;
	int				m_iFirstSelectedItemNo;
	QString			m_sFindText;
	QString			m_sLanguage;
	double			m_dDissolveTime;			// in seconds
	bool			m_bAutoRun;
	bool			m_bShowScreen;
	bool			m_bExitOnFinished;
	int				m_iScreenX;
	int				m_iScreenY;
	int				m_iPosX;
	int				m_iPosY;
};

#endif

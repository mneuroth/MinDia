/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/doccontroler.h,v $
 *
 *  $Revision: 1.2 $
 *
 *	$Log: not supported by cvs2svn $
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

#ifndef _DOCCONTROLER_H
#define _DOCCONTROLER_H

#include "iscript.h"
#include "diacallback.h"

#include "diapresentation.h"
#include "rolleicom.h"
#include "minisound.h"
#include "miniini.h"
#include "minlog.h"

#include <qobject.h>
#include <qwidget.h>

// *******************************************************************

extern const string g_sLastFilesKey;

// *******************************************************************
/** Controls User Input and holds the document-data.
  * user input is: selection, copy, delete, move, etc.
  */
class DocumentAndControler : public QObject,
							 public DiaCallback,
							 public IDiaPresentation
{
	Q_OBJECT

public:
	DocumentAndControler( bool bIgnoreComSettings,
						  bool bSimulation,
						  int iProjectorType,
						  QWidget * pMainWindow,
						  IDiaOutputWindowInternal * pOutputWindowProxy,
						  minLoggingInterface * pLoggingChannel
						  );
	virtual ~DocumentAndControler();

	// ** implement part of the IDiaPresentation-Interface **
	virtual bool			IsChanged() const;

	// ** there are only three modi: "edit", "play" and "pause"
	virtual bool			IsPlayModus() const;
	virtual bool			IsPauseModus() const;
	virtual bool			IsEditModus() const;
	virtual const char *	GetPlayModusStrg() const;

	virtual bool			IsSimulation() const;
	virtual void			SetSimulation( bool bSimulation );
	// ** implementation done. **

	string					GetName() const;

	//int						GetProjectorCount() const;
	//minHandle<DiaProjector>	GetProjectorPtr( int iNo ) const;

	MiniIniDB &			GetIniDB();

	RolleiCom &			GetDiaCom();

	DiaPresentation &	GetPresentation();

	// ** implements the DiaCallback-Interface **
	virtual void TriggerDissolveActDiaNo( int iNo, const char * sScript, const char * sFileName, double dDissolveTime );
	virtual void TriggerShowActDiaNo( int iNo, const char * sScript, const char * sFileName, double dShowTime );
	virtual void TriggerSetNextDiaNo( int iNextNo, const char * sNextFileName );
	virtual void PresentationModusChanged();
    virtual void ShowError( const char * sMsg );

	// ** implements the IDiaPresentation-(Script)Interface **
	virtual const char *	GetDocName() const;
	virtual const char *	GetDescription() const;
	virtual int  			GetDiaCount() const;
	virtual IDia * 			GetDia( int iIndex );
	virtual IDia *			AddNewDia();
	virtual int				GetSoundFileCount() const;
	virtual const char *	GetSoundFileNameAt( int iIndex ) const;
	virtual bool			SetSoundFileNameAt( int iIndex, const char * sFileName );
	virtual IDiaOutputWindow * GetOutputWindow();
	virtual bool			LoadPresentation( const char * sFileName, bool bExecuteScript );
	virtual bool			SavePresentation();
	virtual bool			SavePresentationAs( const char * sFileName );
	virtual bool			GotoPosition( int iPosition );
	virtual bool			PlayFrom( int iFromDiaNo );
	virtual bool			Play();
	virtual bool			Stop();
	virtual bool			Pause();
	virtual bool			Continue();

public slots:

	// *** menu operations ***
	void sltEmptyEvent();
    void sltNewDoc();
	void sltLoadDoc( const QString & sFileName, bool bExecuteEvent );
	void sltSaveDoc();
	void sltSaveAsDoc( const QString & sFileName );
	void sltLoadDoc( const QString & sFileName, bool & bOk, bool bExecuteScript );
	void sltSaveDoc( bool & bOk );
	void sltSaveAsDoc( const QString & sFileName, bool & bOk );
	void sltSaveDocAsXML( const QString & sFileName );
	void sltSaveDocAsXML( const QString & sFileName, bool & bOk );
	void sltExportDynGraphicData( const QString & sFileName );
	void sltImportDynGraphicData( const QString & sFileName );

	void sltUndoOperation();
	void sltRedoOperation();

	void sltCopyClipboard();
	void sltCutClipboard();
	void sltPasteClipboard();
	void sltSelectAllClipboard();
	void sltDeleteSelectedItems();

	void sltSelectItemWithText( const QString & sText );
	void sltSelectNextItemWithText( const QString & sText );

// folgende slots koennen von der View verwendet werden um Aenderungen zu signalisieren...
	// *** other user imput operations (via gui) ***
	//void sltAddItemAt( int iIndex, ... );
	void sltSelectItem( int iNo, int iDissolveTimeInMS );
	//void sltDeleteItem( int iNo );

	void sltPlayPresentation();
	void sltStopPresentation();
	void sltPausePresentation();
	void sltPlayPresentationAt( int iSelectedIndex );
	void sltGotoPosition( int iPosition );
	void sltAddSoundComment();
	void sltAddGraphicOperation();
	void sltTimerEvent();

	// *** other slots ***
	void sltDataChanged();

	void sltUpdateStatusBar();

signals:
	void sigNewItem();
	void sigShowErrorMessage( const QString & sMessage );
	void sigShowWarningMessage( const QString & sMessage );
	void sigShowStatusMessage( const QString & sMessage );
	void sigDocumentNameChanged();
	void sigDocumentChanged();
	void sigSyncAllViews();
	void sigSwitchToEditModus();
	void sigModusIsSwitched();
	void sigPlayFinished();
	void sigSelectItem( int iIndex, int iDissolveTimeInMS );
	void sigCopyClipboard();
	void sigCutClipboard();
	void sigPasteClipboard();
	void sigSelectAllClipboard();
	void sigDeleteSelectedItems();
	void sigShowActImage( const QString & sFileName );

private:
	// ** help methods **
	void ExecuteScript( bool bDissolve, const char * sScript, int iNo );
	void ReadIniValues();
	void WriteIniValues();

	// *** data ***
	MiniIniDB			m_aIniDB;				// a database for configuration data
	RolleiCom			m_aCom;					// the projector control
	miniSound			m_aSoundPlayer;			// to play the sound for the presentation
	DiaPresentation		m_aPresentation;		// this is the document !
	minLoggingInterface * m_pLoggingChannel;	// No Owner !

	QTimer *			m_pTimer;

	// *** temp data ***
	int					m_iLastFoundPos;

	IDiaOutputWindowInternal *	m_pOutputWindowProxy;
};

#endif

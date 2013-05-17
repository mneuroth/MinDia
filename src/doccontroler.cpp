/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/doccontroler.cpp,v $
 *
 *  $Revision: 1.9 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.8  2004/04/09 15:19:02  min
 *	Added image cache for movie support.
 *	
 *	Revision 1.7  2004/03/19 13:32:46  min
 *	Bugfix for the script command GetDocName(), saving presentation name in a local member variable
 *	
 *	Revision 1.6  2004/02/26 22:19:07  min
 *	Fixes to compile MinDia for the Zaurus.
 *	
 *	Revision 1.5  2004/01/18 23:50:57  min
 *	Windows only: get path .mindia.ini from registry.
 *	
 *	Revision 1.4  2003/10/26 22:38:48  min
 *	calculate home directory for linux
 *	
 *	Revision 1.3  2003/10/26 17:35:01  min
 *	Saved more values in the ini-file.
 *	
 *	Revision 1.2  2003/10/03 23:07:16  min
 *	saving ini-file in unix-home directory
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

#include "doccontroler.h"
#include "minexception.h"
#include "writexml.h"
#include "qtmtlock.h"
#include "misctools.h"

#include <qpainter.h>
#include <qimage.h>
#include <qapplication.h>
#include <QPixmap>
#include <QFile>
#include <QSettings>
#include <QProgressDialog>
#include <QTimer>

#include <fstream>

#include <stdio.h>

// *******************************************************************
// *******************************************************************
// *******************************************************************

DocumentAndControler::DocumentAndControler( bool bEnableScript,
                                            bool bIgnoreComSettings,
											bool bSimulation,
											int iProjectorType,
											QWidget * pMainWindow,
											IDiaOutputWindowInternal * pOutputWindowProxy,
											minLoggingInterface * pLoggingChannel
											)
: 
  m_aCom( bIgnoreComSettings, bSimulation, iProjectorType, pLoggingChannel ),
  m_aPresentation( bEnableScript, this, "newpresentation.dia", pLoggingChannel, pOutputWindowProxy ),
  m_pLoggingChannel( pLoggingChannel ),
  m_pTimer( 0 ),
  m_iLastFoundPos( -1 ),
  m_pOutputWindowProxy( pOutputWindowProxy )
{
	m_aPresentation.SetProjectorCom( &m_aCom );
	m_aPresentation.SetSoundPlayer( &m_aSoundPlayer );

	// *** connect signals to slots ***
    connect( this, SIGNAL( sigShowStatusMessage(const QString &) ), pMainWindow, SLOT( sltShowStatusBarMessage(const QString &) ) );
    connect( this, SIGNAL( sigShowErrorMessage(const QString &) ), pMainWindow, SLOT( sltShowErrorMessage(const QString &) ) );
    connect( this, SIGNAL( sigShowWarningMessage(const QString &) ), pMainWindow, SLOT( sltShowWarningMessage(const QString &) ) );
    connect( this, SIGNAL( sigNewItem() ), pMainWindow, SLOT( sltNewItem() ) );
    connect( this, SIGNAL( sigDocumentNameChanged() ), pMainWindow, SLOT( sltDoDocumentStateUpdate() ) );
    connect( this, SIGNAL( sigDocumentChanged() ), pMainWindow, SLOT( sltDoDataChanged() ) );		// sltDoDocumentStateUpdate
    connect( this, SIGNAL( sigSyncAllViews() ), pMainWindow, SLOT( sltDoSyncAllViews() ) );

    connect( this, SIGNAL( sigCopyClipboard() ), pMainWindow, SLOT( sltCopyClipboard() ) );
    connect( this, SIGNAL( sigCutClipboard() ), pMainWindow, SLOT( sltCutClipboard() ) );
    connect( this, SIGNAL( sigPasteClipboard() ), pMainWindow, SLOT( sltPasteClipboard() ) );
    connect( this, SIGNAL( sigSelectAllClipboard() ), pMainWindow, SLOT( sltSelectAllClipboard() ) );
    connect( this, SIGNAL( sigDeleteSelectedItems() ), pMainWindow, SLOT( sltDeleteSelectedItems() ) );

    connect( this, SIGNAL( sigModusIsSwitched() ), pMainWindow, SLOT( sltModusIsSwitched() ) );
    connect( this, SIGNAL( sigPlayFinished() ), pMainWindow, SLOT( sltPlayFinished() ) );
    connect( this, SIGNAL( sigSelectItem(int,int) ), pMainWindow, SLOT( sltSelectItem(int,int) ) );

	ReadIniValues();
}

DocumentAndControler::~DocumentAndControler()
{
	if( m_pTimer )
	{
		delete m_pTimer;
	}

	WriteIniValues();
}

void DocumentAndControler::ReadIniValues()
{
    QSettings aSettings;
   
    SetSimulation( aSettings.value("App/IsSimulation",true).toBool() );
    GetDiaCom().SetLogging( aSettings.value("App/IsLogging",true).toBool() );
    GetPresentation().GetScriptEnvironment().SetEnabled( aSettings.value("App/EnableScript",true).toBool() );
    GetDiaCom().SetProjectorType( aSettings.value("App/ProjectorType",RolleiCom::TWIN_DIGITAL_P).toInt() );
}

void DocumentAndControler::WriteIniValues()
{
    QSettings aSettings;

    aSettings.setValue("App/IsSimulation",IsSimulation());
    aSettings.setValue("App/IsLogging",GetDiaCom().IsLogging());
    aSettings.setValue("App/EnableScript",GetPresentation().GetScriptEnvironment().IsEnabled());
    aSettings.setValue("App/ProjectorType",GetDiaCom().GetProjectorType());
}

bool DocumentAndControler::IsChanged() const
{
	return m_aPresentation.IsChanged();
}

bool DocumentAndControler::IsPlayModus() const
{
	return m_aPresentation.IsPlaying();
}

bool DocumentAndControler::IsPauseModus() const
{
	return m_aPresentation.IsPause();
}

bool DocumentAndControler::IsEditModus() const
{
	return m_aPresentation.IsEdit();
}

string DocumentAndControler::GetPlayModusStrg() const
{
	if( IsPlayModus() )
	{
        return "PLAY";
	}
	else if( IsPauseModus() )
	{
        return "PAUSE";
	}
	else
	{
        return "EDIT";
	}
    return "UNKNOWN";
}

string DocumentAndControler::GetName() const
{
    return m_aPresentation.GetFullName();
}

bool DocumentAndControler::IsSimulation() const
{
	return m_aCom.IsSimulation();
}

void DocumentAndControler::SetSimulation( bool bSimulation )
{
	m_aCom.SetSimulation( bSimulation );
}

/*
int	DocumentAndControler::GetProjectorCount() const
{
	return m_aPresentation.GetProjectorCount();
}

minHandle<DiaProjector> DocumentAndControler::GetProjectorPtr( int iNo ) const
{
	if( iNo>0 && iNo<GetProjectorCount() )
	{
		return m_aPresentation.GetProjectorPtr( iNo );
	}
	return 0;
}
*/

DiaPresentation & DocumentAndControler::GetPresentation()
{
	return m_aPresentation;
}

miniSound & DocumentAndControler::GetSoundInfo()
{
    return m_aSoundPlayer;
}

RolleiCom & DocumentAndControler::GetDiaCom()
{
	return m_aCom;
}

void DocumentAndControler::sltNewDoc()
{
	m_aPresentation.Clear();

	emit sigDocumentNameChanged();
	emit sigSyncAllViews();
}

void DocumentAndControler::sltLoadDoc( const QString & sFileName, bool bExecuteEvent )
{
	bool bOk;

	sltLoadDoc( sFileName, bOk, bExecuteEvent );

	if( !bOk )
	{
		QString sMsg( tr( "Error reading file \"" ) );
		sMsg += sFileName;
		sMsg += "\" !";
		emit sigShowErrorMessage( sMsg );
	}
	else
	{
		//emit sigShowErrorMessage( tr( "reading was ok !" ) );
	}
}

void DocumentAndControler::sltSaveDoc()
{
	bool bOk;

	sltSaveDoc( bOk );

	if( !bOk )
	{
		QString sMsg( tr( "Error saving file \"" ) );
        sMsg += ToQString( m_aPresentation.GetName() );
		sMsg += "\" !";
		emit sigShowErrorMessage( sMsg );
	}
	else
	{
		//emit sigShowErrorMessage( tr( "saving was ok !" ) );
	}
}

void DocumentAndControler::sltSaveAsDoc( const QString & sFileName )
{
	bool bOk;

	sltSaveAsDoc( sFileName, bOk );
}

void DocumentAndControler::sltLoadDoc( const QString & sFileName, bool & bOk, bool bExecuteScript )
{
    fstream aFile( sFileName.toLocal8Bit(), ios::in );

	bOk = aFile.good();
	if( bOk )
	{
 		try {
			bOk = m_aPresentation.Read( aFile, bExecuteScript );

			// ** if the original file (with stored a file path) does not exists any more,
			// ** change it to the filename which was used to load this file !
            if( bOk && !QFile::exists( ToQString( m_aPresentation.GetName() ) ) )
			{
                m_aPresentation.SetName( ToStdString(sFileName) );

				//emit sigShowWarningMessage( tr( "Path of file has changed !" ) );
			}

            AddToFileHistory(sFileName);
            
		}
		catch( MinException & aException )
		{
            emit sigShowWarningMessage( ToQString( aException.GetMsg() ) );
			bOk = false;
		}
		if( !bOk )
		{
			emit sigShowWarningMessage( tr( "Error reading file !" ) );
		}
	}

	emit sigDocumentNameChanged();
	emit sigSyncAllViews();
}

void DocumentAndControler::sltSaveDoc( bool & bOk )
{
    fstream aFile( m_aPresentation.GetFullName().c_str(), ios::out );

	bOk = aFile.good();
	if( bOk )
	{
		bOk = m_aPresentation.Write( aFile );
	}

	emit sigDocumentNameChanged();	// update changed mark in window-title
	//emit sigSyncAllViews();
}

void DocumentAndControler::sltSaveAsDoc( const QString & sFileName, bool & bOk )
{
    m_aPresentation.SetName( ToStdString(sFileName) );
	// ** if a file is saved with a new name, update the last-file history
    AddToFileHistory(sFileName);

	sltSaveDoc( bOk );

	emit sigDocumentNameChanged();
	//emit sigSyncAllViews();
}

void DocumentAndControler::sltSaveDocAsXML( const QString & sFileName )
{
	bool bOk = false;

	sltSaveDocAsXML( sFileName, bOk );
}

void DocumentAndControler::sltSaveDocAsXML( const QString & sFileName, bool & bOk )
{
	XmlTree aTree = m_aPresentation.GetXMLTree();

    bOk = aTree.WriteXML( ToStdString(sFileName) );
}

void DocumentAndControler::sltExportDynGraphicData( const QString & sFileName )
{
    fstream aFile( sFileName.toLocal8Bit(), ios::out );

	if( aFile.good() )
	{
		m_aPresentation.GetDynGraphicData().Write( aFile );
	}
}

void DocumentAndControler::sltImportDynGraphicData( const QString & sFileName )
{
    fstream aFile( sFileName.toLocal8Bit(), ios::in );

	if( aFile.good() )
	{
		m_aPresentation.GetDynGraphicData().Read( aFile );
	
		emit sigSyncAllViews();
	}
}

void DocumentAndControler::sltDataChanged()
{
    m_aPresentation.SyncDataContainers();

	// ** pass signal to observer
	emit sigDocumentChanged();
}

void DocumentAndControler::sltSelectItem( int iNo, int iDissolveTimeInMS )
{
	emit sigSelectItem( iNo, iDissolveTimeInMS );
}

/*
void DocumentAndControler::sltDeleteItem( int iNo )
{
	sigShowErrorMessage( "not implemented yet (a)" );
}
*/

void DocumentAndControler::sltUndoOperation()
{
	sigShowErrorMessage( "not implemented yet (0)" );
}

void DocumentAndControler::sltRedoOperation()
{
	sigShowErrorMessage( "not implemented yet (1)" );
}

void DocumentAndControler::sltCopyClipboard()
{
	emit sigCopyClipboard();
}

void DocumentAndControler::sltCutClipboard()
{
	emit sigCutClipboard();
}

void DocumentAndControler::sltPasteClipboard()
{
	emit sigPasteClipboard();
}

void DocumentAndControler::sltSelectAllClipboard()
{
	emit sigSelectAllClipboard();
}

void DocumentAndControler::sltDeleteSelectedItems()
{
	emit sigDeleteSelectedItems();
}

void DocumentAndControler::sltEmptyEvent()
{
	sigShowErrorMessage( "not implemented yet (EmptyEvent)" );
}

void DocumentAndControler::sltPlayPresentation()
{
	sltPlayPresentationAt( -1 );
}

void DocumentAndControler::sltStopPresentation()
{
	if( m_pTimer )
	{
		m_pTimer->stop();
	}

	// ** stop the presentation
	GetPresentation().StopPlay();

	delete m_pTimer;
	m_pTimer = 0;

	sltUpdateStatusBar();

	emit sigModusIsSwitched();

	emit sigPlayFinished();			// new since 26.3.2003
}

void DocumentAndControler::sltPausePresentation()
{
	if( m_pTimer )
	{
		m_pTimer->stop();
	}

	// ** suspend the presentation
	GetPresentation().PausePlay();

	sltUpdateStatusBar();

	emit sigModusIsSwitched();
}

void DocumentAndControler::sltPlayPresentationAt( int iSelectedIndex )
{
	if( !m_pTimer )
	{
		m_pTimer = new QTimer( this );
		connect( m_pTimer, SIGNAL( timeout() ), this, SLOT( sltTimerEvent() ) );
	}

	// ** start/continue the presentation --> process next slide
	bool bOk = GetPresentation().StartPlay( iSelectedIndex );

	// bugfix 26.1.2003, check for successfull start !!!
	if( bOk )
	{
		// ** and start timer imediately, to process the next/first slide
        m_pTimer->setSingleShot(TRUE);
        m_pTimer->start( 1 );
	}

	sltUpdateStatusBar();

	emit sigModusIsSwitched();
}

void DocumentAndControler::sltGotoPosition( int iPosition )
{
	m_aCom.GotoPosition( iPosition );
}

void DocumentAndControler::sltSelectItemWithText( const QString & sText )
{
    int iFoundPos = m_aPresentation.FindItemWithText( ToStdString(sText), 0 );

	if( iFoundPos>=0 )
	{
		m_iLastFoundPos = iFoundPos;

		emit sigSelectItem( iFoundPos, 0 );
	}
	else
	{
		m_iLastFoundPos = -1;

		emit sigShowErrorMessage( tr( "Text not found !" ) );
	}
}

void DocumentAndControler::sltSelectNextItemWithText( const QString & sText )
{
    int iFoundPos = m_aPresentation.FindItemWithText( ToStdString(sText), m_iLastFoundPos+1 );

	if( iFoundPos>=0 )
	{
		m_iLastFoundPos = iFoundPos;

		emit sigSelectItem( iFoundPos, 0 );
	}
	else
	{
		m_iLastFoundPos = -1;

		emit sigShowErrorMessage( tr( "Text not found !" ) );
	}
}

void DocumentAndControler::sltAddSoundComment()
{
	double dActTime = GetPresentation().GetActSoundPlayTime() * 1000.0;

	GetPresentation().GetSoundCommentData().AddComment( dActTime, "?" );

	emit sigDocumentChanged();
}

void DocumentAndControler::sltAddGraphicOperation()
{
	double dActTime = GetPresentation().GetActSoundPlayTime() * 1000.0;

	GetPresentation().GetDynGraphicData().AddDefaultDynText( "?", dActTime );

	emit sigDocumentChanged();
}

void DocumentAndControler::sltTimerEvent()
{
	if( m_pTimer )
	{
		m_pTimer->stop();
	}

	// process next slide
	double dTimerInSeconds = 0;
	
	bool bContinue = GetPresentation().NextStep( dTimerInSeconds );

	// ** is the presentation finished ?
	if( /*dTimerInSeconds < 0*/ !bContinue )	// Bugfix for spontanious stop in play-modus (21.1.2003)
	{
		// ** yes --> stop the presentation
		sltStopPresentation();

		{
			QtMTLock aMTLock;

			sltUpdateStatusBar();
		}
	}
	else
	{
		// start timer for the next step
		int iTimer = (int)(dTimerInSeconds*1000);	// s --> ms

		{
			QtMTLock aMTLock;

			sltUpdateStatusBar();
		}

		if( m_pTimer )
		{
            m_pTimer->setSingleShot(TRUE);
            m_pTimer->start( iTimer );
		}
	}

	//sltUpdateStatusBar();
}

void DocumentAndControler::sltUpdateStatusBar()
{
    QString s = ToQString( GetPresentation().GetStepInfo() );
	emit sigShowStatusMessage( s );
}

void DocumentAndControler::TriggerDissolveActDiaNo( int iNo, const string & sScript, const string & /*sFileNameOrScript*/, double dDissolveTime )
{
	int iDissolveTimeInMS = (int)(dDissolveTime*1000.0);
	sltSelectItem( iNo, iDissolveTimeInMS );

    // ** no signal to update actual image here, because image will be updated in method sltSelectItem() !!!

    if( sScript.size() > 0 )
	{
        ExecuteScript( /*bDissolve*/true, sScript, iNo );
	}
}

void DocumentAndControler::TriggerShowActDiaNo( int iNo, const string & sScript, const string & /*sFileNameOrScript*/, double /*dShowTime*/ )
{
	// cout << "*** nr=" << iNo << endl;
    if( sScript.length() > 0 )
	{
        ExecuteScript( /*bDissolve*/false, sScript, iNo );
	}
}

void DocumentAndControler::TriggerSetNextDiaNo( int /*iNextNo*/, const string & /*sNextFileName*/ )
{
	// optional, ist anscheinend momentan nicht notwendig !
	// lade naechtes Image im voraus, damit der Dia-Wechsel ohne zeitliche
	// Verzoegerung statt finden kann !
}

void DocumentAndControler::PresentationModusChanged()
{
	emit sigModusIsSwitched();
}

void DocumentAndControler::ShowError( const string & sMsg )
{
    QString s( ToQString( sMsg ) );
    emit sigShowErrorMessage( s );
}

string DocumentAndControler::GetDocName() const
{
    return GetName();
}

string DocumentAndControler::GetDescription() const
{
    return m_aPresentation.GetComment();
}

int DocumentAndControler::GetDiaCount() const
{
	return m_aPresentation.GetDiaCount();
}

IDia * DocumentAndControler::GetDia( int iIndex )
{
	if( (iIndex>=0) && (iIndex<m_aPresentation.GetDiaCount()) )
	{
		return m_aPresentation.GetDiaAt( iIndex ).GetPtr();
	}
	return 0;
}

IDia * DocumentAndControler::AddNewDia()
{
	// ** create a new dia at the end of the presentation
	emit sigNewItem();

	// ** and get the pointer for the new created dia
	return GetDia( GetDiaCount()-1 );
}

int DocumentAndControler::GetSoundFileCount() const
{
	return m_aPresentation.GetSoundInfoData().size();
}

string DocumentAndControler::GetSoundFileNameAt( int iIndex ) const
{
	if( (iIndex>=0) && (iIndex<GetSoundFileCount()) )
	{
        return m_aPresentation.GetSoundInfoData()[ iIndex ]->GetFileName();
	}
	return "";
}

bool DocumentAndControler::SetSoundFileNameAt( int iIndex, const string & sFileName )
{
	if( (iIndex>=0) && (iIndex<GetSoundFileCount()) )
	{
		m_aPresentation.GetSoundInfoData()[ iIndex ]->SetFileName( sFileName );
		return true;
	}
	return false;
}

IDiaOutputWindow * DocumentAndControler::GetOutputWindow()
{
	return m_pOutputWindowProxy;
}

bool DocumentAndControler::LoadPresentation( const string & sFileName, bool bExecuteScript )
{
	bool bOk;

    sltLoadDoc( ToQString( sFileName ), bOk, bExecuteScript );

	return bOk;
}

bool DocumentAndControler::SavePresentation()
{
	bool bOk;

	sltSaveDoc( bOk );

	return bOk;
}

bool DocumentAndControler::SavePresentationAs( const string & sFileName )
{
	bool bOk;

    sltSaveAsDoc( ToQString( sFileName ), bOk );

	return bOk;
}

bool DocumentAndControler::GotoPosition( int iPosition )
{
	sltGotoPosition( iPosition );
	return true;
}

bool DocumentAndControler::PlayFrom( int iFromDiaNo )
{
	sltPlayPresentationAt( iFromDiaNo );
	return true;
}

bool DocumentAndControler::Play()
{
	sltPlayPresentation();
	return true;
}

bool DocumentAndControler::Stop()
{
	sltStopPresentation();
	return true;
}

bool DocumentAndControler::Pause()
{
	sltPausePresentation();
	return true;
}

bool DocumentAndControler::Continue()
{
	sltPlayPresentation();
	return true;
}

void DocumentAndControler::ExecuteScript( bool bDissolve, const string & sScript, int iNo )
{
	// ** WARNING: execution of scripts in own thread not supported yet, because
	// **          there are some problems with a blocked GUI !
	minClientHandle<IGeneralScriptEnvironment>	hScriptEnv( g_IGeneralScriptEnvironmentID );

	if( hScriptEnv.IsValid() && m_aPresentation.IsScriptEnabled() )
	{
		string s;

		// ** set a global variable in the (python-)script
		// ** to signal the right process (dissolve or show)
		// ** every dia-script will be called twice:
		// ** first in the dissolve process and
		// ** second in the show process.
		// ** The script can distinguish the proces with the varialbe bDissolve
		if( bDissolve )
		{
			s = "bDissolve = 1\n\n";
		}
		else
		{
			s = "bDissolve = 0\n\n";
		}
		s += sScript;

		int iRet = hScriptEnv->ExecuteScript( s, IGeneralScriptEnvironment::PYTHON );

		if( m_pLoggingChannel )
		{
			char sBuffer[512];

			sprintf( sBuffer, "Running script, ret=%d, no=%d dissolve=%d", iRet, iNo, (int)bDissolve );
			m_pLoggingChannel->LogMsg( sBuffer );
		}
	}
}

QApplication * GetApplication();

int DocumentAndControler::CreateImagesForMovie( 
        QWidget * pOwner,
		const string & sOutputDirectory, 
		const string & sFileNameOffset, 
        const string & sDirSeparator,
        const string & sImageExtension,
		int iWidth, 
		int iHeight, 
		double dStartMS, 
		double dStopMS, 
		double dDeltaMS )
{
	int iCount = 0;

    QProgressDialog aProgress( QObject::tr("creating images for movie"), QObject::tr("Cancel"), 0, (int)dStopMS, pOwner/*, 0, "progress", TRUE*/ );
    aProgress.setValue( 0 );
    aProgress.show();

    string sLastFileName;
    double dTimeMS = dStartMS;
    while( dTimeMS < dStopMS )
    {
        QString sBuffer;
        QString sFileName;
        sFileName.sprintf( sFileNameOffset.c_str(), iCount );
        sBuffer = QString( "%1%2%3.%4" ).arg( ToQString( sOutputDirectory ) ).arg( ToQString( sDirSeparator ) ).arg( sFileName ).arg( ToQString(sImageExtension) );
#ifdef Q_WS_WIN
        QString sLinkExt = ".lnk";
#else
        QString sLinkExt;
#endif

        if( GetPresentation().IsNextSlideChanging( dTimeMS, dDeltaMS ) )
        {
            QImage aPixmap = GetPresentation().GetSlideForTime( dTimeMS, iWidth, iHeight );

            /*bool bOk =*/ aPixmap.save(sBuffer,/*"JPEG"*/sImageExtension.c_str(),100);

            sLastFileName = ToStdString( sBuffer );
        }
        else
        {
            // use last generated file for new movie image
#ifdef Q_WS_WIN
            // ffmpeg does not work with windows shortcuts *.lnk :-(
            /*bool ok =*/ QFile::copy( ToQString( sLastFileName ), sBuffer );
#else
            /*bool ok =*/ QFile::link( ToQString( sLastFileName ), sBuffer+sLinkExt );
#endif
        }

        dTimeMS += dDeltaMS;
        iCount++;

        aProgress.setValue( (int)dTimeMS );
        GetApplication()->processEvents();
        if( aProgress.wasCanceled() )
        {
            dTimeMS = dStopMS;
        }
    }

    aProgress.setValue( (int)dStopMS );

    return iCount;
}

QStringList DocumentAndControler::GetFileHistoryList() const
{
    return m_aFileHistoryList;
}

void DocumentAndControler::SetFileHistoryList(const QStringList & aLst)
{
    m_aFileHistoryList = aLst;
}

void DocumentAndControler::AddToFileHistory( const QString & sFileName )
{
    m_aFileHistoryList.push_back(sFileName);
    m_aFileHistoryList.removeDuplicates();
    while( m_aFileHistoryList.size()>9 )
    {
        m_aFileHistoryList.removeFirst();
    }
}

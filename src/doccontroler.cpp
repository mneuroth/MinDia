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
#ifndef ZAURUS
#include "qtmtlock.h"
#include <qpainter.h>
#include <qimage.h>
#include <qapplication.h>
#include <QPixmap>
#else
class QtMTLock {};
#endif
#include <QFile>
#include <QSettings>
#include <QProgressDialog>

#include <fstream>

#include <stdio.h>

#include <qtimer.h>

// *******************************************************************

#if defined(__linux__) || defined(__APPLE__)
#define _OFFSET_INI_FILE_PATH "~/"
#else
#include <windows.h>
#define _OFFSET_INI_FILE_PATH ""
#endif

string GetPathToIniFile()
{
	string sPath;
#if defined( __linux__ ) || defined(__APPLE__) || defined( ZAURUS )
	const char * s = getenv( "HOME" );
	if( s )
	{
		sPath += s;
	}
	return sPath+"/";
#else
	// HKEY_CURRENT_USER.Software.mindia.inifile
	HKEY hKey;
	if( RegOpenKeyEx( HKEY_CURRENT_USER, (WCHAR *)"Software", 0, KEY_ALL_ACCESS, &hKey ) == ERROR_SUCCESS )
	{
		HKEY hKeyMindia;
		if( RegOpenKeyEx( hKey, (WCHAR *)"mindia", 0, KEY_ALL_ACCESS, &hKeyMindia ) == ERROR_SUCCESS )
		{
			char sBuffer[512];
			DWORD dwLength = 512;
			DWORD dwType = REG_SZ;

			RegQueryValueEx( hKeyMindia, (WCHAR *)"inifile", NULL, &dwType, (LPBYTE)sBuffer, &dwLength );

			sPath = sBuffer;

			RegCloseKey( hKeyMindia );
		}

		RegCloseKey( hKey );

		return sPath+"\\";
	}
	return _OFFSET_INI_FILE_PATH;
#endif
}

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
#ifndef ZAURUS
    connect( this, SIGNAL( sigNewItem() ), pMainWindow, SLOT( sltNewItem() ) );
    connect( this, SIGNAL( sigDocumentNameChanged() ), pMainWindow, SLOT( sltDoDocumentStateUpdate() ) );
    connect( this, SIGNAL( sigDocumentChanged() ), pMainWindow, SLOT( sltDoDataChanged() ) );		// sltDoDocumentStateUpdate
    connect( this, SIGNAL( sigSyncAllViews() ), pMainWindow, SLOT( sltDoSyncAllViews() ) );

    connect( this, SIGNAL( sigCopyClipboard() ), pMainWindow, SLOT( sltCopyClipboard() ) );
    connect( this, SIGNAL( sigCutClipboard() ), pMainWindow, SLOT( sltCutClipboard() ) );
    connect( this, SIGNAL( sigPasteClipboard() ), pMainWindow, SLOT( sltPasteClipboard() ) );
    connect( this, SIGNAL( sigSelectAllClipboard() ), pMainWindow, SLOT( sltSelectAllClipboard() ) );
    connect( this, SIGNAL( sigDeleteSelectedItems() ), pMainWindow, SLOT( sltDeleteSelectedItems() ) );

    connect( this, SIGNAL( sigShowActImage(const QString &) ), pMainWindow, SLOT( sltShowImageFile(const QString &) ) );
#endif
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

const char * DocumentAndControler::GetPlayModusStrg() const
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

/*
// min todo --> probleme bei der Anzeige ???	
	if( IsPlayModus() )
	{
		return (const char *)tr( "PLAY" );
	}
	else if( IsPauseModus() )
	{
		return (const char *)tr( "PAUSE" );
	}
	else
	{
		return (const char *)tr( "EDIT" );
	}	
	return (const char *)tr( "UNKNOWN" );
*/
}

const string & DocumentAndControler::GetName() const
{
	// update the local variable for the presentation
	// local variable needed for a correct handling of the
	// script command GetDocName()
	((DocumentAndControler *)this)->m_sFullName = m_aPresentation.GetFullName();
	return m_sFullName;
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
		sMsg += m_aPresentation.GetName();
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
	const char * s = (const char *)sFileName;

	fstream aFile( s, ios::in );

	bOk = aFile.good();
	if( bOk )
	{
#ifndef ZAURUS
 		try {
#endif
			bOk = m_aPresentation.Read( aFile, bExecuteScript );

			// ** if the original file (with stored a file path) does not exists any more,
			// ** change it to the filename which was used to load this file !
			if( bOk && !FileUtilityObj::ExistsFile( m_aPresentation.GetName() ) )
			{
				m_aPresentation.SetName( s );

				//emit sigShowWarningMessage( tr( "Path of file has changed !" ) );
			}

            AddToFileHistory(sFileName);
            
#ifndef ZAURUS
		}
		catch( MinException & aException )
		{
			emit sigShowWarningMessage( aException.GetMsg().c_str() );
			bOk = false;
		}
#else
		if( !bOk )
		{
			emit sigShowWarningMessage( tr( "Error reading file !" ) );
		}
#endif
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
	const char * s = (const char *)sFileName;
	m_aPresentation.SetName( s );
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

	bOk = aTree.WriteXML( (const char *)sFileName );
}

void DocumentAndControler::sltExportDynGraphicData( const QString & sFileName )
{
	fstream aFile( (const char *)sFileName, ios::in );

	if( aFile.good() )
	{
		m_aPresentation.GetDynGraphicData().Write( aFile );
	}
}

void DocumentAndControler::sltImportDynGraphicData( const QString & sFileName )
{
	fstream aFile( (const char *)sFileName, ios::in );

	if( aFile.good() )
	{
		m_aPresentation.GetDynGraphicData().Read( aFile );
	
		emit sigSyncAllViews();
	}
}

void DocumentAndControler::sltDataChanged()
{
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
		m_pTimer->start( 1, /*single shot*/TRUE );
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
	const char * s =(const char *)sText;
	string sTemp = (s ? s : "");

	int iFoundPos = m_aPresentation.FindItemWithText( s, 0 );

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
	const char * s =(const char *)sText;
	string sTemp = (s ? s : "");

	int iFoundPos = m_aPresentation.FindItemWithText( s, m_iLastFoundPos+1 );

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
			m_pTimer->start( iTimer, /*single shot*/TRUE );
		}
	}

	//sltUpdateStatusBar();
}

void DocumentAndControler::sltUpdateStatusBar()
{
	QString s = GetPresentation().GetStepInfo().c_str();
	emit sigShowStatusMessage( s );
}

void DocumentAndControler::TriggerDissolveActDiaNo( int iNo, const char * sScript, const char * /*sFileNameOrScript*/, double dDissolveTime )
{
	int iDissolveTimeInMS = (int)(dDissolveTime*1000.0);
	sltSelectItem( iNo, iDissolveTimeInMS );

	//QString s = sFileNameOrScript;

	// ** not needed here, because image will be updated in method sltSelectItem() !!!
	//emit sigShowActImage( s );

	if( strlen( sScript ) > 0 )
	{
		ExecuteScript( /*bDissolve*/true, sScript, iNo );
	}
}

void DocumentAndControler::TriggerShowActDiaNo( int iNo, const char * sScript, const char * /*sFileNameOrScript*/, double /*dShowTime*/ )
{
	// cout << "*** nr=" << iNo << endl;
	if( strlen( sScript ) > 0 )
	{
		ExecuteScript( /*bDissolve*/false, sScript, iNo );
	}
}

void DocumentAndControler::TriggerSetNextDiaNo( int /*iNextNo*/, const char * /*sNextFileName*/ )
{
	// optional, ist anscheinend momentan nicht notwendig !
	// lade naechtes Image im voraus, damit der Dia-Wechsel ohne zeitliche
	// Verzoegerung statt finden kann !
}

void DocumentAndControler::PresentationModusChanged()
{
	emit sigModusIsSwitched();
}

void DocumentAndControler::ShowError( const char * sMsg )
{
    QString s( sMsg );
    emit sigShowErrorMessage( s );
}


const char * DocumentAndControler::GetDocName() const
{
	return GetName().c_str();
}

const char * DocumentAndControler::GetDescription() const
{
	return m_aPresentation.GetComment().c_str();
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

const char * DocumentAndControler::GetSoundFileNameAt( int iIndex ) const
{
	if( (iIndex>=0) && (iIndex<GetSoundFileCount()) )
	{
		return m_aPresentation.GetSoundInfoData()[ iIndex ]->GetFileName().c_str();
	}
	return "";
}

bool DocumentAndControler::SetSoundFileNameAt( int iIndex, const char * sFileName )
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

bool DocumentAndControler::LoadPresentation( const char * sFileName, bool bExecuteScript )
{
	bool bOk;

	sltLoadDoc( sFileName, bOk, bExecuteScript );

	return bOk;
}

bool DocumentAndControler::SavePresentation()
{
	bool bOk;

	sltSaveDoc( bOk );

	return bOk;
}

bool DocumentAndControler::SavePresentationAs( const char * sFileName )
{
	bool bOk;

	sltSaveAsDoc( sFileName, bOk );

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


// ** WARNING: asynchronious script execution results in a 'blocked' gui !
//#define _with_async_scripts

#ifdef _with_async_scripts

#include "osdep2.h"

extern "C" void _CALLING_CONV _MinScriptProcThreadStarter( void * pData )
{
	const char * sFileNameOrScript = (const char *)pData;

	minClientHandle<IGeneralScriptEnvironment>	hScriptEnv( g_IGeneralScriptEnvironmentID );

	if( hScriptEnv.IsValid() )
	{
		hScriptEnv->ExecuteScript( sFileNameOrScript, IGeneralScriptEnvironment::PYTHON, 0 );
	}
}

#endif

void DocumentAndControler::ExecuteScript( bool bDissolve, const char * sScript, int iNo )
{
#ifndef ZAURUS
	// ** WARNING: execution of scripts in own thread not supported yet, because
	// **          there are some problems with a blocked GUI !
#ifdef _with_async_scripts
	unsigned long ulThreadId = minBeginThread( _MinScriptProcThreadStarter, _DEFAULT_STACK_SIZE, (void *)sFileNameOrScript );
#else
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
#endif
#endif // ZAURUS
}

QApplication * GetApplication();

static void ClearImageCache( QImageCache & aImageCache )
{
#ifndef ZAURUS
	aImageCache.clear();
#endif
}

static bool InitImageCache( QImageCache & aImageCache, const DiaPresentation & aPresentation, int iWidth, int iHeight )
{
#ifndef ZAURUS
    QProgressDialog aProgress( QObject::tr("creating image cache"), QObject::tr("Cancel"), 0, aPresentation.GetDiaCount()/*, 0, "progress"*//*, TRUE*/ );

	aProgress.show();
    aProgress.setValue( 0 );

	ClearImageCache( aImageCache );
	
	for( int i=0; i<aPresentation.GetDiaCount(); i++ )
	{
		minHandle<DiaInfo> hDia = aPresentation.GetDiaAt( i );

		if( strlen( hDia->GetImageFile() )>0 )
		{
			QImage aImage( hDia->GetImageFile() );
			aImage = aImage.smoothScale( iWidth, iHeight );
			aImageCache[hDia->GetImageFile()] = aImage;
		}
		else
		{
			// add an empty image
			QImage aEmptyImage( iWidth, iHeight, 32 );
			aEmptyImage.fill( ~0 );

			aImageCache[""] = aEmptyImage;
		}

        aProgress.setValue( i+1 );
		GetApplication()->processEvents();
		if( aProgress.wasCanceled() )
		{
			return false;
		}
	}
	
    aProgress.setValue( aPresentation.GetDiaCount() );

	return true;
#endif
}

int DocumentAndControler::CreateImagesForMovie( 
		const string & sOutputDirectory, 
		const string & sFileNameOffset, 
		int iWidth, 
		int iHeight, 
		double dStartMS, 
		double dStopMS, 
		double dDeltaMS )
{
#ifndef ZAURUS
	// 1600x1200 = 1,3333333 = 4:3
	// 540 x 360 = 1,5
	QImageCache aImageCache;
	QPixmap aPixmap( iWidth, iHeight );
	QPainter aPainter;

	//QCanvas::drawArea()
	//QImage aImage = aPixmap.convertToImage();

	int iCount = 0;
	if( InitImageCache( aImageCache, GetPresentation(), iWidth, iHeight ) )
	{		
        QProgressDialog aProgress( QObject::tr("creating images for movie"), QObject::tr("Cancel"), 0, (int)dStopMS/*, 0, "progress", TRUE*/ );

		aProgress.show();
        aProgress.setValue( 0 );

		string sLastFileName;
		double dTimeMS = dStartMS;
		while( dTimeMS < dStopMS )
		{
			//bool bForcePainting = true;
			char sBuffer[512];
            // /Users/min/Documents/home/Entwicklung/projects/mindia_qt4/src/
            sprintf( sBuffer, "%s/%s%05d.jpg", sOutputDirectory.c_str(), sFileNameOffset.c_str(), iCount );
 
			if( GetPresentation().IsNextSlideChanging( dTimeMS, dDeltaMS ) )
			{
				aPixmap.fill();
				aPainter.begin( &aPixmap );
				GetPresentation().PaintSlideForTime( aImageCache, aPainter, dTimeMS );
				aPainter.end();

				/*bool bOk =*/ aPixmap.save(sBuffer,"JPEG",100);

				sLastFileName = sBuffer;
			}
			else
			{
				// use last generated file for new movie image
                /*bool ok =*/ QFile::copy(sLastFileName.c_str(), sBuffer );
				//bool ok = FileUtilityObj::_CopyFile( sLastFileName.c_str(), sBuffer );
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
	}

	ClearImageCache( aImageCache );

	return iCount;
#endif // ZAURUS
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

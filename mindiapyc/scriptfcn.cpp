/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/mindiapyc/scriptfcn.cpp,v $
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

#include <qaction.h>
#include <qpopupmenu.h>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qtranslator.h>
#include <qapplication.h>
#include <qcolor.h>

#include <stdio.h>

#include <fstream>

#include "scriptfcn.h"

#include "minhandle.h"
#include "minutils.h"
#include "osdep2.h"

#include "scriptdlgimpl.h"

#define _PYTHON_EXT		"*.py"

// *************************************************************************
// *************************************************************************
// *************************************************************************

int	DoMessageBox( const char * sMsg, const char * sTitle )
{
	QWidget * pParentWindow = 0;

	minClientHandle<IGeneralScriptFcn> hScriptFcn( g_IGeneralScriptFcnID );

	if( hScriptFcn.IsValid() && hScriptFcn->IsOk() )
	{
		pParentWindow = hScriptFcn->GetMainWindowPtr();
	}

	return QMessageBox::information( pParentWindow, sTitle, sMsg, 
									 QObject::tr( "&Ok" ), 
									 QObject::tr( "&Cancel" ) );
}


// ** not very nice, but it works for only one script engine
static ScriptMenuHandler *	g_pScriptMenuHandler = 0;

void PrintLn( const char * sText )
{
	if( g_pScriptMenuHandler )
	{
		g_pScriptMenuHandler->sltPrintToScriptOutput( sText );
	}
}

void SleepMS( int iMS )
{
	minSleep( iMS );
}

IHsv RgbToHsv( const IColor & aColor )
{
	QColor aQtColor( aColor.iRed, aColor.iGreen, aColor.iBlue );

	IHsv aHsv;
	aQtColor.hsv( &aHsv.iHue, &aHsv.iSaturation, &aHsv.iValue );

	return aHsv;
}

IColor HsvToRgb( const IHsv & aHsv )
{
	QColor aQtColor( aHsv.iHue, aHsv.iSaturation, aHsv.iValue, QColor::Hsv );

	IColor aColor;
	aColor.iRed = aQtColor.red();
	aColor.iGreen = aQtColor.green();
	aColor.iBlue = aQtColor.blue();
	return aColor;
}

// *************************************************************************
// *************************************************************************
// *************************************************************************

// Helper class
class DynamicSubMenuContainer : public vector< pair< int, string > >
{
public:
	string	FindScriptForItem( int iId ) const
	{
		const_iterator aIter = begin();

		while( aIter != end() )
		{
			const_reference aItem = *aIter;

			if( aItem.first == iId )
			{
				return aItem.second;
			}

			++aIter;
		}

		return "";
	}

	void Clear()
	{
		erase( begin(), end() );
	}
};

// *************************************************************************

static bool IsWhitespace( char ch )
{
	if( (ch==' ') || (ch=='\t') )
	{
		return true;
	}
	return false;
}

static bool IsPythonCommentLine( const string & sLine, int & iPosOut )
{
	int i = 0;
	iPosOut = i;
	while( (i<(int)sLine.length()) && IsWhitespace( sLine[i] ) )
	{
		i++;
	}
	if( i<(int)sLine.length() )
	{
		iPosOut = i;
		return ( sLine[i] == '#' );
	}
	return false;
}

static string ReadFirstCommentLineForPythonFile( const char * sFileName )
{
	fstream aFile( sFileName, ios::in );

	if( aFile.good() )
	{
		const int _MAX_BUFFER = 1024;
		char sBuffer[ _MAX_BUFFER ];
		strcpy( sBuffer, "" );

		aFile.getline( sBuffer, _MAX_BUFFER );

		string s = sBuffer;

		int iPos = 0;
		if( IsPythonCommentLine( s, iPos ) )
		{
			return s.substr( iPos+1 );
		}
	}

	return "";
}

// *************************************************************************
// *************************************************************************
// *************************************************************************

#define _UNKNOWN_STRG	"Unknown"
#define _PYTHON_STRG	"Python"
#define _PERL_STRG		"Perl"
#define _TCL_STRG		"Tcl"
#define _SCHEME_STRG	"Scheme"
#define _CPP_STRG		"Cpp"

ScriptMenuHandler::ScriptMenuHandler( IGeneralScriptEnvironment * pScriptEnv )
: m_pScriptModifyDlg( 0 ),
  m_pModifyScriptAction( 0 ),
  m_pPluginMenu( 0 ),
  m_pMainWindow( 0 ),
  m_pScriptEnv( pScriptEnv )
{
	Init();

	m_pMenuItemContainer = new DynamicSubMenuContainer();
}

ScriptMenuHandler::~ScriptMenuHandler()
{
	delete m_pModifyScriptAction;
	delete m_pScriptMenu;
	delete m_pScriptModifyDlg;

	delete m_pMenuItemContainer;
}

bool ScriptMenuHandler::IsOk() const
{
	return ( m_pPluginMenu != 0 );
}

void ScriptMenuHandler::Init()
{
	minClientHandle<IGeneralScriptFcn> hScriptFcn( g_IGeneralScriptFcnID );

	if( hScriptFcn.IsValid() && hScriptFcn->IsOk() )
	{
		// ** install translator for language handling
		QTranslator aTranslator( this );
		QString sLang = "mindia_";
		sLang += hScriptFcn->GetLanguage();
		sLang += ".qm";
		if( strcmp( hScriptFcn->GetLanguage(), "en" ) != 0 )
		{
			bool bOk = aTranslator.load( sLang, "." );
			qApp->installTranslator( &aTranslator );
		}

		m_pPluginMenu = hScriptFcn->GetPluginsMenuPtr();
		m_pMainWindow = hScriptFcn->GetMainWindowPtr();

		m_pScriptMenu = new QPopupMenu( m_pPluginMenu );

		m_pModifyScriptAction = new QAction( tr( "Modify Python scripts" ), tr( "&Modify Python scripts..." ), ALT+Key_M, m_pMainWindow, "python_scripts"/*, TRUE*/ );
		connect( m_pModifyScriptAction, SIGNAL( activated() ), this, SLOT( sltDoModifyScript() ) );

		connect( m_pPluginMenu, SIGNAL( aboutToShow() ), this, SLOT( sltUpdatePluginsMenu() ) );
		connect( m_pScriptMenu, SIGNAL( aboutToShow() ), this, SLOT( sltUpdateScriptsMenu() ) );
		connect( m_pScriptMenu, SIGNAL( aboutToHide() ), this, SLOT( sltHideScriptsMenu() ) );
		connect( m_pScriptMenu, SIGNAL( activated(int) ), this, SLOT( sltScriptsMenuActivated(int) ) );

	    connect( this, SIGNAL( sigShowStatusMessage(const QString &) ), m_pMainWindow, SLOT( sltShowStatusBarMessage(const QString &) ) );

		// ** update the menu
		m_pModifyScriptAction->addTo( m_pPluginMenu );		
		int iId = m_pPluginMenu->insertItem( tr( "&Run Python scripts" ), m_pScriptMenu );
		//m_pPluginMenu->setToolTip( iId, tr( "Run a Python script" ) );
	}
}

bool ScriptMenuHandler::DoModifyScript( const QString & sUserText, QString & sScript )
{
	m_pScriptModifyDlg = new ScriptDlgImpl( m_pMainWindow, "scripts", /*modal*/TRUE, 0, /*bCheckForSave*/false );

	// ** show the user text in the dialog-title 
	m_pScriptModifyDlg->setCaption( m_pScriptModifyDlg->caption() + sUserText );

	m_pScriptModifyDlg->sltSetScript( sScript );

	int iRet = m_pScriptModifyDlg->exec();

	if( iRet == 1 )
	{
		sScript = m_pScriptModifyDlg->GetScript();
	}

	// ** destroy modal dialog ! **
	delete m_pScriptModifyDlg;
	m_pScriptModifyDlg = 0;

	return ( iRet == 1 );
}

void ScriptMenuHandler::sltDoModifyScript()
{
/*	if( !m_pExtrasScriptAction->isOn() )
	{
		if( m_pScriptModifyDlg )
		{
			m_pScriptModifyDlg->hide();
		}
	}
	else
*/	{
		if( !m_pScriptModifyDlg )
		{
			m_pScriptModifyDlg = new ScriptDlgImpl( m_pMainWindow, "scripts", /*modal*/TRUE );
			//m_pScriptModifyDlg->move( 10, 350 );
		}

		m_pScriptModifyDlg->exec();

		// ** destroy modal dialog ! **
//		delete m_pScriptModifyDlg;
//		m_pScriptModifyDlg = 0;
	}
}

void ScriptMenuHandler::sltUpdateScriptsMenu()
{
	// ** erase all old menu-entries in menu and in the menu-item-map
	m_pScriptMenu->clear();
	m_pMenuItemContainer->Clear();

	int iIndexCount = 0;

	// ** search allways the actual directory and the script directory 
	// ** for available scripts
	for( int j=0; j<2; j++ )
	{
		// ** get the directory where the python scripts are installed
		string sPythonDir;
	
		// ** in second loop look for scripts in the installation directory
		if( j==1 )
		{
			minClientHandle<IGeneralScriptFcn> hScriptFcn( g_IGeneralScriptFcnID );

			if( hScriptFcn.IsValid() && hScriptFcn->IsOk() )
			{
				sPythonDir = hScriptFcn->GetScriptDirecotry();

				// ** if MinDia is not installed, don't search 
				// ** in the installation directory !
				if( (sPythonDir.length()==0) || (sPythonDir=="./") || (sPythonDir==".\\") )
				{
					break;
				}
			}
		}

		sPythonDir += ".";

		// ** create for every script found in the current directory a menu item
		Directory aDir;
		FileSystemUtils::GetDirectory( sPythonDir.c_str(), FSU_OnlyFiles, aDir, _PYTHON_EXT );

		for( int i=0; i<aDir.GetNoOfFiles(); i++ )
		{
			// ** build menu name: filename (without extention) and 
			// ** comment-string of first line in script

			// ** but ignore the SWIG generated file mindiapy.py
			if( aDir[ i ].GetName() != "mindiapy.py" )
			{
				string sAccelerator = "  ";
				if( iIndexCount<10 )
				{
					char sBuffer[64];
					sprintf( sBuffer, "&%d ", iIndexCount );
					sAccelerator = sBuffer;
				}
				string sComment = sAccelerator +
								  aDir[ i ].GetName() +
								  ": " +
								  ReadFirstCommentLineForPythonFile( aDir[ i ].GetFirstName().c_str() );
				m_pScriptMenu->insertItem( sComment.c_str(), iIndexCount );

				// ** save the name of the script in a menu-dictionary,
				// ** this is needed, because we use only one callback-method
				// ** to handle all script-menu-items (sltScriptsMenuActivated())
				DynamicSubMenuContainer::value_type aItem( iIndexCount, aDir[ i ].GetFirstName()/*GetName()*/ );
				m_pMenuItemContainer->push_back( aItem );

				iIndexCount++;
			}
		}
	}
}

void ScriptMenuHandler::sltHideScriptsMenu()
{
	// ** not needed yet !
}

void ScriptMenuHandler::sltScriptsMenuActivated( int iIndex )
{
	// ** search for the script-name, load it and run it

	string sScriptName = m_pMenuItemContainer->FindScriptForItem( iIndex );

	if( (sScriptName.length()>0) && m_pScriptEnv )
	{
		string sScript;

		if( ReadTextFile( sScriptName.c_str(), sScript ) )
		{
			int iRet = m_pScriptEnv->ExecuteScript( sScript, IGeneralScriptEnvironment::PYTHON );

			// ** show the result of the script in the status bar **
			QString sScriptResult;
			sScriptResult = sScriptResult.sprintf( tr( "Script %s returned: %d" ), sScriptName.c_str(), iRet );
			emit sigShowStatusMessage( sScriptResult );
		}
	}
}

void ScriptMenuHandler::sltUpdatePluginsMenu()
{
	minClientHandle<IMinDiaScriptFcn> hScriptFcn( g_IMinDiaScriptFcnID );

	if( hScriptFcn.IsValid() && hScriptFcn->IsOk() )
	{
		bool bIsPlaying = hScriptFcn->IsPlayMode();
		bool bIsPause = hScriptFcn->IsPauseMode();
		bool bEdit = !(bIsPlaying || bIsPause);

		// ** enable script dialog only in edit modus and if there is a script-engine available
		// ** but if we are here, we have a sciprt engine
		m_pModifyScriptAction->setEnabled( bEdit );
	}
}

void ScriptMenuHandler::sltPrintToScriptOutput( const QString & sText )
{
	if( m_pScriptModifyDlg )
	{
		m_pScriptModifyDlg->sltPrintToOutput( sText );
	}
}

// *************************************************************************

IGeneralScriptEnvironmentImpl::IGeneralScriptEnvironmentImpl( const string & sModuleName, minDll * pDll )
: IGeneralScriptEnvironment( sModuleName, pDll ),
  m_hScriptVM( g_IGeneralScriptVMID ),
  m_pScriptMenuHandler( 0 )
{
	InitEnvironment();
}

IGeneralScriptEnvironmentImpl::~IGeneralScriptEnvironmentImpl()
{
	delete m_pScriptMenuHandler;
}

bool IGeneralScriptEnvironmentImpl::IsOkImpl() const
{
	return m_hScriptVM.IsValid() && m_hScriptVM->IsOk();
}

bool IGeneralScriptEnvironmentImpl::IsOk() const
{
	// ** if script-vm is not valid, try to get a valid one !
	((IGeneralScriptEnvironmentImpl *)this)->m_hScriptVM.UpdateIfNotValid();
	((IGeneralScriptEnvironmentImpl *)this)->InitEnvironment();

	return IsOkImpl();
}

bool IGeneralScriptEnvironmentImpl::HasLanguage( ScriptLanguage aLanguage ) const
{
	// ** only python is supported yet (30.12.2001)
	if( aLanguage == PYTHON )
	{
		return true;
	}
	return false;
}

string IGeneralScriptEnvironmentImpl::GetLanguageStrg( ScriptLanguage aLanguage ) const
{
	switch( aLanguage )
	{
		case PYTHON:
			return _PYTHON_STRG;
		case PERL:
			return _PERL_STRG;
		case TCL:
			return _TCL_STRG;
		case SCHEME:
			return _SCHEME_STRG;
		case CPP:
			return _CPP_STRG;

		default:
			return _UNKNOWN_STRG;
	}
	return "";
}

IGeneralScriptEnvironmentImpl::ScriptLanguage IGeneralScriptEnvironmentImpl::GetLanguageFromStrg( const string & sLanguage ) const
{
	if( sLanguage == _PYTHON_STRG )
	{
		return PYTHON;
	}
	else if( sLanguage == _PERL_STRG )
	{
		return PERL;
	}
	else if( sLanguage == _TCL_STRG )
	{
		return TCL;
	}
	else if( sLanguage == _SCHEME_STRG )
	{
		return SCHEME;
	}
	else if( sLanguage == _CPP_STRG )
	{
		return CPP;
	}
	else
	{
		return UNKNOWN;
	}
}

bool IGeneralScriptEnvironmentImpl::ModifyScript( const string & sUserComment, string & sScriptInOut, ScriptLanguage aLanguage )
{
	QString sTemp = sScriptInOut.c_str();
	QString sCommentTemp = sUserComment.c_str();

	bool bOk = m_pScriptMenuHandler->DoModifyScript( sCommentTemp, sTemp );
	
	if( bOk )
	{
		const char * s = (const char *)sTemp;
		sScriptInOut = s ? s : "";
	}

	return bOk;
}

int IGeneralScriptEnvironmentImpl::ExecuteScript( const string & sScriptIn, ScriptLanguage aLanguage )
{
	if( IsOk() && HasLanguage( aLanguage ) )
	{
		return m_hScriptVM->ExecuteScript( sScriptIn );
	}

	return -1;
}


bool IGeneralScriptEnvironmentImpl::IsInitialized() const
{
	return ( m_pScriptMenuHandler && m_pScriptMenuHandler->IsOk() );
}

void IGeneralScriptEnvironmentImpl::InitEnvironment()
{
	if( !IsInitialized() )
	{
		minClientHandle<IGeneralScriptFcn> hScriptFcn( g_IGeneralScriptFcnID );

		if( hScriptFcn.IsValid() && hScriptFcn->IsOk() )
		{
			m_pScriptMenuHandler = new ScriptMenuHandler( this );

			// ** set the global ScriptMenuHandler, for PrintLn() Implementation
			g_pScriptMenuHandler = m_pScriptMenuHandler;
		}
	}
}


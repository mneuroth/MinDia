/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/main.cpp,v $
 *
 *  $Revision: 1.6 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.5  2004/01/18 23:42:05  min
 *	Windows only: get path .mindia.ini from registry.
 *	
 *	Revision 1.4  2003/11/09 14:25:45  min
 *	gendev project added
 *	
 *	Revision 1.3  2003/10/26 17:25:01  min
 *	Added new directories: images, sounds, data, scripts
 *	
 *	Revision 1.2  2003/10/03 23:05:26  min
 *	Scripts in own directory moved, python conform dll nameing: mindiapy_d.dll
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

#include <qapplication.h>
//#include <qimage.h>

#include "mindiawindow.h"
#include "doccontroler.h"

#include "minbase.h"
#include "iscript.h"

#ifdef __linux__
#include "minutils.h"
#include "config.h"				// for autoconf values...
#define _MINDIAPYC_DLL_NAME		"libmindiapyc.so"
#define _GENDEV_DLL_NAME		"libgendev.so"
//#define _LINUX_MINDIA_SHARED	"/usr/local/shared/mindia/";
#else
#include <windows.h>
#ifdef _DEBUG
#define _MINDIAPYC_DLL_NAME		"mindiapyc_d.dll"
#else
#define _MINDIAPYC_DLL_NAME		"mindiapyc.dll"
#endif
#define _GENDEV_DLL_NAME		"gendev.dll"
#endif

#define _SCRIPTS_DIR	"scripts"
#define _IMAGES_DIR		"images"
#define _MUSIC_DIR		"music"
#define _DATA_DIR		"data"

// *************************************************************************

/** Get the directory where all the help- and language-files are installed. */
string GetMinDiaSharedDirectory()
{
	string sRet;
	string sSep( FileUtilityObj::GetDirectorySeparatorStrg() );

#ifdef __linux__
	// ** if MinDia is installed, than the german language file is in this directory
	string sTestFileInShared;
	
	sTestFileInShared += MINDIA_INSTALLATION_DIR;		//_LINUX_MINDIA_SHARED;
	sTestFileInShared += sSep;
	sTestFileInShared += "mindia_de.qm";

	if( FileUtilityObj::ExistsFile( sTestFileInShared.c_str() ) )
	{
		sRet = MINDIA_INSTALLATION_DIR;		//_LINUX_MINDIA_SHARED;
		sRet += sSep;
	}
	else
	{
		// ** if MinDia is not installed, try the local directory
		sRet = ".";
		sRet += sSep;
	}
#endif

#ifdef _MSC_VER
	sRet = ".";
	// HKEY_CURRENT_USER.Software.mindia.inifile
	HKEY hKey;
	if( RegOpenKeyEx( HKEY_CURRENT_USER, "Software", 0, KEY_ALL_ACCESS, &hKey ) == ERROR_SUCCESS )
	{
		HKEY hKeyMindia;
		if( RegOpenKeyEx( hKey, "mindia", 0, KEY_ALL_ACCESS, &hKeyMindia ) == ERROR_SUCCESS )
		{
			char sBuffer[512];
			DWORD dwLength = 512;
			DWORD dwType = REG_SZ;

			RegQueryValueEx( hKeyMindia, "inifile", NULL, &dwType, (LPBYTE)sBuffer, &dwLength );

			sRet = sBuffer;

			RegCloseKey( hKeyMindia );
		}

		RegCloseKey( hKey );

		return sRet+sSep;
	}
	sRet = ".";
	sRet += sSep;
#endif

	return sRet;
}

// *************************************************************************

class IGeneralScriptFcnImpl : public IGeneralScriptFcn
{
public:
	IGeneralScriptFcnImpl( const char * sLanguage, const string & sModuleName, MinDiaWindow * pMainWindow )
	: IGeneralScriptFcn( sModuleName ),
	  m_pMainWindow( pMainWindow ),
	  m_sLanguage( sLanguage ? sLanguage : "en" )
	{
		Init();
	}

	virtual bool IsOk() const;

	virtual QWidget *		GetMainWindowPtr();
	virtual QPopupMenu *	GetPluginsMenuPtr();
	virtual const char *	GetLanguage() const;
	virtual const char *	GetHelpDirecotry() const;
	virtual const char *	GetScriptDirecotry() const;
	virtual const char *	GetImageDirecotry() const;
	virtual const char *	GetMusicDirecotry() const;
	virtual const char *	GetDataDirecotry() const;

private:
	void Init();

	MinDiaWindow *		m_pMainWindow;		// no owner !!!
	string				m_sLanguage;
	string				m_sHelpDirectory;
	string				m_sScriptDirectory;
	string				m_sImageDirectory;
	string				m_sMusicDirectory;
	string				m_sDataDirectory;
};

bool IGeneralScriptFcnImpl::IsOk() const
{
	return ( m_pMainWindow != 0 );
}

QWidget * IGeneralScriptFcnImpl::GetMainWindowPtr()
{
	return m_pMainWindow;
}

QPopupMenu * IGeneralScriptFcnImpl::GetPluginsMenuPtr()
{
	if( m_pMainWindow )
	{
		return m_pMainWindow->GetPluginsMenuPtr();
	}
	return 0;
}

const char * IGeneralScriptFcnImpl::GetLanguage() const
{
	return m_sLanguage.c_str();
}

const char * IGeneralScriptFcnImpl::GetHelpDirecotry() const
{
	return m_sHelpDirectory.c_str();
}

const char * IGeneralScriptFcnImpl::GetScriptDirecotry() const
{
	return m_sScriptDirectory.c_str();
}

const char * IGeneralScriptFcnImpl::GetImageDirecotry() const
{
	return m_sImageDirectory.c_str();
}

const char * IGeneralScriptFcnImpl::GetMusicDirecotry() const
{
	return m_sMusicDirectory.c_str();
}

const char * IGeneralScriptFcnImpl::GetDataDirecotry() const
{
	return m_sDataDirectory.c_str();
}

void IGeneralScriptFcnImpl::Init()
{
	string sTemp = GetMinDiaSharedDirectory();
	string sSep( FileUtilityObj::GetDirectorySeparatorStrg() );

	m_sHelpDirectory = sTemp;	// this path endst with a directory separator
	m_sScriptDirectory = sTemp + _SCRIPTS_DIR /*+ sSep*/;
	m_sImageDirectory = sTemp + _IMAGES_DIR /*+ sSep*/;
	m_sMusicDirectory = sTemp + _MUSIC_DIR /*+ sSep*/;
	m_sDataDirectory = sTemp + _DATA_DIR /*+ sSep*/;
}

// *************************************************************************

class IMinDiaScriptFcnImpl : public IMinDiaScriptFcn
{
public:
	IMinDiaScriptFcnImpl( const string & sModuleName, MinDiaWindow * pMainWindow, int iArgc, char * * pArgv )
	: IMinDiaScriptFcn( sModuleName ),
	  m_pMainWindow( pMainWindow ),
	  m_iArgc( iArgc ),
	  m_pArgv( pArgv )
	{}

	virtual bool IsOk() const;

	virtual bool				IsEditMode() const;
	virtual bool				IsPlayMode() const;
	virtual bool				IsPauseMode() const;

	virtual IDiaPresentation * 	GetDiaPresentation();

	virtual int 				GetArgCount() const;
	virtual char * * 			GetArgVec() const;

private:
	MinDiaWindow *		m_pMainWindow;
	int 				m_iArgc;
	char * * 			m_pArgv;			// no owner !!!
};


bool IMinDiaScriptFcnImpl::IsOk() const
{
	return ( m_pMainWindow != 0 );
}

bool IMinDiaScriptFcnImpl::IsEditMode() const
{
	if( m_pMainWindow && m_pMainWindow->GetDocument() )
	{
		return m_pMainWindow->GetDocument()->IsEditModus();
	}
	return false;
}

bool IMinDiaScriptFcnImpl::IsPlayMode() const
{
	if( m_pMainWindow && m_pMainWindow->GetDocument() )
	{
		return m_pMainWindow->GetDocument()->IsPlayModus();
	}
	return false;
}

bool IMinDiaScriptFcnImpl::IsPauseMode() const
{
	if( m_pMainWindow && m_pMainWindow->GetDocument() )
	{
		return m_pMainWindow->GetDocument()->IsPauseModus();
	}
	return false;
}

IDiaPresentation * IMinDiaScriptFcnImpl::GetDiaPresentation()
{
	if( m_pMainWindow && m_pMainWindow->GetDocument() )
	{
		return m_pMainWindow->GetDocument();
	}
	return 0;
}

int IMinDiaScriptFcnImpl::GetArgCount() const
{
	return m_iArgc;
}

char * * IMinDiaScriptFcnImpl::GetArgVec() const
{
	return m_pArgv;
}

// *************************************************************************
// *************************************************************************
// *************************************************************************

static QApplication * g_pApplication = 0;

QApplication * GetApplication()
{
	return g_pApplication;
}

int main( int argc, char** argv )
{
	QApplication aApp( argc, argv );

	g_pApplication = &aApp;

	bool bIgnoreComSettings = false;
	bool bSimulation = true;
	bool bExecuteEvent = true;
	bool bAutoRun = false;
	bool bShowScreen = false;
	bool bExitOnFinished = false;
	int  iScreenX = 0;
	int  iScreenY = 0;
	int  iPosX = 0;
	int  iPosY = 0;
	int  iProjectorType = RolleiCom::TWIN_DIGITAL_P;
	QString sFileName;
	QString sLanguage = "en";		// defautl is english

	// ** do a very simple argument parsing
	for( int i=1; i<argc; i++ )
	{
		QString s = argv[i];

		// ** check for simulation-mode
		if( s == "-sim" )
		{
			bSimulation = true;
		}
		else if( s == "-real" )
		{
			bSimulation = false;
		}
		else if( s == "-ignore" )
		{
			bIgnoreComSettings = true;
		}
		else if( s == "-edit" )
		{
			bExecuteEvent = false;
		}
		else if( s == "-msc" )
		{
			iProjectorType = RolleiCom::MSC_300_P;
		}
		else if( s == "-twin" )
		{
			iProjectorType = RolleiCom::TWIN_DIGITAL_P;
		}
		//else if( s == "-cd" )
		//{
		//	// not necassary !
		//}
		else if( s == "-autorun" )
		{
			bAutoRun = true;
		}
		else if( s == "-autoquit" )
		{
			bExitOnFinished = true;
		}
		else if( s == "-screen" )
		{
			bShowScreen = true;
		}
		else if( s.find( "-dx=" )==0 )
		{
			QString sSize = s.right( s.length()-4 );
			iScreenX = sSize.toInt();
		}
		else if( s.find( "-dy=" )==0 )
		{
			QString sSize = s.right( s.length()-4 );
			iScreenY = sSize.toInt();
		}
		else if( s.find( "-x=" )==0 )
		{
			QString sSize = s.right( s.length()-3 );
			iPosX = sSize.toInt();
		}
		else if( s.find( "-y=" )==0 )
		{
			QString sSize = s.right( s.length()-3 );
			iPosY = sSize.toInt();
		}
		else if( s == "-en" )
		{
			sLanguage = "en";
		}
		else if( s == "-de" )
		{
			sLanguage = "de";
		}
		else if( s == "-fr" )
		{
			sLanguage = "fr";
		}
		else if( s == "-nl" )
		{
			sLanguage = "nl";
		}
		// ** everything else will be treated as a filename
		else
		{
			sFileName = s;
		}
	}

	MinDiaWindow aWindow( sLanguage, bIgnoreComSettings, bSimulation, iProjectorType );
	aApp.setMainWidget( &aWindow );

	// ** environment for dll starting... **
	minServiceManager * pSrvManager = new minServiceManager;
	minServiceManager::SetGlobServiceManagerPtr( pSrvManager );

	IMinDiaScriptFcnImpl * pMinDiaScriptFcn = new IMinDiaScriptFcnImpl( "mindia_for_py", &aWindow, argc, argv );
	pSrvManager->RegisterService( pMinDiaScriptFcn, /*bOwnerIn*/true );

	IGeneralScriptFcnImpl * pGenScriptFcn = new IGeneralScriptFcnImpl( (const char *)sLanguage, "mindia_for_py", &aWindow );
	pSrvManager->RegisterService( pGenScriptFcn, /*bOwnerIn*/true );

	minDll * pNewDll = 0;
	if( !pSrvManager->GetDllManager().LoadMinDll( _MINDIAPYC_DLL_NAME, pNewDll ) )
	{
		cout << "Error loading mindiapy-dll" << endl;
	}
	else
	{
		cout << "Successfully loaded mindiapy-dll" << endl;
	}
	if( !pSrvManager->GetDllManager().LoadMinDll( _GENDEV_DLL_NAME, pNewDll ) )
	{
		cout << "Error loading gendev-dll" << endl;
	}
	else
	{
		cout << "Successfully loaded gendev-dll" << endl;
	}

	//aSrvManager.GetDllManager().Dump( cout );

	/*
	cout << "Service count = " << aSrvManager.GetNoOfServices() << endl;
	for( int i=0; i<aSrvManager.GetNoOfServices(); i++ )
	{
		cout << i << " " << aSrvManager.GetServiceInfoStringFromIndex( i ).c_str() << endl;
	}
	*/

	// ** done **

	// ** need global Translator for the other dialogs...
	QTranslator aTranslator( &aWindow );
	QString sLang = GetMinDiaSharedDirectory().c_str();
	sLang += "mindia_";
	sLang += sLanguage;
	sLang += ".qm";
	if( sLanguage != "en" )
	{
		/*bool bOk =*/ aTranslator.load( sLang, "." );
		qApp->installTranslator( &aTranslator );
	}

	// ** load file if any filename is given as an argument
	if( !sFileName.isEmpty() )
	{
		aWindow.sltLoadDoc( sFileName, bExecuteEvent );
	}

	aWindow.show();

	if( bAutoRun || bShowScreen )
	{
		aWindow.sltStartAutoStartTimer( bAutoRun, bShowScreen, bExitOnFinished, iScreenX, iScreenY, iPosX, iPosY );
	}
    /* test code to checke which image-formats are supported.
	QStringList aList = QImage::inputFormatList () ;
	QStringList::Iterator aIter = aList.begin();
	while( aIter != aList.end() )
	{
		QString sTemp = *aIter;
		const char * s = (const char *) sTemp;
		++aIter;
	}
    */
	int iRet = aApp.exec();

	pSrvManager->GetDllManager().UnLoadAll();

	delete pSrvManager;

	g_pApplication = 0;

	return iRet;
}


/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2013 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QMenu>
#include <QLocale>
#include <QLibraryInfo>
#include <QDir>

#include "mindiawindow.h"
#include "doccontroler.h"
#include "misctools.h"

#include "minbase.h"
#include "iscript.h"

#if defined(__linux__) || defined(__APPLE__)
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

#ifdef __BORLANDC__
#define main qMain
#endif

#define _SCRIPTS_DIR	"scripts"
#define _IMAGES_DIR		"images"
#define _MUSIC_DIR		"music"
#define _DATA_DIR		"data"

// *************************************************************************

class IGeneralScriptFcnImpl : public IGeneralScriptFcn
{
public:
    IGeneralScriptFcnImpl( const string & sLanguage, const string & sModuleName, MinDiaWindow * pMainWindow )
	: IGeneralScriptFcn( sModuleName ),
	  m_pMainWindow( pMainWindow ),
      m_sLanguage( sLanguage.size()>0 ? sLanguage : "en" )
	{
		Init();
	}

	virtual bool IsOk() const;

    virtual QWidget *	GetMainWindowPtr();
    virtual QMenu *	    GetPluginsMenuPtr();
    virtual string     	GetLanguage() const;
    virtual string     	GetHelpDirecotry() const;
    virtual string     	GetScriptDirecotry() const;
    virtual string     	GetImageDirecotry() const;
    virtual string     	GetMusicDirecotry() const;
    virtual string     	GetDataDirecotry() const;

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

QMenu * IGeneralScriptFcnImpl::GetPluginsMenuPtr()
{
	if( m_pMainWindow )
	{
		return m_pMainWindow->GetPluginsMenuPtr();
	}
	return 0;
}

string IGeneralScriptFcnImpl::GetLanguage() const
{
    return m_sLanguage;
}

string IGeneralScriptFcnImpl::GetHelpDirecotry() const
{
    return m_sHelpDirectory;
}

string IGeneralScriptFcnImpl::GetScriptDirecotry() const
{
    return m_sScriptDirectory;
}

string IGeneralScriptFcnImpl::GetImageDirecotry() const
{
    return m_sImageDirectory;
}

string IGeneralScriptFcnImpl::GetMusicDirecotry() const
{
    return m_sMusicDirectory;
}

string IGeneralScriptFcnImpl::GetDataDirecotry() const
{
    return m_sDataDirectory;
}

void IGeneralScriptFcnImpl::Init()
{
    string sTemp = ToStdString(QCoreApplication::applicationDirPath());
    //string sSep( FileUtilityObj::GetDirectorySeparatorStrg() );

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

class MindiaApp : public QApplication
{
public:
    MindiaApp(int & argc, char ** argv);
    ~MindiaApp();

    void init(const QString & sFileName, MinDiaWindow * pWindow);

#if defined( Q_WS_MACX )
protected:
    bool event(QEvent *event);
#endif

private:
    MinDiaWindow *      m_pMainWindow;  // not an owner
    QString             m_sMacFile;
};

MindiaApp::MindiaApp(int & argc, char ** argv)
: QApplication(argc,argv),
  m_pMainWindow(0)
{
}

MindiaApp::~MindiaApp()
{
}

void MindiaApp::init(const QString & sFileName, MinDiaWindow * pWindow)
{
    m_pMainWindow = pWindow;
    m_pMainWindow->show();
//    if( !m_sMacFile.isEmpty() )
//    {
//        m_pMainWindow->sltLoadDoc(m_sMacFile,/*bExecuteEvent*/true);
//    }
    if( !sFileName.isEmpty() )
    {
        m_pMainWindow->sltLoadDoc(sFileName,/*bExecuteEvent*/true);
    }
}

#if defined( Q_WS_MACX )
bool MindiaApp::event( QEvent * event )
{
    if( event->type() == QEvent::FileOpen )
    {
        QFileOpenEvent *oe = static_cast<QFileOpenEvent *>(event);
        if ( m_pMainWindow )
        {
            m_pMainWindow->sltLoadDoc( oe->file(), /*bExecuteEvent*/true );
        }
        else
        {
            m_sMacFile = oe->file();
        }
    }
    return QApplication::event(event);
}
#endif

static QApplication * g_pApplication = 0;

QApplication * GetApplication()
{
	return g_pApplication;
}

QString GetLocaleName(QStringList aArgs)
{
    if( aArgs.size()>1 )
    {
        // see: http://de.selfhtml.org/diverses/sprachenlaenderkuerzel.htm
        for( int i=1; i<aArgs.size(); i++ )
        {
            if( aArgs.at(i)=="-en" )
            {
                return "en";
            }
            if( aArgs.at(i)=="-nl" )
            {
                return "nl";
            }
            if( aArgs.at(i)=="-de" )
            {
                return "de";
            }
            if( aArgs.at(i)=="-fr" )
            {
                return "fr";
            }
            if( aArgs.at(i)=="-es" )
            {
                return "es";
            }
        }
    }
    return QLocale::system().name().mid(0,2);
//    return QLocale::system()./*name*/languageToString(QLocale::system().language());
}

int main( int argc, char** argv )
{
    Q_INIT_RESOURCE(mindia);

//    QApplication::setGraphicsSystem("opengl");  // raster, native, opengl

    MindiaApp myApp( argc, argv );

#if defined(Q_OS_ANDROID)
    //qApp->setStyle("plastique");        // motif cde windows windowsxp windowsvista mac plastique gtk cleanlooks
    QFont f = qApp->font();
    f.setBold(true);
    f.setPixelSize(16);
    //    f.setPointSize(6);
    qApp->setFont(f);

    //qApp->setStyleSheet("QTextEdit { background-color: yellow }");
    //qApp->setStyleSheet("QSvgWidget { background-color: yellow }");
    //qApp->setStyleSheet("QListWidget { background-color: yellow }");
    qApp->setStyleSheet("QFontListView { background-color: black }");
#endif

    g_pApplication = &myApp;

	bool bIgnoreComSettings = false;
    bool bSimulation = true;
	bool bExecuteEvent = true;
	bool bAutoRun = false;
	bool bShowScreen = false;
	bool bExitOnFinished = false;
	bool bExpand = false;	// expand images
	int  iScreenX = 0;
	int  iScreenY = 0;
	int  iPosX = 0;
	int  iPosY = 0;
	int  iProjectorType = RolleiCom::TWIN_DIGITAL_P;
	QString sFileName;
    QString sLanguage = GetLocaleName(g_pApplication->arguments());

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
        else if( s.indexOf( "-dx=" )==0 )
		{
			QString sSize = s.right( s.length()-4 );
			iScreenX = sSize.toInt();
		}
        else if( s.indexOf( "-dy=" )==0 )
		{
			QString sSize = s.right( s.length()-4 );
			iScreenY = sSize.toInt();
		}
        else if( s.indexOf( "-x=" )==0 )
		{
			QString sSize = s.right( s.length()-3 );
			iPosX = sSize.toInt();
		}
        else if( s.indexOf( "-y=" )==0 )
		{
			QString sSize = s.right( s.length()-3 );
			iPosY = sSize.toInt();
		}
		else if( s == "-expand" )	// otherwise: original size !
		{
			bExpand = true;
		}
        else if( s == "-en" ||
                 s == "-de" ||
                 s == "-nl" ||
                 s == "-fr" )
        {
            // already processed
        }
		// ** everything else will be treated as a filename
		else
		{
			sFileName = s;
		}
	}

    // Remark: add translation objects before creating window !
    QTranslator qtTranslator/*( &aWindow )*/;
    bool bOk = qtTranslator.load(":/translations/qt_" + sLanguage,QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    if( !bOk )
    {
        bOk = qtTranslator.load("qt_" + sLanguage);
#if defined(Q_OS_MAC)
        if( !bOk )
        {
            bOk = qtTranslator.load(QDir::currentPath()+QString("/mindia.app/Contents/Resources/qt_")+sLanguage);
        }
#elif defined(Q_OS_UNIX)
        // should never be called because we read from the translation path before...
        if( !bOk )
        {
            bOk = qtTranslator.load(QString("/usr/share/qt4/translations/qt_")+sLanguage);
        }
#endif
    }
    g_pApplication->installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    bOk = myappTranslator.load(":/translations/mindia_" + sLanguage);
#if defined(Q_OS_MAC)
    if( !bOk )
    {
        // QCoreApplication::applicationDirPath();  + /../Resources/cliphist_
        bOk = myappTranslator.load(QDir::currentPath()+QString("/mindia.app/Contents/Resources/mindia_")+sLanguage);
    }
#elif defined(Q_OS_UNIX)
    if( !bOk )
    {
        // PREFIX"/share/cliphist2" --> PREFIX from qmake PREFIX=/usr ==> is -DPREFIX=/usr option for compiler (define)
        bOk = myappTranslator.load(QString(PREFIX)+QString("/share/mindia/mindia_")+sLanguage);
    }
#else
    bOk = bOk;      // disable compiler warning for other platforms than Mac
#endif
    g_pApplication->installTranslator(&myappTranslator);


    MinDiaWindow aWindow( sLanguage, bIgnoreComSettings, bSimulation, iProjectorType );

	// ** environment for dll starting... **
	minServiceManager * pSrvManager = new minServiceManager;
	minServiceManager::SetGlobServiceManagerPtr( pSrvManager );

	IMinDiaScriptFcnImpl * pMinDiaScriptFcn = new IMinDiaScriptFcnImpl( "mindia_for_py", &aWindow, argc, argv );
	pSrvManager->RegisterService( pMinDiaScriptFcn, /*bOwnerIn*/true );

    IGeneralScriptFcnImpl * pGenScriptFcn = new IGeneralScriptFcnImpl( ToStdString(sLanguage), "mindia_for_py", &aWindow );
	pSrvManager->RegisterService( pGenScriptFcn, /*bOwnerIn*/true );

    /*
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
    */
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
//	QTranslator * pTranslator = new QTranslator( &aWindow );
//	myProcessLanguage( pTranslator, sLanguage, qApp );

    // ** load file if any filename is given as an argument
//	if( !sFileName.isEmpty() )
//	{
//		aWindow.sltLoadDoc( sFileName, bExecuteEvent );
//	}

    myApp.init(sFileName,&aWindow);

    aWindow.show();

	if( bAutoRun || bShowScreen )
	{
		aWindow.sltStartAutoStartTimer( bAutoRun, bShowScreen, bExpand, bExitOnFinished, iScreenX, iScreenY, iPosX, iPosY );
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

	int iRet = myApp.exec();

	pSrvManager->GetDllManager().UnLoadAll();

	delete pSrvManager;

	g_pApplication = 0;

//	delete pTranslator;

	return iRet;
}


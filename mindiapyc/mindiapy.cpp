/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/mindiapyc/mindiapy.cpp,v $
 *
 *  $Revision: 1.4 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.3  2003/10/26 17:31:45  min
 *	Bugfix.
 *	
 *	Revision 1.2  2003/10/03 23:02:43  min
 *	Scripts in own directory moved, better error-handling (defining own exception handler)
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

// *** tests ***
//  g++ -c -fPIC mindiapy.cpp -I/usr/include/python2.1
//  g++ -c -fPIC minbase.cpp
//  g++ -c -fPIC dllbase.cpp
//  g++ -c -fPIC minutils.cpp
//  g++ -shared mindiapy.o minbase.o dllbase.o minutils.o -o mindiapy.so.1.0 /usr/lib/python2.1/config/libpython2.1.a

//  swig -c++ -python -shadow -o mindia_wrap.c mindia.i
//  g++ -c -fPIC mindia_wrap.c  -I/usr/include/python2.1


#include "dllbase.h"
#include "minbase.h"
#include "minhandle.h"
#include "minutils.h"

#include "iscript.h"

#include "scriptfcn.h"

#define _with_python
#ifdef _with_python

#include <Python.h>

/*
extern "C" int openpty()
{
	return 0;
}

extern "C" int forkpty()
{
	return 0;
}
*/

#endif

// *************************************************************************
// *************************************************************************
// *************************************************************************

#define _with_service
#ifdef _with_service

// ** register module mindiapy
extern "C" void initmindiapyc(void);

// *************************************************************************
/** Implements a script engine, for implementation python is used.
  */
class IGeneralScriptVMImpl : public IGeneralScriptVM
{
public:
	IGeneralScriptVMImpl( minClientHandle<IMinDiaScriptFcn> hScriptFcn, const string & sModuleName, minDll * pDll );
	virtual ~IGeneralScriptVMImpl();

	virtual bool IsOk() const;

	virtual int  ExecuteScript( const string & sScript );

	minClientHandle<IMinDiaScriptFcn>	GetScriptFcn();

private:
	// ** helper methods **
	void InitVM( int argc, char** argv );
	void ExitVM();

	// ** data **
	minClientHandle<IMinDiaScriptFcn> 	m_hScriptFcn;
};


IGeneralScriptVMImpl::IGeneralScriptVMImpl( minClientHandle<IMinDiaScriptFcn> hScriptFcn, const string & sModuleName, minDll * pDll )
: IGeneralScriptVM( sModuleName, pDll ),
  m_hScriptFcn( hScriptFcn )
{
	//cout << "IGeneralScriptVMImpl()" << endl;

	InitVM( hScriptFcn->GetArgCount(), hScriptFcn->GetArgVec() );
}

IGeneralScriptVMImpl::~IGeneralScriptVMImpl()
{
	//cout << "~IGeneralScriptVMImpl()" << endl;

	ExitVM();
}

bool IGeneralScriptVMImpl::IsOk() const
{
// min todo
	return true;
}

int IGeneralScriptVMImpl::ExecuteScript( const string & sScript )
{
	//cout << "Excuting Script:" << endl;
	//cout << sScript.c_str() << endl;

#ifdef _with_python
	// *** old script execution mode
	int iRet = PyRun_SimpleString( (char *)sScript.c_str() );
	//cout << "Run iRet=" << iRet << endl;
	return iRet;
#endif

}

minClientHandle<IMinDiaScriptFcn> IGeneralScriptVMImpl::GetScriptFcn()
{
	return m_hScriptFcn;
}

void IGeneralScriptVMImpl::InitVM( int argc, char** argv )
{
	//cout << "InitVM" << endl;

#ifdef _with_python

	Py_SetProgramName( argv[0] );

	Py_Initialize();

	// ok, this call is important ! otherwies external modules will not be found...
	PySys_SetArgv( argc, argv );

	// ** register mindia-functions at the python interpreter
	initmindiapyc();

	// update the search path, so that all the scripts will be found
	int ok = PyRun_SimpleString("import sys, traceback\n");
	// loop through the paths you want to add
	char sBuffer[512];
	minClientHandle<IGeneralScriptFcn>	hScriptFcn( g_IGeneralScriptFcnID );
	sprintf( sBuffer, 
		     "sys.path.insert(0, \"%s\")\nsys.path.insert(0,\"..%c%s\")\n\n", 
			 hScriptFcn->GetScriptDirecotry(), FileUtilityObj::GetDirectorySeparator(), hScriptFcn->GetScriptDirecotry() );
	ok = PyRun_SimpleString( sBuffer );

	ok = PyRun_SimpleString("import mindiapyc\n");
	//ok = PyRun_SimpleString("import mindiapy\n");
	sprintf( sBuffer, 
		     "def my_error_handler(type,value,tracebk):\n    list = traceback.format_tb(tracebk, None) + traceback.format_exception_only(type, value)\n    s = \"Error in script:\\n\" + ''.join(list[:-1]) + list[-1]\n    mindiapyc.PrintLn(s)\n    del tracebk\n\nsys.excepthook = my_error_handler" );
	ok = PyRun_SimpleString( sBuffer );

	//Py_VerboseFlag = 2;

//	PyObject * p = PyImport_ImportModule( "Tkinter" );
#endif
}

void IGeneralScriptVMImpl::ExitVM()
{
	//cout << "ExitVM" << endl;

#ifdef _with_python
	//Py_Exit( 0 );
	//Py_Finalize();
#endif
}

#endif

// *************************************************************************
// *************************************************************************
// *************************************************************************

#define	_DLL_NAME						"mindiapy"

static int								g_nModulInitCounter	= 0;

#ifdef _with_service
static IGeneralScriptVMImpl *			g_pScriptModule = 0;
static IGeneralScriptEnvironmentImpl *	g_pScriptEnvironment = 0;
#endif


extern "C" int MINDLLEXPORT minModuleInit( void * pData, void * pDllData )
{
	//cout << "Init DLL called ! data=" << pData << endl;

	// Modul soll NICHT mehrfach geladen werden !!!
	if( g_nModulInitCounter>0 )
	{
		return -1;
	}

	g_nModulInitCounter++;

	// Daten in entsprechende Klassen casten
	minDll * pDll = (minDll *)pDllData;
	minServiceManager * pSrvManager = (minServiceManager *)pData;

#ifdef _with_service
	minClientHandle<IMinDiaScriptFcn> hScriptFcn( g_IMinDiaScriptFcnID );

	g_pScriptModule = new IGeneralScriptVMImpl( hScriptFcn, _DLL_NAME, pDll );

	// Service-Objekte beim Service-Manager registrieren
	if( pSrvManager )
	{
		pSrvManager->RegisterService( g_pScriptModule, /*bOwnerIn*/false );
	}

	// ** wichtig: erst script modul registrieren, da script-environment dieses benoetigt !
	g_pScriptEnvironment = new IGeneralScriptEnvironmentImpl( _DLL_NAME, pDll );
	// Service-Objekte beim Service-Manager registrieren
	if( pSrvManager )
	{
		pSrvManager->RegisterService( g_pScriptEnvironment, /*bOwnerIn*/false );
	}
#endif

	return 0;
}

extern "C" int MINDLLEXPORT minModuleExit( void * pData )
{
	//cout << "Exit DLL called ! data=" << pData << endl;

	// Service-Objekte beim Service-Manager abmelden (optional,
	// da beim Loeschen des Services automatisch abgemeldet wird)

//	return 1;	// Funktionen am Interpreter registriert, d.h. darf nicht freigegeben werden !

#ifdef _with_service
	minServiceManager * pSrvManager = (minServiceManager *)pData;
	if( pSrvManager )
	{
		pSrvManager->UnRegisterService( g_pScriptEnvironment );
		pSrvManager->UnRegisterService( g_pScriptModule );
	}

	// Service-Objekte wieder loeschen, dabei wird der Service automatisch
	// beim Service-Manager abgemeldet ! (umgekehrte Reihenfolge)
	delete g_pScriptEnvironment;
	g_pScriptEnvironment = 0;
	delete g_pScriptModule;
	g_pScriptModule = 0;

#endif

	return 0;
}


// *************************************************************************
// *************************************************************************
// *************************************************************************

IDiaPresentation * GetDiaPresentation()
{
#ifdef _with_service
	if( g_pScriptModule && (g_pScriptModule->GetScriptFcn().IsValid()) )
	{
		return g_pScriptModule->GetScriptFcn()->GetDiaPresentation();
	}
#endif
	return 0;
}



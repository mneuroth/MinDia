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

#include <algorithm>		// for: remove()

#include <string.h>         // for: strlen()

#include "dllbase.h"

// *************************************************************************

#if defined(__linux__) || defined(__APPLE__)

static char _GetDirectorySeparator()
{
#if defined(__linux__) || defined(__APPLE__)
    return '/';
#else
    return '\\';
#endif
}

// Hilfsfunktion fuer SplitPath()
static void _InsertChar( char * sBuffer, char ch )
{
    int nLen = strlen( sBuffer );
    sBuffer[ nLen ] = ch;
    sBuffer[ nLen+1 ] = 0;
}

// Hilfsfunktion fuer SplitPath()
// Transformation durchfuehren "abc" --> "cba"
static void _RotateString( char * sBuffer )
{
    char sBuf[512];
    int nLen = strlen( sBuffer );
    for( int i=0; i<nLen; i++ )
    {
        sBuf[ nLen-i-1 ] = sBuffer[i];
    }
    sBuf[ nLen ] = 0;
    strcpy( sBuffer, sBuf );
}

#endif

static bool SplitPath( const char * sPath, string & sDrive, string & sDir, string & sFileName, string & sExt )
{
	char sDriveBuf[512];
	char sDirBuf[512];
	char sNameBuf[512];
	char sExtBuf[512];
#if defined(__linux__) || defined(__APPLE__)
	// Simuliere _splitpath fuer Linux...
	strcpy( sDriveBuf, "" );
	strcpy( sDirBuf, "" );
	strcpy( sNameBuf, "" );
	strcpy( sExtBuf, "" );
	// Pfad von hinten analysieren
	char sBuffer[512];
	strcpy( sBuffer, "" );
	bool bFoundExt = false;
	bool bFoundFile = false;
	for( int i=strlen( sPath )-1; i>=0; i-- )
	{
		if( !bFoundExt && sPath[i]=='.' )
		{
			bFoundExt = true;
			_InsertChar( sBuffer, sPath[i] );
			_RotateString( sBuffer );
			strcpy( sExtBuf, sBuffer );
			strcpy( sBuffer, "" );
		}
		else if( !bFoundFile && sPath[i]==/*FileUtilityObj::*/_GetDirectorySeparator() )
		{
			bFoundFile = true;
			_RotateString( sBuffer );
			strcpy( sNameBuf, sBuffer );
			strcpy( sBuffer, "" );
			_InsertChar( sBuffer, sPath[i] );
		}
		else
		{
			_InsertChar( sBuffer, sPath[i] );
		}
	}
	_RotateString( sBuffer );
	if( bFoundFile )
	{
		strcpy( sDirBuf, sBuffer );
	}
	else
	{
		strcpy( sNameBuf, sBuffer );
	}
#else
	_splitpath( /*(CHAR_CAST)*/sPath, sDriveBuf, sDirBuf, sNameBuf, sExtBuf );
#endif
	sDrive = sDriveBuf;
	sDir = sDirBuf;
	sFileName = sNameBuf;
	sExt = sExtBuf;
	//cout << "SPLIT: " << sDirBuf << " *** " << sNameBuf << " *** " << sExtBuf << endl;
	return true;
}

// *************************************************************************

// +++ Plattform abhaengige Funktionen kapseln +++

#ifdef _WIN32
#include <windows.h>
typedef FARPROC minGenDllProcT;
#endif

#ifdef __IBMCPP__
#define INCL_DOSMODULEMGR
#include <os2.h>
#define _OS2
typedef PFN minGenDllProcT;
#endif

#ifdef __EMX__
#define INCL_DOSMODULEMGR
#include <os2.h>
#define _OS2
typedef PFN minGenDllProcT;
#endif

#if defined(__linux__) || defined(__APPLE__)
#define _LINUX
#include <dlfcn.h>
typedef void (*minGenDllProcT)();
#endif

inline long minLoadLibrary( const char * sDllName )
{
#ifdef _WIN32
	return (long)LoadLibrary( (WCHAR*)sDllName );
#endif
#ifdef _OS2
	// 3.2.2000: DLL-Name immer ohne .dll Extention angeben... (Modul-Name !!!)
	char sDriveBuf[12];
	char sDirBuf[512];
	char sNameBuf[512];
	char sExtBuf[512];
	_splitpath( (char *)sDllName, sDriveBuf, sDirBuf, sNameBuf, sExtBuf );
	HMODULE hModule = 0;
	APIRET rc = DosLoadModule( 0, 0, /*sDllName*/sNameBuf, &hModule );
	//cout << "Load Lib " << sDllName << " == " << sNameBuf << " nRet=" << rc << endl;
	return (long)hModule;
#endif
#ifdef _LINUX
	return (long)dlopen( sDllName, RTLD_LAZY );
#endif
}

inline bool minFreeLibrary( long hDllModule )
{
#ifdef _WIN32
	return FreeLibrary( (HMODULE)hDllModule );
#endif
#ifdef _OS2
	return DosFreeModule( (HMODULE)hDllModule )==0;
#endif
#ifdef _LINUX
	return dlclose( (void *)hDllModule );
#endif
}

inline minGenDllProcT minGetProcAddress( long hDllModule, const char * sProcName )
{
#ifdef _WIN32
	return GetProcAddress( (HMODULE)hDllModule, sProcName );
#endif
#ifdef _OS2
	minGenDllProcT pProc = 0;
	APIRET rc = DosQueryProcAddr( (HMODULE)hDllModule, 0L, sProcName, &pProc );
	return pProc;
#endif
#ifdef _LINUX
	return (minGenDllProcT)dlsym( (void *)hDllModule, sProcName );
#endif
}

inline minString GetDirectorySeparator()
{
#ifdef _WIN32
  return "\\";
#endif
#ifdef _OS2
  return "\\";
#endif
#ifdef _LINUX
  return "/";
#endif
}

// *************************************************************************

minDllManager::minDllManager( void * const pServiceManager )
	: m_pServiceManager( pServiceManager )
{
}

minDllManager::~minDllManager()
{
	// alle DLLs Unloaden...
	UnLoadAll();
}

bool minDllManager::LoadMinDll( const char * sFullDLLNameIn, minDll * & pDllHandleOut, bool bPushInContainer )
{
	long hModuleHandle = minLoadLibrary( sFullDLLNameIn );
	// Bem.: unter NT wird der gleich Handle geliefert, wenn die DLL schon geladen ist !!!
	// FRAGE: ist dies auch unter anderen Betriebssystemen der fall ?
	if( hModuleHandle )
	{
		// Laden der DLL war erfolgreich
		// gibt es nun eine als Funktion, die die Dll as minModul kennzeichnet ?
		minInitMinModuleProcT pInitFcn = (minInitMinModuleProcT)minGetProcAddress( hModuleHandle, _MINMODULE_INIT_PROC_NAME );
		if( pInitFcn )
		{
			// Modul-Objekt erzeugen:
			minDll * pDll = 0;
			pDll = new minDll( hModuleHandle );
			{
				minString sDrive, sDirectory, sFileName, sExt;
                SplitPath( sFullDLLNameIn, sDrive, sDirectory, sFileName, sExt );
				pDll->m_sModulName = sFileName;
				pDll->m_sDllName = sFileName+sExt;
				pDll->m_sDllPath = sDrive+sDirectory;
			}
			// es gibt eine entsprechende Funktion, diese nun aufrufen um Modul zu registrieren
			int nRet = (*pInitFcn)( m_pServiceManager, pDll );
			// Bem.: wenn das Modul nicht mehrfach geladen werden soll, so muss es beim
			//		 zweiten Aufruf der Init-Funktion einen Wert ungleich 0 liefern!
			if( nRet==0 )
			{
				// Initialisierung war erfolgreich, das Modul-Objekt auch merken (falls angefordert) und zuruckliefern !
				if( bPushInContainer )
				{
					m_aDllList.push_back( pDll );
				}
				pDllHandleOut = pDll;
				return true;
			}
			else
			{
				// Initialisierung war NICHT erfolgreich, Modul wieder loeschen
				delete pDll ;
				// und DLL-Handle wieder freigeben
				minFreeLibrary( hModuleHandle );
			}
		}
	}
	pDllHandleOut = 0;
	return false;
}

bool minDllManager::UnLoadMinDll( minDll * & pDllHandleIn, bool bRemoveFromContainer )
{
	if( !pDllHandleIn )
	{
		return false;
	}
	
	minExitMinModuleProcT pExitFcn = (minExitMinModuleProcT)minGetProcAddress( pDllHandleIn->m_hModuleHandle, _MINMODULE_EXIT_PROC_NAME );
	if( pExitFcn )
	{
		// es gibt eine entsprechende Funktion, diese nun aufrufen um Modul zu registrieren
		int nRet = (*pExitFcn)( m_pServiceManager );
		if( nRet == 0 )
		{
			if( minFreeLibrary( pDllHandleIn->m_hModuleHandle/*GetModuleHandle()*/ ) )
			{
				// nur dann aus dem Container entfernen, wenn ausdruecklich erwunscht !
				// Wird benoetigt, damit UnLoadAll() funktioniert, ansonsten wird der Eintrag
				// waehrend des iterierens ueber den Container geloescht !!!
				if( bRemoveFromContainer )
				{
					DllContainerT::iterator aRemoveIter = remove( m_aDllList.begin(), m_aDllList.end(), pDllHandleIn );
					// jetzt die Elemente auch wirklich aus dem vector entfernen !
					if( aRemoveIter != m_aDllList.end() )
					{
						m_aDllList.erase( aRemoveIter, m_aDllList.end() );
					}
				}
				// Handle loeschen und auf zurueck setzten !
				delete pDllHandleIn;
				pDllHandleIn = 0;
				return true;
			}
		}
	}

	return false;
}

bool minDllManager::UnLoadMinDll( const char * sFullDLLNameIn )
{
	minDll * pDllHandle = GetDllWithName( sFullDLLNameIn );
	if( pDllHandle )
	{
		return UnLoadMinDll( pDllHandle );
	}
	return false;
}

bool minDllManager::UnLoadAll()
{
	DllContainerT::iterator aIter = m_aDllList.begin();

	while( aIter != m_aDllList.end() )
	{
		minDll * pDll = *aIter;
		if( pDll )
		{
			UnLoadMinDll( pDll, /*bRemoveFromContainer*/false );	// Handle wird automatisch geloescht, aber selbst aus dem Container geloescht !
			// TODO: was soll passieren, wenn das Modul nicht geunloaded werden will ???
			//		 --> in diesem Fall gibt es ein Speicherleck, da gleich die Referenz
			//			 in dem Dll-Container geloescht wird !!!
		}
		++aIter;
	}
	// den gesamten Container loeschen !
	m_aDllList.erase( m_aDllList.begin(), m_aDllList.end() );
	return true;
}

bool minDllManager::SimpleLoadMinDll( const char * sFullDLLNameIn )
{
	minDllManager aTempDllManager( /*pServiceManager*/ 0 );

	minDll * pTempDllHandle = 0;
	if( !aTempDllManager.LoadMinDll( sFullDLLNameIn, pTempDllHandle, /*bPushInContainer*/false ) )	// werde Eigentuemer an zurueckgelieferten Handle !
	{
		//cerr << "Error loading min-dll " << sFullDllName << endl;
		return false;	// Fehler
	}
	// temporaeren Dll-Daten wieder zerstoeren, hier werden jedoch
	// nur die nicht mehr benoetigten Verwaltungsdaten geloescht,
	// die DLL bleibt jedoch im Speicher geladen!
	if( pTempDllHandle )
	{
		delete pTempDllHandle;
	}

	return true;		// ok
}

int minDllManager::GetNoOfLoadedDlls() const			
{ 
	return m_aDllList.size(); 
}

minDll * minDllManager::GetDllWithNo( int nNo ) const
{
	if( nNo>=0 && nNo<(int)m_aDllList.size() )		// Bereichsueberpruefung durchfuehren
	{
		return m_aDllList[ nNo ];
	}
	return 0;									// DLL nicht gefunden
}

minDll * minDllManager::GetDllWithName( const char * sFullDLLNameIn ) const
{
	minString sName( sFullDLLNameIn );
	for( int i=0; i<(int)m_aDllList.size(); i++ )
	{
		minString sTestName = m_aDllList[ i ]->GetModuleName();		// bis 6.12.1999: GetFullDllName();
		if( sTestName == sName )
		{
			return m_aDllList[ i ];
		}
	}
	return 0;
}

minString minDllManager::GetDirSeparator()
{
	return GetDirectorySeparator();
}

void minDllManager::Dump( ostream & aStream ) const
{
	int nMax = GetNoOfLoadedDlls();
	aStream << "List of loaded DLLs: " << nMax << endl;
	for( int i=0; i<nMax; i++ )
	{
		minDll * pDll = GetDllWithNo( i );
		if( pDll )
		{
			aStream << "" << (i+1) << " Module=" << pDll->GetModuleName().c_str() << " DLL=" << pDll->GetFullDllName().c_str() << endl;
		}
	}
	aStream << endl;
}

// *************************************************************************

#ifdef _with_main

//Linux: gcc -c -I../stl_sgi minbase.cpp
//Linux: gcc -o dllbase -I../stl_sgi -D_with_main dllbase.cpp minbase.o -ldl -lg++

#include "umstl_stream.h"

int main( int argc, char *argv[] )
{
	minDllManager aDllManager( 0 );

	minDll * pNewDll = 0;
	if( !aDllManager.LoadMinDll( /*"i:\\projects\\minkrnl\\templdll.dll"*/"./libtempldll.so.1.0", pNewDll ) )
		cout << "Error loading dll" << endl;
	else
		cout << "Successfully loaded dll" << endl;

	aDllManager.Dump( cout );

	return 0;
}

#endif


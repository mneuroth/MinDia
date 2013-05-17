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
/*
	Klassen zur Behandlung von DLLs.
	
	Start: 14.11.1998
	Stand: 21. 3.1999

	Arbeitsaufwand:	14.11.1998		2 Stunden (OS/2 und Windows)
					21. 3.1999		1 Stunde (Linux)

	(c) by Michael Neuroth, 1998, 1999

*/

#ifndef _DLLBASE_H
#define _DLLBASE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

// *************************************************************************

#ifdef _MSC_VER
#ifdef _USE_DLL
#define MINDLLEXPORT		__declspec( dllimport )
#define MINPREDLLEXPORT		__declspec( dllimport )
#else
#define MINDLLEXPORT		__declspec( dllexport )
#define MINPREDLLEXPORT		__declspec( dllexport )
#endif
#endif

#ifdef __BORLANDC__
#ifdef _USE_DLL
#define MINDLLEXPORT 
#define MINPREDLLEXPORT 
#else
#define MINDLLEXPORT		_Export
#define MINPREDLLEXPORT 
#endif
#endif

#ifdef __IBMCPP__
#ifdef _USE_DLL
#define MINDLLEXPORT 
#define MINPREDLLEXPORT 
#else
#define MINDLLEXPORT		_Export
#define MINPREDLLEXPORT 
#endif
#endif

#if defined(__linux__) || defined( __APPLE__ ) || defined(__MINGW32__)
#define MINDLLEXPORT 
#define MINPREDLLEXPORT 
#endif

// *************************************************************************

class minDll;

typedef string minString;

// Name der Init/Exit-Funktion fuer ein minModul
#define _MINMODULE_INIT_PROC_NAME	"minModuleInit"
#define _MINMODULE_EXIT_PROC_NAME	"minModuleExit"

// Typ der Init/Exit-Funktion fuer ein minModul
typedef int (*minInitMinModuleProcT)( void * pServiceManager, void * pDllModul );
typedef int (*minExitMinModuleProcT)( void * pServiceManager );

// BEMERKUNG:
// wird beim Aufruf der Init-Funktion ein Wert ungleich 0 zurueck geliefert,
// so wird das Modul NICHT geladen (d.h. ein Returnwert ungleich 0 zeigt einen
// Fehler an). So kann sich ein Modul auch gegen mehrfaches laden schuetzten!

// *************************************************************************
/** Klasse zur Verwaltung von zur Laufzeit hinzugefuegter Dienste in DLLs. */
// ACHTUNG: diese Klasse ist nicht multithread sicher !
class MINDLLEXPORT minDllManager
{
	typedef vector<minDll *>	DllContainerT;

public:
	minDllManager( void * const pServiceManager );
	~minDllManager();

	// Methoden zum Laden/Unladen von min-DLLs
	bool		LoadMinDll( const char * sFullDLLNameIn, minDll * & pDllHandleOut, bool bPushInContainer = true );	// ACHTUNG: der Aufrufer wir nur dann Eigentuemer des Zeigers auf die Dll, wenn bPushInContainer == false !
	bool		UnLoadMinDll( minDll * & pDllHandleIn, bool bRemoveFromContainer = true );							// ACHTUNG: uebergebener DllHandle wird geloescht und auf 0 gesetzt !
	bool		UnLoadMinDll( const char * sFullDLLNameIn );
	bool		UnLoadAll();

	int			GetNoOfLoadedDlls() const;
	minDll *	GetDllWithNo( int nNo ) const;
	minDll *	GetDllWithName( const char * sFullDLLNameIn ) const;

	// *** globale Methoden ***
	// einfaches Laden einer min-Dll (d.h. Dll mit _MINMODULE_INIT_PROC_NAME Aufruf, falls vorhanden),
	// die geladene DLL wird NICHT weiter verwaltet, es wird kein Handle angelegt, 
	// beim Aufruf an _MINMODULE_INIT_PROC_NAME wird 0 uebergeben !
	// Gedacht ist dies Methode zum platformunabhaengigen Laden einer DLL mit automatischem Aufruf
	// von _MINMODULE_INIT_PROC_NAME Funktionen in der Dll, falls diese vorhanden sind.
	// Diese Methode wird von use (ungerer script environment) verwendet
	static bool SimpleLoadMinDll( const char * sFullDLLNameIn );

	// platformabhaengiger Separator fuer Verzeichnisse liefern
	static minString GetDirSeparator();

	// zum Debuggen
	void		Dump( ostream & aStream ) const;

private:
	DllContainerT			m_aDllList;			// Liste aller geladenen DLLs
	void * const			m_pServiceManager;	// Referenz auf den Service-Manager (NICHT Eigentuemer !)
};

// *************************************************************************
/** Hilfs-Klasse zur Repraesentation einer zur Laufzeit geladener DLL. */
class minDll
{
	friend class minDllManager;

public:
	minDll( long hModuleHandle ) : m_hModuleHandle( hModuleHandle ) {}

	minString	GetModuleName() const			{ return m_sModulName; }
 
	minString	GetDllName() const				{ return m_sDllName; }
	minString	GetFullDllName() const			{ return m_sDllPath/*+minDllManager::GetDirSeparator()*/+m_sDllName; }

private:
	// Hilfsmethoden
	long		GetModuleHandle()				{ return m_hModuleHandle; }

	// +++ Daten +++
	long		m_hModuleHandle;	// der Betriebssystem-Handle auf die DLL
	minString	m_sModulName;		// der Modul-Name
	minString	m_sDllName;			// der Name der Dll, z.B. "service.dll"
	minString	m_sDllPath;			// der volle Pfad der Dll, z.B. "c:\winnt\min\" 
};


#endif

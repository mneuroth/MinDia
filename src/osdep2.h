/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/osdep2.h,v $
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

/**************************************************************************
*
*	OSDEP2 --- Hilfs-Klassen und Funktionen zur Plattformunabhängigen 
*			   Behandlung von Threads
*
*				minBeginThread() 
*				minGetThreadId()
*				minEnterCriticalSection() 		// nicht alles implementiert
*				minLeaveCriticalSection()		// s.o.
*				minSetThreadPriority()			// s.o.
*				minGetThreadPriority()			// gar nicht implementiert
*				minSleep()
*				minBeep()
*				minGetCurrentTickCount()
*
*				minSyncException
*				minSyncObject
*				minMultiSyncObject				// auf max. 4 SyncObjects gleichzeitig warten	
*				minSignalObject
*
*   ACHTUNG: diese Datei sollte NUR inline Funktionen/Methoden enthalten !
*
**************************************************************************/

/*
	folgende defines sind bekannt und unterscheiden die Plattformen

		_WIN32			Windows NT
		__IBMCPP__		OS/2
		__EMX__			OS/2
		__linux__		Linux		// teilweise noch nicht vollstaendig implementiert
*/

#ifndef _OSDEP2_H
#define _OSDEP2_H

//#include "umnew.h"
#define UM_NEW(p,x)			p = new x;
#define UM_DELETE(x)		delete x;
#define UM_DELETE_ARR(x)	delete [] x;
//#include "dllexport.h"
#define UMDLLEXPORT

//************************** OS/2 **********************************
// +++ fuer IBM Visual Age C++ 3.x +++
#ifdef __IBMCPP__

#include <process.h>
#define INCL_DOSMISC
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_DOSMODULEMGR
#define INCL_ERRORS
#define INCL_DOSSESMGR
#include <os2.h>

#define _OS2

#define _PRIO_CLASS_BATCH			PRTYC_IDLETIME
#define _PRIO_CLASS_NORMAL			PRTYC_REGULAR
#define _PRIO_CLASS_HIGH			PRTYC_TIMECRITICAL
#define _PRIO_CLASS_TIMECRITICAL	PRTYC_FOREGROUNDSERVER

#define  _INFINITE	 				SEM_INDEFINITE_WAIT
#define _WAIT_FAILED				1
#define _WAIT_SIGNALED				NO_ERROR
#define _WAIT_TIMEOUT				ERROR_TIMEOUT
#define _WAIT_ABANDONED				ERROR_INTERRUPT

#define _CALLING_CONV				_Optlink

#define _DEFAULT_STACK_SIZE			32768

// +++ C-Set 3.0 unterstuetzt noch kein dynamic_cast<>() !
#define DYNAMIC_CAST(ClassPtr,pObj) (ClassPtr)pObj
#define STATIC_CAST(ClassPtr,pObj)	(ClassPtr)pObj

// verwende die UVBASE.DLL
#define _with_uvbase_dll

#endif

//------------------------------------------------------------------
// +++ fuer EMX/GCC 2.7.x +++
#ifdef __EMX__

#include <process.h>
#include <stdlib.h>			// fuer _beginthread()
#define INCL_DOSMISC
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_ERRORS
#include <os2.h>

#define _OS2

#define _PRIO_CLASS_BATCH			PRTYC_IDLETIME			
#define _PRIO_CLASS_NORMAL			PRTYC_REGULAR
#define _PRIO_CLASS_HIGH			PRTYC_TIMECRITICAL
#define _PRIO_CLASS_TIMECRITICAL	PRTYC_FOREGROUNDSERVER	

#define  _INFINITE	 				SEM_INDEFINITE_WAIT
#define _WAIT_FAILED				1
#define _WAIT_SIGNALED				NO_ERROR
#define _WAIT_TIMEOUT				ERROR_TIMEOUT
#define _WAIT_ABANDONED				ERROR_INTERRUPT

#define _CALLING_CONV 

#define _DEFAULT_STACK_SIZE			32768

#define DYNAMIC_CAST(ClassPtr,pObj) (ClassPtr)pObj
#define STATIC_CAST(ClassPtr,pObj)  (ClassPtr)pObj

//#define DYNAMIC_CAST(ClassPtr,pObj)		dynamic_cast<ClassPtr>( pObj )
//#define STATIC_CAST(ClassPtr,pObj)		static_cast<ClassPtr>( pObj )

// verwende NICHT die UVBASE.DLL
#undef _with_uvbase_dll

#endif

//************************** WNT ***********************************
// +++ fuer MS Visual C++ 5.0 and Borland C++ 5.5 +++
#ifdef _WIN32

#include <process.h>
#include <windows.h>

#define _PRIO_CLASS_BATCH			IDLE_PRIORITY_CLASS
#define _PRIO_CLASS_NORMAL			NORMAL_PRIORITY_CLASS
#define _PRIO_CLASS_HIGH			HIGH_PRIORITY_CLASS
#define _PRIO_CLASS_TIMECRITICAL	REALTIME_PRIORITY_CLASS

#define  _INFINITE	 				INFINITE		// Timeout fuer minSyncObject-Objekt
#define _WAIT_FAILED				WAIT_FAILED
#define _WAIT_SIGNALED				WAIT_OBJECT_0
#define _WAIT_TIMEOUT				WAIT_TIMEOUT
#define _WAIT_ABANDONED				WAIT_ABANDONED 

#define _CALLING_CONV				//__cdecl

#define _DEFAULT_STACK_SIZE			0

#define DYNAMIC_CAST(ClassPtr,pObj)	dynamic_cast<ClassPtr>( pObj )
#define STATIC_CAST(ClassPtr,pObj)	static_cast<ClassPtr>( pObj )

// verwende die UVBASE.DLL (seit 3.5.2000 !)
#define _with_uvbase_dll

#ifdef _MSC_VER
// einige Compiler-Warnings ausschalten:
#pragma warning( disable : 4786 )		// Fehler-Info auf 255 Zeichen verkuerzt
#pragma warning( disable : 4355 )		// warning C4355: this' : wird in Initialisierungslisten fuer Basisklasse verwendet
#pragma warning( disable : 4503 )		//
#pragma warning( disable : 4005 )		// warning C4005: 'TEXT' : Makro-Neudefinition				--> in uvbase/defs/portab.h
#pragma warning( disable : 4227 )		// warning C4227: Anachronismus verwendet : auf Verweise werden ignoriert Qualifizierer	--> in uvbase/defs/uvdata.hpp
#endif

#endif


//************************** WNT ***********************************
// +++ fuer Linux gcc +++
#ifdef __linux__

//#define _GCC_BUG			// nur fuer alten GCC 2.7.x notwendig (z.B. in SuSE 5.x)

#define _USE_PTHREAD		// z.Z. funktionieren die Threads noch nicht mit X11
//#undef _USE_PTHREAD

#ifndef _USE_PTHREAD
#define _NO_THREADS			// ggf. muss ohne Thread gearbeitet werden !
#endif

#include <sys/time.h>		// fuer: clock()
#include <sys/wait.h>		// fuer: wait()
#include <unistd.h>			// fuer: usleep(), execvp(), vfork()
#ifdef _USE_PTHREAD
#include <pthread.h>		// fuer: thread's
#endif

#define _UNIX

//TESTS/DEBUGGING
//#include <iostream.h>

#define _PRIO_CLASS_BATCH			0
#define _PRIO_CLASS_NORMAL			1
#define _PRIO_CLASS_HIGH			2
#define _PRIO_CLASS_TIMECRITICAL	3

#define  _INFINITE	 				0
#define _WAIT_FAILED				1
#define _WAIT_SIGNALED				0
#ifdef _USE_PTHREAD
#define _WAIT_TIMEOUT				ETIMEDOUT
#define _WAIT_ABANDONED				EINTR
#else
#define _WAIT_TIMEOUT				0
#define _WAIT_ABANDONED				1
#endif

#define _CALLING_CONV

#define _DEFAULT_STACK_SIZE			32768

#define DYNAMIC_CAST(ClassPtr,pObj) (ClassPtr)pObj
#define STATIC_CAST(ClassPtr,pObj)  (ClassPtr)pObj

//#define DYNAMIC_CAST(ClassPtr,pObj)		dynamic_cast<ClassPtr>( pObj )
//#define STATIC_CAST(ClassPtr,pObj)		static_cast<ClassPtr>( pObj )

// verwende NICHT die UVBASE.DLL
#undef _with_uvbase_dll

#endif

//******************************************************************
//******************************************************************

#ifdef _WIN32
#define _SPAWN_WAIT			_P_WAIT
#define _SPAWN_NOWAIT		_P_NOWAIT
#endif
#ifdef _OS2
#define _SPAWN_WAIT			P_WAIT
#define _SPAWN_NOWAIT		P_NOWAIT
#endif
#ifdef __linux__
#define _SPAWN_WAIT			0 	// P_WAIT
#define _SPAWN_NOWAIT		1	// P_NOWAIT
#endif

//******************************************************************
/** Funktion zum Starten von externen Programmen (neu seit 2.2.2000) */
inline int minSpawn( int nModus, const char * sCmdName, const char * const * sArgv )
{
#ifdef _WIN32
#ifdef __BORLANDC__
	return spawnvp( nModus, sCmdName, (char * const *)sArgv );
#else
	return spawnvp( nModus, sCmdName, sArgv );
#endif
#endif
#ifdef _OS2
	return _spawnvp( nModus, (char *)sCmdName, (char **)sArgv );
#endif
#ifdef __linux__
	// 4.11.2000 aus mcp-Projekt
	int nSpawnNo,iStatus,cstatus;

	if( (iStatus=vfork())!=0 )
	{
		if( iStatus<0 )
			return iStatus;
		else
		{
			if( nModus==_SPAWN_WAIT )
			{
				iStatus = wait( &cstatus );
				return iStatus;
			}
		}
	}
	else
	{
		nSpawnNo = ::execvp( sCmdName, (char * const *)sArgv );
		return nSpawnNo;
	}
	return -1;
#endif
}

//******************************************************************

typedef void( * _CALLING_CONV minThreadStartAddress ) ( void * );
typedef void *( * _CALLING_CONV minThreadStartAddress2 ) ( void * );

//******************************************************************
/** Funktion zum Starten eine Threads */
inline unsigned long minBeginThread( minThreadStartAddress start_address, unsigned stack_size, void *arglist )
{
#ifdef _WIN32
	return _beginthread( start_address, stack_size, arglist );
#endif
#ifdef _OS2
	return _beginthread( start_address, /*Pointer-To-Stack*/0, stack_size, arglist );
#endif
#ifdef __linux__
#ifdef _USE_PTHREAD
	pthread_t aThread;
	/*int nRet =*/ pthread_create( &aThread, NULL, (minThreadStartAddress2)start_address, arglist );
	//return (unsigned long)&aThread;
	return 0;
#endif
#else
	return 0;
#endif
}

//******************************************************************
/** Funktion zur Bestimmung der ID des gerade aktuellen Threads. */
inline unsigned long minGetThreadId()
{
#ifdef _WIN32
	return (unsigned long)::GetCurrentThreadId();
#endif
#ifdef _OS2
	PTIB pTIB = 0;
 	PPIB pPIB = 0;
	::DosGetInfoBlocks( &pTIB, &pPIB );
 	return pTIB->tib_ptib2->tib2_ultid;
 	//return pTIB->tib_ordinal;
#endif
#ifdef __linux__
	return pthread_self();
#endif
}

//******************************************************************
/** Funktion zum Lahmlegen des Multitaskings in dem laufenden Prozess,
  * d.h. das Multithreading fuer diesen Prozess wird ausgesetzt. */
inline void * minEnterCriticalSection()	
{
#ifdef _WIN32
	LPCRITICAL_SECTION lpCriticalSection; UM_NEW( lpCriticalSection, CRITICAL_SECTION );
	::InitializeCriticalSection( lpCriticalSection );
	::EnterCriticalSection( lpCriticalSection );
	return (void *)lpCriticalSection;
#endif
#ifdef _OS2
	::DosEnterCritSec();
	return 0;				// kein Handle notwendig !
#endif
#ifdef __linux__
	return 0;				// TODO
#endif
}

//******************************************************************
/** Funktion zur Wiederaufnahme des Multitaskings in dem laufenden Prozess,
  * d.h. das Multithreading fuer diesen Prozess wird eingeschaltet. */
inline void minLeaveCriticalSection( void * hHandle )
{
#ifdef _WIN32
	LPCRITICAL_SECTION lpCriticalSection = (LPCRITICAL_SECTION)hHandle;
	::LeaveCriticalSection( lpCriticalSection );
	::DeleteCriticalSection( lpCriticalSection );
	UM_DELETE( lpCriticalSection );		// jetzt auch den C++ Speicher wieder frei geben !!! (17.7.2000)
#endif
#ifdef _OS2
	::DosExitCritSec();
#endif
#ifdef __linux__
							// TODO
#endif
}

//******************************************************************
/** Funktion zum Seten der Prioritaet eines Threads, return true falls ok. */
inline int minSetThreadPriority( unsigned long nThreadId, long nClass, long nLevel = 0 )
{
#ifdef _WIN32
	return ::SetPriorityClass( (HANDLE)nThreadId, nClass );
	//return ::SetThreadPriority( (HANDLE)nThreadId, nLevel );
#endif
#ifdef _OS2
	return DosSetPriority( PRTYS_THREAD, nClass, nLevel, nThreadId );				// TODO
#endif
#ifdef __linux__
	return 0;				// TODO
#endif
}
 
//******************************************************************
/** Funktion zum Seten der Prioritaet eines Threads */
inline int minGetThreadPriority( unsigned long nThreadId, long & nClass, long & nLevel )	
{
#ifdef _WIN32
	return 0;				// TODO
#endif
#ifdef _OS2
	return 0;				// TODO
#endif
#ifdef __linux__
	return 0;				// TODO
#endif
}
 
//******************************************************************
/** Funktion zum Schlafenlegen eines Threads, Angaben in ms */
inline void minSleep( unsigned long nDelay )
{
#ifdef _WIN32
	::Sleep( nDelay );
#endif
#ifdef _OS2
	::DosSleep( nDelay );
#endif
#ifdef __linux__
	::usleep( nDelay*1000 );	// delay in micro seconds
#endif
}
 
//******************************************************************
/** Funktion zur Erzeugung eines Signal-Tons */
inline void minBeep( unsigned long nFrequency, unsigned long nDurationInMS )	
{
#ifdef _WIN32
							// TODO
#endif
#ifdef _OS2
	::DosBeep( nFrequency, nDurationInMS );
#endif
#ifdef __linux__
							// TODO
#endif
}
 
//******************************************************************
/** Funktion zur Bestimmug einer Zeitmarke */
inline unsigned long minGetCurrentTickCount()
{
#ifdef _WIN32
	return ::GetTickCount();
#endif
#ifdef _OS2
	ULONG nRet;
	::DosQuerySysInfo( QSV_MS_COUNT, QSV_MS_COUNT, &nRet, sizeof(nRet) );
	return nRet;
#endif
#ifdef __linux__
	//static time_t g_nStartTime = time( 0 );
	//return g_nStartTime+clock();
	return (unsigned long)clock();	// siehe auch CLK_TCK und CLOCKS_PER_SEC	
#endif
}

//******************************************************************
/** Klasse zum Raepraesentation eines MT-Fehlers. */
class UMDLLEXPORT minSyncException
{
public:
	minSyncException( const char * sMsg ) 
	{
		// Bemerkung: eigene String-Behandlung, damit keine Runtime-Library 
		//			  fuer diese Include-Datei notwendig wird !!!
		int nCount = 0;
		char * sTemp = (char *)sMsg;
		// Stringlaenge bestimmen
		while( sTemp ) 
			nCount++;
		// Stringbuffer erzeugen
		UM_NEW( m_sMsg, char[ nCount+1 ] );
		// String kopieren
		for( int i=0; i<=nCount; i++ )		// kopiere auch terminierende 0 !
		{
			m_sMsg[i] = sMsg[i];
		}
	}
	~minSyncException()
	{
		UM_DELETE_ARR( /*[]*/ m_sMsg );	
	}

	const char *	GetMessage() const	{ return m_sMsg; }

private:
	char *			m_sMsg;
};

// statische Variable zum Initialisieren eines Rekursiven Mutex
#ifdef __linux__
#ifdef _USE_PTHREAD
extern pthread_mutex_t g_aRecursiveMutex;
#endif
#endif

//******************************************************************
/** Klasse zum synchronisieren von Resourcen zwischen verschiedenen 
  * Threads (nur Prozess intern!). 
  * Verhaelt sich wie ein rekursiver MUTEX! */
//#include <iostream.h>
class UMDLLEXPORT minSyncObject
{
	friend class minMultiSyncObject;

public:
	// Konstruktor, damit auch ein Zeiger auf das angelegte Objekt verwaltet wird,
	// gedacht zur Verwendung von globalen Objekten
	minSyncObject() 
	{
#ifdef _WIN32
		m_aCritSec = ::CreateMutex( NULL, /*initOwner*/FALSE, /*Name*/NULL );
#endif
#ifdef _OS2
		//cout << "CreateMutex " << (void *)&m_aCritSec << endl;
  		// falls ein Name angegeben ist, muss dieser mit \SEM32\ beginnen
		m_aCritSec = 0;		// Initialisierung neu seit 18.11.1999
		/*APIRET rc =*/ ::DosCreateMutexSem( NULL, &m_aCritSec, DC_SEM_SHARED, FALSE );
  		//cout << "ret = " << rc << endl;
		//cout << "CreateMutex " << (void *)&m_aCritSec << "DONE" << endl;
#endif
#ifdef __linux__
#ifdef _USE_PTHREAD
		// statische Inititalisierung...
		//m_aCritSec = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
		//m_aCritSec = PTHREAD_MUTEX_INITIALIZER;
		m_aCritSec = g_aRecursiveMutex;
#ifdef _with_setkind_ok		
		// dynamische Initialisierung:
		pthread_mutexattr_t aMutexAttr = g_aRecursiveMutexAttr;
		pthread_mutexattr_init( &aMutexAttr );
		pthread_mutexattr_setkind_np( &aMutexAttr, PTHREAD_MUTEX_RECURSIVE_NP ); // PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_FAST_NP
		int nRet = pthread_mutex_init( &m_aCritSec, &aMutexAttr );
		pthread_mutexattr_destroy( &aMutexAttr );
#endif		
//		cout << (void *) this << "mutex_init " << nRet << endl;
#endif
#endif
		// bei einem Fehler eine Exception werfen Fehlerbehandlung !
		//if( m_aCritSec == 0 )
  		//	throw new minSyncException( "MultiSyncObject" );
	}
 	// 18.11.1999, benannter Semaphor eingefuehrt !
	minSyncObject( const char * sSemName )
	{
#ifdef _WIN32
		m_aCritSec = ::CreateMutex( NULL, /*initOwner*/FALSE, (LPCTSTR)sSemName );
		//cout << "CreateMutex " << (void *)&m_aCritSec << endl;
#endif
#ifdef _OS2
		//cout << "CreateMutex " << (void *)&m_aCritSec << endl;
  		// falls ein Name angegeben ist, muss dieser mit \SEM32\ beginnen
    	APIRET rc;

		m_aCritSec = 0;
    	if( sSemName )
     	{
			rc = ::DosOpenMutexSem( sSemName, &m_aCritSec );
  			//cout << sSemName << "ret = " << rc << endl;
   			// falls der benannte Semaphor noch nict existiert: anlegen
   			if( rc == ERROR_SEM_NOT_FOUND )
				rc = ::DosCreateMutexSem( sSemName, &m_aCritSec, DC_SEM_SHARED, FALSE );
		}
     	else
			rc = ::DosCreateMutexSem( sSemName, &m_aCritSec, DC_SEM_SHARED, FALSE );
  		//cout << "ret = " << rc << endl;
		//cout << "CreateMutex " << (void *)&m_aCritSec << "DONE" << endl;
#endif
#ifdef __linux__
//#error benannte Semaphore sind noch nicht implementiert !
#ifdef _USE_PTHREAD
		// statische Inititalisierung...
		//m_aCritSec = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
		//m_aCritSec = PTHREAD_MUTEX_INITIALIZER;
		m_aCritSec = g_aRecursiveMutex;
#ifdef _with_setkind_ok		
		// dynamische Initialisierung:
		pthread_mutexattr_t aMutexAttr = g_aRecursiveMutexAttr;
		pthread_mutexattr_init( &aMutexAttr );
		pthread_mutexattr_setkind_np( &aMutexAttr, PTHREAD_MUTEX_RECURSIVE_NP ); // PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_FAST_NP
		int nRet = pthread_mutex_init( &m_aCritSec, &aMutexAttr );
		pthread_mutexattr_destroy( &aMutexAttr );
#endif		
//		cout << (void *) this << "mutex_init " << nRet << endl;
#endif
#endif
		// bei einem Fehler eine Exception werfen Fehlerbehandlung !
		//if( m_aCritSec == 0 )
  		//	throw new minSyncException( "MultiSyncObject" );
	}
	~minSyncObject()
	{
#ifdef _WIN32
		::CloseHandle( m_aCritSec );  
#endif
#ifdef _OS2
  		/*APIRET rc =*/ ::DosCloseMutexSem( m_aCritSec );
#endif
#ifdef __linux__
#ifdef _USE_PTHREAD
		/*int nRet =*/ pthread_mutex_destroy( &m_aCritSec );
//		cout << (void *) this << " mutex_destroy " << nRet << endl;
#endif
#endif
	}

	// ist der Mutex von dem aufrufenden Thread gelocked?
	int IsLocked()
	{
#ifdef _WIN32
							// TODO
#endif
#ifdef _OS2
    	TID tid;
     	PID pid;
      	ULONG ulCount;
    	if( DosQueryMutexSem( m_aCritSec, &pid, &tid, &ulCount ) == NO_ERROR )
     		return ((unsigned long)tid)==minGetThreadId();
#endif
#ifdef __linux__
							// TODO
#endif
     	return 0;
    }

	void Lock()
	{
#ifdef _WIN32
		::WaitForSingleObject( m_aCritSec, _INFINITE );
#endif
#ifdef _OS2
 		::DosRequestMutexSem( m_aCritSec, _INFINITE );
#endif
#ifdef __linux__
#ifdef _USE_PTHREAD
		/*int nRet =*/ pthread_mutex_lock( &m_aCritSec );
//		cout << (void *) this << "mutex_lock " << nRet << endl;
#endif
#endif
	}

	void Unlock()
	{
#ifdef _WIN32
		::ReleaseMutex( m_aCritSec );
#endif
#ifdef _OS2
		::DosReleaseMutexSem( m_aCritSec );
#endif
#ifdef __linux__
#ifdef _USE_PTHREAD
		/*int nRet =*/ pthread_mutex_unlock( &m_aCritSec );
//		cout << (void *) this << "mutex_unlock " << nRet << endl;
#endif
#endif
	}

private:
	// +++ Daten +++
#ifdef _WIN32
	HANDLE				m_aCritSec;
#endif
#ifdef _OS2
	HMTX				m_aCritSec;
#endif
#ifdef __linux__
#ifdef _USE_PTHREAD
	pthread_mutex_t		m_aCritSec;
#endif
#endif
};

//******************************************************************
// Warten auf mehrere minSyncObject's gleichzeitig
//******************************************************************
/** Klasse zum synchronisieren von Resourcen zwischen verschiedenen 
  * Threads (nur Prozess intern!). 
  * Verhaelt sich wie ein rekursiver MUTEX! */
#define _MAX_MULTI_SYNC_COUNT 4
class UMDLLEXPORT minMultiSyncObject
{
#ifdef _WIN32
	typedef HANDLE	MyHandleT;
#endif
#ifdef _OS2
	typedef HMUX	MyHandleT;
#endif

public:
	minMultiSyncObject( minSyncObject * pSync1, minSyncObject * pSync2, minSyncObject * pSync3 = 0, minSyncObject * pSync4 = 0 )
	{
		m_aSyncArray[0] = pSync1;
		m_aSyncArray[1] = pSync2;
		m_aSyncArray[2] = pSync3;
		m_aSyncArray[3] = pSync4;
#ifdef _OS2
		long nSyncCount = 0;
		SEMRECORD aMutexArray[_MAX_MULTI_SYNC_COUNT];
		for( int i=0; i<_MAX_MULTI_SYNC_COUNT; i++ )
		{
			if( m_aSyncArray[i] )
			{
				aMutexArray[nSyncCount].hsemCur = (HSEM)m_aSyncArray[i]->m_aCritSec;
				aMutexArray[nSyncCount].ulUser = 0;
      			nSyncCount++;
			}
		}
		/*APIRET rc =*/ ::DosCreateMuxWaitSem( NULL, &m_hMultiMutex, (ULONG)nSyncCount, aMutexArray, DCMW_WAIT_ALL );
		//cout << "OS2 CreateMultMutex " << rc << " " << (void*)m_hMultiMutex << " this=" << (void *)this << endl;
#endif
	}
	~minMultiSyncObject()
	{
#ifdef _OS2
		//cout << "OS2 CloseMultMutex now " << (void *)m_hMultiMutex << " this=" << (void *)this << endl;
		::DosCloseMuxWaitSem( m_hMultiMutex );
#endif
	}

	void Lock()
	{
#ifdef _WIN32
		long nSyncCount = 0;
		MyHandleT aMutexArray[_MAX_MULTI_SYNC_COUNT];
		for( int i=0; i<_MAX_MULTI_SYNC_COUNT; i++ )
		{
			if( m_aSyncArray[i] )
				aMutexArray[nSyncCount++] = m_aSyncArray[i]->m_aCritSec;
		}
		::WaitForMultipleObjects( (DWORD)nSyncCount, aMutexArray,/*waitAll*/TRUE, _INFINITE );
#endif
#ifdef _OS2
		ULONG ulData;
		/*APIRET rc =*/ ::DosWaitMuxWaitSem( m_hMultiMutex, SEM_INDEFINITE_WAIT, &ulData );
		//cout << "OS2 WaitMultMutex " << rc << " " << (void *)m_hMultiMutex << " this=" << (void *)this << endl;
#endif
#ifdef __linux__
		// TODO
#endif
/*
		// zum Debuggen
		for( int i=0; i<_MAX_MULTI_SYNC_COUNT; i++ )
		{
			if( m_aSyncArray[i] )
    			cout << "MUTEX LOCK " << i << " " << m_aSyncArray[i]->IsLocked() << " ThreadId=" << minGetThreadId() << endl;
		}
*/
	}
	void Unlock()
	{
		for( int i=0; i<_MAX_MULTI_SYNC_COUNT; i++ )
		{
			if( m_aSyncArray[i] )
				m_aSyncArray[i]->Unlock();
		}
	}

private:
	minSyncObject *		m_aSyncArray[_MAX_MULTI_SYNC_COUNT];
#ifdef _OS2
	HMUX 				m_hMultiMutex;
#endif
};

#ifdef _not_used
//******************************************************************
/** Klasse zum Austausch von Signalen zwischen verschiedenen Threads. */
// BEMERKUNG: unter Linux noch nicht getestet !!!
class UMDLLEXPORT minSignalObject
{
public:
	minSignalObject( unsigned long nTimeout = _INFINITE/*g_nTimeoutInms*/ )
	{
		m_nTimeout = nTimeout;
		m_bSignaled = 0/*false*/;
		m_bWaiting = 0/*false*/;
#ifdef _WIN32
		m_aHandle = ::CreateEvent( NULL, /*manualReset=*/TRUE, /*initState=*/FALSE, NULL );
#endif
#ifdef _OS2
		/*APIRET rc =*/ ::DosCreateEventSem( NULL, &m_aHandle, DC_SEM_SHARED, FALSE );
#endif
#ifdef __linux__
#ifdef _USE_PTHREAD
		pthread_mutex_init( &m_aMutex, NULL );	// Signal-Objekt benoetigt immer auch einen Mutex !
		//m_aHandle = PTHREAD_COND_INITIALIZER;
		pthread_cond_init( &m_aHandle, NULL );
#endif
#endif
	}
	~minSignalObject()
	{
#ifdef _WIN32
		::CloseHandle( m_aHandle );
#endif
#ifdef _OS2
		::DosCloseEventSem( m_aHandle );
#endif
#ifdef __linux__
#ifdef _USE_PTHREAD
		int nRet = pthread_cond_destroy( &m_aHandle );
		// moeglich: nRet == EBUSY
		pthread_mutex_destroy( &m_aMutex );
#endif
#endif
	}

	/*bool*/int IsWaiting() const
	{
		return m_bWaiting;
	}
	void Signal()
	{
		m_bSignaled = 1/*true*/;
#ifdef _WIN32
		::SetEvent( m_aHandle );
#endif
#ifdef _OS2
  		::DosPostEventSem( m_aHandle );
#endif
#ifdef __linux__
#ifdef _USE_PTHREAD
		pthread_mutex_lock( &m_aMutex );
		pthread_cond_signal( &m_aHandle );
		//pthread_cond_broadcast( &m_aHandle );		// um alle wartenden Threads aufzuwecken !
		pthread_mutex_unlock( &m_aMutex );
#endif
#endif
	}
	void Cancel()
	{
		m_bSignaled = 0/*false*/;
#ifdef _WIN32
		::SetEvent( m_aHandle );
#endif
#ifdef _OS2
  		::DosPostEventSem( m_aHandle );
#endif
#ifdef __linux__
#ifdef _USE_PTHREAD
		pthread_mutex_lock( &m_aMutex );
		pthread_cond_signal( &m_aHandle );
		//pthread_cond_broadcast( &m_aHandle );		// um alle wartenden Threads aufzuwecken !
		pthread_mutex_unlock( &m_aMutex );
#endif
#endif
	}
	// warten bis ein (anderer) Thread auf dieses Ereignis-Objekt wartet !
	int WaitForWait( unsigned long nTimeout = _INFINITE )
	{
		unsigned long nCounter = 0;

		while( !IsWaiting() && (nTimeout==_INFINITE || nCounter<nTimeout) )
		{
			minSleep( 4 );
			nCounter += 4;
		}

		return IsWaiting();
	}
	// siehe _WAIT_*-Defines fuer die moeglichen Rueckgabewerte
	int WaitForSignal()
	{
		m_bWaiting = 1/*true*/;
#ifdef _WIN32
		DWORD nRet = ::WaitForSingleObject( m_aHandle,(DWORD)m_nTimeout );
		if( nRet == _WAIT_SIGNALED )
		{
			::ResetEvent( m_aHandle );
			m_bWaiting = 0/*false*/;
		}
		// falls doch unterbrochen wurde:
		if( !m_bSignaled && nRet == _WAIT_SIGNALED )
			nRet = _WAIT_ABANDONED;
		return (int)nRet;
#endif
#ifdef _OS2
  		APIRET rc = ::DosWaitEventSem(  m_aHandle,(ULONG)m_nTimeout );
		if( rc == _WAIT_SIGNALED )
		{
			ULONG nCount;
			::DosResetEventSem( m_aHandle, &nCount );
			m_bWaiting = 0/*false*/;
		}
		// falls doch unterbrochen wurde:
		if( !m_bSignaled && rc == _WAIT_SIGNALED )
			rc = _WAIT_ABANDONED;
		return (int)rc;
#endif
#ifdef __linux__
#ifdef _USE_PTHREAD
		int nRet;

		pthread_mutex_lock( &m_aMutex );
		if( m_nTimeout!=_INFINITE )
		{
			struct timeval now;
			struct timespec aTimeout;
			struct timezone aTimezone;

			gettimeofday( &now, &aTimezone );
			aTimeout.tv_sec = now.tv_sec + m_nTimeout/1000;
			aTimeout.tv_nsec = now.tv_usec * 1000 + (m_nTimeout%1000)*1000000;
			nRet = pthread_cond_timedwait( &m_aHandle, &m_aMutex, &aTimeout );
		}
		else
			nRet = pthread_cond_wait( &m_aHandle, &m_aMutex );
		if( nRet == _WAIT_SIGNALED )
		{
			m_bWaiting = 0/*false*/;
		}
		// falls doch unterbrochen wurde:
		if( !m_bSignaled && nRet == _WAIT_SIGNALED )
			nRet = _WAIT_ABANDONED;
		pthread_mutex_unlock( &m_aMutex );
		return (int)nRet;
#endif
#endif
		/* Beispiel fuer ein korrektes Warten (damit Threads immer abgebrochen werden koennen):
			...
			// jetzt soll gewartet werden
			bool bContinue = true;
			do {
				// ???
			} while( bContinue );
			...
		*/
	}

	void SetTimeout( unsigned long nTimeout )
	{
		m_nTimeout = nTimeout;
	}

private:
	// +++ Daten +++
#ifdef _WIN32
	HANDLE				m_aHandle;
#endif
#ifdef _OS2
	HEV					m_aHandle;  
#endif
#ifdef __linux__
#ifdef _USE_PTHREAD
	pthread_mutex_t		m_aMutex;
	pthread_cond_t		m_aHandle;
#endif
#endif
	unsigned long		m_nTimeout;
	/*bool*/int			m_bSignaled;
	/*bool*/int			m_bWaiting;
};
#endif

//*************************************************************************
/** Klasse um einen geschuetzten Zugriff auf eine Resource zu gewaehrleisten.
  * Verwendet dazu ein Objekt der Klasse minSyncObj und verwendet die automatischen
  * Block-Eigenschaften von C++, d.h. automatisches Aufrufen (und Unlocken) wenn
  * die Ausfuehrung den aktuellen Block verlaesst (sei es normal, durch Exception
  * oder Sprung(break, return, etc.)) */
class UMDLLEXPORT minLock
{
public:
	minLock( minSyncObject & aSyncObj )	
	{ 
 		m_pSyncObj = &aSyncObj; 
		m_pSyncObj->Lock();
 	}
	minLock( minSyncObject * pSyncObj )	
	{ 
		m_pSyncObj = pSyncObj; 
		m_pSyncObj->Lock();
	}
	~minLock()												
	{ 
		m_pSyncObj->Unlock(); 
    }

private:
	minSyncObject *		m_pSyncObj;		// Objekt ist KEIN Eigentuemer an diesem Zeiger !
};

#endif


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

#include "minbase.h"

#include <stdio.h>	// fuer printf

// *************************************************************************

minServiceManager *	minServiceManager::m_pGlobServiceManager = 0;

const char * const_InvalidServiceStrg	= "INVALID SERVICE";

// *************************************************************************

minService::~minService()
{
	// WICHTIG: beim Service-Manager austragen
	TRYCALLSRVMAN( UnRegisterService( this ) );
}

// *************************************************************************

// Hilfsklasse zum Finden eines Services anhand der ID
/** Helper class, to find a service with a given id.
  */
class _FindServiceWithId : public unary_function<minServiceData,bool>
{
public:
	_FindServiceWithId( long nId ) : m_nId( nId ) {}

	bool operator() ( const minServiceData & aData ) const 
	{
		return m_nId == aData.GetServiceId();
	}

private:
	long 		m_nId;
};

// Hilfsklasse zum Finden eines Services anhand des Namens
/** Helper class, to find a service with a given name.
  */
class _FindServiceWithName : public unary_function<minServiceData,bool>
{
public:
	_FindServiceWithName( const minString & sName ) : m_sName( sName ) {}

	bool operator() ( const minServiceData & aData ) const 
	{
		return m_sName == aData.GetServiceName();
	}

private:
	minString 		m_sName;
};

/** Helper class, to count service-data clients.
  */
class _SumClients
{
public:
	_SumClients() : m_nSum( 0 ) {}

	long GetSum() const { return m_nSum; }

	void operator() ( const minServiceData & aData )
	{
		m_nSum += aData.GetNoOfClients();
	}

private:
	long			m_nSum;
};

// *************************************************************************

minServiceManager::minServiceManager()
	: m_aDllManager( this )
{
}

minServiceManager::~minServiceManager()
{
	// TODO alle Services von den Clients trennen und alle DLL-Module unloaden 
}

/** Register a service at the manager. If the flag @a bOwnerIn is true, the manager
  * takes the ownership of the pointer.
  */
bool minServiceManager::RegisterService( const minService * pServiceIn, bool bOwnerIn )
{
	// ueberpruefen ob der Service schon registriert ist !!!
	ServiceContainerT::const_iterator aElement = find( m_aServiceContainer.begin(), 
													   m_aServiceContainer.end(), 
													   pServiceIn );
	if( aElement != m_aServiceContainer.end() )
		// JA, Service existiert schon, Fehler an Aufrufer liefern
		return false;

	m_aServiceContainer.push_back( minServiceData( pServiceIn, bOwnerIn ) );

	// jetzt noch allen interessierten Clienten ueber die Registrierung des Service Bescheid geben 
	NotifyAll( &minServiceManagerEventInterface::ServiceRegisterd, pServiceIn->GetServiceId() );

	return true;
}

bool minServiceManager::UnRegisterService( const minService * pServiceIn )
{
	// ueberpruefen ob der Service ueberhaupt registriert ist !!!
	ServiceContainerT::iterator aElement = find( m_aServiceContainer.begin(), 
											     m_aServiceContainer.end(),
												 pServiceIn );
	if( aElement != m_aServiceContainer.end() )
	{
		// allen interessierten Clienten ueber die Entfernung des Service Bescheid geben
		NotifyAll( &minServiceManagerEventInterface::ServiceUnRegisterd, pServiceIn->GetServiceId() );

		// alle Clienten benachrichtigen, dass der verwendete Service
		// nun nicht mehr gueltig ist !
		(*aElement).ClearAllClients();

		// jetzt das Element aus dem Kontainer loeschen
		//list: m_aServiceContainer.remove( /*verwende Konstruktor mit Service als Parameter...*/pServiceIn );
		ServiceContainerT::iterator aRemoveElement = remove( m_aServiceContainer.begin(), m_aServiceContainer.end(), pServiceIn );
		// jetzt die Elemente auch wirklich aus dem vector entfernen !
		m_aServiceContainer.erase( aRemoveElement );

		// und nur falls der Manager Eigentuemer des Services geworden ist,
		// den Service auch loeschen...
		if( (*aElement).IsOwner() )
		{
			delete (minService *)pServiceIn;		// Casten wegen const
		}

		return true;
	}
	return false;
}

const minService * minServiceManager::QueryService( const long nServiceId, minClient * const pClient )
{
	//ServiceContainerT::const_iterator aElement = find_if( m_aServiceContainer.begin(), m_aServiceContainer.end(), bind2nd( mem_fun1_ref( &minServiceData::GetServiceId ), nServiceId ) );

	ServiceContainerT::iterator aElement = find_if( m_aServiceContainer.begin(), 
													m_aServiceContainer.end(), 
													_FindServiceWithId( nServiceId ) );
	if( aElement != m_aServiceContainer.end() )
	{
		(*aElement).AddClient( pClient );
		return (*aElement).GetService(); 
	}
	return 0;		// bedeutet Fehler (nicht gefunden)
}

const minService * minServiceManager::QueryService( const minString & sServiceName, minClient * const pClient )
{
	ServiceContainerT::iterator aElement = find_if( m_aServiceContainer.begin(), 
													m_aServiceContainer.end(), 
													_FindServiceWithName( sServiceName ) );
	if( aElement != m_aServiceContainer.end() )
	{
		(*aElement).AddClient( pClient );
		return (*aElement).GetService(); 
	}
	return 0;		// bedeutet Fehler (nicht gefunden)
}

bool minServiceManager::ReleaseService( const minService * pService, minClient * const pClient )
{
	ServiceContainerT::iterator aElement = find( m_aServiceContainer.begin(), 
												 m_aServiceContainer.end(), 
												 pService );
	if( aElement != m_aServiceContainer.end() )
	{
		(*aElement).RemoveClient( pClient );
		return true; 
	}
	return false;
}

bool minServiceManager::IsServiceEnabled( const long nServiceId ) const
{
	ServiceContainerT::const_iterator aElement = find_if( m_aServiceContainer.begin(), 
													m_aServiceContainer.end(), 
													_FindServiceWithId( nServiceId ) );
	if( aElement != m_aServiceContainer.end() )
	{
		const minServiceData & aServiceData = *aElement;
		return aServiceData.IsEnabled();
	}
	return false;
}

bool minServiceManager::SetServiceEnabled( const long nServiceId, bool bEnable )
{
	ServiceContainerT::iterator aElement = find_if( m_aServiceContainer.begin(), 
													m_aServiceContainer.end(), 
													_FindServiceWithId( nServiceId ) );
	if( aElement != m_aServiceContainer.end() )
	{
		minServiceData & aServiceData = *aElement;
		aServiceData.SetEnabled( bEnable );
		return true;
	}
	return false;
}

bool minServiceManager::ToggleServiceEnabled( const long nServiceId )
{
	ServiceContainerT::iterator aElement = find_if( m_aServiceContainer.begin(), 
													m_aServiceContainer.end(), 
													_FindServiceWithId( nServiceId ) );
	if( aElement != m_aServiceContainer.end() )
	{
		minServiceData & aServiceData = *aElement;
		aServiceData.SetEnabled( !aServiceData.IsEnabled() );
		return true;
	}
	return false;
}

bool minServiceManager::RegisterObserver( minServiceManagerEventInterface * pClient )
{
	m_aClientContainer.push_back( pClient );
	return true;
}

bool minServiceManager::UnRegisterObserver( minServiceManagerEventInterface * pClient )
{
	m_aClientContainer.remove( pClient );
	return true;
}

void minServiceManager::NotifyAll( ServiceManagerEventT pCallback, long nServiceIdIn )
{
	ClientContainerT::/*const_*/iterator aIter = m_aClientContainer.begin();	// /**/ wegen STLport3.2

	while( aIter != m_aClientContainer.end() )
	{
		((*aIter)->*pCallback)( nServiceIdIn );
		++aIter;
	}
}

const minService * minServiceManager::GetServiceFromIndex( int nIndex )
{
	if( nIndex>=0 && nIndex<(int)m_aServiceContainer.size() )
	{
		return m_aServiceContainer[ nIndex ].GetService();
	}
	return 0;
}

const minString minServiceManager::GetServiceInfoStringFromIndex( int nIndex )
{
	if( nIndex>=0 && nIndex<(int)m_aServiceContainer.size() )
	{
		return m_aServiceContainer[ nIndex ].GetInfoString();
	}
	return "";
}

long minServiceManager::GetNoOfServices() const		
{ 
	return m_aServiceContainer.size(); 
}

minDllManager &	minServiceManager::GetDllManager()				
{ 
	return m_aDllManager; 
}

long minServiceManager::GetNoOfClients() const
{
	_SumClients aSum = for_each( m_aServiceContainer.begin(), 
								 m_aServiceContainer.end(), 
								 _SumClients() );

	return aSum.GetSum();
}

minServiceManager * minServiceManager::GetGlobServiceManagerPtr()		
{ 
	// ACHTUNG: kein automatisches Anlegen des Service-Managers, dieser MUSS immer 
	//			explizit fuer jede Applikation angelegt werden (am besten von dem
	//			jeweiligen Applikations-Objekt) !!!
	//if( !m_pGlobServiceManager )
	//	m_pGlobServiceManager = new minServiceManager;
	return m_pGlobServiceManager; 
}

void minServiceManager::SetGlobServiceManagerPtr( minServiceManager * pNewGlob )
{
	m_pGlobServiceManager = pNewGlob;
}

void minServiceManager::Dump( ostream & aStream ) const
{
	aStream << "Dumping ServiceManager: NoOfServices=" << m_aServiceContainer.size() << endl;

	ServiceContainerT::const_iterator aIter = m_aServiceContainer.begin();
	int i = 0;

	while( aIter != m_aServiceContainer.end() )
	{
		const minServiceData & aData = *aIter;
		aStream << ++i << " ";
		aData.Dump( aStream );
		aStream << endl;
		++aIter;
	}

	aStream << endl;
}

// *************************************************************************

const minString minServiceData::GetInfoString( const minString & sSeparatorStrg ) const
{
	minString sStrg;
	char sBuffer[512];


	sStrg += m_pService->GetServiceName();
	sStrg += sSeparatorStrg;
	sprintf( sBuffer, "%d", (int)m_pService->GetServiceId() );
	sStrg += sBuffer;
	sStrg += sSeparatorStrg;
	sStrg += m_pService->GetModulName();
	sStrg += sSeparatorStrg;
	sStrg += m_bEnabled ? "1" : "0";
	sStrg += sSeparatorStrg;
	sprintf( sBuffer, "%d", (int)GetNoOfClients() );
	sStrg += sBuffer;
	sStrg += sSeparatorStrg;
	// alle Clienten ausgeben
	ClientContainerT::const_iterator aIter = m_aClientContainer.begin();
	while( aIter != m_aClientContainer.end() )
	{
		const minClient * pClient = *aIter;
		sStrg += ( pClient ? pClient->GetClientName() : minString( "unknown" ) );
		++aIter;
		if( aIter != m_aClientContainer.end() )
			sStrg += ",";
	}

	return sStrg;
}

bool minServiceData::ClearAllClients()
{
    ClientContainerT::iterator aIter = m_aClientContainer.begin();
    while( aIter != m_aClientContainer.end() )
    {
        minClient * pClient = *aIter;
        pClient->ClearRep();
        ++aIter;
    }
    return true;
}

void minServiceData::Dump( ostream & aStream ) const
{
	aStream << "SERVICE: " << m_pService->GetServiceName().c_str() << " CLIENTS(" << m_aClientContainer.size() << "): ";

	// alle Clienten ausgeben
	ClientContainerT::const_iterator aIter = m_aClientContainer.begin();
	while( aIter != m_aClientContainer.end() )
	{
		const minClient * pClient = *aIter;
		aStream << ( pClient ? pClient->GetClientName() : minString( "unknown" ) );
		++aIter;
		if( aIter != m_aClientContainer.end() )
			aStream << ", ";
	}

	aStream << endl;
}

// *************************************************************************

#ifdef _with_main

#include <iostream>

const long g_TestID = 100;

class minSimpleLoggingService : public minService
{
public:
	minSimpleLoggingService()
		: minService( g_TestID, "Logging" )
	{}

	bool IsOk() const
	{
		return true;
	}

	virtual void Print( const char * sStrg )
	{
		cout << "LOG: " << sStrg << endl;
	}
};

// BEMERKUNG: minClient ist so etwas wie ein Handle auf einen Server !!!
/*
		minClient<minAnyServerClass> aClient( nServiceId );

		aClient->aServiceCall( ... );
*/

int main()
{
	{
		minSimpleLoggingService aLogger;
		SRVMAN()->RegisterService( &aLogger );

		cout << "ServiceManager: NoOfServices = " << minServiceManager::GetServiceManagerPtr()->GetNoOfServices() << endl;
		cout << "ServiceManager: NoOfClients = " << minServiceManager::GetServiceManagerPtr()->GetNoOfClients() << endl;

		minClientHandle<minSimpleLoggingService> aClient1( g_TestID );
		minClientHandle<minSimpleLoggingService> aClient2( 4 );

		aClient1->Print( "Hallo Welt!" );
		CALL(aClient2)->Print( "Hallo Welt!" );

		TRYCALL(aClient1,Print( "Hallo Welt 2!" ));
		TRYCALL( aClient2 , Print( "Hallo Welt 2!" ) );

		bool bok = TRYCALL2(aClient2,IsOk(),false);
		cout << "bok=" << bok << endl;
		bok = TRYCALL2(aClient1,IsOk(),false);
		cout << "bok=" << bok << endl;

		cout << "ServiceManager: NoOfClients = " << minServiceManager::GetServiceManagerPtr()->GetNoOfClients() << endl;
	}

//	SRVMAN()->UnRegisterService( &aLogger );

	cout << "ServiceManager: NoOfServices = " << SRVMAN()->GetNoOfServices() << endl;
	cout << "ServiceManager: NoOfClients = " << SRVMAN()->GetNoOfClients() << endl;

	return 0;
}

#endif


#ifdef _with_main2

//Linux: gcc -c -I../stl_sgi minbase.cpp
//Linux: gcc -o dllbase -I../stl_sgi -D_with_main dllbase.cpp minbase.o -ldl -lg++

#include "umstl_stream.h"

int main( int argc, char *argv[] )
{
	minServiceManager aSrvManager;
	minServiceManager::SetGlobServiceManagerPtr( &aSrvManager );

	minDll * pNewDll = 0;
	if( !aSrvManager.GetDllManager().LoadMinDll( /*"i:\\projects\\minkrnl\\templdll.dll"*/"./libtempldll.so.1.0", pNewDll ) )
		cout << "Error loading dll" << endl;
	else
		cout << "Successfully loaded dll" << endl;

	aSrvManager.GetDllManager().Dump( cout );

	minSimpleLoggingServiceInterface * pLogging = 
		(minSimpleLoggingServiceInterface *)aSrvManager.QueryService( 100, 0 );

	if( pLogging )
		pLogging->Print( "Hallo DLL" );

	minServiceManager::SetGlobServiceManagerPtr( 0 );

	return 0;
}

#endif


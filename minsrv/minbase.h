/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/minsrv/minbase.h,v $
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
 *	Klassen zur Behandlung von zur Laufzeit
 *	aenderbarer/konfigurierbarer Dienste.
 *
 *	Start: 28.10.1998
 *	Stand: 21. 3.1999
 *
 *	Arbeitsaufwand:	28.10.1998    3 Stunden
 *					21. 3.1999    1 Stunde
 *
 *	(c) by Michael Neuroth, 1998 - 2002
 *
 *************************************************************************/

#ifndef _MINBASE_H
#define _MINBASE_H

#ifdef _MSC_VER
#pragma warning( disable : 4251 )
// disable warning: class '...' benoetigt eine DLL-Schnittstelle, die von Clients von class 'minDllManager' verwendet wird
#pragma warning( disable : 4786 )
// disable warning: '...' : Bezeichner wurde auf '255' Zeichen in den Debug-Informationen reduziert
// for stl-structs
#endif

#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;

#include "dllbase.h"		// fuer minDllManager, string

// *************************************************************************
class minDllManager;
class minService;

/** Interface, von dem sich abgeleitet werden kann, wenn man an Service-Manager
  * Ereignissen interessiert ist. */
class MINDLLEXPORT minServiceManagerEventInterface
{
public:
    virtual ~minServiceManagerEventInterface()                      {}

	virtual void ServiceRegisterd( long /*nServiceIdIn*/ )			{}
	virtual void ServiceUnRegisterd( long /*nServiceIdIn*/ )		{}
};

typedef void (minServiceManagerEventInterface::*ServiceManagerEventT)( long nServiceIdIn );

// *************************************************************************
/** Basisklasse fuer alle Client-Klassen. */
class MINDLLEXPORT minClient
{
	friend class minServiceManager;
	friend class minServiceData;

protected:
	// +++ Konstruktor/Destruktor +++
	minClient( const minString & sClientName ) 
		: m_sClientName( sClientName ) {}
	virtual ~minClient() {}

	// muss von abgeleiteten Klassen ueberschrieben werden !
	virtual bool		ClearRep() = 0;

public:

	// +++ oeffentliche Schnittstelle +++
	const minString	&	GetClientName() const	{ return m_sClientName; }

private:
	// +++ Daten +++
	const minString		m_sClientName;
};

// *************************************************************************
// *************************************************************************
// *************************************************************************

const long		const_InvalidServiceId		= -1;
extern const /*minString*/char * const_InvalidServiceStrg;


// *************************************************************************
/** Basisklasse fuer alle Service-Klassen. */
class MINDLLEXPORT minService
{
	friend class minServiceManager;

protected:
	// +++ Konstruktor +++
	minService( long nServiceId, const minString & sServiceName, const minDll *	pModule = 0 ) 
		: m_nServiceId( nServiceId ), m_sServiceName( sServiceName ), m_pModule( pModule )
	{}

public:
	// +++ Destruktor +++
	virtual ~minService();

	// +++ oeffentliche Schnittstelle +++
	long				GetServiceId() const		{ return m_nServiceId; }
	const minString &	GetServiceName() const		{ return m_sServiceName; }
	minString			GetModulName() const		{ return m_pModule ? m_pModule->GetModuleName() : minString( "build_in" ); }

	// +++ eigenes Interface +++
	// kann dieser Service wieder entfernt werden, wenn er einmal geladen ist ?
	virtual bool		IsRemoveable() const															{ return true; }
	// Callback fuer Ereignisse vom ServiceManager (fuer Service-Protokoll)
	virtual bool		HandleServiceManagerEvent( int /*nEventId*/, int /*nUser1*/, int /*nUser2*/ )	{ return true; }

	// Methoden zur Konfiguration des Services: ggf. ein allgemeines Inspection Interface definieren
	virtual long		GetNoOfProperties() const																	{ return 0; }
	virtual bool		GetPropertyWithNo( long /*nNo*/, minString & /*sPropName*/, minString & /*sPropValue*/ )	{ return false; }
	virtual bool		SetPropertyWithNo( long /*nNo*/, const minString & /*sPropValue*/ )						{ return false; }

private:
	// +++ Daten +++
	const long			m_nServiceId;
	const minString		m_sServiceName;
	const minDll *		m_pModule;			// kein Eigentuemer, neu seit 6.12.1999
};

// *************************************************************************
// *************************************************************************
// *************************************************************************

class minServiceData
{
public:
    typedef list<minClient *>	ClientContainerT;

	minServiceData()
	{
		m_bOwner = false;
		m_bEnabled = true;
		m_pService = 0;
	}
	minServiceData( const minService * pServiceIn, bool bOwnerIn = false )
	{
		m_bOwner = bOwnerIn;
		m_bEnabled = true;
		m_pService = pServiceIn;
	}
#ifdef __linux__  // Zuweisungsoperator angeben ist notwendig fuer GCC 2.7.2
	minServiceData & operator=( const minServiceData & aOther )
	{
		if( this != &aOther )
		{
			m_bOwner = aOther.m_bOwner;
			m_bEnabled = aOther.m_bEnabled;
			m_pService = aOther.m_pService;
			m_aClientContainer = aOther.m_aClientContainer;			
		}
		return *this;
	}
#endif

	bool HasNoClients() const								{ return m_aClientContainer.size()==0; }
	long GetNoOfClients() const								{ return m_aClientContainer.size(); }
	bool AddClient( minClient * const pClient )				{ m_aClientContainer.push_back( pClient ); return true; }
	bool RemoveClient( minClient * const pClient )			{ m_aClientContainer.remove( pClient ); return true; }
	bool ClearAllClients()									{ for_each( m_aClientContainer.begin(), m_aClientContainer.end(), mem_fun( &minClient::ClearRep ) ); return true; }

	bool IsEnabled() const									{ return m_bEnabled; }
	void SetEnabled( bool bVal )							{ m_bEnabled = bVal; }
	bool IsOwner() const									{ return m_bOwner; }
	bool HasServiceId( long nId ) const						{ return nId==GetServiceId(); }
	long GetServiceId() const								{ return m_pService ? m_pService->GetServiceId() : const_InvalidServiceId; }
	const minString /*&*/ GetServiceName() const			{ return m_pService ? m_pService->GetServiceName() : minString( const_InvalidServiceStrg ); }
	const minService * GetService() const					{ return m_pService; }

	const minString GetInfoString( const minString & sSeparatorStrg = "|" ) const;

	// zum Debuggen
	void Dump( ostream & aStream ) const;

	bool operator<( const minServiceData & rObj ) const		{ return rObj>*this; }
	bool operator>( const minServiceData & rObj ) const		{ return rObj<*this; }
	bool operator==( const minServiceData & rObj ) const	{ return m_pService==rObj.m_pService; }
	bool operator!=( const minServiceData & rObj ) const	{ return !(*this==rObj); }

private:
	bool				m_bOwner;
	bool				m_bEnabled;							// neu seit 10.12.1999
	const minService *	m_pService;
	ClientContainerT	m_aClientContainer;
};

// *************************************************************************
/** Klasse zur Verwaltung von Service-Objekten. */
// ACHTUNG: diese Klasse ist nicht multithread sicher !
class MINDLLEXPORT minServiceManager
{
	typedef list<minServiceManagerEventInterface *>	ClientContainerT;
	typedef vector<minServiceData>					ServiceContainerT;

public:
	// +++ Konstruktor/Destruktor +++
	minServiceManager();
	~minServiceManager();

	// +++ Schnittstelle ueber die Clienten dem Manager neue Dienste anbieten koennen +++
	bool				RegisterService( const minService * pServiceIn, bool bOwnerIn = false );
	bool				UnRegisterService( const minService * pServiceIn );

	// +++ Schnittstelle ueber die Clienten den Manager nach Diensten fragen koennen +++
	const minService *	QueryService( const long nServiceId, minClient * const pClient );
	const minService *	QueryService( const minString & sServiceName, minClient * const pClient );
	// +++ Schnittstelle ueber die Clienten vorher angeforderte Dienste an den Manager zureuckgeben koennen +++
	bool				ReleaseService( const minService * pService, minClient * const  pClient );

	// +++ einzelne Services ein- und ausschalten +++
	bool				IsServiceEnabled( const long nServiceId ) const;
	bool				SetServiceEnabled( const long nServiceId, bool bEnable );
	bool				ToggleServiceEnabled( const long nServiceId );

	// +++ Schnittstelle um Ereignisse des ServiceManagers an interessierte Clienten weiter zu geben +++
	bool				RegisterObserver( minServiceManagerEventInterface * pClient );
	bool				UnRegisterObserver( minServiceManagerEventInterface * pClient );

	// +++ weitere Methoden +++
	long				GetNoOfServices() const;
	long				GetNoOfClients() const;
	const minService *	GetServiceFromIndex( int nIndex );
	const minString		GetServiceInfoStringFromIndex( int nIndex );

	minDllManager &		GetDllManager();

	// zum Debuggen
	void				Dump( ostream & aStream ) const;

private:
	// +++ Hilfsmethoden +++
	void				NotifyAll( ServiceManagerEventT pCallback, long nServiceIdIn );

	// +++ Daten +++
	ClientContainerT	m_aClientContainer;
	ServiceContainerT	m_aServiceContainer;		// enthaelt alle bekannten Service-Interfaces !
	minDllManager		m_aDllManager;				// zum Manipulieren der DLL-Module

	//++++++++++++++++++++++++++++++++++++++++++++++++

public:
	// +++ Zugriff auf DEN globalen Service-Manager (Singelton-Muster) +++
	static minServiceManager * GetGlobServiceManagerPtr();		
	static void SetGlobServiceManagerPtr( minServiceManager * pNewGlob );

private:
	static minServiceManager *	m_pGlobServiceManager;		// ACHTUNG: Klasse ist NICHT Eigentuemer an diesem Zeiger !
};

#define SRVMAN()				minServiceManager::GetGlobServiceManagerPtr()
#define TRYCALLSRVMAN(Call)		if( minServiceManager::GetGlobServiceManagerPtr() ) minServiceManager::GetGlobServiceManagerPtr()->Call

// *************************************************************************
// *************************************************************************
// *************************************************************************

// *************************************************************************
template <class ServiceClass> class minClientHandle : public minClient
{
	// Bem.: Kopieren und zuweisen ist erlaubt, da der Zeiger auf den Service vom Service-Manager verwaltet wird !

public:
	minClientHandle( long nServiceId, const minString & sClientName = "ClientHandle" )
		: minClient( sClientName ), m_nServiceId( nServiceId )
	{
		Update( m_nServiceId );
	}
	~minClientHandle()
	{
		Release();
	}	

	bool					IsValid() const				{ return m_pRep!=0; }
	
	bool					UpdateIfNotValid()	
	{ 
		if( !IsValid() )
		{
			Update( m_nServiceId );
		}
		return IsValid();
	}

	const ServiceClass * 	operator->() const			{ return m_pRep; }
	ServiceClass * 			operator->()				{ return m_pRep; }
	ServiceClass & 			operator*()					{ return *m_pRep; }

protected:
	virtual bool			ClearRep()					{ m_pRep = 0; return true; }

private:
	// Hilfsmethoden
	void					Update( long nServiceId )	
	{ 
		m_pRep = SRVMAN() ? (ServiceClass *)SRVMAN()->QueryService( nServiceId, this ) : 0;
	}
	void					Release()
	{
		if( m_pRep )
		{
			TRYCALLSRVMAN( ReleaseService( m_pRep, this ) );
		}
	}

	// +++ Daten +++ 
	ServiceClass *			m_pRep;
	long					m_nServiceId;
};

#define CALL(aClient)			if( aClient.IsValid() ) aClient
#define TRYCALL(aClient,Call)	if( aClient.IsValid() ) aClient->Call;

#define TRYCALL2(aClient,Call,Err)	( aClient.IsValid() ? aClient->Call : Err );

#endif


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

#ifndef _MINHANDLE_H
#define _MINHANDLE_H

// *************************************************************************
/** Implementiert eine Handle-Klassse mit Referenz-Zaehler fuer Zeiger-Objekte. 
  * Abgewandelt fuer Verwaltung von Zeigern, d.h. weitere Indirektionsebene eingefuehrt (3.1.2000)
  * Siehe Stroustrup, Seite 843 */
// ACHTUNG: diese Klasse ist NICHT MultiThread sicher !!!
template <class MyClass> class minHandle
{
public:
	typedef MyClass TargetClass;

	// +++ Konstruktor/Destruktor +++
	minHandle( MyClass * p = 0 )				: m_ppRep( new MyClass * ( p ) ), m_pCount( new int( 1 ) ), m_pbOwner( new bool( true ) )
	{}	
	minHandle( bool bOwner, MyClass * p )		: m_ppRep( new MyClass * ( p ) ), m_pCount( new int( 1 ) ), m_pbOwner( new bool( bOwner ) )
	{}	
	minHandle( const minHandle<MyClass> & r )	: m_ppRep( r.m_ppRep ), m_pCount( r.m_pCount ), m_pbOwner( r.m_pbOwner )
	{ (*m_pCount)++; }
	~minHandle()
	{
		if( --(*m_pCount) == 0 )
		{
			//if( m_ppRep )
			//	cout << "~minHandle delete->" << (void *)m_ppRep << " bOwner=" << *m_pbOwner << endl;
			if( *m_pbOwner )
			{
				delete *m_ppRep;	// Client nur loeschen wenn auch Eigentuemer !
			}
			// Verwaltungs-Infos loeschen
			delete m_ppRep;			// Zeiger auf Zeiger MUSS immer geloescht werden
			delete m_pCount;
			delete m_pbOwner;
		}
	}

	// +++ Methoden +++
	void bind( MyClass * pp )
	{
		if( pp != *m_ppRep )
		{
			if( --*m_pCount == 0 )
			{
				// dieses Handle-Objekt ist das einzige auf die Client-Daten
				if( *m_pbOwner )
				{
					delete *m_ppRep;	// (alten) Client loeschen
				}
				*m_pCount = 1;			// verwende Verwaltungsdaten wieder !
				*m_ppRep = pp;
				*m_pbOwner = true;
			}
			else
			{
				// es gibt noch andere Handles auf die Client-Daten,
				// daher muessen nun neue Verwaltungsdaten erzeugt weden.
				m_pCount = new int( 1 );
				m_ppRep = new MyClass * ( pp );
				m_pbOwner = new bool( true );
			}
		}
	}

	int *		GetCounterPtr() const			{ return m_pCount; }
	MyClass *	GetPtr() const					{ return *m_ppRep; }
	bool		IsOk() const					{ return *m_ppRep != 0; }
	bool		IsOwner() const					{ return *m_pbOwner; }

	// Methode zum nachtraeglichen Aendern der Eigentuemerschaft an dem Zeiger
	void		SetOwnerModus( bool bOwner )	{ *m_pbOwner = bOwner; }

	// +++ Operatoren +++
	MyClass * 		operator->()				{ return *m_ppRep; }
	const MyClass * operator->() const			{ return *m_ppRep; }

		  MyClass & operator*()					{ return **m_ppRep; }
	const MyClass & operator*() const			{ return **m_ppRep; }

	minHandle<MyClass>& operator=( const minHandle<MyClass> & r )
	{
		if( *m_ppRep == *(r.m_ppRep) )
			return *this;
		if( --(*m_pCount) == 0 )
		{
			if( *m_pbOwner )
			{
				delete *m_ppRep;
			}
			delete m_ppRep;
			delete m_pCount;
			delete m_pbOwner;
		}
		m_ppRep = r.m_ppRep;
		m_pCount = r.m_pCount;
		m_pbOwner = r.m_pbOwner;
		(*m_pCount)++;
		return *this;
	}

	bool operator==( const minHandle<MyClass> & r ) const
	{
		return ( *m_ppRep == *(r.m_ppRep) ) && ( m_ppRep == r.m_ppRep );
	}
	bool operator!=( const minHandle<MyClass> & r ) const
	{
		return !(*this == r);
	}
	// dummy Implementationen
	bool operator<( const minHandle<MyClass> & r ) const
	{
		return false;
	}
	bool operator>( const minHandle<MyClass> & r ) const
	{
		return false;
	}

protected:

	MyClass * *	m_ppRep;
	int *		m_pCount;
	bool *		m_pbOwner;		// am 28.12.1999 von bool auf bool * umgestellt !
};

#endif


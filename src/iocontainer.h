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

#ifndef _IOCONTAINER_H
#define _IOCONTAINER_H

#include "minhandle.h"		// for the smart-ptr
#include "minutils.h"
#include "writexml.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;


/** Simple template class for a io-container. */
template <class Type>
class IOContainer : public vector< minHandle<Type> >
{
public:
	typedef Type			ItemClass;
	typedef minHandle<Type>	HandleType;

	IOContainer( const string & sName )
	: m_sName( sName )
	{}
	virtual ~IOContainer()
	{}

	virtual bool Read( istream & aStream )
	{
		return ReadContainerPtr( *this, aStream );
	}

	virtual bool Write( ostream & aStream ) const
	{
		return WriteContainerPtr( *this, aStream );
	}

	virtual XmlTree	GetXMLTree() const
	{
		XmlTree aTree( m_sName );

		aTree.SetAsContainer( true );

		typename vector< minHandle<Type> >::const_iterator aIter = vector< minHandle<Type> >::begin();
		while( aIter != vector< minHandle<Type> >::end() )
		{
			aTree.PushTag( (*aIter)->GetXMLTree() );

			++aIter;
		}

		return aTree;
	}

private:
	string		m_sName;
};

// *******************************************************************
/** meta interface, to inspect this class dynamically.
  * used for generic table dialog. */
class GenericDataInterface
{
public:
    enum DataType { _VOID, _INT, _DOUBLE, _STRING };

    virtual ~GenericDataInterface()		{}

    virtual string		GetName() const = 0;

    virtual int			GetDataCount() const = 0;
    virtual string		GetDataName( int iIndex ) const = 0;
    virtual DataType	GetDataType( int iIndex ) const = 0;
    virtual string		GetDataValue( int iIndex ) const = 0;
    virtual bool		SetDataValue( int iIndex, const string & sValue ) = 0;
    virtual bool        IsDataEditable( int /*iIndex*/ ) const { return true; }
};

// *******************************************************************
/** generic comment container class interface. */
class GenericCommentContainer : public ObjectChanged
{
public:
    virtual ~GenericCommentContainer()		{}

    virtual int						size() const = 0;
    virtual void					clear() = 0;
    virtual GenericDataInterface *	at( int iIndex ) = 0;
    virtual GenericDataInterface *	push_back_new_item() = 0;

    virtual void                    SortData() = 0;
};

// *******************************************************************
// ** helper class, to compare two objects handled with an smart-pointer
template <class T>
class minHandleCompare : public binary_function< minHandle<T>, minHandle<T>, bool >
{
public:
    bool operator()( minHandle<T> hLeft, minHandle<T> hRight ) const
    {
        return *hLeft < *hRight;
    }
};

#endif

/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/writexml.h,v $
 *
 *  $Revision: 1.1.1.1 $
 *
 *	$Log: not supported by cvs2svn $
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

#ifndef _WRITEXML_H
#define _WRITEXML_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

#include "minhandle.h"

// *******************************************************************
class XmlAttrib : public pair<string,string>
{
public:
	XmlAttrib( const string & sName = "", const string & sValue = "" )
	: pair<string,string>( sName, sValue )
	{}

	string GetName() const
	{
		return first;
	}
	string GetValue() const
	{
		return second;
	}

	bool WriteDTD( const string & sElementName, ostream & aStream ) const;
};

// *******************************************************************
class XmlAttribList : public vector<XmlAttrib>
{
public:
	XmlAttribList()
	: vector<XmlAttrib>()
	{}
	XmlAttribList( const XmlAttrib & aAttrib )
	: vector<XmlAttrib>()
	{
		push_back( aAttrib );
	}
	XmlAttribList( const string & sName, const string & sValue )
	: vector<XmlAttrib>()
	{
		push_back( XmlAttrib( sName, sValue ) );
	}

	bool WriteDTD( const string & sElementName, ostream & aStream ) const;
};

// *******************************************************************
class XmlTag
{
public:
	XmlTag( const string & sTag, const string & sData, bool bOptional = false, const XmlAttribList & aAttribList = XmlAttribList() );
	virtual ~XmlTag();

	bool   IsOptional() const;

	string GetTagName() const;

	bool operator==( const XmlTag & aOther ) const;

	virtual bool WriteXML( ostream & aStream ) const;
	virtual bool WriteDTD( ostream & aStream ) const;

protected:
	string XmlStartTag() const;
	string XmlStopTag() const;

	// ** data **
	string			m_sTag;
	string			m_sData;
	bool			m_bOptional;
	XmlAttribList	m_aAttribList;
};

// *******************************************************************
class XmlTagContainer : public vector< minHandle<XmlTag> >
{
public:
	XmlTagContainer		MakeUnique() const;

	bool				IsTagElement( const string & sTagName ) const;
};

// *******************************************************************
class XmlTree : public XmlTag
{
	typedef XmlTagContainer		TagContainer;

public:
	XmlTree( const string & sTag, const string & sData = "", bool bOptional = false, const XmlAttribList & aAttribList = XmlAttribList() );

	// ** has this tree only elements of the same type ?
	bool IsContainer() const;
	void SetAsContainer( bool bIsContainer );

	bool operator==( const XmlTree & aOther ) const;

	// ** reimplement the XmlTag-Interface
	virtual bool WriteXML( ostream & aStream ) const;
	virtual bool WriteDTD( ostream & aStream ) const;

	// ** own interface **
	void	PushTag( const string & sTag, const string & sData, bool bOptional = false );
	void	PushTag( const string & sTag, int iData, const XmlAttribList & aAttribList = XmlAttribList(), bool bOptional = false );
	void	PushTag( const string & sTag, double dData, const XmlAttribList & aAttribList = XmlAttribList(), bool bOptional = false );
	void	PushTag( const string & sTag, const string & sData, const XmlAttribList & aAttribList, bool bOptional = false );
	void	PushTag( const XmlTree & aSubTree );

	// ** if no DTD file name is given, use inline DTD ! **
	bool	IsDTDInline() const;	

	string	GetDTDName() const;
	void	SetDTDName( const string sFileName );

	bool	WriteXML( const string & sXMLFileName ) const;
	bool	WriteDTD( const string & sDTDFileName ) const;

private:
	// ** data **
	TagContainer	m_aTagContainer;
	string			m_sDTDName;
	bool			m_bIsContainer;
};

#endif

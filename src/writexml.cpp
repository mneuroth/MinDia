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

#include "writexml.h"

#include <stdio.h>

const char * _XML_HEADER            = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>";

const char * _FILE_VERSION_TAG		= "fileversion";

// *******************************************************************
// *******************************************************************
// *******************************************************************

bool XmlAttrib::WriteDTD( const string & sElementName, ostream & aStream ) const
{
	aStream << "<!ATTLIST " << sElementName.c_str() << " " << first << " CDATA #IMPLIED>" << endl;

	return aStream.good();
}

// *******************************************************************

bool XmlAttribList::WriteDTD( const string & sElementName, ostream & aStream ) const
{
	const_iterator aIter = begin();

	while( aIter != end() )
	{
		const XmlAttrib & aAttrib = *aIter;

		aAttrib.WriteDTD( sElementName, aStream );

		++aIter;
	}

	return aStream.good();
}

// *******************************************************************

XmlTag::XmlTag( const string & sTag, const string & sData, bool bOptional, const XmlAttribList & aAttribList )
: m_sTag( sTag ),
  m_sData( sData ),
  m_bOptional( bOptional ),
  m_aAttribList( aAttribList )
{
}

XmlTag::~XmlTag()
{
}

bool XmlTag::IsOptional() const
{
	return m_bOptional;
}

string XmlTag::GetTagName() const
{
	return m_sTag;
}

bool XmlTag::operator==( const XmlTag & aOther ) const
{
	return GetTagName() == aOther.GetTagName();
}

bool XmlTag::WriteXML( ostream & aStream ) const
{
	aStream << XmlStartTag().c_str();
	aStream << m_sData.c_str();
	aStream << XmlStopTag().c_str() << endl;

	return aStream.good();
}

bool XmlTag::WriteDTD( ostream & aStream ) const
{
	aStream << "<!ELEMENT " << m_sTag.c_str() << " (#PCDATA)>" << endl;

	m_aAttribList.WriteDTD( m_sTag, aStream );

	return aStream.good();
}	

string XmlTag::XmlStartTag() const
{
	string sRet = "<" + m_sTag;

	if( m_aAttribList.size()>0 )
	{
		for( int i=0; i<(int)m_aAttribList.size(); i++ )
		{
			sRet += " "   + m_aAttribList[i].GetName() +
					"=\"" + m_aAttribList[i].GetValue() + "\"";
		}
	}

	sRet += ">";

	return sRet;
}

string XmlTag::XmlStopTag() const
{
	return "</" + m_sTag + ">";
}

// *******************************************************************

XmlTree::XmlTree( const string & sTag, const string & sData, bool bOptional, const XmlAttribList & aAttribList )
: XmlTag( sTag, sData, bOptional, aAttribList ),
  m_bIsContainer( false )
{
}

bool XmlTree::IsContainer() const
{
	return m_bIsContainer;
}

void XmlTree::SetAsContainer( bool bIsContainer )
{
	m_bIsContainer = bIsContainer;
}

bool XmlTree::operator==( const XmlTree & aOther ) const
{
	return GetTagName() == aOther.GetTagName();
}

bool XmlTree::WriteXML( ostream & aStream ) const
{
	aStream << XmlStartTag().c_str();
	aStream << m_sData.c_str();
	aStream << endl;

	// ** write all sub elements:
	for( int i=0; i<(int)m_aTagContainer.size(); i++ )
	{
		minHandle<XmlTag> hTag = m_aTagContainer[i];

		hTag->WriteXML( aStream );
	}

	aStream << XmlStopTag().c_str() << endl;

	return aStream.good();
}

bool XmlTree::WriteDTD( ostream & aStream ) const
{
	TagContainer aTagContainer = m_aTagContainer.MakeUnique();

	if( aTagContainer.size()>0 )
	{
		aStream << "<!ELEMENT " << m_sTag.c_str() << " (";

		for( unsigned int j=0; j<aTagContainer.size(); j++ )
		{
			aStream << aTagContainer[j]->GetTagName().c_str();

			if( IsContainer() )
			{
				aStream << "*";
			}

			if( j != aTagContainer.size()-1 )
			{
				aStream << ",";
			}
		}

		aStream << ")>" << endl;
	}

	// ** write all sub elements:
	for( unsigned int i=0; i<aTagContainer.size(); i++ )
	{
		aTagContainer[i]->WriteDTD( aStream );
	}

	return aStream.good();
}

void XmlTree::PushTag( const string & sTag, const string & sData, bool bOptional )
{
	minHandle<XmlTag> hTag( new XmlTag( sTag, sData, bOptional ) );

	m_aTagContainer.push_back( hTag );
}

void XmlTree::PushTag( const string & sTag, int iData, const XmlAttribList & aAttribList, bool bOptional )
{
	char sBuffer[255];

	sprintf( sBuffer, "%d", iData );
	string s = sBuffer;

	PushTag( sTag, s, aAttribList, bOptional );
}

void XmlTree::PushTag( const string & sTag, double dData, const XmlAttribList & aAttribList, bool bOptional )
{
	char sBuffer[255];

	sprintf( sBuffer, "%lf", dData );
	string s = sBuffer;

	PushTag( sTag, s, aAttribList, bOptional );
}

void XmlTree::PushTag( const string & sTag, const string & sData, const XmlAttribList & aAttribList, bool bOptional )
{
	minHandle<XmlTag> hTag( new XmlTag( sTag, sData, bOptional, aAttribList ) );

	m_aTagContainer.push_back( hTag );
}

void XmlTree::PushTag( const XmlTree & aSubTree )
{
	minHandle<XmlTag> hSubTree( new XmlTree( aSubTree ) );

	m_aTagContainer.push_back( hSubTree );
}

bool XmlTree::WriteXML( const string & sXMLFileName ) const
{
	fstream aFile( sXMLFileName.c_str(), ios::out );

	if( aFile.good() )
	{
		aFile << _XML_HEADER << endl;

		if( IsDTDInline() )
		{
			aFile << "<!DOCTYPE " << GetTagName().c_str() << endl;
			aFile << "[" << endl;

			WriteDTD( aFile );

			aFile << "]>" << endl;
		}
		else
		{
			aFile << "<!DOCTYPE " << GetTagName().c_str()  
				  << "SYSTEM=\"" << GetDTDName().c_str() << "\">";

			WriteDTD( GetDTDName() );
		}

		return WriteXML( aFile );
	}

	return false;
}

bool XmlTree::WriteDTD( const string & sDTDFileName ) const
{
	fstream aFile( sDTDFileName.c_str(), ios::out );

	if( aFile.good() )
	{
		return WriteDTD( aFile );
	}

	return false;
}

bool XmlTree::IsDTDInline() const
{
	return m_sDTDName.length()==0;
}

string XmlTree::GetDTDName() const
{
	return m_sDTDName;
}

void XmlTree::SetDTDName( const string sFileName )
{
	m_sDTDName = sFileName;
}

// *******************************************************************

bool XmlTagContainer::IsTagElement( const string & sTagName ) const
{
	const_iterator aIter = begin();

	while( aIter != end() )
	{
		minHandle<XmlTag> hTag = *aIter;

		if( hTag->GetTagName() == sTagName )
		{
			return true;
		}

		++aIter;
	}

	return false;
}

XmlTagContainer XmlTagContainer::MakeUnique() const
{
	XmlTagContainer aRet;

	const_iterator aIter = begin();

	while( aIter != end() )
	{
		minHandle<XmlTag> hTag = *aIter;

		if( !aRet.IsTagElement( hTag->GetTagName() ) )
		{
			aRet.push_back( hTag );
		}
		else
		{
//			aRet.MarkAsSequence( hTag-GetTagName() );
		}

		++aIter;
	}

	return aRet;
}

// *******************************************************************

string XmlStartTag( const string & sTag )
{
	return "<" + sTag + ">";
}

string XmlStopTag( const string & sTag )
{
	return "</" + sTag + ">";
}


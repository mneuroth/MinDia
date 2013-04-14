/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/diainfo.cpp,v $
 *
 *  $Revision: 1.3 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.2  2003/10/26 17:26:30  min
 *	MakeRelativePaths() implemented.
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

#include "diainfo.h"

#include "minutils.h"
#include "misctools.h"

#include <QUuid>

#include <sstream>

using namespace std;

// *******************************************************************

const double c_dDefaultDissolveTime = 2.5;
const double c_dDefaultTimer = 10.0;

// *******************************************************************
// *******************************************************************
// *******************************************************************

const int DiaInfo::ACT_FILE_VERSION = 3;	// 0 until 29.3.2002, then m_sScript introduced
                                            // 1 until 31.3.2013, then m_sUUID introduced
                                            // 2 until 1.4.2013, then m_relX, m_relY, m_relDX, m_relDY introduced

const int DiaInfo::HORIZONTAL = 0;
const int DiaInfo::VERTICAL = 1;


DiaInfo::DiaInfo( const string & sId, const string & sImageFile, const string & sComment )
{
    m_sUUID                 = ToStdString(QUuid::createUuid().toString());
	m_sId					= sId;
	m_sComment				= sComment;
	m_sImageFile			= sImageFile;
	m_dIntensityInPercent	= 1.0;
	m_iFormat				= HORIZONTAL;
    m_relX                  = 0.0;
    m_relY                  = 0.0;
    m_relDX                 = 1.0;
    m_relDY                 = 1.0;

	// ** fill disolve and timer wiht (default) data
	AddOperation( TimeOperation( TimeOperation::DISSOLVE_IN, c_dDefaultDissolveTime ) );
	AddOperation( TimeOperation( TimeOperation::SHOW, c_dDefaultTimer ) );

	// ** init temp data
	m_iPosition				= 0;

	// ** after constructor call the object is unchanged !
	m_aObjectChanged.ClearChanged();
}

DiaInfo::~DiaInfo()
{
}

bool DiaInfo::IsChanged() const
{
	return m_aObjectChanged.IsChanged();
}

string DiaInfo::GetData() const
{
	ostringstream aStrStream;

	Write( aStrStream );

    return string( aStrStream.str() );
}

void DiaInfo::SkipWhitespaces( string & sString )
{
	while( (sString.length()>0) && IsWhitespace( sString[0] ) )
	{
		sString.erase( sString.begin(), sString.begin()+1 );
	}
}

bool DiaInfo::IsWhitespace( char ch ) const
{
	if( (ch==' ') || (ch=='\r') || (ch=='\n') || (ch=='\t') )
	{
		return true;
	}
	return false;
}

bool DiaInfo::SetFromData( string & sStringStreamInOut )
{
    istringstream aStrStream( sStringStreamInOut );

	if( Read( aStrStream ) )
	{
		int iCount = aStrStream.tellg();

        sStringStreamInOut = string( sStringStreamInOut.c_str()+iCount, sStringStreamInOut.length()-iCount );

		// ** erase whitespaces at the begining of the string
		// ** necassery for the Qt-Clipboard-Patch
		SkipWhitespaces( sStringStreamInOut );

		return true;
	}

	return false;
}

void DiaInfo::SetUUID( const string & sUUID )
{
    m_sUUID = sUUID;
}

string DiaInfo::GetUUID() const
{
    return m_sUUID;
}

string DiaInfo::GetId() const
{
    return m_sId;
}

bool DiaInfo::SetId( const string & sId )
{
	m_sId = sId;
	return true;
}

string DiaInfo::GetComment() const
{
    return m_sComment;
}

bool DiaInfo::SetComment( const string & sComment )
{
	m_sComment = sComment;
	return true;
}

string DiaInfo::GetImageFile() const
{
    return m_sImageFile;
}

bool DiaInfo::SetImageFile( const string & sFileName )
{
	m_sImageFile = sFileName;
	return true;
}

string DiaInfo::GetScript() const
{
    return m_sScript;
}

bool DiaInfo::SetScript( const string & sScript )
{
	m_sScript = sScript;
    // ggf. syntax check ?
	return true;
}

double DiaInfo::GetTotalTime() const
{
	double dTime = 0;

	for( int i=0; i<GetOperationCount(); i++ )
	{
		dTime += GetOperation( i ).GetOperationTime();
	}

	return dTime;
}

double DiaInfo::GetDissolveTime() const
{
	double dTime = 0;

	for( int i=0; i<GetOperationCount(); i++ )
	{
		if( GetOperation( i ).GetOperationType()==TimeOperation::DISSOLVE_IN )
		{
			dTime += GetOperation( i ).GetOperationTime();
		}
	}

	return dTime;
}

bool DiaInfo::SetDissolveTime( double dShowTime )
{
	for( int i=0; i<GetOperationCount(); i++ )
	{
		if( GetOperation( i ).GetOperationType()==TimeOperation::DISSOLVE_IN )
		{
			return AccessOperationAndChanged( i ).SetOperationTime( dShowTime );
		}
	}

	return false;
}

double DiaInfo::GetShowTime() const
{
	for( int i=0; i<GetOperationCount(); i++ )
	{
		if( GetOperation( i ).GetOperationType()==TimeOperation::SHOW )
		{
			return GetOperation( i ).GetOperationTime();
		}
	}
	return 0;
}

bool DiaInfo::SetShowTime( double dShowTime )
{
	for( int i=0; i<GetOperationCount(); i++ )
	{
		if( GetOperation( i ).GetOperationType()==TimeOperation::SHOW )
		{
			return AccessOperationAndChanged( i ).SetOperationTime( dShowTime );
		}
	}

	return false;
}

int DiaInfo::GetPosition() const
{
	return m_iPosition;
}

void DiaInfo::SetPosition( int iPos )
{
	// ** this is temporary data, so no SetChanged() is needed !
	m_iPosition = iPos;
}

XmlTree	DiaInfo::GetXMLTree() const
{
	XmlTree aTree( "diainfo", "", false, XmlAttribList( "fileversion", ToString( ACT_FILE_VERSION ) ) );

	aTree.PushTag( "id", m_sId );
	aTree.PushTag( "comment", m_sComment, XmlAttribList( "type", "string" ) );
	aTree.PushTag( "imagefile", m_sImageFile, XmlAttribList( "type", "string" ) );
	aTree.PushTag( "script", m_sScript, XmlAttribList( "type", "string" ) );
	aTree.PushTag( "intensity", m_dIntensityInPercent, XmlAttribList( "scale", "percent" ) );
	aTree.PushTag( "format", m_iFormat );
	
	aTree.PushTag( m_aTimeOpContainer.GetXMLTree() );

	return aTree;
}

bool DiaInfo::Read( istream & aStream )
{
	FileUtilityObj aFU;
	int iActFileVersion = 0;

	aFU.ReadStructBegin( aStream );
	aStream >> iActFileVersion;
	aFU.ReadSeparator( aStream );
    if( iActFileVersion > 1 )
    {
        ReadString( aStream, m_sUUID );
        aFU.ReadSeparator( aStream );
    }
    else
    {
        // use uuid created in constructor
    }
    ReadString( aStream, m_sId );
	aFU.ReadSeparator( aStream );
	ReadString( aStream, m_sComment );
	aFU.ReadSeparator( aStream );
	ReadString( aStream, m_sImageFile );
	aFU.ReadSeparator( aStream );
	if( iActFileVersion > 0 )
	{
		ReadString( aStream, m_sScript );
		aFU.ReadSeparator( aStream );
	}
	else
	{
		// ** in old versions the script was stored in the filename variable !
		m_sScript = "#" + m_sImageFile;
	}
	aStream >> m_dIntensityInPercent;
	aFU.ReadSeparator( aStream );
	aStream >> m_iFormat;
	aFU.ReadSeparator( aStream );
    if( iActFileVersion > 2 )
    {
        aStream >> m_relX;
        aFU.ReadSeparator( aStream );
        aStream >> m_relY;
        aFU.ReadSeparator( aStream );
        aStream >> m_relDX;
        aFU.ReadSeparator( aStream );
        aStream >> m_relDY;
        aFU.ReadSeparator( aStream );
    }
	m_aTimeOpContainer.Read( aStream );
	aFU.ReadStructEnd( aStream );

	if( aStream.good() )
	{
		m_aObjectChanged.ClearChanged();
	}

	return aStream.good();
}

bool DiaInfo::Write( ostream & aStream ) const
{
	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	aStream << ACT_FILE_VERSION;			// since 21.10.2001
	aFU.WriteSeparator( aStream );
    if( ACT_FILE_VERSION > 1 )
    {
        WriteString( aStream, m_sUUID );
        aFU.WriteSeparator( aStream );
    }
	WriteString( aStream, m_sId );
	aFU.WriteSeparator( aStream );
	WriteString( aStream, m_sComment );
	aFU.WriteSeparator( aStream );
	WriteString( aStream, m_sImageFile );
	aFU.WriteSeparator( aStream );
	if( ACT_FILE_VERSION > 0 )
	{
		WriteString( aStream, m_sScript );
		aFU.WriteSeparator( aStream );
	}
	aStream << m_dIntensityInPercent;
	aFU.WriteSeparator( aStream );
	aStream << m_iFormat;
	aFU.WriteSeparator( aStream );
    if( ACT_FILE_VERSION > 2 )
    {
        aStream << m_relX;
        aFU.WriteSeparator( aStream );
        aStream << m_relY;
        aFU.WriteSeparator( aStream );
        aStream << m_relDX;
        aFU.WriteSeparator( aStream );
        aStream << m_relDY;
        aFU.WriteSeparator( aStream );
    }
	aStream << endl;
	m_aTimeOpContainer.Write( aStream );
	aFU.WriteStructEnd( aStream );

	if( aStream.good() )
	{
		// cast because of const, change only transient data...
		((DiaInfo *)this)->m_aObjectChanged.ClearChanged();
	}

	return aStream.good();
}

void DiaInfo::SetData( const string & sId, const string & sImageFile, const string & sComment, const string & sScript )
{
	m_sId			= sId;
	m_sImageFile	= sImageFile;
	m_sComment		= sComment;
	m_sScript		= sScript;

	m_aObjectChanged.SetChanged();
}

bool DiaInfo::IsEmpty() const
{
	return (m_sId=="") && (m_sImageFile=="") && (m_sComment=="") && (m_sScript=="");
}

bool DiaInfo::IsHorizontalFormat() const
{
	return m_iFormat == HORIZONTAL;
}

bool DiaInfo::HasScript() const
{
	return m_sScript.length() > 0;
}

bool DiaInfo::HasText( const string & sText ) const
{
	if( (m_sId.find( sText )!=string::npos) ||
		(m_sComment.find( sText )!=string::npos) ||
		(m_sImageFile.find( sText )!=string::npos) ||
		(m_sScript.find( sText )!=string::npos) )
	{
		return true;
	}
	return false;
}

bool DiaInfo::GetHorizontalFormat() const
{
	if( m_iFormat == HORIZONTAL )
	{
		return true;
	}
	else
	{
		return false;
	}
}

double DiaInfo::GetRelX() const
{
    return m_relX;
}

double DiaInfo::GetRelY() const
{
    return m_relY;
}

double DiaInfo::GetRelDX() const
{
    return m_relDX;
}

double DiaInfo::GetRelDY() const
{
    return m_relDY;
}

bool DiaInfo::SetRelX( double dVal )
{
    m_relX = dVal;
    return true;
}

bool DiaInfo::SetRelY( double dVal )
{
    m_relY = dVal;
    return true;
}

bool DiaInfo::SetRelDX( double dVal )
{
    m_relDX = dVal;
    return true;
}

bool DiaInfo::SetRelDY( double dVal )
{
    m_relDY = dVal;
    return true;
}

bool DiaInfo::SetHorizontalFormat( bool bValue )
{
	if( bValue )
	{
		m_iFormat = HORIZONTAL;
	}
	else
	{
		m_iFormat = VERTICAL;
	}
	return true;
}

int DiaInfo::GetOperationCount() const
{
	return m_aTimeOpContainer.size();
}

TimeOperation DiaInfo::GetOperation( int iIndex ) const
{
	if( iIndex>=0 && iIndex<GetOperationCount() )
	{
		return m_aTimeOpContainer[ iIndex ];
	}
	return TimeOperation();
}

TimeOperation &	DiaInfo::AccessOperationAndChanged( int iIndex )
{
	m_aObjectChanged.SetChanged();

	return m_aTimeOpContainer[ iIndex ];
}

void DiaInfo::ClearAllOperations()
{
	m_aTimeOpContainer.erase( m_aTimeOpContainer.begin(), m_aTimeOpContainer.end() );

	m_aObjectChanged.SetChanged();
}

void DiaInfo::AddOperation( const TimeOperation & aOperation )
{
	m_aTimeOpContainer.push_back( aOperation );

	m_aObjectChanged.SetChanged();
}

bool DiaInfo::ModifyOperation( int iIndex, const TimeOperation & aOperation )
{
	if( iIndex>=0 && iIndex<GetOperationCount() )
	{
		m_aTimeOpContainer[ iIndex ] = aOperation;

		m_aObjectChanged.SetChanged();

		return true;
	}
	return false;
}

void DiaInfo::MakeRelativePaths()
{
    m_sImageFile = ConvertToRelPath( m_sImageFile );
}

void DiaInfo::MakeAbsolutePaths( const string & sDir )
{
    m_sImageFile = ConvertToAbsPath( m_sImageFile, sDir );
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

XmlTree	TimeOperationContainer::GetXMLTree() const
{
	XmlTree aTree( "timeoperations" );

	aTree.SetAsContainer( true );

	const_iterator aIter = begin();
	while( aIter != end() )
	{
		aTree.PushTag( (*aIter).GetXMLTree() );

		++aIter;
	}

	return aTree;
}

bool TimeOperationContainer::Read( istream & aStream )
{
	return ReadContainer( *this, aStream );
}

bool TimeOperationContainer::Write( ostream & aStream ) const
{
	return WriteContainer( *this, aStream );
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

const int TimeOperation::ACT_FILE_VERSION = 0;

// temp. constants because of Borland C++ 5.5 
const int _UNDEFINED = -1;
const int _DISSOLVE_IN = 0;
const int _SHOW = 1;
const int _DISSOLVE_OUT = 2;
const int _FLASH = 3;
const int TimeOperation::UNDEFINED		= _UNDEFINED;
const int TimeOperation::DISSOLVE_IN	= _DISSOLVE_IN;
const int TimeOperation::SHOW			= _SHOW;
const int TimeOperation::DISSOLVE_OUT	= _DISSOLVE_OUT;
const int TimeOperation::FLASH			= _FLASH;


TimeOperation::TimeOperation( int iOperationType, double dOperationTime )
{
	m_iOperationType = iOperationType;
	m_dOperationTime = dOperationTime;
}

string TimeOperation::GetOperationTypeName() const
{
	string s;

	switch( m_iOperationType )
	{
		case _DISSOLVE_IN :
			s = "IN";
			break;
		case _DISSOLVE_OUT :
			s = "OUT";
			break;
		case _SHOW :
			s = "SHOW";
			break;
		case _FLASH :
			s = "FLASH";
			break;
		default:
			s = "UNDEFINED";
	}

	return s;
}

int	TimeOperation::GetOperationType() const
{
	return m_iOperationType;
}

double TimeOperation::GetOperationTime() const
{
	return m_dOperationTime;
}

bool TimeOperation::SetOperationTime( double dTime )
{
	if( dTime>=0 )
	{
		m_dOperationTime = dTime;
		return true;
	}
	return false;
}

XmlTree	TimeOperation::GetXMLTree() const
{
	XmlTree aTree( "timeoperation", "", false, XmlAttribList( "fileversion", ToString( ACT_FILE_VERSION ) ) );

	aTree.PushTag( "optype", m_iOperationType );
	aTree.PushTag( "optime", m_dOperationTime, XmlAttribList( "scale", "s" ) );

	return aTree;
}

bool TimeOperation::Read( istream & aStream )
{
	FileUtilityObj aFU;
	int iActFileVersion = 0;

	aFU.ReadStructBegin( aStream );
	aStream >> iActFileVersion;
	aFU.ReadSeparator( aStream );
	aStream >> m_iOperationType;
	aFU.ReadSeparator( aStream );
	aStream >> m_dOperationTime;
	aFU.ReadStructEnd( aStream );

	return aStream.good();
}

bool TimeOperation::Write( ostream & aStream ) const
{
	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	aStream << ACT_FILE_VERSION;			// since 21.10.2001
	aFU.WriteSeparator( aStream );
	aStream << m_iOperationType;
	aFU.WriteSeparator( aStream );
	aStream << m_dOperationTime;
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}


/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/minsrv/miniini.cpp,v $
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

#include "miniini.h"

#include <fstream>

#include <stdio.h>			// for: sprintf

// *******************************************************************

static const string c_sCommentStart = "#";
static const string c_sAssignment	= "=";

const int _MAX_BUFFER				= 512;

const int _COMMENT_LINE				= 0;
const int _VALUE_LINE				= 1;

// *******************************************************************

MiniIniDB::MiniIniDB( const string & sFileName )
: m_sFileName( sFileName )
{
	Load();
}

MiniIniDB::~MiniIniDB()
{
	Save();
}

void MiniIniDB::Clear()
{
	erase( begin(), end() );
	m_aFileAsText.erase( m_aFileAsText.begin(), m_aFileAsText.end() );
}

bool MiniIniDB::Load()
{
	return ReadFromFile( m_sFileName.c_str() );
}

bool MiniIniDB::Save()
{
	return WriteToFile( m_sFileName.c_str() );
}

bool MiniIniDB::HasKey( const string & sKey ) const
{
	return find( sKey ) != end();
}

int MiniIniDB::GetValueAsInt( const string & sKey ) const
{
	string sValue = GetValue( sKey );
	
	if( sValue.length()>0 )
	{
		return atoi( sValue.c_str() );
	}

	return 0;
}

double MiniIniDB::GetValueAsDouble( const string & sKey ) const
{
	string sValue = GetValue( sKey );
	
	if( sValue.length()>0 )
	{
		return atof( sValue.c_str() );
	}

	return 0;
}

string MiniIniDB::GetValue( const string & sKey ) const
{
	const_iterator aIter = find( sKey );
	if( aIter != end() )
	{
		return (*aIter).second;
	}
	return "";
}

string & MiniIniDB::GetValueRef( const string & sKey )
{
	return (*this)[ sKey ];
}

bool MiniIniDB::Add( const string & sKey, const string & sValue )
{
	(*this)[ sKey ] = sValue;

	return true;
}

void MiniIniDB::PushUniqueToHistoryQueue( const string & sQueueKeyId, const string & sValue, int iMaxQueueElements )
{
	// ** exists the value allready in the queue ?

	int iCount = GetHistoryQueueCount( sQueueKeyId );

	char sBuffer[_MAX_BUFFER];

	for( int i=0; i<iCount; i++ )
	{
		sprintf( sBuffer, "%s.%d", sQueueKeyId.c_str(), i );

		if( (*this)[ sBuffer ] == sValue )
		{
			// ** value exists, return now
			return;
		}
	}

	// ** value not found, push to history queue
	PushToHistoryQueue( sQueueKeyId, sValue, iMaxQueueElements );
}

void MiniIniDB::PushToHistoryQueue( const string & sQueueKeyId, const string & sValue, int iMaxQueueElements )
{
	int iCount = GetHistoryQueueCount( sQueueKeyId );

	char sBuffer[_MAX_BUFFER];
	char sBuffer2[_MAX_BUFFER];

	if( iCount < iMaxQueueElements )
	{
		sprintf( sBuffer, "%s.%d", sQueueKeyId.c_str(), iCount );

		Add( sBuffer, sValue);
	}
	else
	{
		// ** queue is full, make place for the new element 
		// ** by deleting the first element from the queue.
		// ** shift all queue elements to lower indizes (index==1 to index==0, etc.)
		for( int i=0; i<iMaxQueueElements-1; i++ )
		{
			sprintf( sBuffer,  "%s.%d", sQueueKeyId.c_str(), i );
			sprintf( sBuffer2, "%s.%d", sQueueKeyId.c_str(), i+1 );

			(*this)[ sBuffer ] = (*this)[ sBuffer2 ];
		}

		// ** and add the new element as last element
		sprintf( sBuffer,  "%s.%d", sQueueKeyId.c_str(), iMaxQueueElements-1 );
		Add( sBuffer, sValue );
	}
}

bool MiniIniDB::GetHistoryQueueValue( const string & sQueueKeyId, int iIndex, string & sValueOut ) const
{
	char sBuffer[_MAX_BUFFER];

	sprintf( sBuffer, "%s.%d", sQueueKeyId.c_str(), iIndex );
	if( HasKey( sBuffer ) )
	{
		sValueOut = GetValue( sBuffer );
		return true;
	}

	return false;
}

int MiniIniDB::GetHistoryQueueCount( const string & sQueueKeyId ) const
{
	int iCount = 0;

	char sBuffer[_MAX_BUFFER];

	do {
		sprintf( sBuffer, "%s.%d", sQueueKeyId.c_str(), iCount );
		iCount++;
	} while( HasKey( sBuffer ) );

	return iCount-1;	// we run at least once through the loop !
}

bool MiniIniDB::WriteToFile( const char * sFileName ) const
{
	fstream aFile( sFileName, ios::out );

	if( aFile.good() )
	{
		return Write( aFile );
	}

	return false;
}

bool MiniIniDB::ReadFromFile( const char * sFileName )
{
	fstream aFile( sFileName, ios::in );

	if( aFile.good() )
	{
		return Read( aFile );
	}

	return false;
}

bool MiniIniDB::Write( ostream & aStream ) const
{
	LineContainer::const_iterator aIter = m_aFileAsText.begin();

	map< string, string > aTempMap = *this;

	// ** output all the key/value pairs and comments found in originally file
	while( aIter != m_aFileAsText.end() )
	{
		LineContainer::const_reference aItem = *aIter;

		if( aItem.first == _VALUE_LINE )
		{
			const_iterator aFoundIter = find( aItem.second );

			if( aFoundIter != end() )
			{
				aStream << aItem.second.c_str() << c_sAssignment.c_str() << (*aFoundIter).second.c_str() << endl;

				// ** remove item from temp map, 
				// ** so we can see if we have all items writen to the stream.
				aTempMap.erase( aItem.second );
			}
			else
			{
				// ** key is removed from the original file !
			}
		}
		else if( aItem.first == _COMMENT_LINE )
		{
			aStream << aItem.second.c_str() << endl;
		}
		else
		{
			//assert
		}

		++aIter;
	}

	// ** than output all other (new) key/value pairs
	{
		map< string, string >::const_iterator aIter = aTempMap.begin();

		while( aIter != aTempMap.end() )
		{
			map< string, string >::const_reference aItem = *aIter;

			aStream << aItem.first.c_str() << c_sAssignment.c_str() << aItem.second.c_str() << endl;

			++aIter;
		}
	}

	return aStream.good();
}

bool MiniIniDB::Read( istream & aStream )
{
	Clear();

	while( aStream.good() && !aStream.eof() )
	{
		char sBuffer[ _MAX_BUFFER ];

		aStream.getline( sBuffer, _MAX_BUFFER );

		string sLine = sBuffer;

		if( !IsCommentLine( sLine ) && !IsEmptyLine( sLine ) )
		{
			string sKey;
			string sValue;

			if( ProcessLine( sLine, sKey, sValue ) )
			{
				Add( sKey, sValue );

				m_aFileAsText.push_back( LineItem( _VALUE_LINE, sKey ) );
			}
			else
			{
				m_aFileAsText.push_back( LineItem( _COMMENT_LINE, sLine ) );
			}
		}
		else
		{
			// ** save all comment lines, to conserve the format of the ini-file
			m_aFileAsText.push_back( LineItem( _COMMENT_LINE, sLine ) );
		}
	}

	// ignore the last comment line, otherwise the ini-file will increase
	if( (m_aFileAsText.begin() != m_aFileAsText.end()) &&
		((*(m_aFileAsText.end()-1)).first == _COMMENT_LINE) )
	{
		m_aFileAsText.erase( m_aFileAsText.end()-1 );
	}

	return aStream.good();
}

bool MiniIniDB::IsCommentLine( const string & sLine ) const
{
	unsigned int iPos = sLine.find( c_sCommentStart );

	return iPos != string::npos;
}

bool MiniIniDB::IsEmptyLine( const string & sLine ) const
{
	if( sLine.length()==0 )
	{
		return true;
	}

	return false;
}

bool MiniIniDB::ProcessLine( const string & sLine, string & sKeyOut, string & sValueOut ) const
{
	unsigned int iPos = sLine.find( c_sAssignment );

	if( iPos != string::npos )
	{
		sKeyOut		= sLine.substr( 0, iPos );
		sValueOut	= sLine.substr( iPos+1, sLine.length()-iPos );

		return true;
	}

	return false;
}

/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/minsrv/miniini.h,v $
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

#ifndef _MINIINI_H
#define _MINIINI_H

#ifdef _MSC_VER
// einige Compiler-Warnings ausschalten:
#pragma warning( disable : 4786 )		// Fehler-Info auf 255 Zeichen verkuerzt
#endif

#include <map>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/** Very simple ini-file / registry handling for applications.
 */
class MiniIniDB : public map< string, string >
{
	typedef pair<int,string>		LineItem;
	typedef vector< LineItem >		LineContainer;

public:
	MiniIniDB( const string & sFileName = "miniini.ini" );
	~MiniIniDB();

	void		Clear();

	bool		Load();
	bool		Save();

	bool		HasKey( const string & sKey ) const;

	int			GetValueAsInt( const string & sKey ) const;
	double		GetValueAsDouble( const string & sKey ) const;
	string		GetValue( const string & sKey ) const;
	string &	GetValueRef( const string & sKey );

	bool		Add( const string & sKey, const string & sValue );

	void		PushUniqueToHistoryQueue( const string & sQueueKeyId, const string & sValue, int iMaxQueueElements = 6 );
	void		PushToHistoryQueue( const string & sQueueKeyId, const string & sValue, int iMaxQueueElements = 6 );
	bool		GetHistoryQueueValue( const string & sQueueKeyId, int iIndex, string & sValueOut ) const;
	int			GetHistoryQueueCount( const string & sQueueKeyId ) const;

	bool		WriteToFile( const char * sFileName ) const;
	bool		ReadFromFile( const char * sFileName );

	bool		Write( ostream & aStream ) const;
	bool		Read( istream & aStream );

public:
	// ** helper methods **
	bool		IsCommentLine( const string & sLine ) const;
	bool		IsEmptyLine( const string & sLine ) const;
	bool		ProcessLine( const string & sLine, string & sKeyOut, string & sValueOut ) const;

	// ** data **
	string				m_sFileName;

	// ** temp **
	LineContainer		m_aFileAsText;		// to save the original layout of the ini-file
};

#endif

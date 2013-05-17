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

// ** this file is from the old mcp-project (1992-1996) **

#ifndef _MINUTILS_H
#define _MINUTILS_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// *************************************************************************
/* some definitions for FileUtilityObj: */
/*--------------------------------------*/
#define _NullChar		'\x0'                /*   every definition starts with _ */
#define _Space			32
#define _Tab			8
#define _CR				13
#define _LF				10
#define _DataFileBegin  '@'
#define _DataFileEnd    '@'
#define _StructBegin    '<'
#define _StructEnd      '>'
#define _ControlBegin  '('
#define _ControlEnd    ')'
#define _TypeIdBegin    '{'
#define _TypeIdEnd      '}'
#define _NameBegin      '['
#define _NameEnd        ']'
#define _StringBegin    '"'
#define _StringEnd      '"'
#define _Separator      ','
#define _Equal			'='
#define _ContinueChar   'c'
#define _BreakChar      'q'
#define _SpecialChar	'\\'
#define _bad_double     0.0
#define _bad_long       0

// *************************************************************************
/** Klasse zur Unterstuetzung von Ausgaben auf Streams */
class FileUtilityObj
{
public:
  static unsigned long GetFileSize( const char * sFileName );

  char PeekChar(istream& IS);                       // peeks (looks for but not reads) next char != CR, LF, TAB, SPACE
/* io-operations for formated io: */
  bool ReadDataFileBegin(istream& IS)		{ return ReadToChar(IS,_DataFileBegin); }
  bool ReadDataFileEnd(istream& IS)			{ return ReadToChar(IS,_DataFileEnd); }
  bool ReadStructBegin(istream& IS)			{ return ReadToChar(IS,_StructBegin); }
  bool ReadStructEnd(istream& IS)			{ return ReadToChar(IS,_StructEnd); }
  bool ReadControlBegin(istream& IS)		{ return ReadToChar(IS,_ControlBegin); }
  bool ReadControlEnd(istream& IS)			{ return ReadToChar(IS,_ControlEnd); }
  bool ReadNameBegin(istream& IS)			{ return ReadToChar(IS,_NameBegin); }
  bool ReadNameEnd(istream& IS)				{ return ReadToChar(IS,_NameEnd); }
  bool ReadTypeBegin(istream& IS)			{ return ReadToChar(IS,_TypeIdBegin); }
  bool ReadTypeEnd(istream& IS)				{ return ReadToChar(IS,_TypeIdEnd); }
  bool ReadStringBegin(istream& IS)			{ return ReadToChar(IS,_StringBegin); }
  bool ReadStringCharOrEnd(istream& IS, char & ch);
  bool ReadStringCharOrEndExt(istream& IS, char & ch);
  bool ReadSeparator(istream& IS)			{ return ReadToChar(IS,_Separator); }
  bool ReadEqual(istream& IS)				{ return ReadToChar(IS,_Equal); }
  int PeekStringBegin(istream& IS)			{ return (PeekChar(IS)==_StringBegin); }
  int PeekControlBegin(istream& IS)			{ return (PeekChar(IS)==_ControlBegin); }
  int PeekControlEnd(istream& IS)			{ return (PeekChar(IS)==_ControlEnd); }
  int PeekStructBegin(istream& IS)			{ return (PeekChar(IS)==_StructBegin); }
  int PeekStructEnd(istream& IS)			{ return (PeekChar(IS)==_StructEnd); }
  int PeekSeparator(istream& IS)			{ return (PeekChar(IS)==_Separator); }
  int PeekEqual(istream& IS)				{ return (PeekChar(IS)==_Equal); }
  int PeekTypeInfoBegin(istream& IS)		{ return (PeekChar(IS)==_TypeIdBegin); }
  int ReadSeparatorOrStructEnd(istream& IS);        // == 1 if Separator or StructEnd found, else = 0
  void WriteDataFileBegin(ostream& OS)		{ Write(OS,_DataFileBegin); }
  void WriteDataFileEnd(ostream& OS)		{ Write(OS,_DataFileEnd); }
  void WriteStructBegin(ostream& OS)		{ Write(OS,_StructBegin); }
  void WriteStructEnd(ostream& OS)			{ Write(OS,_StructEnd); }
  void WriteControlBegin(ostream& OS)		{ Write(OS,_ControlBegin); }
  void WriteControlEnd(ostream& OS)		{ Write(OS,_ControlEnd); }
  void WriteNameBegin(ostream& OS)			{ Write(OS,_NameBegin); }
  void WriteNameEnd(ostream& OS)			{ Write(OS,_NameEnd); }
  void WriteTypeBegin(ostream& OS)			{ Write(OS,_TypeIdBegin); }
  void WriteTypeEnd(ostream& OS)			{ Write(OS,_TypeIdEnd); }
  void WriteStringBegin(ostream& OS)		{ Write(OS,_StringBegin); }
  void WriteStringEnd(ostream& OS)			{ Write(OS,_StringEnd); }
  void WriteSeparator(ostream& OS)			{ Write(OS,_Separator); }
  void WriteEqual(ostream& OS)				{ Write(OS,_Equal); }
  // Methoden zum ueberspringen von Bloecken:
  bool ReadBlockToStructEnd(istream& IS);
/* other things... */
  bool WriteHeader( ostream& OS, const char * sHeaderMsg, int nFileVersion );
  bool ReadHeader( istream& IS, const char * sHeaderMsg, int & nFileVersion );

  static bool GetOnlyFileName( const char * sFileName, char * sOnlyFileName );
  static bool CheckDirSeparatorAtEnd( char * sPath );
  static bool GetTmpPath( char * sTempPath );
  static bool MakeRelPathsToAbsPaths( char * sName );

  static string ConvertToRelPath( const char * sFileName );
  static string StripPath( const char * sFileName, string * psPath );

private:
  bool Write(ostream& OS, char ch)
	{
	  OS.put(ch);
	  return OS.good();
	}
  bool Read(istream& IS, char& ch)
	{
	  IS.get(ch);
	  return IS.good();
	  //DEBUG_FILEUTILITY(ch)
	}
  bool ReadToChar(istream& IS, char ch)
	{
	  char ch_buffer;
	  bool bOk;

	  while( (bOk=Read(IS,ch_buffer)) && (ch_buffer != ch) && (ch_buffer != IS.eof()) )
	  {
	  }
	  /*
	  do {
		bOk = Read(IS,ch_buffer);
	  } while (bOk && (ch_buffer != ch) && (ch_buffer != EOF));
	  */
	  if ((ch_buffer==IS.eof()/*std::eof*/) /*&& (ch_buffer!=EOF)*/ )		// auskommentiert am 27.1.1999
		 { bOk = false; };
	  return bOk;
	}
};

// *******************************************************************
// *******************************************************************
// *******************************************************************

bool ReadString( istream & aIn, string & sString );
bool WriteString( ostream & aOut, const string & sString );

// *******************************************************************

template<class Container> bool WriteContainer( const Container & aContainer, ostream & aStream )
{
	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );

	typename Container::const_iterator aIter = aContainer.begin();
	while( aIter != aContainer.end() )
	{
		(*aIter).Write( aStream );

		++aIter;

		if( aIter != aContainer.end() )
		{
			aFU.WriteSeparator( aStream );
			aStream << endl;
		}
	}

	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

template<class Container> bool ReadContainer( Container & aContainer, istream & aStream )
{
	// clear container first
	aContainer.erase( aContainer.begin(), aContainer.end() );

	// and than read the container contents from the stream
	FileUtilityObj aFU;

	aFU.ReadStructBegin( aStream );

	while( !aStream.eof() && !aFU.PeekStructEnd( aStream ) && aStream.good() )
	{
		typename Container::value_type aElement;

		aElement.Read( aStream );

		aContainer.push_back( aElement );

		if( aFU.PeekSeparator( aStream ) )
		{
			aFU.ReadSeparator( aStream );
		}
	}

	aFU.ReadStructEnd( aStream );

	return aStream.good();
}

// *******************************************************************

template<class Container> bool WriteContainerPtr( const Container & aContainer, ostream & aStream )
{
	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );

	typename Container::const_iterator aIter = aContainer.begin();
	while( aIter != aContainer.end() )
	{
		(*aIter)->Write( aStream );

		++aIter;

		if( aIter != aContainer.end() )
		{
			aFU.WriteSeparator( aStream );
			aStream << endl;
		}
	}

	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

template<class Container> bool ReadContainerPtr( Container & aContainer, istream & aStream )
{
	// clear container first
	aContainer.erase( aContainer.begin(), aContainer.end() );

	// and than read the container contents from the stream
	FileUtilityObj aFU;

	aFU.ReadStructBegin( aStream );

	while( !aStream.eof() && !aFU.PeekStructEnd( aStream ) && aStream.good() )
	{
		typename Container::value_type aElement( new typename Container::ItemClass );

		aElement->Read( aStream );

		aContainer.push_back( aElement );

		if( aFU.PeekSeparator( aStream ) )
		{
			aFU.ReadSeparator( aStream );
		}
	}

	aFU.ReadStructEnd( aStream );

	return aStream.good();
}

// *******************************************************************
/** Helperclass to handle the modification state for an object.
  */
class ObjectChanged
{
public:
	ObjectChanged();

	bool IsChanged() const;

	void SetChanged();
	void ClearChanged();

private:
	// *** transient data ***
	bool	m_bChanged;
};

// *******************************************************************

bool ReadTextFile( const char * sFullFileName, string & sText );
bool WriteTextFile( const char * sFullFileName, const string & sText, bool bAppend = false );

// *******************************************************************

string ToString( int iValue );
string ToString( double dValue );

#endif

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
 * Copyright (C) 2012 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "minutils.h"

#include <fstream>
#include <string.h>

// *************************************************************************

const int MAX_STRING_LENGTH =	8192;

// *******************************************************************

  bool FileUtilityObj::ReadStringCharOrEndExt(istream& IS, char & ch )
    {
      bool bOk = Read(IS,ch);
      if( !bOk )
         return false;
	  if(ch==_SpecialChar)					// jezt kann man " mit \" in einem String einbetten
   		return Read(IS,ch);
     /*
      else if (ch_buffer == _StringEnd)
		return 0;
  	 */
	  return ch != _StringEnd;				// liefert true, falls KEIN String Ende erreicht wurde !
    }

  char FileUtilityObj::PeekChar(istream& IS)
    {
      char ch;

      while ( ((ch=IS.peek())==_Space) || (ch==_Tab) || (ch==_CR) || (ch==_LF) )
        { ch = IS.get(); };
      return ch;
    }

  unsigned long FileUtilityObj::GetFileSize( const char *sFileName )
  {
	fstream aFile( sFileName, ios::in );

	if( aFile.good() )
	{
		// fuer die Progress-Anzeige die Dateigroesse merken
		aFile.seekg( 0, ios::end );
		return (unsigned long)aFile.tellg();
	}
 	return (unsigned long)-1;		// Datei nicht gefunden !
  }

// *******************************************************************
// *******************************************************************
// *******************************************************************

bool ReadString( istream & aIn, string & sString )
{
	FileUtilityObj aFU;
 	bool bOk = true;
	char ch = 0;
	char sCharBuffer[MAX_STRING_LENGTH];

	bOk = aFU.ReadStringBegin( aIn );
	if( bOk )
	{
		int i = 0;
		while( (i < MAX_STRING_LENGTH-1 ) && aIn.good() && !aIn.eof() && aFU.ReadStringCharOrEndExt( aIn, ch ) /*&& ch!=_StringEnd*/ )
		{ 
			sCharBuffer[i++] = ch; 
		};
		if( ch!=_StringEnd )		// if maxStringLength reached, read to _StringEnd
		{ 
			while( aIn.good() && !aIn.eof() && aFU.ReadStringCharOrEndExt( aIn, ch ) )  
				;
		};   
		sCharBuffer[i] = 0;
		sString = sCharBuffer;
		return aIn.good();
	}
 	return bOk;
}

bool WriteString( ostream & aOut, const string & sString )
{
	FileUtilityObj aFU;

	aFU.WriteStringBegin( aOut );

	//old: aOut << sString.c_str();
 	const char * sTemp = sString.c_str();
 	int nMaxLen = strlen( sTemp );
	for( int i=0; i<nMaxLen; i++ )
	{
		char ch = sTemp[i];  	  	
  		if( ch == _SpecialChar )
    		aOut << ch;				// \ immer in \\ umwandeln, d.h. zusaetzliches \ schreiben
		if( ch == _StringBegin )
  			aOut << _SpecialChar;
		aOut << ch;
    }

	aFU.WriteStringEnd( aOut );
	return aOut.good();
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

ObjectChanged::ObjectChanged()
{
	m_bChanged = false;
}

bool ObjectChanged::IsChanged() const
{
	return m_bChanged;
}

void ObjectChanged::SetChanged()
{
	m_bChanged = true;
}

void ObjectChanged::ClearChanged()
{
	m_bChanged = false;
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

// liest ein Textfile in einen String ein
bool ReadTextFile( const char * sFullFileName, string & sText )
{
	fstream aFile( sFullFileName, ios::in /*| ios::binary*/ );
 	unsigned long nSize = FileUtilityObj::GetFileSize( sFullFileName );

	if( aFile.good() )
	{
    	char * sBuffer = new char [ nSize + 1 ];	// Bugfix (+1): 12.1.2000

		unsigned long i = 0;
		for( i=0; (i<nSize) && (!aFile.eof()); i++ )
		{
			aFile.get( sBuffer[i] );
		}
		sBuffer[ (nSize == i) ? nSize : i-1 ] = 0;
  		sText = sBuffer;
		delete [] sBuffer;
		return true;
	}
	return false;
}

bool WriteTextFile( const char * sFullFileName, const string & sText, bool bAppend )
{
	fstream aFile( sFullFileName, (bAppend ? ios::app : ios::out) );

	if( aFile.good() )
	{
		aFile << sText.c_str();
		return true;
	}
	return false;
}

string ToString( int iValue )
{
	char sBuffer[255];

	sprintf( sBuffer, "%d", iValue );

	return sBuffer;
}

string ToString( double dValue )
{
	char sBuffer[255];

	sprintf( sBuffer, "%lf", dValue );

	return sBuffer;
}


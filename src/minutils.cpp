/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/minutils.cpp,v $
 *
 *  $Revision: 1.7 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.6  2004/02/16 19:46:56  min
 *	Fixes for Borland C++
 *	
 *	Revision 1.5  2004/01/28 19:11:11  min
 *	Bugfix for linux: robuster directory scan
 *	
 *	Revision 1.4  2003/10/26 22:40:18  min
 *	Bugfixes for linux port
 *	
 *	Revision 1.3  2003/10/26 17:21:01  min
 *	New Methods include: IsAbsPath(), ConvertToRelPath();
 *	
 *	Revision 1.2  2003/10/03 23:00:41  min
 *	Bugfix in GetNoOfFiles() not only readonly files counting.
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

#include <fstream>
#include <algorithm>
#include <functional>

#include <QRegExp>

#include "minutils.h"

#include "osdep2.h"

#include <stdio.h>

#ifdef _WIN32
#include <io.h>
#endif

#ifdef __BORLANDC__
#include <dos.h>
#endif

// TODO
#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/stat.h>			// fuer: mkdir()
#include <dirent.h>				// fuer scandir, seekdir, etc.
#include <time.h>
#include <qregexp.h>			// for wildcard search (*.py)
#else
#include <direct.h>
#endif

// *******************************************************************

#ifdef _WIN32
#define CHAR_CAST const char *
#endif
#if defined(__linux__) || defined(__APPLE__)
#define CHAR_CAST const char *
#endif

#ifdef _WIN32
typedef struct
{
	WORD day:5;
	WORD month:4;
	WORD year:7;
} DOSDATE;

typedef struct
{
	WORD twosecs:5;
	WORD minutes:6;
	WORD hours:5;
} DOSTIME;
#endif

// *************************************************************************

const int MAX_STRING_LENGTH =	8192;
const int MAX_BUFFER_LENGTH =	512;

// *******************************************************************

#if defined(__linux__) || defined(__APPLE__)
  // Hilfsfunktion fuer SplitPath()
  static void _InsertChar( char * sBuffer, char ch )
  {
	int nLen = strlen( sBuffer );
	sBuffer[ nLen ] = ch;
	sBuffer[ nLen+1 ] = 0;
  }

  // Hilfsfunktion fuer SplitPath()
  // Transformation durchfuehren "abc" --> "cba"
  static void _RotateString( char * sBuffer )
  {
	char sBuf[MAX_BUFFER_LENGTH];
	int nLen = strlen( sBuffer );
	for( int i=0; i<nLen; i++ )
	{
		sBuf[ nLen-i-1 ] = sBuffer[i];
	}
	sBuf[ nLen ] = 0;
	strcpy( sBuffer, sBuf );
  }

#endif
  
  // function necassary because of linux gcc3.3
  static char _mytolower( char ch )
  {
  	return tolower( ch );
  }

  string string_tolower( const string & s )
  {
	  string sTemp = s;
      transform( sTemp.begin(), sTemp.end(), sTemp.begin(), _mytolower );
	  return sTemp;
  }

  vector<string> SplitPathInItems( const string & sPath )
  {
	  vector<string> aContainer;

	  string sDir;

	  int iLength = sPath.length();
	  for( int i=0; i<iLength; i++ )
	  {
		  if( sPath[i]=='/' || sPath[i]=='\\' )
		  {
			  if( sDir.length()>0 )
			  {
				  aContainer.push_back( sDir );
			  }
			  sDir = "";
		  }
		  else
		  {
			  sDir += sPath[i];
		  }
	  }

	  return aContainer;
  }

  // strip the path information from a filename.
  // example: c:\data\test.dat --> test.dat
  string FileUtilityObj::StripPath( const char * strFileName, string * psPath )
  {
	  // split the path
	  string sDrive;
	  string sDir;
	  string sFileName;
	  string sExt;
	  SplitPath( strFileName, sDrive, sDir, sFileName, sExt );

      if( psPath )
	  {
		  *psPath = sDrive+sDir;
	  }

	  return sFileName+sExt;
  }

  // convert an absolute path to a relative path.
  // example: c:\data\sim\test.dat --> ..\..\sim\test.dat
  string FileUtilityObj::ConvertToRelPath( const char * strFileName )
  {
	  // split the path
	  string sDrive;
	  string sDir;
	  string sFileName;
	  string sExt;
	  SplitPath( strFileName, sDrive, sDir, sFileName, sExt );

	  // if actual drive != drive of path ? --> error
#ifdef _WIN32
	  if( sDrive.length()>0 )
	  {
		  string sActDrive;

		  sActDrive += (char)(_getdrive()-1+'A');

		  if( string_tolower( sActDrive ) == string_tolower( sDrive ) )
		  {
			  return strFileName;
		  }
	  }
#endif

	  // pfad relativ machen
	  if( IsAbsPath( sDir.c_str() ) )
	  {
		  char sBuffer[MAX_BUFFER_LENGTH];

		  string sActPath = getcwd( sBuffer, MAX_BUFFER_LENGTH );
		  sActPath += GetDirectorySeparator();
		  string _sDrive;
		  string sActDir;
		  string _sFileName;
		  string _sExt;
		  SplitPath( sActPath.c_str(), _sDrive, sActDir, _sFileName, _sExt );

#ifdef _WIN32
		  sActDir = string_tolower( sActDir );
		  sDir = string_tolower( sDir );
#endif

		  // relative path = absolute path - (actual drive + actual directory)
		  // ../../../f/g    = /a/f/g   /a/b/c/d
		  // ../../../../f/g = /x/f/g   /a/b/c/d
		  // ./d             = /a/b/c/d /a/b/c

		  vector<string> aActDir = SplitPathInItems( sActDir );
		  vector<string> aPathDir = SplitPathInItems( sDir );

		  // subtract equal path-parts (from root)
		  // (/a) = /f/g  /b/c/d
		  int i = 0;
		  for( i=0; i<(int)min(aActDir.size(),aPathDir.size()); i++ )
		  {
			  if( aActDir[i] != aPathDir[i] )
			  {
				  break;
			  }
		  }
		  if( i>0 )
		  {
			  aActDir.erase( aActDir.begin(), aActDir.begin()+i );
			  aPathDir.erase( aPathDir.begin(), aPathDir.begin()+i );
		  }

		  // count the directoreis and add the rest of the path
		  // ../../../f/g
		  string s;
		  const string sCdUp = "..";

		  if( i==0 )
		  {
			  s += sCdUp;
			  s += GetDirectorySeparator();
		  }

		  // absolute path is the same (and longer) than the actual path
		  if( aActDir.size()==0 )
		  {
			  s += ".";
			  s += GetDirectorySeparator();
		  }
		  for( i=0; i<(int)aActDir.size(); i++ )
		  {
			  s += sCdUp;
			  s += GetDirectorySeparator();
		  }
		  for( i=0; i<(int)aPathDir.size(); i++ )
		  {
			  s += aPathDir[i];
			  s += GetDirectorySeparator();
		  }
		  s += sFileName;
		  s += sExt;

		  return s;
	  }

	  // it was already a relative path
	  return strFileName;
  }

  bool FileUtilityObj::IsAbsPath( const char * sPath )
  {
	string sDriveBuf;
	string sDirBuf;
	string sNameBuf;
	string sExtBuf;

	SplitPath( sPath, sDriveBuf, sDirBuf, sNameBuf, sExtBuf );

	if( sDirBuf.length()>0 && (sDirBuf[0]=='/' || sDirBuf[0]=='\\') )
	{
		return true;
	}

	return false;
  }

  bool FileUtilityObj::SplitPath( const char * sPath, string & sDrive, string & sDir, string & sFileName, string & sExt )
  {
	char sDriveBuf[12];
	char sDirBuf[MAX_BUFFER_LENGTH];
	char sNameBuf[MAX_BUFFER_LENGTH];
	char sExtBuf[MAX_BUFFER_LENGTH];
#if defined(__linux__) || defined(__APPLE__)
	// Simuliere _splitpath fuer Linux...
	strcpy( sDriveBuf, "" );
	strcpy( sDirBuf, "" );
	strcpy( sNameBuf, "" );
	strcpy( sExtBuf, "" );
	// Pfad von hinten analysieren
	char sBuffer[MAX_BUFFER_LENGTH];
	strcpy( sBuffer, "" );
	bool bFoundExt = false;
	bool bFoundFile = false;
	for( int i=strlen( sPath )-1; i>=0; i-- )
	{
		// ist the path something like that: '/abc/def/'
		if( !bFoundExt && sPath[i]==FileUtilityObj::GetDirectorySeparator() )
		{
			bFoundExt = true;
			bFoundFile = true;
			// means: no extention and no filename found !
			strcpy( sExtBuf, "" );			// bugfix 1.6.2004
			_RotateString( sBuffer );		// bugfix 1.6.2004
			strcpy( sNameBuf, sBuffer );
			strcpy( sBuffer, "" );
		}
		if( !bFoundExt && !bFoundFile && sPath[i]=='.' )
		{
			bFoundExt = true;
			_InsertChar( sBuffer, sPath[i] );
			_RotateString( sBuffer );
			strcpy( sExtBuf, sBuffer );
			strcpy( sBuffer, "" );
		}
		else if( !bFoundFile && sPath[i]==FileUtilityObj::GetDirectorySeparator() )
		{
			bFoundFile = true;
			_RotateString( sBuffer );
			strcpy( sNameBuf, sBuffer );
			strcpy( sBuffer, "" );
			_InsertChar( sBuffer, sPath[i] );
		}
		else
		{
			_InsertChar( sBuffer, sPath[i] );
		}
	}
	_RotateString( sBuffer );
	if( bFoundFile )
	{
		strcpy( sDirBuf, sBuffer );
	}
	else
	{
		strcpy( sNameBuf, sBuffer );
	}
#else
	_splitpath( /*(CHAR_CAST)*/sPath, sDriveBuf, sDirBuf, sNameBuf, sExtBuf );
#endif
	sDrive = sDriveBuf;
	sDir = sDirBuf;
	sFileName = sNameBuf;
	sExt = sExtBuf;
	//cout << "SPLIT: " << sPath << " --> " << sDirBuf << " *** " << sNameBuf << " *** " << sExtBuf << endl;
	return true;
  }

  string FileUtilityObj::GetFullPath( const char * sFileName )
  {
	char sBuffer[2048];
	strcpy( sBuffer, "" );

#ifdef _WIN32
	char * pBuffer;
	::GetFullPathName( (LPCTSTR)sFileName, 2048, (LPTSTR)sBuffer, (LPTSTR *)&pBuffer );
#endif
#if defined(__linux__) || defined(__APPLE__)

#ifdef _do_not_ignore_linux_errors
#error not implemented yet
#endif
	strcpy( sBuffer, sFileName );		// wenigstens den original Filenamen liefern, TODO verbessern !!!
#endif

	return sBuffer;
  }

    const char * FileUtilityObj::GetDirectorySeparatorStrg()
    {
#if defined(__linux__) || defined(__APPLE__)
	  return "/";
#else
	  return "\\";
#endif
    }

  char FileUtilityObj::GetDirectorySeparator()
    {
#if defined(__linux__) || defined(__APPLE__)

	  return '/';
#else
	  return '\\';
#endif
    }


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

  bool FileUtilityObj::_DeleteFile( const char * sFileName )
  {
	  return remove( sFileName ) == 0;
  }

  bool FileUtilityObj::_RenameFile( const char * sName, const char * sNewName )
  {
  	  return rename( sName, sNewName ) == 0;
  }

  bool FileUtilityObj::_MoveFile( const char * sSource, const char * sTarget )
  {
	  bool bOk = false;
#ifdef _WIN32
	  bOk = (bool)::MoveFile( (LPCTSTR)sSource, (LPCTSTR)sTarget );
#endif
#if defined(__linux__) || defined(__APPLE__)
    sSource = sSource;
    sTarget = sTarget;
#ifdef _do_not_ignore_linux_errors
#error not implemented yet (move)
#endif
#endif
	  return bOk;
  }
  
  bool FileUtilityObj::_CopyFile( const char * sSource, const char * sTarget )
  {
	  bool bOk = false;
#ifdef _WIN32
	  bOk = (bool)::CopyFile( (LPCTSTR)sSource, (LPCTSTR)sTarget, /*bFailIfExists*/(BOOL)false );	// immer ueberschreiben
#endif
#if defined(__linux__) || defined(__APPLE__)
    sSource = sSource;
    sTarget = sTarget;
#ifdef _do_not_ignore_linux_errors
#error not implemented yet (?)
#endif
#endif
	  return bOk;
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

  bool FileUtilityObj::_CreateDirectory( const char *sDirName )
  {
#if defined(__linux__) || defined(__APPLE__)
	  return mkdir( (CHAR_CAST)sDirName, /*mode*/0 )==0;		// TODO: was ist mit mode-Argument ?
#else
	  return mkdir( (CHAR_CAST)sDirName )==0;
#endif
	  //::CreateDirectory()		WIN32
	  //::DosCreateDir()		OS/2
  }

  bool FileUtilityObj::_RemoveDirectory( const char *sDirName )
  {
	  return rmdir( (CHAR_CAST)sDirName )==0;
	  //::RemoveDirectory()		WIN32
	  //::DosRemoveDir()		OS/2
  }

  bool FileUtilityObj::ChangeActDirectory( const char *sDirName )
  {
	  return chdir( (CHAR_CAST)sDirName )==0;
	  //::SetCurrentDirectory()		WIN32
	  //::DosSetCurrentDirectory()	OS/2
  }

  string FileUtilityObj::GetActDirectory()
  {
	  char sBuffer[1024];
	  char * sRet = getcwd( sBuffer, 1023 );
	  return sRet ? string( sRet ) : string( "" );
  }

  bool FileUtilityObj::ChangeActDrive( int nDriveNo )
  {
#if defined(__linux__) || defined(__APPLE__)
      nDriveNo = nDriveNo;
	  return false;		// Problem mit Linux ? gibt kein Aequivalent !
#else
	  return _chdrive( nDriveNo )==0;
#endif
  }

//  bool FileUtilityObj::_GetFileTimeInfos( const char * sFileNameIn, FileTime & aFileTimeOut )
//  {
//	  bool bOk = false;
//#ifdef _WIN32
//	  HFILE		hFile;
//	  OFSTRUCT	aOpenFileInfo;

//	  hFile = OpenFile( sFileNameIn, &aOpenFileInfo, OF_READ );
//	  if( hFile ==  HFILE_ERROR )
//		  return false;

//	  FILETIME aCreateTime;
//	  FILETIME aLastAccessTime;
//	  FILETIME aLastWriteTime;
//	  bOk = (bool)::GetFileTime( (Qt::HANDLE)hFile, &aCreateTime, &aLastAccessTime, &aLastWriteTime );

//	  if( bOk )
//	  {
//		  FILETIME fLocTime;
//		  DOSDATE aDate;
//		  DOSTIME aTime;
//		  ::FileTimeToLocalFileTime( &aLastWriteTime, &fLocTime );
//		  ::FileTimeToDosDateTime( &fLocTime, (WORD *)&aDate, (WORD *)&aTime );

// 		  FileTime aFileTime( aDate.day, aDate.month, aDate.year+1980, aTime.hours, aTime.minutes, aTime.twosecs );
//		  aFileTimeOut = aFileTime;
//	  }

//	  CloseHandle( (Qt::HANDLE)hFile );
//#endif
//#if defined(__linux__) || defined(__APPLE__)

//	// ** todo --> test

//	struct stat aStat_p;
//	int iRet = stat( sFileNameIn, &aStat_p);

//	if( iRet == 0 )
//	{
//#if defined(Q_OS_ANDROID)
//        /*ANDROID*/                struct tm * pDate = localtime( 0 );
//#else
//                struct tm * pDate = localtime( &(aStat_p.st_ctime) );
//#endif
//                FileTime aFileTime( pDate->tm_mday, pDate->tm_mon+1, pDate->tm_year+1900, pDate->tm_hour, pDate->tm_min, pDate->tm_sec );

//                aFileTimeOut = aFileTime;

//		bOk = true;
//	}
//#endif
//	return bOk;
//  }
/*
  bool FileUtilityObj::GetFileDate( const char * sFileNameIn, string & sDateOut )
  {
//	  FileTime aFileTime;
      bool bOk = false; //FileUtilityObj::_GetFileTimeInfos( sFileNameIn, aFileTime );
//	  sDateOut = aFileTime.GetDateString();
	  return bOk;
  }

  bool FileUtilityObj::GetFileTime( const char * sFileNameIn, string & sTimeOut )
  {
//	  FileTime aFileTime;
      bool bOk = false; //FileUtilityObj::_GetFileTimeInfos( sFileNameIn, aFileTime );
//	  sTimeOut = aFileTime.GetTimeString();
	  return bOk;
  }
*/
  bool FileUtilityObj::ExistsFile(const char *sFileName)
    {
#if defined( _STL_SGI ) && defined( _MSC_VER )
      ifstream aTestFile(sFileName,/*ios::in*//*org: ios::nocreate*/ios::in | ios::nocreate);
			// Probleme bei alter stream-io unter WNT (bei Verwendung von SGI_STL) falls ohne ios::nocreate angelegt !!!
#else
      ifstream aTestFile(sFileName,/*ios::in*//*org: ios::nocreate*/ios::in );
#endif
      //bool bNotOK = !aTestFile;
      //return !bNotOK;
	  return aTestFile.good();
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


// *************************************************************************
// *************************************************************************
// *************************************************************************

//FileTime::FileTime( int nDay, int nMonth, int nYear, int nHour, int nMinute, int nSecond )
//: m_nDay( nDay ),
//  m_nMonth( nMonth ),
//  m_nYear( nYear ),
//  m_nSecond( nSecond ),
//  m_nMinute( nMinute ),
//  m_nHour( nHour )
//{
//}

//string FileTime::GetString( int /*nLanguage*/ ) const
//{
//	// TODO: laenderspezifische Ausgabe implementieren

//	char sBuffer[MAX_BUFFER_LENGTH];
//	sprintf( sBuffer,"%02d:%02d:%02d %02d.%02d.%d", m_nHour, m_nMinute, m_nSecond, m_nDay, m_nMonth, m_nYear );
//	return sBuffer;
//}

//string FileTime::GetDateString( int /*nLanguage*/ ) const
//{
//	// TODO: laenderspezifische Ausgabe implementieren

//	char sBuffer[MAX_BUFFER_LENGTH];
//	sprintf( sBuffer,"%02d.%02d.%d", m_nDay, m_nMonth, m_nYear );
//	return sBuffer;
//}

//string FileTime::GetTimeString( int /*nLanguage*/ ) const
//{
//	// TODO: laenderspezifische Ausgabe implementieren

//	char sBuffer[MAX_BUFFER_LENGTH];
//	sprintf( sBuffer,"%02d:%02d:%02d", m_nHour, m_nMinute, m_nSecond );
//	return sBuffer;
//}

//bool FileTime::operator<( const FileTime & aObj ) const
//{
//	// dies ist sicher keine optimale Implementation...

//	if( m_nYear < aObj.m_nYear )
//		return true;
//	else if( m_nYear > aObj.m_nYear )
//		return false;
//	else
//	{
//		if( m_nMonth < aObj.m_nMonth )
//			return true;
//		else if( m_nMonth > aObj.m_nMonth )
//			return false;
//		else
//		{
//			if( m_nDay < aObj.m_nDay )
//				return true;
//			else if( m_nDay > aObj.m_nDay )
//				return false;
//			else
//			{
//				if( m_nHour < aObj.m_nHour )
//					return true;
//				else if( m_nHour > aObj.m_nHour )
//					return false;
//				else
//				{
//					if( m_nMinute < aObj.m_nMinute )
//						return true;
//					else if( m_nMinute > aObj.m_nMinute )
//						return false;
//					else
//					{
//						if( m_nSecond < aObj.m_nSecond )
//							return true;
//						else if( m_nSecond > aObj.m_nSecond )
//							return false;
//						else
//						{
//							return false;
//						}
//					}
//				}
//			}
//		}
//	}
//	return false;
//}

//bool FileTime::operator==( const FileTime & aObj ) const
//{
//	if( m_nYear == aObj.m_nYear &&
//		m_nMonth == aObj.m_nMonth &&
//		m_nDay == aObj.m_nDay &&
//		m_nHour == aObj.m_nHour &&
//		m_nMinute == aObj.m_nMinute &&
//		m_nSecond == aObj.m_nSecond )
//	{
//		return true;
//	}
//	return false;
//}

//int FileTime::GetDay() const
//{
//	return m_nDay;
//}

//int	FileTime::GetMonth() const
//{
//	return m_nMonth;
//}

//int	FileTime::GetYear() const
//{
//	return m_nYear;
//}

//int	FileTime::GetSecond() const
//{
//	return m_nSecond;
//}

//int	FileTime::GetMinute() const
//{
//	return m_nMinute;
//}

//int	FileTime::GetHour() const
//{
//	return m_nHour;
//}

// *************************************************************************

//DirectoryItem::DirectoryItem( const string & sAbsFileName, FileSizeT nFileSize, const FileTime & aFileTime, FileAttributesT aFileAttributes )
//: m_sAbsFileName( sAbsFileName ),
//  m_aFileTime( aFileTime ),
//  m_nFileSize( nFileSize ),
//  m_aFileAttributes( aFileAttributes ),
//  m_bIsDeleted( false )
//{
////	cout << "new DirectoryItem() " << (void *)this << " " << sAbsFileName.c_str() << endl;
//}

///* zum testen
//DirectoryItem::DirectoryItem( const DirectoryItem & aCopyObj )
//{
//	cout << "COPY DirectoryItem() " << (void *)this << " " << aCopyObj.m_sAbsFileName.c_str() << endl;
//	m_sAbsFileName = aCopyObj.m_sAbsFileName;
//	m_aFileTime = aCopyObj.m_aFileTime;
//	m_nFileSize = aCopyObj.m_nFileSize;
//	m_aFileAttributes = aCopyObj.m_aFileAttributes;
//	m_bIsDeleted = aCopyObj.m_bIsDeleted;
//}
//*/

//DirectoryItem::~DirectoryItem()
//{
////	cout << "~DirectoryItem() " << (void *)this << endl;
//}

//string DirectoryItem::GetName() const
//{
//	string sDrive, sPath, sFileName, sFileExt;
//    FileUtilityObj::SplitPath( (CHAR_CAST)m_sAbsFileName.c_str(), sDrive, sPath, sFileName, sFileExt );

//	string sRet;

//	if( sFileName.length()>0 )
//		sRet += sFileName;
//	if( sFileExt.length()>0 )
//		sRet += sFileExt;

//	return sRet;
//}

//string DirectoryItem::GetFirstName() const
//{
//	return m_sAbsFileName;
//}

///*
//	string				GetExtention() const;		// liefert nur die Extention, d.h. alles nach dem ersten Punkt: a.b.c --> b.c
//	string				GetPath() const;			// liefert den gesammten Pfad zu dieser Datei, d.h. c:\dir1\dir2\a.b.c --> "\dir1\dir2\"
//	string				GetDrive() const;			// liefert den Laufwerksbuchstaben, d.h. "c:"
//	string				GetNetName() const;			// liefert (falls vorhanden) den Namen des Servers, d.h. \\source\dir1\dir2\a.b.c --> \\source
//*/
//FileSizeT	DirectoryItem::GetRecursiveSize() const
//{
//	if( IsDirectory() && !IsSelf() && !IsParent() )
//	{
//		// rekursive Behandlung von Sub-Verzeichnis-Groessen ...
//		Directory aSubDir;
//		FileSystemUtils::GetDirectory( m_sAbsFileName + FileUtilityObj::GetDirectorySeparatorStrg() + "*", FSU_AllFiles, aSubDir );
//		return aSubDir.GetTotalSize();
//	}
//	return GetFileSize();
//}

//FileSizeT DirectoryItem::GetFileSize() const
//{
//	return m_nFileSize;
//}

//FileTime	DirectoryItem::GetFileTime() const
//{
//	return m_aFileTime;
//}

//FileAttributesT DirectoryItem::GetFileAttrib() const
//{
//	return m_aFileAttributes;
//}

//bool DirectoryItem::IsSelf() const
//{
//	return GetName()==".";
//}

//bool DirectoryItem::IsParent() const
//{
//	return GetName()==".." || GetName().length()==0;
//}

//bool DirectoryItem::IsDirectory() const
//{
//#ifdef _WIN32
//	return (m_aFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
//#else
//	return false;
//#endif
//}

//bool DirectoryItem::IsHidden() const
//{
//#ifdef _WIN32
//	return (m_aFileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN;
//#else
//	return false;
//#endif
//}

//bool DirectoryItem::IsReadOnly() const
//{
//#ifdef _WIN32
//	return m_aFileAttributes & FILE_ATTRIBUTE_READONLY;
//#else
//	return false;
//#endif
//}

//bool DirectoryItem::IsSystem() const
//{
//#ifdef _WIN32
//	return (m_aFileAttributes & FILE_ATTRIBUTE_SYSTEM) == FILE_ATTRIBUTE_SYSTEM;
//#else
//	return false;
//#endif
//}

//bool DirectoryItem::IsDeleted() const
//{
//	return m_bIsDeleted;
//}

//bool DirectoryItem::DoDelete()
//{
//	// wurde schon geloescht !
//	if( m_bIsDeleted || IsDirectory() )
//		return false;

//	bool bOk = FileUtilityObj::_DeleteFile( m_sAbsFileName.c_str() );

//	if( bOk )
//		m_bIsDeleted = true;

//	return bOk;
//}

//bool DirectoryItem::CompareLessWith( const DirectoryItem & aObj, int nModus ) const
//{
//	if( nModus==0 )
//		return *this < aObj;
//	else if( nModus==1 )
//		return GetFileSize() < aObj.GetFileSize();
//	else if( nModus==2 )
//		return m_aFileTime < aObj.m_aFileTime;
//	return false;
//}

// *************************************************************************

//// Hilfsklasse zum Sortieren von Directory-Eintraegen
//class _CompareDirectoryItem : public binary_function<const DirectoryItem,const DirectoryItem,bool>
//{
//public:
//	_CompareDirectoryItem( int nModus, bool bInverse ) : m_nModus( nModus ), m_bInverse( bInverse ) {}

//	bool operator() ( const DirectoryItem & aLObj, const DirectoryItem & aRObj ) const
//	{
//		if( m_bInverse )
//			return !aLObj.CompareLessWith( aRObj, m_nModus );
//		return aLObj.CompareLessWith( aRObj, m_nModus );
//	}

//private:
//	int		m_nModus;
//	bool	m_bInverse;
//};


//FileSizeT Directory::GetSumSize() const
//{
//	FileSizeT nCount = 0;
//	const_iterator aIter = begin();
//	while( aIter != end() )
//	{
//		nCount += (*aIter).GetFileSize();
//		++aIter;
//	}
//	return nCount;
//}

//FileSizeT Directory::GetTotalSize() const
//{
//	FileSizeT nCount = 0;
//	const_iterator aIter = begin();
//	while( aIter != end() )
//	{
//		nCount += (*aIter).GetRecursiveSize();
//		++aIter;
//	}
//	return nCount;
//}

//int	Directory::GetNoOfFiles() const
//{
//	int nCount = 0;
//	const_iterator aIter = begin();
//	while( aIter != end() )
//	{
//		if( !(*aIter).IsDirectory() /*&& !(*aIter).IsReadOnly()*/ )		// 27.9.2003: was sollte readonly ?
//			++nCount;
//		++aIter;
//	}
//	return nCount;
//}

//bool Directory::DoDeleteItem( int nNo )
//{
//	if( nNo>=0 && nNo<(int)size() )
//	{
//		if( (*this)[nNo].DoDelete() )	// ist das Loesche wirklich erfolgreich ?
//		{
//			erase( begin()+nNo );
//			return true;
//		}
//	}
//	return false;
//}

//bool Directory::DoSort( int nModus, bool bInverse )
//{
//	/* zum debuggen
//	const_iterator aIter = begin();
//	while( aIter != end() )
//	{
//		cout << "item=" << (*aIter).GetName().c_str() << endl;
//		++aIter;
//	}
//	*/

//	if( nModus>=0 && nModus<=2 )
//	{
//		sort( begin(), end(), _CompareDirectoryItem( nModus, bInverse ) );
//		return true;
//	}
//	return false;
//}

// *************************************************************************

//bool FileSystemUtils::IsDirectory( const string & sPath )
//{
//	string aActDirTemp = FileUtilityObj::GetActDirectory();
//	// versuche in den uebergebenen Pfad zu wechseln, ist dies erfolgreich, dann ist es ein gueltiger Pfad
//	bool bOk = FileUtilityObj::ChangeActDirectory( sPath.c_str() );
//	FileUtilityObj::ChangeActDirectory( aActDirTemp.c_str() );
//	return bOk;
//}

#if defined(__linux__) || defined(__APPLE__)

#define _PATH_SEP		'/'
#define _BAD_PATH_SEP	'\\'
#else
#define _PATH_SEP		'\\'
#define _BAD_PATH_SEP	'/'
#endif

static string MakePathToOSPath( const string & sPathIn )
{
	string sTemp( sPathIn );
	for( int i=0; i<(int)sTemp.length(); i++ )
	{
		if( sTemp[i]==_BAD_PATH_SEP )
			sTemp[i] = _PATH_SEP;
	}
	return sTemp;
}

/*
static string MakeRealPath( const string & sPathIn )
{
	string sTemp( sPathIn );
	if( sTemp.length()>0 && sTemp[ sTemp.length()-1 ]!=_PATH_SEP )
	{
		sTemp += _PATH_SEP;
	}
	return sTemp;
}
*/
// es sollte KEIN Wildcard-Zeichen im Pfad enthalten sein !
//bool FileSystemUtils::GetDirectory( const string & sPath, int nShowFlagsIn, Directory & aDir, const string & sFileMask )
//{
//	string sTempPath = MakePathToOSPath( sPath );
//	string sRealPath = MakeRealPath( sTempPath );
//	string sSearchStrg = sRealPath+sFileMask;

//#ifdef _WIN32
//	WIN32_FIND_DATA aData;
//	// WARNING: conversion problems in the case of unicode version !!!
// 	Qt::HANDLE hDir = ::FindFirstFile( (LPCTSTR)sSearchStrg.c_str(), &aData );
//	bool bContinue = (hDir !=  INVALID_HANDLE_VALUE);
//	if( !bContinue )
//	{
//		return false;
//	}
//	while( bContinue )
//	{
//		if( string( (const char *)aData.cFileName )!=".." && string( (const char *)aData.cFileName )!="." )
//		{
//			FILETIME fLocTime;
//			DOSDATE aDate;
//			DOSTIME aTime;
//			::FileTimeToLocalFileTime( &aData.ftLastWriteTime, &fLocTime );
//			::FileTimeToDosDateTime( &fLocTime, (WORD *)&aDate, (WORD *)&aTime );

//			// den absoluten Pfad bestimmen !!!
//			char sBuffer[2048];
//			char * pBuffer;
//			string sRelPath = string( sRealPath ) + string( (char *)aData.cFileName );
//			::GetFullPathName( (LPCTSTR)sRelPath.c_str(), 2048, (LPTSTR)sBuffer, (LPTSTR *)&pBuffer );
//			string sAbsFileName = string( sBuffer );

//			FileSizeT nFileSize = aData.nFileSizeLow;
//			FileTime aFileTime( aDate.day, aDate.month, aDate.year+1980, aTime.hours, aTime.minutes, aTime.twosecs );
//			FileAttributesT aFileAttributes = aData.dwFileAttributes;

//			if( ((nShowFlagsIn & 1) && (aFileAttributes & FILE_ATTRIBUTE_ARCHIVE)) ||
//				//((nShowFlagsIn & 1) && (aFileAttributes & FILE_ATTRIBUTE_NORMAL)) ||
//				((nShowFlagsIn & 2) && (aFileAttributes & FILE_ATTRIBUTE_HIDDEN)) ||
//				((nShowFlagsIn & 4) && (aFileAttributes & FILE_ATTRIBUTE_SYSTEM)) ||
//				((nShowFlagsIn & 8) && (aFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) )
//			{
//				aDir.push_back( DirectoryItem( sAbsFileName, nFileSize, aFileTime, aFileAttributes ) );
//			}
//		}
//		bContinue = (bool)::FindNextFile( hDir, &aData );
//	}
//	FindClose( hDir );
//#endif
//#if defined(__linux__) || defined(__APPLE__)

//	// ** use qt for wildcard search !
//    QRegExp aRegExp( sFileMask.c_str()/*, TRUE, TRUE*/ );

//	struct dirent * pEntry = 0;
//	DIR * pDir = 0;

//	pDir = opendir( sPath.c_str() );

//	while( (pDir != 0) && (pEntry = readdir( pDir )) != 0 )
//	{
//		// ** check for wildcards
//		int iRet = aRegExp.exactMatch( pEntry->d_name );

//		if( iRet != -1 )
//		{
//			// ** bugfix (12.2.2002): full path needed for stat() !
//			string sFullFileName = sPath;
//			if( sPath.length()>0 )
//			{
//				sFullFileName += FileUtilityObj::GetDirectorySeparatorStrg();
//			}
//			sFullFileName += pEntry->d_name;

//			// ** item fits wildcard --> save item
//			struct stat aStat_p;
//			int iRet = stat( /*pEntry->d_name*/sFullFileName.c_str(), &aStat_p);

//			if( iRet == 0 )
//			{
//				if( ((nShowFlagsIn & 1) && S_ISREG( aStat_p.st_mode )) ||
//					//((nShowFlagsIn & 1) && (aFileAttributes & FILE_ATTRIBUTE_NORMAL)) ||
//					//((nShowFlagsIn & 2) && (aFileAttributes & FILE_ATTRIBUTE_HIDDEN)) ||
//					//((nShowFlagsIn & 4) && S_ISREG( aStat_p.st_mode )) ||
//					((nShowFlagsIn & 8) && S_ISDIR( aStat_p.st_mode )) )
//				{
//					FileSizeT nFileSize = aStat_p.st_size;
//#if defined(Q_OS_ANDROID)
///*ANDROID*/					struct tm * pDate = localtime( 0 );
//#else
//                            struct tm * pDate = localtime( &(aStat_p.st_ctime) );
//#endif
//                                        FileTime aFileTime( pDate->tm_mday, pDate->tm_mon+1, pDate->tm_year+1900, pDate->tm_hour, pDate->tm_min, pDate->tm_sec );
//					FileAttributesT aFileAttributes = 0;	// ??? (todo ?)
//					//cout << "--> " << pEntry->d_name << " size=" << nFileSize << " " << aFileTime.GetString().c_str() << endl;

//					aDir.push_back( DirectoryItem( /*pEntry->d_name*/sFullFileName.c_str(), nFileSize, aFileTime, aFileAttributes ) );
//				}
//			}
//		}
//	}

//	closedir( pDir );
//#endif

//	return true;
//}

//void FileSystemUtils::DumpDirectory( ostream & aStream, const Directory & aDirectory )
//{
//	Directory::const_iterator aIter = aDirectory.begin();
//	aStream << "Found " << aDirectory.size() << " files." << endl;
//	while( aIter != aDirectory.end() )
//	{
//		aStream << (*aIter).GetName().c_str() << " " << (*aIter).GetFileSize() << "bytes " << (*aIter).GetRecursiveSize() << "bytes time=" << (*aIter).GetFileTime().GetString().c_str()
//				<< " IsDir=" << (*aIter).IsDirectory() << " IsHidden=" << (*aIter).IsHidden() << " IsReadOnly=" << (*aIter).IsReadOnly() << endl;
//		++aIter;
//	}
//}

string SearchForExistingFileInEnv( const char * sEnvironment, const char * sFullFileName )
{
	if( FileUtilityObj::ExistsFile( sFullFileName )  )
	{
  		return FileUtilityObj::GetFullPath( sFullFileName );
	}
	else if( sEnvironment )
	{
		// nur wenn kein Laufwerk und kein Pfad angegeben wurde die Suche durchfuehren
		string sDrive, sDir, sFileName, sExt;
		// falls notwendig / in \ umwandeln
		string sFullFileNameIn( sFullFileName );
		sFullFileNameIn = MakePathToOSPath( sFullFileNameIn );
		FileUtilityObj::SplitPath( sFullFileNameIn.c_str(), sDrive, sDir, sFileName, sExt );
		if( sDrive.length()==0 /*&& sDir.length()==0*/ )
		//if( FileUtilityObj::IsOnlyFileName( sFullFileName ) )
		{
			// nicht gefunden, d.h. jetzt in der uebergebenen Environment-Variable suchen
			char * sEnvPath = getenv( sEnvironment );
			if( sEnvPath )
			{
				string sTempEnvPath( sEnvPath );

				bool bContinue = true;
				int nPos = 0;
				int nLastPos = 0;
				do
				{
					nPos = sTempEnvPath.find( ";", nLastPos );
					if( nPos!=(int)string::npos || nLastPos<(int)sTempEnvPath.length() )
					{
						// ist es ggf. der letzte Eintrag in der Environment-Variable ?
						if( nPos==(int)string::npos )
							nPos = sTempEnvPath.length();

						string sTempPath = sTempEnvPath.substr( nLastPos, (nPos-nLastPos) );
						if( sTempPath.length()>0 )
						{
							sTempPath += FileUtilityObj::GetDirectorySeparatorStrg();
							sTempPath += sDir;
							sTempPath += sFileName;
							sTempPath += sExt;
							if( FileUtilityObj::ExistsFile( sTempPath.c_str() )  )
								return FileUtilityObj::GetFullPath( sTempPath.c_str() );
						}
						nLastPos = nPos+1;	// eins hinter dem gefundenen ; weitersuchen
					}
					else
					{
						bContinue = false;
					}
				} while( bContinue );
			}
		}
	}
	return "";
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

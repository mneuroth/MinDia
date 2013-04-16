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

#ifdef _MSC_VER
#pragma warning( disable : 4786 )
// disable warning: '...' : Bezeichner wurde auf '255' Zeichen in den Debug-Informationen reduziert
// for stl-structs
#endif

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//class FileTime;

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
  static bool _DeleteFile( const char * sFileName );
  static bool _RenameFile( const char * sName, const char * sNewName );
  static bool _MoveFile( const char * sSource, const char * sTarget );
  static bool _CopyFile( const char * sSource, const char * sTarget );
  static unsigned long GetFileSize( const char * sFileName );
  //static bool _GetFileTimeInfos( const char * sFileNameIn, FileTime & aFileTimeOut );
  static bool GetFileDate( const char * sFileNameIn, string & sDateOut );
  static bool GetFileTime( const char * sFileNameIn, string & sTimeOut );
  static bool ExistsFile( const char *sFileName );
  static bool _CreateDirectory( const char *sDirName );
  static bool _RemoveDirectory( const char *sDirName );
  static bool ChangeActDirectory( const char *sDirName );
  static string GetActDirectory();
  static bool ChangeActDrive( int nDriveNo /*0==A:, 1==B:, ...*/ );
  static bool GetFreeDiskSpace( int nDriveNo, double & dFreeSpaceOut, double * pdTotalSpaceOut = 0 );
  static int GetActDrive();
  static int GetDriveNoFromString( const string & sDriveStrg );
//  static DriveInfoList GetAllLogicalDrives();
  static bool IsAbsPath( const char * sPath );
  static bool SplitPath( const char * sPath, string & sDrive, string & sDir, string & sFileName, string & sExt );
  static string GetFullPath( const char * sFileName );
  static string MakeSaveDOSPath( const char * sFullFileName );
  static string GetOnlyFileNameWithoutExt( const char * sFullFileName );	// liefert Dateinamen ohne Pfad und ohne Extention aus dem uebergebenen vollstaendigen Pfad
  static string GetOnlyExtention( const char * sFullFileName );			// liefert die Extention einer Datei (inkl. des Punktes, d.h. ".exe") aus dem uebergebenen vollstaendigen Pfad
  static string GetOnlyPath( const char * sFullFileName );				// liefert den vollstaendigen Pfad, inkl. Laufwerksbuchstaben (falls angegeben) aus dem uebergebenen vollstaendigen Pfad
  static string GetTmpPath();
  static bool IsOnlyFileName( const char * sFullFileName );
  static bool IsWhiteSpaceChar( char ch );
  static const char * GetDirectorySeparatorStrg();
  static char GetDirectorySeparator();
//  static bool IsDirectory( FileAttributesT aAttributesIn );
//  static bool IsHidden( FileAttributesT aAttributesIn );
//  static bool IsReadOnly( FileAttributesT aAttributesIn );

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

// *************************************************************************
// *************************************************************************
// *************************************************************************

//typedef unsigned long	FileSizeT;
//typedef unsigned long	FileAttributesT;

//// Konstanten fuer ShowFlags bei GetDirectory
//const int FSU_AllFiles = 1+2+4+8;
//const int FSU_OnlyFiles = 1;

//// *************************************************************************
///** Klasse repraesentiert einen Zeitpunkt. */
//class FileTime
//{
//public:
//	FileTime( int nDay = 1, int nMonth = 1, int nYear = 2000, int nHour = 0, int nMinute = 0, int nSecond = 0 );

//	string		GetString( int nLanguage = 49 ) const;
//	string		GetDateString( int nLanguage = 49 ) const;
//	string		GetTimeString( int nLanguage = 49 ) const;

//	bool		operator<( const FileTime & aObj ) const;
//	bool		operator==( const FileTime & aObj ) const;

//	int			GetDay() const;
//	int			GetMonth() const;
//	int			GetYear() const;
//	int			GetSecond() const;
//	int			GetMinute() const;
//	int			GetHour() const;

//private:
//	int			m_nDay;			// 1..31
//	int			m_nMonth;		// 1..12
//	int			m_nYear;		// 1999
//	int			m_nSecond;		// 0..60
//	int			m_nMinute;		// 0..60
//	int			m_nHour;		// 0..23
//};

// *************************************************************************
/** Klasse repraesentiert einen Eintrag in einem Verzeichnis. */
//class DirectoryItem
//{
//public:
//	DirectoryItem( const string & sAbsFileName = "", FileSizeT nFileSize = 0, const FileTime & aFileTime = FileTime(), FileAttributesT aFileAttributes = 0 );
//	//DirectoryItem( const DirectoryItem & aCopyObj );
//	~DirectoryItem();

//	string				GetName() const;			// liefert Dateinamen mit Extention, d.h. c:\dir1\dir2\a.b.c --> a.b.c
//	string				GetFirstName() const;		// liefert Dateinamen ohne Extention, d.h. alles vor dem ersten Punkt: a.b.c --> a
//	string				GetExtention() const;		// liefert nur die Extention, d.h. alles nach dem ersten Punkt: a.b.c --> b.c
//	string				GetPath() const;			// liefert den gesammten Pfad zu dieser Datei, d.h. c:\dir1\dir2\a.b.c --> "\dir1\dir2\"
//	string				GetDrive() const;			// liefert den Laufwerksbuchstaben, d.h. "c:"
//	string				GetNetName() const;			// liefert (falls vorhanden) den Namen des Servers, d.h. \\source\dir1\dir2\a.b.c --> \\source

//	FileSizeT			GetRecursiveSize() const;	// liefert die Dateigroesse, bzw. bei Subverzeichnissen die gesammt Groesse des Verzeichnisses
//	FileSizeT			GetFileSize() const;		// liefert die Dateigroesse
//	FileTime			GetFileTime() const;
//	FileAttributesT		GetFileAttrib() const;

//	bool				IsSelf() const;
//	bool				IsParent() const;
//	bool				IsDirectory() const;		// ist es ein Unterverzeichnis
//	bool				IsHidden() const;			// ist die Datei/Verzeichnis versteckt
//	bool				IsReadOnly() const;			// ist nur ein lesender Zugriff moeglich
//	bool				IsSystem() const;

//	bool				IsDeleted() const;
//	bool				DoDelete();

//	// fuehre Vergleich mit uebergebenem Objekt nach uebergebenem Modus durch
//	// nModus: 0==Name, 1==Datum, 2==Groesse
//	bool				CompareLessWith( const DirectoryItem & aObj, int nModus ) const;

//	bool operator<( const DirectoryItem & aObj ) const	{ return m_sAbsFileName < aObj.m_sAbsFileName; }
//	bool operator==( const DirectoryItem & aObj ) const	{ return m_sAbsFileName == aObj.m_sAbsFileName; }
//	bool operator!=( const DirectoryItem & aObj ) const	{ return !(*this==aObj); }

//private:
//	string				m_sAbsFileName;
//	FileTime			m_aFileTime;
//	FileSizeT			m_nFileSize;
//	FileAttributesT		m_aFileAttributes;
//	bool				m_bIsDeleted;
//};

// *************************************************************************
//class Directory : public vector<DirectoryItem>
//{
//public:
//	FileSizeT			GetSumSize() const;			// liefert die Summe der Groesse aller Dateien im Verzeichnis
//	FileSizeT			GetTotalSize() const;		// liefert die gesammt Groesse, inkl. Unterverzeichnisse
//	int					GetNoOfFiles() const;		// nur die Anzahl der (sichtbaren) Dateien liefern

//	bool				DoDeleteItem( int nNo );

//	bool				DoSort( int nModus, bool bInverse = false );	// sortiert das Verzeichnis: nModus: 0==Name, 1==Groesse, 2==Datum
//};

// *************************************************************************
//class FileSystemUtils
//{
//public:
//	static bool			IsDirectory( const string & sPath );

	// nShowFlagsIn: 1==normal-files 2==hidden-files, 4==system-files, 8==directories
	// im sPath koennen folgende (korrekte) Ausdruecke stehen (ohne Beachtung von Sonderzeichen, z.B. von '\')
	//  "\a\b\c", "\a\b\c\", "a\b\c", "..", ".", "/a/b/c", "/a/b/c/"
//	static bool			GetDirectory( const string & sPath, int nShowFlagsIn, Directory & aDirectoryOut, const string & sFileMask = "*" );

//	static void			DumpDirectory( ostream & aStream, const Directory & aDirectory );
//};

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

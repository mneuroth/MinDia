/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/diainfo.h,v $
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

#ifndef _DIAINFO_H
#define _DIAINFO_H

//#ifndef ZAURUS
#include "iscript.h"
//#endif

#include <iostream>
#include <vector>
#include <string>

#ifndef SWIG
using namespace std;
#endif

#include "minutils.h"		// for: ObjectChanged
#include "writexml.h"

// *******************************************************************
/** Helperclass to handle a time operation for a dia.
  */
class TimeOperation
{
public:
	TimeOperation( int iOperationType = UNDEFINED, double dOperationTime = 0 );

	string	GetOperationTypeName() const;
	int		GetOperationType() const;
	double	GetOperationTime() const;

	bool	SetOperationTime( double dTime );

	XmlTree	GetXMLTree() const;

	bool	Read( istream & aStream );
	bool	Write( ostream & aStream ) const;

private:
	// *** data ***
	int					m_iOperationType;
	double				m_dOperationTime;

public:
	// *** some public constants for this class ***
	static const int	ACT_FILE_VERSION;

	static const int    UNDEFINED;
	static const int	DISSOLVE_IN;
	static const int	SHOW;
	static const int	DISSOLVE_OUT;
	static const int	FLASH;
};

// *******************************************************************
#ifndef SWIG
class TimeOperationContainer : public vector<TimeOperation>
{
public:
	XmlTree	GetXMLTree() const;

	bool	Read( istream & aStream );
	bool	Write( ostream & aStream ) const;
};
#endif

// *******************************************************************
/** Represents the information for one dia/slide.
  */
class DiaInfo
//#ifndef ZAURUS
 : public IDia
//#endif
{
public:
	DiaInfo( const string & sId = "", const string & sImageFile = "", const string & sComment = "" );
	virtual ~DiaInfo();

	XmlTree	GetXMLTree() const;

	bool	Read( istream & aStream );
	bool	Write( ostream & aStream ) const;

	bool	IsChanged() const;

	bool	IsEmpty() const;
	bool	IsHorizontalFormat() const;

	bool	HasScript() const;

	bool	HasText( const string & sText ) const;

	// ** methods for streaming/serializing this class **
	string	GetData() const;
	bool	SetFromData( string & sStringStreamInOut );

	// ** implements interface IDia **
	virtual double			GetTotalTime() const;

	virtual const char *	GetId() const;
	virtual bool			SetId( const char * sId );
	virtual const char *	GetComment() const;
	virtual bool			SetComment( const char * sComment );
	virtual const char *	GetImageFile() const;
	virtual bool			SetImageFile( const char * sFileName );
	virtual const char *	GetScript() const;
	virtual bool			SetScript( const char * sScript );

	virtual double	GetDissolveTime() const;
	virtual bool	SetDissolveTime( double dShowTime );

	virtual double	GetShowTime() const;
	virtual bool	SetShowTime( double dShowTime );

	virtual bool	GetHorizontalFormat() const;
	virtual bool	SetHorizontalFormat( bool bValue );

	void			SetData( const string & sId,
							 const string & sImageFile = "",
							 const string & sComment = "",
							 const string & sScript = "" );

	int				GetPosition() const;
	void			SetPosition( int iPos );

	int				GetOperationCount() const;
	TimeOperation	GetOperation( int iIndex ) const;
	TimeOperation &	AccessOperationAndChanged( int iIndex );

	void			ClearAllOperations();
	void			AddOperation( const TimeOperation & aOperation );
	bool			ModifyOperation( int iIndex, const TimeOperation & aOperation );

	void			MakeRelativePaths();

private:
	void			SkipWhitespaces( string & sString );
	bool			IsWhitespace( char ch ) const;

	// *** data ***
	string					m_sId;
	string					m_sComment;
	string					m_sImageFile;
	string					m_sScript;				// new since 29.3.2002
	double					m_dIntensityInPercent;
	int						m_iFormat;				// 0 == horizontal, 1 == vertical
	TimeOperationContainer	m_aTimeOpContainer;

	// *** temp data ***
	int						m_iPosition;
	ObjectChanged			m_aObjectChanged;

public:
	// *** some public constants for this class ***
	static const int		ACT_FILE_VERSION;
	static const int		HORIZONTAL;
	static const int		VERTICAL;
};

#endif

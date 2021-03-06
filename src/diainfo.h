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

#ifndef _DIAINFO_H
#define _DIAINFO_H

#include "iscript.h"
#include "dyngraphop.h"

#include <iostream>
#include <vector>
#include <string>

#ifndef SWIG
using namespace std;
#endif

#include "minutils.h"		// for: ObjectChanged
#include "writexml.h"

// *******************************************************************

const double c_dDefaultDissolveTime = 2.5;
const double c_dDefaultTimer = 5.0;

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
class DiaInfo : public IDia
{
public:
    DiaInfo( const string & sId = "", const string & sImageFile = "",  double dDissolveTime = c_dDefaultDissolveTime, double dShowTime = c_dDefaultTimer, const string & sComment = "" );
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

    void    SetUUID( const string & sUUID );

	// ** implements interface IDia **
    virtual double	GetTotalTime() const;

    virtual string  GetUUID() const;

    virtual string  GetId() const;
    virtual bool	SetId( const string & sId );
    virtual string  GetComment() const;
    virtual bool	SetComment( const string & sComment );
    virtual string  GetImageFile() const;
    virtual bool	SetImageFile( const string & sFileName );
    virtual string  GetScript() const;
    virtual bool	SetScript( const string & sScript );

	virtual double	GetDissolveTime() const;
	virtual bool	SetDissolveTime( double dShowTime );

	virtual double	GetShowTime() const;
	virtual bool	SetShowTime( double dShowTime );

	virtual bool	GetHorizontalFormat() const;
	virtual bool	SetHorizontalFormat( bool bValue );

    virtual double  GetRelX() const;
    virtual bool    SetRelX( double dVal );
    virtual double  GetRelY() const;
    virtual bool    SetRelY( double dVal );
    virtual double  GetRelDX() const;
    virtual bool    SetRelDX( double dVal );
    virtual double  GetRelDY() const;
    virtual bool    SetRelDY( double dVal );

    virtual bool    IsKenBurns() const;
    virtual void    SetKenBurns( bool enable );

    virtual double  GetRelXEnd() const;
    virtual bool    SetRelXEnd( double dVal );
    virtual double  GetRelYEnd() const;
    virtual bool    SetRelYEnd( double dVal );
    virtual double  GetRelDXEnd() const;
    virtual bool    SetRelDXEnd( double dVal );
    virtual double  GetRelDYEnd() const;
    virtual bool    SetRelDYEnd( double dVal );

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
    void			MakeAbsolutePaths( const string & sDir );

private:
	void			SkipWhitespaces( string & sString );
	bool			IsWhitespace( char ch ) const;

	// *** data ***
    string                  m_sUUID;                // new since 31.3.2013
	string					m_sId;
	string					m_sComment;
	string					m_sImageFile;
	string					m_sScript;				// new since 29.3.2002
    double                  m_relX;                 // new since 1.4.2013
    double                  m_relY;                 // dito
    double                  m_relDX;                // dito
    double                  m_relDY;                // dito
    bool                    m_bIsKenBurns;          // new since 28.8.2015
    double                  m_relXEnd;              // new since 28.8.2015
    double                  m_relYEnd;              // dito
    double                  m_relDXEnd;             // dito
    double                  m_relDYEnd;             // dito
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

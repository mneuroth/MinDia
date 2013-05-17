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

#ifndef _SOUNDINFO_H
#define _SOUNDINFO_H

#include "iocontainer.h"
#include "minutils.h"
#include "writexml.h"

#include <string>

#include <QThread>

using namespace std;

// *******************************************************************
/** sound info item. */
class SoundInfo
{
public:
    SoundInfo( const string & sFileName = "", int iTotalLength = 0, int iStartPosInMS = -1, int iStopPosInMS = -1 );

	string	GetFileName() const;
	bool	SetFileName( const string & sNewFileName );

	// ** all time values in ms !!! **
	int		GetStartPos() const;
	int		GetStopPos() const;
	int		GetDelta() const;
    int		GetTotalLength() const;             // in milli seconds
	int		GetFadeInStartPos() const;
	int		GetFadeInLength() const;
	int		GetFadeOutStartPos() const;
	int		GetFadeOutLength() const;
    void    SetTotalLength( int iTotalLength );

	bool	HasFadeInData() const;
	bool	HasFadeOutData() const;
	void	SetFadeInData( int iStart, int iLength );
	void	SetFadeOutData( int iStart, int iLength );

	XmlTree	GetXMLTree() const;

	bool	Read( istream & aStream );
	bool	Write( ostream & aStream ) const;

	void	MakeRelativePaths();
    void	MakeAbsolutePaths( const string & sDir );

private:
	// ** helper methods **
    void	SetData( const string & sFileName, int iTotalLength, int iStartPosInMS, int iStopPosInMS );

	// ** data **
	string	m_sFileName;
	int		m_iStartPosInMS;
	int		m_iStopPosInMS;
	int		m_iTotalLengthInMS;
	// ** new since 2.1.2002
	int		m_iFadeInStartInMS;
	int		m_iFadeInLengthInMS;
	int		m_iFadeOutStartInMS;
	int		m_iFadeOutLengthInMS;

public:
	// *** some public constants for this class ***
	static const int		ACT_FILE_VERSION;
};

// *******************************************************************
/** sound comment item. */
class SoundComment : public GenericDataInterface
{
public:
	SoundComment( double dTimePosInMS = 0, const string & sComment = "" );

	double	GetTimePos() const;
	string	GetComment() const;

	XmlTree	GetXMLTree() const;

	bool	Read( istream & aStream );
	bool	Write( ostream & aStream ) const;

	bool	operator<( const SoundComment & aOther ) const;

	// ** implement GenericDataInterface
	virtual string		GetName() const;
	virtual int			GetDataCount() const;
	virtual string		GetDataName( int iIndex ) const;
	virtual DataType	GetDataType( int iIndex ) const;
	virtual string		GetDataValue( int iIndex ) const;
	virtual bool		SetDataValue( int iIndex, const string & sValue );

private:
	double	m_dTimePosInMS;
	string	m_sComment;
};

class SoundInfoContainer;

// *******************************************************************
/** Container to hold sound-info-items. */
class SoundInfoContainer : public IOContainer<SoundInfo>, public ObjectChanged
{
public:
	SoundInfoContainer();

	int			GetTotalPlayLength() const;
	int			GetAbsPlayPos( int iIndex ) const;
	iterator	GetItemForAbsPlayPos( int iAbsTimeInMS, int & iOffsetTime );
	void		MakeRelativePaths();
    void		MakeAbsolutePaths( const string & sDir );
};

// *******************************************************************
/** Container to hold sound-comment-items. */
class SoundCommentContainer : public IOContainer<SoundComment>, public GenericCommentContainer
{
public:
	SoundCommentContainer();

	void	AddComment( double dTimePosInMS, const string & sComment );

	// ** implements the GenericCommentContainer-interface
	virtual int						size() const;
	virtual void					clear();
	virtual GenericDataInterface *	at( int iIndex );
	virtual GenericDataInterface *	push_back_new_item();

	virtual void	SortData();
};

// *******************************************************************

string SecondsInMinSec( int iSeconds );


// *******************************************************************
// *******************************************************************
// *******************************************************************
/** plot comment item. */
class PlotComment : public GenericDataInterface
{
public:
	PlotComment( int iStartDiaIndex = -1, int iStopDiaIndex = -1, const string & sComment = "" );

	int		GetStartIndex() const;
	int		GetStopIndex() const;
	string	GetComment() const;

	XmlTree	GetXMLTree() const;

	bool	Read( istream & aStream );
	bool	Write( ostream & aStream ) const;

	bool	operator<( const PlotComment & aOther ) const;

	// ** implement GenericDataInterface
	virtual string		GetName() const;
	virtual int			GetDataCount() const;
	virtual string		GetDataName( int iIndex ) const;
	virtual DataType	GetDataType( int iIndex ) const;
	virtual string		GetDataValue( int iIndex ) const;
	virtual bool		SetDataValue( int iIndex, const string & sValue );

private:
	int		m_iStartIndex;
	int		m_iStopIndex;
	string	m_sComment;
};

// *******************************************************************
/** Container to hold plot-comment-items. */
class PlotCommentContainer : public IOContainer<PlotComment>, public GenericCommentContainer
{
public:
	PlotCommentContainer();

	void	AddComment( int iStartDiaIndex, int iStopDiaIndex, const string & sComment );

	// ** implements the GenericCommentContainer-interface
	virtual int						size() const;
	virtual void					clear();
	virtual GenericDataInterface *	at( int iIndex );
	virtual GenericDataInterface *	push_back_new_item();

	virtual void	SortData();
};

#endif

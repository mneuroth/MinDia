/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/soundinfo.cpp,v $
 *
 *  $Revision: 1.3 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.2  2003/10/26 17:18:48  min
 *	MakeRelativePaths() added.
 *	
 *	Revision 1.1.1.1  2003/08/15 16:38:22  min
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

#include <qobject.h>

#include <algorithm>
#include <functional>

#include "soundinfo.h"

#include "minisound.h"

#include <stdio.h>		// sprintf
#include <stdlib.h>		// atoi

// ** XML Tags **
const char * _SOUND_INFO_CONTAINER		= "soundinfos";
const char * _SOUND_COMMENT_CONTAINER	= "soundcomments";
const char * _PLOT_COMMENT_CONTAINER	= "plotcomments";

// *******************************************************************
// *******************************************************************
// *******************************************************************

string SecondsInMinSec( int iSeconds )
{
	char sBuffer[255];

	sprintf( sBuffer, "%d:%02d", iSeconds / 60, iSeconds % 60 );

	return sBuffer;
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

const int SoundInfo::ACT_FILE_VERSION = 2;
							// 0 until 2.1.2002 --> FadeIn/FadeOut infos added
                            // 1 until 13.2.20013 --> add length of sound file


SoundInfo::SoundInfo( const string & sFileName, int iTotalLength, int iStartPosInMS, int iStopPosInMS )
: m_iStartPosInMS( iStartPosInMS ),
  m_iStopPosInMS( iStopPosInMS ),
  m_iTotalLengthInMS( iTotalLength ),
  m_iFadeInStartInMS( -1 ),
  m_iFadeInLengthInMS( 0 ),
  m_iFadeOutStartInMS( -1 ),
  m_iFadeOutLengthInMS( 0 )
{
    SetData( sFileName, iTotalLength, iStartPosInMS, iStopPosInMS );
}

string SoundInfo::GetFileName() const
{
	return m_sFileName;
}

bool SoundInfo::SetFileName( const string & sNewFileName )
{
	m_sFileName = sNewFileName;
	return true;
}

int SoundInfo::GetStartPos() const
{
	return m_iStartPosInMS;
}

int	SoundInfo::GetStopPos() const
{
	return m_iStopPosInMS;
}

int	SoundInfo::GetDelta() const
{
	int iStart = 0;
	int iStop = GetTotalLength();

	if( GetStartPos()>=0 )
	{
		iStart = GetStartPos();
	}
	if( GetStopPos()>=0 )
	{
		iStop = GetStopPos();
	}

	return iStop-iStart;
}

int SoundInfo::GetTotalLength() const
{
	return m_iTotalLengthInMS;
}

void SoundInfo::SetTotalLength( int iTotalLength )
{
    m_iTotalLengthInMS = iTotalLength;
}

/*
int GetTotalLengthInMSForSoundFile( const string & sFileName );

int SoundInfo::GetTotalLengthImpl() const
{
    return GetTotalLengthInMSForSoundFile( m_sFileName );
// min todo --> hier besser noch inidb uebergeben !
//	miniSound aSound( m_sFileName.c_str() );

//	return aSound.GetTotalLengthInMS();
}
*/
int	SoundInfo::GetFadeInStartPos() const
{
	return m_iFadeInStartInMS;
}

int	SoundInfo::GetFadeInLength() const
{
	return m_iFadeInLengthInMS;
}

int	SoundInfo::GetFadeOutStartPos() const
{
	return m_iFadeOutStartInMS;
}

int	SoundInfo::GetFadeOutLength() const
{
	return m_iFadeOutLengthInMS;
}

void SoundInfo::SetFadeInData( int iStart, int iLength )
{
	m_iFadeInStartInMS = iStart;
	m_iFadeInLengthInMS = iLength;
}

void SoundInfo::SetFadeOutData( int iStart, int iLength )
{
	m_iFadeOutStartInMS = iStart;
	m_iFadeOutLengthInMS = iLength;
}

bool SoundInfo::HasFadeInData() const
{
	return m_iFadeInStartInMS != -1;
}

bool SoundInfo::HasFadeOutData() const
{
	return m_iFadeOutStartInMS != -1;
}

void SoundInfo::SetData( const string & sFileName, int iTotalLength, int iStartPosInMS, int iStopPosInMS )
{
	m_sFileName = sFileName;
    m_iTotalLengthInMS = iTotalLength;
	m_iStartPosInMS = iStartPosInMS;
	m_iStopPosInMS = iStopPosInMS;

	if( (m_iStartPosInMS > m_iStopPosInMS) &&
		(m_iStartPosInMS != -1) && 
		(m_iStopPosInMS != -1) )
	{
		swap( m_iStartPosInMS, m_iStopPosInMS );
	}

    //UpdateTotalLength();
}
/*
void SoundInfo::UpdateTotalLength()
{
	m_iTotalLengthInMS = GetTotalLengthImpl();
}
*/
bool SoundInfo::Read( istream & aStream )
{
	FileUtilityObj aFU;
	int iActFileVersion = 0;

	aFU.ReadStructBegin( aStream );
	aStream >> iActFileVersion;
	aFU.ReadSeparator( aStream );
	ReadString( aStream, m_sFileName );
	aFU.ReadSeparator( aStream );
	aStream >> m_iStartPosInMS;
	aFU.ReadSeparator( aStream );
	aStream >> m_iStopPosInMS;
	if( iActFileVersion > 0 )				// since 2.1.2002
	{
		aFU.ReadSeparator( aStream );
		aStream >> m_iFadeInStartInMS;
		aFU.ReadSeparator( aStream );
		aStream >> m_iFadeInLengthInMS;
		aFU.ReadSeparator( aStream );
		aStream >> m_iFadeOutStartInMS;
		aFU.ReadSeparator( aStream );
		aStream >> m_iFadeOutLengthInMS;
	}
	else
	{
		m_iFadeInStartInMS = -1;
		m_iFadeInLengthInMS = 0;
		m_iFadeOutStartInMS = -1;
		m_iFadeOutLengthInMS = 0;
	}
    if( iActFileVersion > 1 )
    {
        aFU.ReadSeparator( aStream );
        aStream >> m_iTotalLengthInMS;
    }
    else
    {
        m_iTotalLengthInMS = 0;
    }
	aFU.ReadStructEnd( aStream );

    //UpdateTotalLength();

	return aStream.good();
}

XmlTree	SoundInfo::GetXMLTree() const
{
	XmlTree aTree( "soundinfo", "", false, XmlAttribList( "fileversion", ToString( ACT_FILE_VERSION ) ) );

	aTree.PushTag( "filename", m_sFileName );
    aTree.PushTag( "length", m_iTotalLengthInMS, XmlAttribList( "scale", "ms" ) );
    aTree.PushTag( "startpos", m_iStartPosInMS, XmlAttribList( "scale", "ms" ) );
	aTree.PushTag( "stoppos", m_iStopPosInMS, XmlAttribList( "scale", "ms" ) );
	aTree.PushTag( "startfadein", m_iFadeInStartInMS, XmlAttribList( "scale", "ms" ) );
	aTree.PushTag( "lengthfadein", m_iFadeInLengthInMS, XmlAttribList( "scale", "ms" ) );
	aTree.PushTag( "startfadeout", m_iFadeOutStartInMS, XmlAttribList( "scale", "ms" ) );
	aTree.PushTag( "lengthfadeout", m_iFadeOutLengthInMS, XmlAttribList( "scale", "ms" ) );

	return aTree;
}

bool SoundInfo::Write( ostream & aStream ) const
{
	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	aStream << ACT_FILE_VERSION;
	aFU.WriteSeparator( aStream );
	WriteString( aStream, m_sFileName );
	aFU.WriteSeparator( aStream );
	aStream << m_iStartPosInMS;
	aFU.WriteSeparator( aStream );
	aStream << m_iStopPosInMS;
	if( ACT_FILE_VERSION > 0 )
	{
		aFU.WriteSeparator( aStream );
		aStream << m_iFadeInStartInMS;
		aFU.WriteSeparator( aStream );
		aStream << m_iFadeInLengthInMS;
		aFU.WriteSeparator( aStream );
		aStream << m_iFadeOutStartInMS;
		aFU.WriteSeparator( aStream );
		aStream << m_iFadeOutLengthInMS;
	}
    if( ACT_FILE_VERSION > 1 )
    {
        aFU.WriteSeparator( aStream );
        aStream << m_iTotalLengthInMS;
    }
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

void SoundInfo::MakeRelativePaths()
{
	m_sFileName = FileUtilityObj::ConvertToRelPath( m_sFileName.c_str() );
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

SoundInfoContainer::SoundInfoContainer()
: IOContainer<SoundInfo>( _SOUND_INFO_CONTAINER )
  //,m_aHelperThread(*this)
{
}

int SoundInfoContainer::GetTotalPlayLength() const
{
	int iTotal = 0;
	const_iterator aIter = begin();

	while( aIter != end() )
	{
		minHandle<SoundInfo> hItem = *aIter;
		iTotal += hItem->GetDelta();
		++aIter;
	}

	return iTotal;
}

int SoundInfoContainer::GetAbsPlayPos( int iIndex ) const
{
	int iTotal = 0;

	for( int i=0; i<iIndex; i++ )
	{
		iTotal += (*this)[i]->GetDelta();
	}

	return iTotal;
}

SoundInfoContainer::iterator SoundInfoContainer::GetItemForAbsPlayPos( int iAbsTimeInMS, int & iOffsetTime )
{
	iterator aIter = begin();

	int iTime = 0;
	while( aIter != end() )
	{
		iOffsetTime = iAbsTimeInMS - iTime;

		iTime += (*aIter)->GetDelta();

		// ** is the absolute time pos included in this item ?
		if( iTime >= iAbsTimeInMS )
		{
			return aIter;
		}

		++aIter;
	}

	iOffsetTime = 0;
	return aIter;
}

void SoundInfoContainer::MakeRelativePaths()
{
	iterator aIter = begin();

	while( aIter != end() )
	{
		minHandle<SoundInfo> hItem = *aIter;
		hItem->MakeRelativePaths();
		++aIter;
	}
}

/*
UpdateLengths::UpdateLengths(SoundInfoContainer & aContainer)
    : m_aContainer(aContainer)
{
}

#include <QApplication>
#include <QWidget>
extern QWidget * GetMainWindow();

// TODO gulp --> dies hier loeschen !!!
void UpdateLengths::run()
{
    SoundInfoContainer::iterator aIter = m_aContainer.begin();
    miniSound aSound;

    while( aIter != m_aContainer.end() )
    {
        minHandle<SoundInfo> hItem = *aIter;
        aSound.SetWavFile(hItem->GetFileName().c_str());
        int iTotalLength = aSound.GetTotalLengthInMS();
        while( iTotalLength<=0 )
        {
            sleep(10);
            iTotalLength = aSound.GetTotalLengthInMS();
        }
        hItem->SetTotalLength(iTotalLength);
        // request update of total length for item
        // await bis resulata da ist
        ++aIter;
    }
    // TODO --> trigger update der View ! --> F5 in Menu aufnehmen !
    QEvent * pUserEvent = new QEvent(QEvent::User); //new MyCheckReloadEvent();
    QWidget * pWidget = GetMainWindow();
    QApplication::postEvent(pWidget,pUserEvent);
//    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
//             QApplication::postEvent((QObject *)widget,pUserEvent);
//    }
}

// test um das ansync problem bei sound files zu loesen
void SoundInfoContainer::UpdateAllLengths()
{
    m_aHelperThread.start();
}
*/

// *******************************************************************
// *******************************************************************
// *******************************************************************

SoundComment::SoundComment( double dTimePosInMS, const string & sComment )
{
	m_dTimePosInMS = dTimePosInMS;
	m_sComment = sComment;
}

double SoundComment::GetTimePos() const
{
	return m_dTimePosInMS;
}

string SoundComment::GetComment() const
{
	return m_sComment;
}

bool SoundComment::Read( istream & aStream )
{
	FileUtilityObj aFU;

	aFU.ReadStructBegin( aStream );
	aStream >> m_dTimePosInMS;
	aFU.ReadSeparator( aStream );
	ReadString( aStream, m_sComment );
	aFU.ReadStructEnd( aStream );

	return aStream.good();
}

XmlTree	SoundComment::GetXMLTree() const
{
	XmlTree aTree( "soundcomment" );

	aTree.PushTag( "timepos", m_dTimePosInMS, XmlAttribList( "scale", "ms" ) );
	aTree.PushTag( "comment", m_sComment, XmlAttribList( "type", "string" ) );

	return aTree;
}

bool SoundComment::Write( ostream & aStream ) const
{
	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	aStream << m_dTimePosInMS;
	aFU.WriteSeparator( aStream );
	WriteString( aStream, m_sComment );
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

bool SoundComment::operator<( const SoundComment & aOther ) const
{
	return ( m_dTimePosInMS < aOther.m_dTimePosInMS );
}

string SoundComment::GetName() const
{
    return QObject::tr( "Sound comments" ).toStdString();
}

int SoundComment::GetDataCount() const
{
	return 2;
}

string SoundComment::GetDataName( int iIndex ) const
{
	string sRet;

	switch( iIndex )
	{
		case 0 :
			{
                sRet = QObject::tr( "time pos. [ms]" ).toStdString();
			}
			break;
		case 1 :
			{
                sRet = QObject::tr( "comment" ).toStdString();
			}
			break;

		default:
			sRet = "";
	}

	return sRet;
}

SoundComment::DataType SoundComment::GetDataType( int iIndex ) const
{
	switch( iIndex )
	{
		case 0 :
			return _DOUBLE;
		case 1 :
			return _STRING;

		default:
			return _VOID;
	}
}

string SoundComment::GetDataValue( int iIndex ) const
{
	string sRet;

	switch( iIndex )
	{
		case 0 :
			{
				char sBuffer[512];

				sprintf( sBuffer, "%8.0lf", GetTimePos() );

				sRet = sBuffer;
			}
			break;
		case 1 :
			{
				sRet = GetComment();
			}
			break;

		default:
			sRet = "";
	}

	return sRet;
}

bool SoundComment::SetDataValue( int iIndex, const string & sValue )
{
	bool bRet = false;

	switch( iIndex )
	{
		case 0 :
			{
				m_dTimePosInMS = atof( sValue.c_str() );

				bRet = true;
			}
			break;
		case 1 :
			{
				m_sComment = sValue;

				bRet = true;
			}
			break;

		default:
			bRet = false;
	}

	return bRet;
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

// ** helper class, to compare two objects handled with an smart-pointer
template <class T>
class minHandleCompare : public binary_function< minHandle<T>, minHandle<T>, bool >
{
public:
	bool operator()( minHandle<T> hLeft, minHandle<T> hRight ) const
	{
		return *hLeft < *hRight;
	}
};


SoundCommentContainer::SoundCommentContainer()
: IOContainer<SoundComment>( _SOUND_COMMENT_CONTAINER )
{
}

void SoundCommentContainer::AddComment( double dTimePosInMS, const string & sComment )
{
	push_back( minHandle<SoundComment>( new SoundComment( dTimePosInMS, sComment ) ) );

	SetChanged();
}

void SoundCommentContainer::SortData()
{
	sort( begin(), end(), minHandleCompare< ItemClass >() );
}

int	SoundCommentContainer::size() const
{
	return IOContainer<SoundComment>::size();
}

void SoundCommentContainer::clear()
{
	IOContainer<SoundComment>::erase( IOContainer<SoundComment>::begin(), IOContainer<SoundComment>::end() );
}

GenericDataInterface * SoundCommentContainer::at( int iIndex )
{
	return /*IOContainer<SoundComment>*/(*this)[ iIndex ].GetPtr();
}

GenericDataInterface * SoundCommentContainer::push_back_new_item()
{
	minHandle<SoundComment> hItem( new ItemClass( 0, "" ) );
	IOContainer<SoundComment>::push_back( hItem );
	return at( size()-1 );
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

PlotComment::PlotComment( int iStartDiaIndex, int iStopDiaIndex, const string & sComment )
: m_iStartIndex( iStartDiaIndex ),
  m_iStopIndex( iStopDiaIndex ),
  m_sComment( sComment )
{
}

int PlotComment::GetStartIndex() const
{
	return m_iStartIndex;
}

int PlotComment::GetStopIndex() const
{
	return m_iStopIndex;
}

string PlotComment::GetComment() const
{
	return m_sComment;
}

XmlTree	PlotComment::GetXMLTree() const
{
	XmlTree aTree( "plotcomment" );

	aTree.PushTag( "startindex", m_iStartIndex );
	aTree.PushTag( "stopindex", m_iStopIndex );
	aTree.PushTag( "comment", m_sComment, XmlAttribList( "type", "string" ) );

	return aTree;
}

bool PlotComment::Read( istream & aStream )
{
	FileUtilityObj aFU;

	aFU.ReadStructBegin( aStream );
	aStream >> m_iStartIndex;
	aFU.ReadSeparator( aStream );
	aStream >> m_iStopIndex;
	aFU.ReadSeparator( aStream );
	ReadString( aStream, m_sComment );
	aFU.ReadStructEnd( aStream );

	return aStream.good();
}

bool PlotComment::Write( ostream & aStream ) const
{
	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	aStream << m_iStartIndex;
	aFU.WriteSeparator( aStream );
	aStream << m_iStopIndex;
	aFU.WriteSeparator( aStream );
	WriteString( aStream, m_sComment );
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

bool PlotComment::operator<( const PlotComment & aOther ) const
{
	return ( m_iStartIndex < aOther.m_iStartIndex );
}

string PlotComment::GetName() const
{
    return QObject::tr( "Plot comments" ).toStdString();
}

int PlotComment::GetDataCount() const
{
	return 3;
}

string PlotComment::GetDataName( int iIndex ) const
{
	string sRet;

	switch( iIndex )
	{
		case 0 :
			{
                sRet = QObject::tr( "start index" ).toStdString();
			}
			break;
		case 1 :
			{
                sRet = QObject::tr( "stop index" ).toStdString();
			}
			break;
		case 2 :
			{
                sRet = QObject::tr( "comment" ).toStdString();
			}
			break;

		default:
			sRet = "";
	}

	return sRet;
}

PlotComment::DataType PlotComment::GetDataType( int iIndex ) const
{
	switch( iIndex )
	{
		case 0 :
			return _INT;
		case 1 :
			return _INT;
		case 2 :
			return _STRING;

		default:
			return _VOID;
	}
}

string PlotComment::GetDataValue( int iIndex ) const
{
	string sRet;

	switch( iIndex )
	{
		case 0 :
			{
				char sBuffer[512];

				sprintf( sBuffer, "%d", GetStartIndex() );

				sRet = sBuffer;
			}
			break;
		case 1 :
			{
				char sBuffer[512];

				sprintf( sBuffer, "%d", GetStopIndex() );

				sRet = sBuffer;
			}
			break;
		case 2 :
			{
				sRet = GetComment();
			}
			break;

		default:
			sRet = "";
	}

	return sRet;
}

bool PlotComment::SetDataValue( int iIndex, const string & sValue )
{
	bool bRet = false;

	switch( iIndex )
	{
		case 0 :
			{
				m_iStartIndex = atoi( sValue.c_str() );

				bRet = true;
			}
			break;
		case 1 :
			{
				m_iStopIndex = atoi( sValue.c_str() );

				bRet = true;
			}
			break;
		case 2 :
			{
				m_sComment = sValue;

				bRet = true;
			}
			break;

		default:
			bRet = false;
	}

	return bRet;
}

// *******************************************************************

PlotCommentContainer::PlotCommentContainer()
: IOContainer<PlotComment>( _PLOT_COMMENT_CONTAINER )
{
}

void PlotCommentContainer::AddComment( int iStartDiaIndex, int iStopDiaIndex, const string & sComment )
{
	push_back( minHandle<PlotComment>( new PlotComment( iStartDiaIndex, iStopDiaIndex, sComment ) ) );

	SetChanged();
}

void PlotCommentContainer::SortData()
{
	sort( begin(), end(), minHandleCompare< ItemClass >() );
}

int	PlotCommentContainer::size() const
{
	return IOContainer<PlotComment>::size();
}

void PlotCommentContainer::clear()
{
	IOContainer<PlotComment>::erase( IOContainer<PlotComment>::begin(), IOContainer<PlotComment>::end() );
}

GenericDataInterface * PlotCommentContainer::at( int iIndex )
{
	return /*IOContainer<PlotComment>*/(*this)[ iIndex ].GetPtr();
}

GenericDataInterface * PlotCommentContainer::push_back_new_item()
{
	minHandle<PlotComment> hItem( new ItemClass( 0, 0, "" ) );
	IOContainer<PlotComment>::push_back( hItem );
	return at( size()-1 );
}

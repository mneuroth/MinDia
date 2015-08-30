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

#include "diapresentation.h"
#include "diacallback.h"
#include "rolleicom.h"
#include "minisound.h"
#include "minexception.h"
#include "minutils.h"		// for: FileUtilityObj
#include "minlog.h"
#include "misctools.h"

#include <stdio.h>

#include <QObject>
#include <QImage>
#include <QPainter>
#include <QFileInfo>
#include <QDir>

const char * g_sDefPresentationFileName = "diapresentation.dia";

extern bool IsExecuteScriptAllowed();

void _FadeImage( QPainter * pPainter, int iFadeFactor, const QImage & aImagePrevious, const QImage & aImage );

// *******************************************************************
// *******************************************************************
// *******************************************************************

//const double c_dMinSlideChangeTime = 3.5;	// in seconds

const int DiaPresentation::ACT_FILE_VERSION = 8;
				// History: ver. 0 --> until 10.11.2001; change: SoundInfoContainer
				//          ver. 1 --> until 17.11.2001; change: SoundCommentContainer
				//			ver. 2 --> until 27.12.2001; change: PlotCommentContainer
				//			ver. 3 --> until 30.12.2001; change: ApplScriptEnvironment
				//			ver. 4 --> until 22. 2.2003; change: DynGraphicOpContainer
				//			ver. 5 --> until 22. 3.2003; change: DynContainer, removed: DynGraphicOpContainer
                //			ver. 6 --> until 27. 3.2013; change: ImageRatio
                //			ver. 7 --> until 29. 4.2013; change: OutputWidth, OutputHeight

const char * DiaPresentation::MAGIC_FILE_STRING = "MINDIA";

// ** XML Tags **
const char * _DIA_PROJECTOR_CONTAINER	= "diaprojectors";
const char * _DIA_INFO_CONTAINER		= "diainfos";

// *******************************************************************

DiaPresentation::DiaPresentation( bool bEnableScript, DiaCallback * pCallback, const string & sName, minLoggingInterface * pLogging, IDiaOutputWindowInternal *	pOutputWindowProxy )
:
  //m_pOutputWindowProxy( pOutputWindowProxy ),
  //m_aDynGraphicOpContainer( pOutputWindowProxy ),
  //m_aDynGraphicOpProcessor( m_aDynGraphicOpContainer ),
  //m_aDiaContainer(),
  m_aDynTextContainer( pOutputWindowProxy ),
  m_aScriptEnv( true ),
  m_hGenDev( g_IGeneralDeviceID ),
  m_aImageRatio( RATIO_VARIABLE ),
  m_pProjectorCom( 0 ),
  m_pSoundPlayer( 0 ),
  m_pCallback( pCallback ),
  m_pLogging( pLogging )
{
	m_aObjectChanged.ClearChanged();

	Init();

	m_sName = sName;

	// ** init the known event types
	m_aScriptEnv.AddEventType( _PRESENTATION_STARTED );
	m_aScriptEnv.AddEventType( _PRESENTATION_STOPED );
	m_aScriptEnv.AddEventType( _PRESENTATION_PAUSED );
	m_aScriptEnv.AddEventType( _PRESENTATION_SAVED );
	m_aScriptEnv.AddEventType( _PRESENTATION_LOADED );
	m_aScriptEnv.SetEnabled( bEnableScript );
}

void DiaPresentation::Init()
{
	ResetPlay();

	m_sName = "";
	m_sComment = "";

	// you need at least two projectors for a slide show...
	minHandle<DiaProjector> hProjector1( new DiaProjector( "Projector 1" ) );
	minHandle<DiaProjector> hProjector2( new DiaProjector( "Projector 2" ) );
	m_aProjectorContainer.push_back( hProjector1 );
	m_aProjectorContainer.push_back( hProjector2 );
}

void DiaPresentation::Clear()
{
	m_aProjectorContainer.erase( m_aProjectorContainer.begin(), m_aProjectorContainer.end() );
	m_aDiaContainer.erase( m_aDiaContainer.begin(), m_aDiaContainer.end() );
	m_aSoundInfoContainer.erase( m_aSoundInfoContainer.begin(), m_aSoundInfoContainer.end() );
	m_aSoundCommentContainer.erase( m_aSoundCommentContainer.begin(), m_aSoundCommentContainer.end() );
	m_aPlotCommentContainer.erase( m_aPlotCommentContainer.begin(), m_aPlotCommentContainer.end() );
	//m_aDynGraphicOpContainer.erase( m_aDynGraphicOpContainer.begin(), m_aDynGraphicOpContainer.end() );
    m_aDynTextContainer.erase( m_aDynTextContainer.begin(), m_aDynTextContainer.end() );

	m_aScriptEnv.Clear();

	Init();

	m_sName = g_sDefPresentationFileName;
}

bool DiaPresentation::IsChanged() const
{
	return m_aObjectChanged.IsChanged() || m_aDiaContainer.IsChanged() || 
		   m_aSoundInfoContainer.IsChanged() || m_aSoundCommentContainer.IsChanged() ||
		   m_aPlotCommentContainer.IsChanged() || 
		   //m_aDynGraphicOpContainer.IsChanged() ||
           m_aDynTextContainer.IsChanged() ||
           m_aScriptEnv.IsChanged();
}

bool DiaPresentation::IsPlaying() const
{
	return m_bIsPlaying;
}

bool DiaPresentation::IsPause() const
{
	return m_bIsPause;
}

bool DiaPresentation::IsEdit() const
{
	return !(IsPause() || IsPlaying());
}

int DiaPresentation::GetDiaCount() const
{
	return m_aDiaContainer.size();
}

int DiaPresentation::GetProjectorCount() const
{
	return m_aProjectorContainer.size();
}

minHandle<DiaProjector> DiaPresentation::GetProjectorPtr( int iNo ) const
{
	return m_aProjectorContainer[ iNo ];
}

SoundInfoContainer & DiaPresentation::GetSoundInfoData()
{
	return m_aSoundInfoContainer;
}

const SoundInfoContainer & DiaPresentation::GetSoundInfoData() const 
{
	return m_aSoundInfoContainer;
}

SoundCommentContainer &	DiaPresentation::GetSoundCommentData()
{
	return m_aSoundCommentContainer;
}

PlotCommentContainer & DiaPresentation::GetPlotCommentData()
{
	return m_aPlotCommentContainer;
}

/*
DynGraphicOpContainer &	DiaPresentation::GetDynGraphicOpData()
{
	return m_aDynGraphicOpContainer;
}
*/

DynTextContainer & DiaPresentation::GetDynGraphicData()
{
    return m_aDynTextContainer;
}

const DynTextContainer & DiaPresentation::GetDynGraphicData() const
{
    return m_aDynTextContainer;
}

ApplScriptEnvironment &	DiaPresentation::GetScriptEnvironment()
{
	return m_aScriptEnv;
}

string DiaPresentation::GetFullName() const
{
	return m_sPathInfo+m_sName;
}

string DiaPresentation::GetName() const
{
    return m_sName;
}

void DiaPresentation::SetName( const string & sNewName )
{
    QFileInfo aNewFileInfo( ToQString(sNewName) );
    m_sPathInfo = ToStdString( aNewFileInfo.absolutePath()+QDir::separator() );
    m_sName = ToStdString( aNewFileInfo.fileName() );

	// name is only for view, not a logical data item for a dia presentation (29.3.2003)
	//m_aObjectChanged.SetChanged();
}

string DiaPresentation::GetComment() const
{
	return m_sComment;
}

void DiaPresentation::SetComment( const string & sComment )
{
	m_sComment = sComment;

	m_aObjectChanged.SetChanged();
}

int	DiaPresentation::FindItemWithText( const string & sText, int iStartIndex ) const
{
	if( (iStartIndex >= 0) && (iStartIndex < (int)m_aDiaContainer.size()) )
	{
		DiaInfoContainer::const_iterator aStartIter = m_aDiaContainer.begin() + iStartIndex; 
        DiaInfoContainer::const_iterator aIter = m_aDiaContainer.FindItemWithText( sText, aStartIter, GetDynGraphicData() );

		if( aIter != m_aDiaContainer.end() )
		{
			return (*aIter)->GetPosition();
		}
	}

	return -1;
}

void DiaPresentation::MakeRelativePaths()
{
    m_sName = ConvertToRelPath( m_sName );
	m_aDiaContainer.MakeRelativePaths();
	m_aSoundInfoContainer.MakeRelativePaths();
	m_aObjectChanged.SetChanged();
}

void DiaPresentation::MakeAbsolutePaths( const string & sDir )
{
    m_sName = ConvertToAbsPath( m_sName, sDir );
    m_aDiaContainer.MakeAbsolutePaths( sDir );
    m_aSoundInfoContainer.MakeAbsolutePaths( sDir );
    m_aObjectChanged.SetChanged();
}

double DiaPresentation::GetOffsetForSound() const
{
	return GetDissolveTimeOfSlide( 0 );
}

ImageRatio DiaPresentation::GetImageRatio() const
{
    return m_aImageRatio;
}

void DiaPresentation::SetImageRatio( ImageRatio value )
{
    m_aImageRatio = value;

    m_aObjectChanged.SetChanged();
}

void DiaPresentation::GetImageSize( unsigned long & iWidth, unsigned long & iHeight ) const
{
    iWidth = m_ulOutputWidth;
    iHeight = m_ulOutputHeight;
}

void DiaPresentation::SetImageSize( unsigned long iWidth, unsigned long iHeight )
{
    m_ulOutputWidth = iWidth;
    m_ulOutputHeight = iHeight;

    m_aObjectChanged.SetChanged();
}

bool DiaPresentation::Read( istream & aStream, bool bExecuteEventScript )
{
	// clear all old data
	//Clear();

	// and read new data...
	FileUtilityObj aFU;
	int iActFileVersion = 0;
	string sMagicFileString;

	aFU.ReadStructBegin( aStream );
	ReadString( aStream, sMagicFileString );
	if( sMagicFileString != MAGIC_FILE_STRING )
	{
		// ** use an exception to show an error,
		// ** so you don't have to know here something about the gui !!!
		throw MinException( "This is not an valid data-file !" );
		return false;
	}
	aFU.ReadSeparator( aStream );
	aStream >> iActFileVersion;
	aFU.ReadSeparator( aStream );
	ReadString( aStream, m_sName );
	aFU.ReadSeparator( aStream );
	ReadString( aStream, m_sComment );
    if( iActFileVersion > 6 )				// since 27. 3.2013
    {
        int iImageRatio;

        aFU.ReadSeparator( aStream );
        aStream >> iImageRatio;
        SetImageRatio( (ImageRatio)iImageRatio );
    }
    else
    {
        SetImageRatio( RATIO_IMAGE_RATIO );
    }
    if( iActFileVersion > 7 )				// since 29. 4.2013
    {
        unsigned long ulWidth, ulHeight;

        aFU.ReadSeparator( aStream );
        aStream >> ulWidth;
        aFU.ReadSeparator( aStream );
        aStream >> ulHeight;
        SetImageSize( ulWidth, ulHeight );
    }
    else
    {
        SetImageSize( 1920, 1080 );
    }
    aFU.ReadSeparator( aStream );
	m_aProjectorContainer.Read( aStream );
	aFU.ReadSeparator( aStream );
	m_aDiaContainer.Read( aStream );
	if( iActFileVersion > 0 )				// since 10.11.2001
	{
		aFU.ReadSeparator( aStream );
		m_aSoundInfoContainer.Read( aStream );
		m_aSoundInfoContainer.ClearChanged();
	}
	else
	{
		// ** if an old file is read, erase the sound-info container of the old (replaced) file
		m_aSoundInfoContainer.erase( m_aSoundInfoContainer.begin(), m_aSoundInfoContainer.end() );
	}
	if( iActFileVersion > 1 )				// since 17.11.2001
	{
		aFU.ReadSeparator( aStream );
		m_aSoundCommentContainer.Read( aStream );
		m_aSoundCommentContainer.ClearChanged();
	}
	else
	{
		m_aSoundCommentContainer.erase( m_aSoundCommentContainer.begin(), m_aSoundCommentContainer.end() );
	}
	if( iActFileVersion > 2 )				// since 27.12.2001
	{
		aFU.ReadSeparator( aStream );
		m_aPlotCommentContainer.Read( aStream );
		m_aPlotCommentContainer.ClearChanged();
	}
	else
	{
		m_aPlotCommentContainer.erase( m_aPlotCommentContainer.begin(), m_aPlotCommentContainer.end() );
	}
	if( iActFileVersion > 3 )				// since 30.12.2001
	{
		aFU.ReadSeparator( aStream );
		m_aScriptEnv.Read( aStream );
		//not needed here, is done in the class: m_aScriptEnv.ClearChanged();
	}
	else
	{
		m_aScriptEnv.Clear();
		m_aScriptEnv.ClearChanged();
	}
/*	if( iActFileVersion > 4 )				// since 22. 2.2003
	{
		aFU.ReadSeparator( aStream );
		m_aDynGraphicOpContainer.Read( aStream );
		m_aDynGraphicOpContainer.ClearChanged();
	}
	else
	{
		m_aDynGraphicOpContainer.erase( m_aDynGraphicOpContainer.begin(), m_aDynGraphicOpContainer.end() );
	}
*/	if( iActFileVersion > 5 )				// since 22. 3.2003
	{
		aFU.ReadSeparator( aStream );
        m_aDynTextContainer.Read( aStream );
        m_aDynTextContainer.ClearChanged();
	}
	else
	{
        m_aDynTextContainer.erase( m_aDynTextContainer.begin(), m_aDynTextContainer.end() );
	}
	aFU.ReadStructEnd( aStream );

	if( aStream.good() )
	{
		m_aObjectChanged.ClearChanged();
	}

	if( bExecuteEventScript )
	{
		MyExecuteScript( _PRESENTATION_LOADED );
	}

	return aStream.good();
}

bool DiaPresentation::Write( ostream & aStream ) const
{
	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	WriteString( aStream, MAGIC_FILE_STRING );
	aFU.WriteSeparator( aStream );
	aStream << endl;
	aStream << ACT_FILE_VERSION;			// since 21.10.2001
	aFU.WriteSeparator( aStream );
	WriteString( aStream, m_sName );
	aFU.WriteSeparator( aStream );
	WriteString( aStream, m_sComment );
	aFU.WriteSeparator( aStream );
	aStream << endl;
    if( ACT_FILE_VERSION > 6 )				// since 27. 3.2013
    {
        aStream << (int)GetImageRatio();
        aFU.WriteSeparator( aStream );
    }
    if( ACT_FILE_VERSION > 7 )				// since 29. 4.2013
    {
        aStream << m_ulOutputWidth;
        aFU.WriteSeparator( aStream );
        aStream << m_ulOutputHeight;
        aFU.WriteSeparator( aStream );
    }
    m_aProjectorContainer.Write( aStream );
	aFU.WriteSeparator( aStream );
	aStream << endl;
	m_aDiaContainer.Write( aStream );
	if( ACT_FILE_VERSION > 0 )				// since 10.11.2001
	{
		aFU.WriteSeparator( aStream );
		aStream << endl;
		m_aSoundInfoContainer.Write( aStream );
		((DiaPresentation *)this)->m_aSoundInfoContainer.ClearChanged();
	}
	if( ACT_FILE_VERSION > 1 )				// since 17.11.2001
	{
		aFU.WriteSeparator( aStream );
		aStream << endl;
		m_aSoundCommentContainer.Write( aStream );
		((DiaPresentation *)this)->m_aSoundCommentContainer.ClearChanged();
	}
	if( ACT_FILE_VERSION > 2 )				// since 27.12.2001
	{
		aFU.WriteSeparator( aStream );
		aStream << endl;
		m_aPlotCommentContainer.Write( aStream );
		((DiaPresentation *)this)->m_aPlotCommentContainer.ClearChanged();
	}
	if( ACT_FILE_VERSION > 3 )				// since 30.12.2001
	{
		aFU.WriteSeparator( aStream );
		aStream << endl;
		m_aScriptEnv.Write( aStream );
		//not needed here, is done in the class: ((DiaPresentation *)this)->m_aScriptEnv.ClearChanged();
	}
/*	if( ACT_FILE_VERSION > 4 )				// since 22. 2.2003
	{
		aFU.WriteSeparator( aStream );
		aStream << endl;
		m_aDynGraphicOpContainer.Write( aStream );
		((DiaPresentation *)this)->m_aDynGraphicOpContainer.ClearChanged();
	}
*/	if( ACT_FILE_VERSION > 5 )				// since 22. 3.2003
	{
		aFU.WriteSeparator( aStream );
		aStream << endl;
        m_aDynTextContainer.Write( aStream );
        ((DiaPresentation *)this)->m_aDynTextContainer.ClearChanged();
	}
    aFU.WriteStructEnd( aStream );

	if( aStream.good() )
	{
		// cast because of const, change only transient data...
		((DiaPresentation *)this)->m_aObjectChanged.ClearChanged();
	}

	((DiaPresentation *)this)->MyExecuteScript( _PRESENTATION_SAVED );

	return aStream.good();
}

XmlTree DiaPresentation::GetXMLTree() const
{
	XmlTree aTree( "mindia", "", false, XmlAttribList( "fileversion", ToString( ACT_FILE_VERSION ) ) );

	aTree.PushTag( "name", m_sName );
	aTree.PushTag( "comment", m_sComment, XmlAttribList( "type", "string" ) );

	aTree.PushTag( m_aProjectorContainer.GetXMLTree() );
	aTree.PushTag( m_aDiaContainer.GetXMLTree() );
	aTree.PushTag( m_aSoundInfoContainer.GetXMLTree() );
	aTree.PushTag( m_aSoundCommentContainer.GetXMLTree() );
	aTree.PushTag( m_aPlotCommentContainer.GetXMLTree() );
	//aTree.PushTag( m_aDynGraphicOpContainer.GetXMLTree() );
    aTree.PushTag( m_aDynTextContainer.GetXMLTree() );
	aTree.PushTag( m_aScriptEnv.GetXMLTree() );

	return aTree;
}

minHandle<DiaInfo> DiaPresentation::AddDiaAt( int iIndex, minHandle<DiaInfo> hNewDia )
{
	// ** insert dia at the end of the data ?
	if( iIndex<0 )
	{
		// ** yes
		iIndex = GetDiaCount();
	}
	
	// ** if a valid Dia-Object is given, take it...
	// ** otherwise create a new object.
	minHandle<DiaInfo> aNewDia;
	if( hNewDia.IsOk() )
	{
		aNewDia = hNewDia;
	}
	else
	{
		aNewDia = minHandle<DiaInfo>( new DiaInfo() );
	}

	if( iIndex>=0 && iIndex<GetDiaCount() )
	{
		m_aDiaContainer.insert( m_aDiaContainer.begin()+iIndex, aNewDia );

		// ** new element inserted, update position numbers
		m_aDiaContainer.SyncPositionInfos();

		m_aObjectChanged.SetChanged();

		return m_aDiaContainer[ iIndex];
	}
	else if( (iIndex == GetDiaCount()) || /*insert at the end*/(iIndex < 0) )
	{
		m_aDiaContainer.push_back( aNewDia );

		// ** new element inserted, update position numbers
		m_aDiaContainer.SyncPositionInfos();

		m_aObjectChanged.SetChanged();

		return m_aDiaContainer[ GetDiaCount()-1 ];
	}
	return 0;
}

int DiaPresentation::CountUUIDs( const string & sUUID ) const
{
    int iCount = 0;
    for( unsigned int i=0; i<m_aDiaContainer.size(); i++ )
    {
        if( m_aDiaContainer[i]->GetUUID()==sUUID )
        {
            iCount++;
        }
    }
    return iCount;
}

int DiaPresentation::GetDiaIndexForUUID( const string & sUUID ) const
{
    int iIndex = -1;
    for( unsigned int i=0; i<m_aDiaContainer.size(); i++ )
    {
        if( m_aDiaContainer[i]->GetUUID()==sUUID )
        {
            return i;
        }
    }
    return iIndex;
}

minHandle<DiaInfo> DiaPresentation::GetDia( const string & sUUID ) const
{
    int iIndex = GetDiaIndexForUUID( sUUID );
    if( iIndex>=0 )
    {
        return m_aDiaContainer[iIndex];
    }
    return 0;
}

minHandle<DiaInfo> DiaPresentation::GetDiaAt( int iIndex ) const
{
	if( iIndex>=0 && iIndex<GetDiaCount() )
	{
		return m_aDiaContainer[ iIndex ];
	}
	return 0;
}

QStringList DiaPresentation::GetAllImageFileNames() const
{
    QStringList ret;
    vector<string> lstImageNames = m_aDiaContainer.GetAllImageFileNames();
    foreach (const string & sImageFileName, lstImageNames)
    {
        ret.push_back( ToQString(sImageFileName) );
    }
    return ret;
}

bool DiaPresentation::RemoveDiaAt( int iIndex )
{
	if( iIndex>=0 && iIndex<GetDiaCount() )
	{
		m_aDiaContainer.erase( m_aDiaContainer.begin() + iIndex );
		// ** new element inserted, update position numbers
		m_aDiaContainer.SyncPositionInfos();
		m_aObjectChanged.SetChanged();
		return true;
	}
	return false;
}

double DiaPresentation::GetDiaAbsStartDissolveTime( int iIndex ) const
{
	double dTime = 0;

	for( int i=0; i<iIndex; i++ )
	{
		dTime += m_aDiaContainer[ i ]->GetTotalTime();
	}

	return dTime;
}

double DiaPresentation::GetDiaAbsStartShowTime( int iIndex ) const
{
	double dTime = GetDiaAbsStartDissolveTime( iIndex );

	if( (iIndex>=0) && (iIndex<(int)m_aDiaContainer.size()) )
	{
		dTime += m_aDiaContainer[ iIndex ]->GetDissolveTime();
	}

	return dTime;
}

double DiaPresentation::GetDiaAbsFinishDissolveTime( int iIndex ) const
{
    double dTime = GetDiaAbsStartShowTime( iIndex );

    if( (iIndex>=0) && (iIndex<(int)m_aDiaContainer.size()) )
    {
        dTime += m_aDiaContainer[ iIndex ]->GetShowTime();
    }
    // goto next slide and get the fade in time
    iIndex++;
    if( (iIndex>=0) && (iIndex<(int)m_aDiaContainer.size()) )
    {
        dTime += m_aDiaContainer[ iIndex ]->GetDissolveTime();
    }

    return dTime;
}

bool DiaPresentation::UpdateShowTimeForDia( int iIndex, double dDeltaTime )
{
    bool ok = false;

	if( (iIndex>=0) && (iIndex<(int)m_aDiaContainer.size()) )
	{
        ok = m_aDiaContainer[ iIndex ]->SetShowTime( m_aDiaContainer[ iIndex ]->GetShowTime()+dDeltaTime );

        UpdateAttachedDynamicTexts( iIndex, dDeltaTime );
    }

    return ok;
}

bool DiaPresentation::UpdateDissolveTimeForDia( int iIndex, double dDeltaTime )
{
    bool ok = false;

    if( (iIndex>=0) && (iIndex<(int)m_aDiaContainer.size()) )
	{
        ok = m_aDiaContainer[ iIndex ]->SetDissolveTime( m_aDiaContainer[ iIndex ]->GetDissolveTime()+dDeltaTime );

        UpdateAttachedDynamicTexts( iIndex, dDeltaTime );
    }

    return ok;
}

void DiaPresentation::UpdateAttachedDynamicTexts( int iIndex, double dDeltaTime )
{
    // update the attached Dynamic Text items for all following dias...
    for( unsigned int i=iIndex+1; i<m_aDiaContainer.size(); i++ )
    {
        string sUUID = m_aDiaContainer[ i ]->GetUUID();
        if( sUUID.length()>0 )
        {
            m_aDynTextContainer.UpdateShowTimeForDia( sUUID, dDeltaTime*1000.0 );
        }
    }
}

double DiaPresentation::GetTotalTime() const
{
	double dTime = 0;

	for( int i=0; i<GetDiaCount(); i++ )
	{
		dTime += m_aDiaContainer[ i ]->GetTotalTime();
	}

	return dTime;
}

double DiaPresentation::GetActPlayTime() const
{
	if( !m_bIsPlaying )
	{
		return m_dPlayTime;
	}
	else
	{
		return m_dPlayTime + ((double)m_aPlayTime.elapsed()) * 0.001;
	}
}

// sound has a small offset, because sound starts after dissolve-time for fist slide
double DiaPresentation::GetActSoundPlayTime() /*const*/
{
	return GetActPlayTime() - GetOffsetForSound();
}

double DiaPresentation::GetCountDownTime() /*const*/
{
	if( m_bIsPlaying || m_bIsPause )
	{
		if( m_bIsPause )
		{
			return m_dCountDownStartTime - m_dCountDownTime;
		}
		else
		{
			return m_dCountDownStartTime - m_dCountDownTime - ((double)m_aCountDown.elapsed())*0.001;
		}
	}
	return -1;
}

void DiaPresentation::ResetPlay()
{
	m_bIsPlaying = false;
	m_bIsPause = false;
	m_dPlayTime = 0;
	m_dCountDownTime = 0;
	m_dCountDownStartTime = 0;
	m_iActPos = 0;
	m_iStartPos = 0;
	m_iActSubPos = 0;
	m_sStepInfo = "reset";
	m_aPlayTime = QTime();
	m_aCountDown = QTime();
}

bool DiaPresentation::StartPlay( int iStartPos )
{
	MyExecuteScript( _PRESENTATION_STARTED );

	m_dAbsSoundStartTime = 0;

	m_bIsPlaying = true;
	m_bIsPause = false;

	m_bContinued = (m_dCountDownTime > 0);

    bool bOk = true;

	// ** switch to direct mode, only if comming not from pause-mode !
	if( !m_bContinued )
	{
		// ** switch to direct mode only if in keyboard mode !
		if( m_pProjectorCom && !m_pProjectorCom->IsDirectMode() )
		{
			bOk = m_pProjectorCom->DirectMode( /*bSync*/true );
		}
// min todo --> hier ggf. weiter init-values setzen: light-value, autofocus etc.
	}

	// (8.11.2003) check for other projector devices
	if( ExistsExternalDevice() )
	{
		m_hGenDev->Start();
	}

    // ** sometimes the switch to the direct mode fails
    // ** int that case inform the user about the error and
    // ** stop the presentation...
    if( bOk )
    {
		// ** if there is a positive StartPos given, we want
		// ** to start the presentation from a selected dia 
		if( iStartPos >= 0 )
		{
			m_iActPos = iStartPos;
			m_iStartPos = iStartPos;
		}

        // ** prepare the projector for the first slide:
        // ** magazine is just loaded, no slide is transported;
        // ** set first dissolve-time and load first slide in synchronios mode,
        // ** all other operations in the presentation will run in batch mode
        if( (iStartPos<0) || (m_iActPos==m_iStartPos) )
        {
			int iStartIndex = max( iStartPos, 0 );
            double dDissolveTime = GetDissolveTimeOfSlide( iStartIndex );
			double dAbsStartTime = GetDiaAbsStartDissolveTime( iStartIndex );
			m_dAbsSoundStartTime = dAbsStartTime + dDissolveTime - GetOffsetForSound();
            int iStepTime = (int)(dDissolveTime*10);

			if( m_pProjectorCom )
			{
				bOk = m_pProjectorCom->SetDissolveTime( iStepTime, /*bSync*/true );
	            bOk = m_pProjectorCom->SlideForward( /*bSync*/true );
			}

            // ** the first dissolve-time will not be measured with the timer...
            m_dPlayTime += dDissolveTime + dAbsStartTime;
			// ** Attention: the play time will be recalibrated in the first step

			// ** old: in simulation mode wait the dissolve time (first dia is synchronious)
			// ** new: no special handling for first slide in simulation mode
			if( m_pProjectorCom && m_pProjectorCom->IsSimulation() )
			{
				//old: minSleep( dDissolveTime*1000 );
				m_dPlayTime -= dDissolveTime;
			}

			// (8.11.2003) check for other projector devices
			if( ExistsExternalDevice() )
			{
				m_hGenDev->SetDissolveTime( m_iActPos % m_hGenDev->GetDeviceCount(), dDissolveTime );
				m_hGenDev->SlideForward( m_iActPos % m_hGenDev->GetDeviceCount() );
			}
        }
    }

    if( bOk )
    {
	    // ** now start the timers **
	    m_aPlayTime.restart();
	    m_aCountDown.restart();

		// bugfix 21.1.2003: continue Sound after pause !
		if( m_pSoundPlayer && m_bContinued )
		{
			m_pSoundPlayer->Continue();
		}
		// graphic operations are connected to the music !
		//m_aDynGraphicOpProcessor.Continue();
		//m_aDynItemContainer.Continue();

		/* old (until 26.3.2002) --> now in NextStep
		if( m_pSoundPlayer )
		{
			if( !m_bContinued && (m_aSoundInfoContainer.size()>0) )
			{
				minHandle<SoundInfo> hItem = *m_aSoundInfoContainer.begin(); 
                m_pSoundPlayer->SetWavFile( ToQString( hItem->GetFileName() ) );
			}
			if( m_bContinued )
			{
				m_pSoundPlayer->Continue();
			}
			else
			{
				m_pSoundPlayer->Start( (int)(m_dAbsSoundStartTime*1000) );
			}
		}
		*/
    }
    else
    {
        StopPlay();

        if( m_pCallback )
        {
            m_pCallback->ShowError( "Starting failed !" );
        }
    }

	// ** modus changed, update observers
	if( m_pCallback )
	{
		m_pCallback->PresentationModusChanged();
	}

	return bOk;
}

void DiaPresentation::StopPlay()
{
	m_bIsPlaying = false;
	m_bIsPause = false;
	m_dPlayTime = 0;
	m_dCountDownTime = 0;

	if( m_pProjectorCom )
	{
		// ** stoping of the presentation also in synchronious mode !

		// ** set default dissolve time
		m_pProjectorCom->SetDissolveTime( 25, /*bSync*/true  );
		// ** and stop the presentation, back to keyboard mode
		m_pProjectorCom->Reset( /*bSync*/true );
	}

	// (8.11.2003) check for other projector devices
	if( ExistsExternalDevice() )
	{
		m_hGenDev->Stop();
	}

	if( m_pSoundPlayer )
	{
		m_pSoundPlayer->Stop();
	}
	// graphic operations are connected to the music !
	//m_aDynGraphicOpProcessor.Stop();
    m_aDynTextContainer.Stop();

	// ** modus changed, update observers
	if( m_pCallback )
	{
		m_pCallback->PresentationModusChanged();
	}

	ResetPlay();

	MyExecuteScript( _PRESENTATION_STOPED );
}

void DiaPresentation::PausePlay()
{
	m_bIsPlaying = false;
	m_bIsPause = true;
	m_dPlayTime += ((double)m_aPlayTime.elapsed())*0.001;
	m_dCountDownTime += ((double)m_aCountDown.elapsed())*0.001;

	// (8.11.2003) check for other projector devices
	if( ExistsExternalDevice() )
	{
		m_hGenDev->Pause();
	}

	if( m_pSoundPlayer )
	{
		m_pSoundPlayer->Pause();
	}
	// graphic operations are connected to the music !
	//m_aDynGraphicOpProcessor.Pause();
    m_aDynTextContainer.Pause();

	// ** modus changed, update observers
	if( m_pCallback )
	{
		m_pCallback->PresentationModusChanged();
	}

	MyExecuteScript( _PRESENTATION_PAUSED );
}

bool DiaPresentation::NextStep( double & dNextStepTimeOut )
{
	//QTime aStepTime;
	//aStepTime.start();

	bool bContinue = true;

	bool bWasContinued = m_bContinued;	// comming from pause mode ?
										// in that case the projector-cmd was allready send !
	double dStepTime = -1;
	double dNextStepTime = -1;

    // ** if pause was used, repeat the previous step, to finish the last step
	if( m_bContinued )
	{
		if( m_iActSubPos == 0 )
		{
			m_iActPos--;
			m_iActSubPos = 1;	// min todo --> hActDia->GetOperationCount()
		}
		else
		{
			m_iActSubPos--;
		}
	}

	// ** check for the end of the presentation
	if( (m_iActPos>=0) && (m_iActPos<GetDiaCount()) )
	{
		char sBuffer[255];
		QString sCmd = "";

		// ** get this and if possible the next slide
		minHandle<DiaInfo> hActDia = m_aDiaContainer[ m_iActPos ];
		minHandle<DiaInfo> hNextDia;
		if( m_iActPos+1<GetDiaCount() )
		{
			hNextDia = m_aDiaContainer[ m_iActPos+1 ];
		}

		// ** get time for this operation **
		TimeOperation aOperation = hActDia->GetOperation( m_iActSubPos++ ) ;
		dStepTime = aOperation.GetOperationTime();

		if( m_bContinued )
		{
		    // ** update count-down timer if pause was used
			dStepTime = dStepTime - m_dCountDownTime;
			m_bContinued = false;
		}
		else
		{
			// ** reset count-down timer only for next step
			m_dCountDownStartTime = dStepTime;
		    m_dCountDownTime = 0;
		}

		// ** now start the count-down timer (again)
		m_aCountDown = QTime();
		m_aCountDown.start();

		// ** #################################################################
		// ** #################################################################
		// ** #################################################################
		if( aOperation.GetOperationType() == TimeOperation::DISSOLVE_IN )
		{
			//cout << "DISSOLVE ";

			//int iStepTime = (int)(dStepTime*10);		// in 1/10 seconds

			// ** change dia
			sCmd.sprintf( "BV" );

			// ** the first slide of the presentation is handled specially
			// ** (init projector and first dissolve time is synchronious!)
			if( m_iActPos != m_iStartPos )
			{
				if( m_pProjectorCom && !bWasContinued )
				{
					// ** realtime command !
					m_pProjectorCom->SlideForward( /*bSync*/false );
				}

				// (8.11.2003) check for other projector devices
				if( ExistsExternalDevice() && !bWasContinued )
				{
					m_hGenDev->SlideForward( m_iActPos % m_hGenDev->GetDeviceCount() );
				}

				// ** send an observer the message, that the actual
				// ** image has to be changed (fade in now!).
				if( m_pCallback )
				{
                    m_pCallback->TriggerDissolveActDiaNo( m_iActPos, hActDia->GetScript(), hActDia->GetImageFile(), aOperation.GetOperationTime() );
				}
			}
			else
			{
				// ********************************************************
				// ** handle the first slide !!! **

				double dDissolveTime = 0;
				if( m_pProjectorCom->IsSimulation() )
				{
					// ** dissolve the first slide (only in simulation mode) ! 
					dDissolveTime = aOperation.GetOperationTime();
				}
				else
				{
					// ** dissolve time for first slide is done synchronious (only in real mode) !
					dStepTime = 0;
				}

				// ** send an observer the message, that the actual
				// ** image has to be changed:
				// ** real modus: change now, no fade in (fist slide is synchroniously set !)
				// ** simulation modus: fade in
				if( m_pCallback )
				{
                    m_pCallback->TriggerDissolveActDiaNo( m_iActPos, hActDia->GetScript(), hActDia->GetImageFile(), dDissolveTime );
				}
				// ********************************************************
			}
		}
		// ** #################################################################
		// ** #################################################################
		// ** #################################################################
		else if( aOperation.GetOperationType() == TimeOperation::SHOW )
		{
			//cout << "SHOW ";

			// ********************************************************
			// ** handle the first slide in a special way (new since 26.3.2002):
			// ** start the music with the show-part (this means after the dissolve process)
			// ** of the first slide !!!
			if( m_iActPos == m_iStartPos )
			{
				if( m_pSoundPlayer )
				{
					// bugfix 21.1.2003: m_bContinued replaced with bWasContinued !!
					/* bugfix 25.1.2003: not needed here, will be done in soud thread !
					if( !bWasContinued && (m_aSoundInfoContainer.size()>0) )
					{
						minHandle<SoundInfo> hItem = *m_aSoundInfoContainer.begin(); 
                        m_pSoundPlayer->SetWavFile( ToQString( hItem->GetFileName() ) );
					}
					*/
					if( bWasContinued )
					{
						m_pSoundPlayer->Continue();
					}
					else
					{
						m_pSoundPlayer->Start( (int)(m_dAbsSoundStartTime*1000) );
					}
				}
				// graphic operations are connected to the music !
				if( bWasContinued )
				{
					//m_aDynGraphicOpProcessor.Continue();
                    m_aDynTextContainer.Continue();
				}
				else
				{
					//m_aDynGraphicOpProcessor.Start( (int)(m_dAbsSoundStartTime*1000) );
                    m_aDynTextContainer.Run( (int)(m_dAbsSoundStartTime*1000) );
				}
			}
			// ********************************************************

			// ** send an observer the message, that the actual
			// ** image has to be shown (now!).
			if( m_pCallback )
			{
                m_pCallback->TriggerShowActDiaNo( m_iActPos, hActDia->GetScript(), hActDia->GetImageFile(), aOperation.GetOperationTime() );
			}

			// ** if there is a next slide avaliable, set the next dissolve time
			// ** for the next slide change
			if( hNextDia.IsOk() )
			{
				// ** min todo --> hack, da direkter zugriff ueber index
				TimeOperation aNextOperation = hNextDia->GetOperation( /*always dissolve time needed*/0 ) ;
				dNextStepTime = aNextOperation.GetOperationTime();
				int iNextStepTime = (int)(dNextStepTime*10);		// in 1/10 seconds

				// ** set disolve-time for the next slide
				sCmd.sprintf( "SD:%03d", iNextStepTime );

				// ** when act. slide is visible, set the dissolve time for the next slide !
				if( m_pProjectorCom && !bWasContinued )
				{
					// ** batch command !
					m_pProjectorCom->SetDissolveTime( iNextStepTime, /*bSync*/false );
				}

				// (8.11.2003) check for other projector devices
				if( ExistsExternalDevice() && !bWasContinued )
				{
					// set dissolvetime for next slide
					m_hGenDev->SetDissolveTime( (m_iActPos+1) % m_hGenDev->GetDeviceCount(), dNextStepTime );
				}

				// ** send an observer infos about the next dia,
				// ** so the observer has the posibility to pre-load
				// ** an image and do some time consuming operations,
				// ** because now we have time while we are waiting
				// ** for the next dia-change.
				if( m_pCallback )
				{
                    m_pCallback->TriggerSetNextDiaNo( m_iActPos+1, hNextDia->GetImageFile() );
				}
			}

			// ** in any case (even for the last slide):
			// ** synchronisation of real play time with virtual play time
			// ** recalibrate the time for the next timer-step:
			// ** compare the real act. play time with the expected virtual act. play time
			// ** the recalibration is needed, because the processing of the
			// ** image-displays, timeline- and slidepult-view takes time !
			double dVirtualActTime = GetDiaAbsStartShowTime( m_iActPos );
			double dRealActTime = GetActPlayTime();
			if( dRealActTime > dVirtualActTime )
			{
				dStepTime = dStepTime - (dRealActTime - dVirtualActTime);
				m_dCountDownStartTime = dStepTime;
			}
		}


        sprintf( sBuffer, QObject::tr( "no=%d/%d ** op=%s ** timer=%6.1lf s --> cmd=\"%s\"" ).toLatin1().constData(), m_iActPos+1, GetDiaCount(), aOperation.GetOperationTypeName().c_str(), aOperation.GetOperationTime(), sCmd.toLatin1().constData() );

		m_sStepInfo = sBuffer;

		// ** next slide ?
		if( m_iActSubPos < hActDia->GetOperationCount() )
		{
			// ** no, still the same slide to process
		}
		else
		{
			// ** yes --> prepare for the next step
			m_iActSubPos = 0;
			m_iActPos++;
		}
	}
	else
	{
		// presentation is finished
		bContinue = false;
	}

	// for debugging:
	//cout << m_sStepInfo.c_str() << " TIME=" << dStepTime << endl;

	// ** 'recalibrate' the time for the next operation: subtract the processing time
	/* --> not needed, because of the synchronisation in the SHOW_TIME section !
	double dStepProcessTime = ((double)aStepTime.elapsed())*0.001;
	if( dStepProcessTime < dStepTime )
	{
		dStepTime = dStepTime - dStepProcessTime;
	}
	*/
	//cout << "delta= " << dStepProcessTime << endl;

	//return dStepTime;

	dNextStepTimeOut = dStepTime;

	return bContinue;
}

string DiaPresentation::GetStepInfo() const
{
	return m_sStepInfo;
}

void DiaPresentation::SetProjectorCom( RolleiCom * pProjectorCom )
{
	m_pProjectorCom = pProjectorCom;
}

void DiaPresentation::SetSoundPlayer( miniSound * pSoundPlayer)
{
	// ** reset the sound-info-container for the old sound player
	if( m_pSoundPlayer )
	{
		m_pSoundPlayer->SetSoundInfo( 0 );
	}

	m_pSoundPlayer = pSoundPlayer;

	if( m_pSoundPlayer )
	{
		m_pSoundPlayer->SetSoundInfo( &m_aSoundInfoContainer );
	}

}

double DiaPresentation::GetDissolveTimeOfSlide( int iSlideIndex ) const
{
	if( (iSlideIndex>=0) && (iSlideIndex<GetDiaCount()) )
	{
		minHandle<DiaInfo> hDia = m_aDiaContainer[ iSlideIndex ];

		if( hDia.IsOk() )
		{
			// ** min todo --> hack, da direkter zugriff ueber index
			TimeOperation aOperation = hDia->GetOperation( 0 );
			return aOperation.GetOperationTime();
		}
	}

	return 0;
}

bool DiaPresentation::IsScriptEnabled() const
{
    return m_aScriptEnv.IsEnabled() && IsExecuteScriptAllowed();
}

void DiaPresentation::MyExecuteScript( const string & sEvent )
{
    if( IsScriptEnabled() )
    {
        bool bScriptFound;
        int  iRet;

        m_aScriptEnv.ExecuteScriptForEvent( sEvent, bScriptFound, &iRet );
        CheckScriptResult( sEvent, bScriptFound, iRet );
    }
}

void DiaPresentation::CheckScriptResult( const string & sEvent, bool bFoundScript, int iRet )
{
	if( !bFoundScript )
	{
		cerr << "Warning: script for event " << sEvent.c_str() << " not found." << endl;
	}
	else
	{
		// ** script found, check return value
		if( iRet != 0 )
		{
			cerr << "Warning: script for event " << sEvent.c_str() << " returned value = " << iRet << endl;

			if( m_pLogging )
			{
				char sBuffer[512];
				
				sprintf( sBuffer, "Error in script iRet=%d", iRet );
				m_pLogging->LogMsg( sBuffer );
			}
		}
	}
}

bool DiaPresentation::ExistsExternalDevice()
{
	// checks if an external device is existing.
	// a query for an interface is done, if an interface is found
	// but not initialized, the interface will be created
	if( !m_hGenDev.IsValid() )
	{
		m_hGenDev.UpdateIfNotValid();
	}
	if( m_hGenDev.IsValid() && m_hGenDev->IsOk() )
	{
		return true;
	}
	return false;
}

static QImage ProcessNormalEffect( minHandle<DiaInfo> hDia, double actTime, int iWidth, int iHeight )
{
    QImage aImage1 = ReadQImageOrEmpty( ToQString( hDia->GetImageFile() ), iWidth, iHeight );
    return CopyImageArea( aImage1, hDia->GetRelX(), hDia->GetRelY(), hDia->GetRelDX(), hDia->GetRelDY() );
}

static QImage ProcessKenBurnsEffect( minHandle<DiaInfo> hDia, double diaStartTime, double actTime, int iWidth, int iHeight )
{
    double dShowTime = hDia->GetShowTime()*1000.0;
    double dx_dt = (hDia->GetRelXEnd()-hDia->GetRelX())/dShowTime;
    double dy_dt = (hDia->GetRelYEnd()-hDia->GetRelY())/dShowTime;
    double x = hDia->GetRelX() + dx_dt*(actTime-diaStartTime);
    double y = hDia->GetRelY() + dy_dt*(actTime-diaStartTime);
    double dwidth_dt = (hDia->GetRelDXEnd()-hDia->GetRelDX())/dShowTime;
    double dheight_dt = (hDia->GetRelDYEnd()-hDia->GetRelDY())/dShowTime;
    double dx = hDia->GetRelDX() + dwidth_dt*(actTime-diaStartTime);
    double dy = hDia->GetRelDY() + dheight_dt*(actTime-diaStartTime);

    QImage aImage1 = ReadQImageOrEmpty( ToQString( hDia->GetImageFile() ), iWidth, iHeight );
    return CopyImageArea( aImage1, x, y, dx, dy );
}

QImage DiaPresentation::GetSlideForTime( double dTimeMS, int iWidth, int iHeight, bool bScale, bool bDrawDynItems ) const
{
    int iIndex1 = -1;
    int iIndex2 = -1;
    int iFadeFactor = 0;

    if( GetIndexForTime( dTimeMS, iIndex1, iIndex2, iFadeFactor ) )
    {
        minHandle<DiaInfo> hDia1 = GetDiaAt( iIndex1 );
        minHandle<DiaInfo> hDia2 = GetDiaAt( iIndex2 );

        if( hDia1.IsOk() )
        {
            QImage aImage1;
            if( hDia1->IsKenBurns() )
            {
                aImage1 = ProcessKenBurnsEffect( hDia1, GetDiaAbsStartShowTime( iIndex1 )*1000.0, dTimeMS, iWidth, iHeight );
            }
            else
            {
                aImage1 = ProcessNormalEffect( hDia1, dTimeMS, iWidth, iHeight );
            }

            if( bScale )
            {
                if( iWidth<0 )
                {
                    iWidth = m_ulOutputWidth; // or: aImage1.width();
                }
                if( iHeight<0 )
                {
                    iHeight = m_ulOutputHeight; // or: aImage1.height();
                }
                aImage1 = aImage1.scaled( iWidth, iHeight );
            }
            else
            {
                iWidth = aImage1.width();
                iHeight = aImage1.height();
            }

            QImage aPixmap( iWidth, iHeight, QImage::Format_RGB32 );
            QPainter aPainter;

            aPainter.begin( &aPixmap );

            if( hDia2.IsOk() )
            {
                QImage aImage2;
                if( hDia2->IsKenBurns() )
                {
                    aImage2 = ProcessKenBurnsEffect( hDia2, GetDiaAbsStartShowTime( iIndex2 )*1000.0, dTimeMS, iWidth, iHeight );
                }
                else
                {
                    aImage2 = ProcessNormalEffect( hDia2, dTimeMS, iWidth, iHeight );
                }
                aImage2 = aImage2.scaled( iWidth, iHeight );
                _FadeImage(&aPainter,iFadeFactor,aImage1,aImage2);
            }
            else
            {
                aPainter.drawImage( 0, 0, aImage1 );
            }

            if( bDrawDynItems )
            {
                // scale output font since 21.6.2013
                double dScaleX = GetScaleFactorFor( iWidth, iHeight );
                // after the (backgound) image, draw the text and other elements
                GetDynGraphicData().PaintElementsForTime( aPainter, dTimeMS, dScaleX );
            }

            aPainter.end();
            return aPixmap;
        }
    }
    return QImage();
}

bool DiaPresentation::IsNextSlideChanging( double dTimeMS, double dDeltaMS ) const
{
	int iIndex1a = -1;
	int iIndex2a = -1;
	int iFadeFactora = 0;
	int iIndex1b = -1;
	int iIndex2b = -1;
	int iFadeFactorb = 0;
	GetIndexForTime( dTimeMS, iIndex1a, iIndex2a, iFadeFactora );
	GetIndexForTime( dTimeMS-dDeltaMS, iIndex1b, iIndex2b, iFadeFactorb );

    if( iIndex1a==iIndex1b && iIndex2a==iIndex2b && iFadeFactora==iFadeFactorb && !GetDynGraphicData().IsNextElementChanging(dTimeMS,dDeltaMS) )
	{
		return false;
	}

	return true;
}

bool DiaPresentation::GetIndexForTime( double dTimeMS, int & iIndex1, int & iIndex2, int & iFadeFactor ) const
{
    iIndex1 = -1;
    iIndex2 = -1;
    iFadeFactor = 0;
	if( dTimeMS>=0 )
	{
		double dTime = 0;

		for( int i=0; i<GetDiaCount(); i++ )
		{
			dTime += m_aDiaContainer[ i ]->GetTotalTime()*1000.0;

			if( dTime>=dTimeMS )
			{
				iIndex1 = i;
				if( dTimeMS<=dTime-m_aDiaContainer[ i ]->GetShowTime()*1000.0 && i>0 )
				{
					iIndex2 = i-1;
					iFadeFactor = (dTime-m_aDiaContainer[ i ]->GetShowTime()*1000.0-dTimeMS)/(m_aDiaContainer[ i ]->GetDissolveTime()*1000.0)*255;
				}
				return true;
			}
		}
	}
	return false;
}

void DiaPresentation::ScaleAllDiaShowTimes( double dScaleFactor )
{
    int iMax = GetDiaCount();
    for( int i=0; i<iMax; i++ )
    {
        minHandle<DiaInfo> hDia = GetDiaAt(i);
        double dDissolve = hDia->GetDissolveTime();
        double dShow = hDia->GetShowTime();
        double dNewShow = (dDissolve+dShow)*dScaleFactor-dDissolve;
        hDia->SetShowTime(dNewShow);
    }
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

DiaInfoContainer::DiaInfoContainer()
: IOContainer< DiaInfo >( _DIA_INFO_CONTAINER )
{}

bool DiaInfoContainer::Read( istream & aStream )
{
	bool bOk = IOContainer< DiaInfo >::Read( aStream );
	SyncPositionInfos();
	return bOk;
}

void DiaInfoContainer::SyncPositionInfos()
{
	int i = 0;
	iterator aIter = begin();
	while( aIter != end() )
	{
		(*aIter)->SetPosition( i++ );
		++aIter;
	}
}

bool DiaInfoContainer::IsChanged() const
{
	const_iterator aIter = begin();
	while( aIter != end() )
	{
		// ** if one element of the container is changed,
		// ** the container is changed
		if( (*aIter)->IsChanged() )
		{
			return true;
		}

		++aIter;
	}
	return false;
}

DiaInfoContainer::const_iterator DiaInfoContainer::FindItemWithText( const string & sText, const_iterator aStartPosIterator, const DynTextContainer & aDynTextContainer ) const
{
	while( aStartPosIterator != end() )
	{
		if( (*aStartPosIterator)->HasText( sText ) )
		{
			return aStartPosIterator;
		}
        else
        {
            minHandle<DiaInfo> hItem = (*aStartPosIterator);

            if( aDynTextContainer.HasTextFor( hItem->GetUUID(), sText ) )
            {
                return aStartPosIterator;
            }
        }
		++aStartPosIterator;
	}

	return aStartPosIterator;
}

void DiaInfoContainer::MakeRelativePaths()
{
	iterator aIter = begin();
	while( aIter != end() )
	{
		(*aIter)->MakeRelativePaths();

		++aIter;
	}
}

void DiaInfoContainer::MakeAbsolutePaths( const string & sDir )
{
    iterator aIter = begin();
    while( aIter != end() )
    {
        (*aIter)->MakeAbsolutePaths( sDir );

        ++aIter;
    }
}

vector<string> DiaInfoContainer::GetAllImageFileNames() const
{
    vector<string> ret;
    const_iterator aIter = begin();
    while( aIter != end() )
    {
        ret.push_back((*aIter)->GetImageFile());

        ++aIter;
    }
    return ret;
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

DiaProjectorContainer::DiaProjectorContainer()
: IOContainer< DiaProjector >( _DIA_PROJECTOR_CONTAINER )
{}

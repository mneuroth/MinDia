/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/diapresentation.cpp,v $
 *
 *  $Revision: 1.3 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.2  2003/08/15 19:38:32  min
 *	debug comments deleted
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

#include "osdep2.h"

#include "diapresentation.h"
#include "diacallback.h"
#include "rolleicom.h"
#include "minisound.h"
#include "minexception.h"
#include "minutils.h"		// for: FileUtilityObj
#include "miniini.h"
#include "minlog.h"

#include <stdio.h>

#include <qobject.h>

// *******************************************************************
// *******************************************************************
// *******************************************************************

const double c_dMinSlideChangeTime = 3.5;	// in seconds

const int DiaPresentation::ACT_FILE_VERSION = 6;
				// History: ver. 0 --> until 10.11.2001; change: SoundInfoContainer
				//          ver. 1 --> until 17.11.2001; change: SoundCommentContainer
				//			ver. 2 --> until 27.12.2001; change: PlotCommentContainer
				//			ver. 3 --> until 30.12.2001; change: ApplScriptEnvironment
				//			ver. 4 --> until 22. 2.2003; change: DynGraphicOpContainer
				//			ver. 5 --> until 22. 3.2003; change: DynContainer, removed: DynGraphicOpContainer
				
const char * DiaPresentation::MAGIC_FILE_STRING = "MINDIA";

const char * _SCRIPT_ENABLED			= "script_events.enabled";

// ** XML Tags **
const char * _DIA_PROJECTOR_CONTAINER	= "diaprojectors";
const char * _DIA_INFO_CONTAINER		= "diainfos";

// *******************************************************************

DiaPresentation::DiaPresentation( DiaCallback * pCallback, const string & sName, MiniIniDB * pIniDB, minLoggingInterface * pLogging, IDiaOutputWindowInternal *	pOutputWindowProxy )
:
  //m_pOutputWindowProxy( pOutputWindowProxy ),
  //m_aDynGraphicOpContainer( pOutputWindowProxy ),
  //m_aDynGraphicOpProcessor( m_aDynGraphicOpContainer ),
  m_aDynItemContainer( pOutputWindowProxy ),
#ifndef ZAURUS
  m_aScriptEnv( true ),
#endif
  m_pProjectorCom( 0 ),
  m_pSoundPlayer( 0 ),
  m_pCallback( pCallback ),
  m_pLogging( pLogging )
{
	m_aObjectChanged.ClearChanged();

	Init();

	m_sName = sName;

	// ** init the known event types
#ifndef ZAURUS
	m_aScriptEnv.AddEventType( _PRESENTATION_STARTED );
	m_aScriptEnv.AddEventType( _PRESENTATION_STOPED );
	m_aScriptEnv.AddEventType( _PRESENTATION_PAUSED );
	m_aScriptEnv.AddEventType( _PRESENTATION_SAVED );
	m_aScriptEnv.AddEventType( _PRESENTATION_LOADED );
#endif
	if( pIniDB )
	{
		if( pIniDB->HasKey( _SCRIPT_ENABLED ) )
		{
			int iEnabled = pIniDB->GetValueAsInt( _SCRIPT_ENABLED );
#ifndef ZAURUS
			m_aScriptEnv.SetEnabled( (bool)iEnabled );
#endif
		}
	}
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
	m_aDynItemContainer.erase( m_aDynItemContainer.begin(), m_aDynItemContainer.end() );

#ifndef ZAURUS
	m_aScriptEnv.Clear();
#endif

	Init();

	m_sName = g_sDefPresentationFileName;
}

bool DiaPresentation::IsChanged() const
{
	return m_aObjectChanged.IsChanged() || m_aDiaContainer.IsChanged() || 
		   m_aSoundInfoContainer.IsChanged() || m_aSoundCommentContainer.IsChanged() ||
		   m_aPlotCommentContainer.IsChanged() || 
		   //m_aDynGraphicOpContainer.IsChanged() ||
		   m_aDynItemContainer.IsChanged()
#ifndef ZAURUS
		   || m_aScriptEnv.IsChanged();
#else
		   ;
#endif
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

DynContainer & DiaPresentation::GetDynGraphicData()
{
	return m_aDynItemContainer;
}

#ifndef ZAURUS
ApplScriptEnvironment &	DiaPresentation::GetScriptEnvironment()
{
	return m_aScriptEnv;
}
#endif

string DiaPresentation::GetFullName() const
{
	return m_sPathInfo+m_sName;
}

const char * DiaPresentation::GetName() const
{
	return m_sName.c_str();
}

void DiaPresentation::SetName( const char * sNewName )
{
	m_sName = FileUtilityObj::StripPath( sNewName, &m_sPathInfo );

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
		DiaInfoContainer::const_iterator aIter = m_aDiaContainer.FindItemWithText( sText, aStartIter );

		if( aIter != m_aDiaContainer.end() )
		{
			return (*aIter)->GetPosition();
		}
	}

	return -1;
}

void DiaPresentation::MakeRelativePaths()
{
	m_sName = FileUtilityObj::ConvertToRelPath( m_sName.c_str() );
	m_aDiaContainer.MakeRelativePaths();
	m_aSoundInfoContainer.MakeRelativePaths();
}

double DiaPresentation::GetOffsetForSound() const
{
	return GetDissolveTimeOfSlide( 0 );
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
#ifndef ZAURUS
		throw MinException( "This is not an valid data-file !" );
#endif
		return false;
	}
	aFU.ReadSeparator( aStream );
	aStream >> iActFileVersion;
	aFU.ReadSeparator( aStream );
	ReadString( aStream, m_sName );
	aFU.ReadSeparator( aStream );
	ReadString( aStream, m_sComment );
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
#ifndef ZAURUS
		m_aScriptEnv.Read( aStream );
#endif
		//not needed here, is done in the class: m_aScriptEnv.ClearChanged();
	}
	else
	{
#ifndef ZAURUS
		m_aScriptEnv.Clear();
		m_aScriptEnv.ClearChanged();
#endif
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
		m_aDynItemContainer.Read( aStream );
		m_aDynItemContainer.ClearChanged();
	}
	else
	{
		m_aDynItemContainer.erase( m_aDynItemContainer.begin(), m_aDynItemContainer.end() );
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
#ifndef ZAURUS
		m_aScriptEnv.Write( aStream );
#endif
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
		m_aDynItemContainer.Write( aStream );
		((DiaPresentation *)this)->m_aDynItemContainer.ClearChanged();
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
	aTree.PushTag( m_aDynItemContainer.GetXMLTree() );
#ifndef ZAURUS
	aTree.PushTag( m_aScriptEnv.GetXMLTree() );
#endif

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

minHandle<DiaInfo> DiaPresentation::GetDiaAt( int iIndex ) const
{
	if( iIndex>=0 && iIndex<GetDiaCount() )
	{
		return m_aDiaContainer[ iIndex ];
	}
	return 0;
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

bool DiaPresentation::UpdateShowTimeForDia( int iIndex, double dDeltaTime )
{
	if( (iIndex>=0) && (iIndex<(int)m_aDiaContainer.size()) )
	{
		return m_aDiaContainer[ iIndex ]->SetShowTime( m_aDiaContainer[ iIndex ]->GetShowTime()+dDeltaTime );
	}

	return false;
}

bool DiaPresentation::UpdateDissolveTimeForDia( int iIndex, double dDeltaTime )
{
	if( (iIndex>=0) && (iIndex<(int)m_aDiaContainer.size()) )
	{
		return m_aDiaContainer[ iIndex ]->SetDissolveTime( m_aDiaContainer[ iIndex ]->GetDissolveTime()+dDeltaTime );
	}

	return false;
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

double DiaPresentation::GetActPlayTime() /*const*/
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

    if( m_pProjectorCom )
	{
		// ** switch to direct mode, only if comming not from pause-mode !
		if( !m_bContinued )
		{
			// ** switch to direct mode only if in keyboard mode !
			if( !m_pProjectorCom->IsDirectMode() )
			{
				bOk = m_pProjectorCom->DirectMode( /*bSync*/true );
			}
// min todo --> hier ggf. weiter init-values setzen: light-value, autofocus etc.
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

				bOk = m_pProjectorCom->SetDissolveTime( iStepTime, /*bSync*/true );
                bOk = m_pProjectorCom->SlideForward( /*bSync*/true );

                // ** the first dissolve-time will not be measured with the timer...
                m_dPlayTime += dDissolveTime + dAbsStartTime;
				// ** Attention: the play time will be recalibrated in the first step

				// ** old: in simulation mode wait the dissolve time (first dia is synchronious)
				// ** new: no special handling for first slide in simulation mode
				if( m_pProjectorCom->IsSimulation() )
				{
					//old: minSleep( dDissolveTime*1000 );
					m_dPlayTime -= dDissolveTime;
				}
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
				m_pSoundPlayer->SetWavFile( hItem->GetFileName().c_str() );
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

	if( m_pSoundPlayer )
	{
		m_pSoundPlayer->Stop();
	}
	// graphic operations are connected to the music !
	//m_aDynGraphicOpProcessor.Stop();
	m_aDynItemContainer.Stop();

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

	if( m_pSoundPlayer )
	{
		m_pSoundPlayer->Pause();
	}
	// graphic operations are connected to the music !
	//m_aDynGraphicOpProcessor.Pause();
	m_aDynItemContainer.Pause();

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
						m_pSoundPlayer->SetWavFile( hItem->GetFileName().c_str() );
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
					m_aDynItemContainer.Continue();
				}
				else
				{
					//m_aDynGraphicOpProcessor.Start( (int)(m_dAbsSoundStartTime*1000) );
					m_aDynItemContainer.Run( (int)(m_dAbsSoundStartTime*1000) );
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


#ifdef ZAURUS
		sprintf( sBuffer, QObject::tr( "no=%d/%d op=%s tmr=%6.1lfs->cmd=\"%s\"" ), m_iActPos+1, GetDiaCount(), aOperation.GetOperationTypeName().c_str(), aOperation.GetOperationTime(), (const char *)sCmd );
#else
		sprintf( sBuffer, QObject::tr( "no=%d/%d ** op=%s ** timer=%6.1lf s --> cmd=\"%s\"" ), m_iActPos+1, GetDiaCount(), aOperation.GetOperationTypeName().c_str(), aOperation.GetOperationTime(), (const char *)sCmd );
#endif
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
#ifndef ZAURUS
	return m_aScriptEnv.IsEnabled();
#else
	return false;
#endif
}

void DiaPresentation::MyExecuteScript( const string & sEvent )
{
	bool bScriptFound;
	int  iRet;

#ifndef ZAURUS
	m_aScriptEnv.ExecuteScriptForEvent( sEvent, bScriptFound, &iRet );
#endif
	
	CheckScriptResult( sEvent, bScriptFound, iRet );
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

DiaInfoContainer::const_iterator DiaInfoContainer::FindItemWithText( const string & sText, const_iterator aStartPosIterator ) const
{
	while( aStartPosIterator != end() )
	{
		if( (*aStartPosIterator)->HasText( sText ) )
		{
			return aStartPosIterator;
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

// *******************************************************************
// *******************************************************************
// *******************************************************************

DiaProjectorContainer::DiaProjectorContainer()
: IOContainer< DiaProjector >( _DIA_PROJECTOR_CONTAINER )
{}

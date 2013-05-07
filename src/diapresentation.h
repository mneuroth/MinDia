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

#ifndef _DIAPRESENTATION_H
#define _DIAPRESENTATION_H

#include "minhandle.h"		// for the smart-ptr
#include "diainfo.h"
#include "iocontainer.h"
#include "diaprojector.h"
#include "soundinfo.h"
#include "dyngraphop.h"
#include "writexml.h"
#include "applscriptenv.h"
#include "igendev.h"
#include "misctools.h"

#include <QDateTime>		// for QTime
#include <QMap>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ** pre-declarations
class RolleiCom;
class miniSound;
class DiaCallback;
class minLoggingInterface;

// *******************************************************************

static const char * g_sDefPresentationFileName = "diapresentation.dia";

// *******************************************************************
// ** the possible event types for this application **

#define _PRESENTATION_STARTED		"Presentation_Started"
#define _PRESENTATION_STOPED		"Presentation_Stoped"
#define _PRESENTATION_PAUSED		"Presentation_Paused"
#define _PRESENTATION_SAVED			"Presentation_Saved"
#define	_PRESENTATION_LOADED		"Presentation_Loaded"

// *******************************************************************
class DiaInfoContainer : public IOContainer< DiaInfo >
{
public:
	typedef DiaInfo		ItemClass;

    DiaInfoContainer( /*IDiaOutputWindowInternal * pOutputWindowProxy*/ );

	bool IsChanged() const; 

	const_iterator FindItemWithText( const string & sText, const_iterator aStartPosIterator /*= begin()*/ ) const;
	void MakeRelativePaths();
    void MakeAbsolutePaths( const string & sDir );

	void SyncPositionInfos();

	virtual bool Read( istream & aStream );

//private:
//    IDiaOutputWindowInternal *  m_pOutputWindowProxy;
};

// *******************************************************************
class DiaProjectorContainer : public IOContainer< DiaProjector >
{
public:
	DiaProjectorContainer();
};

// *******************************************************************
/** Represents the information for a dia/slide
  * presentation, this includes infos for every
  * dia and for the sound.
  */
class DiaPresentation
{
public:
	DiaPresentation( bool bEnableScript, DiaCallback * pCallback, const string & sName = g_sDefPresentationFileName, minLoggingInterface * pLogging = 0, IDiaOutputWindowInternal *	pOutputWindowProxy = 0 );

    void SyncDataContainers();          // sync data containers after move of dia

	void Clear();

	bool IsChanged() const;

	bool IsScriptEnabled() const;

	bool IsPlaying() const;
	bool IsPause() const;
	bool IsEdit() const;

    QImage GetSlideForTime( double dTimeMS, int iWidth = -1, int iHeight = -1 ) const;
	bool IsNextSlideChanging( double dTimeMS, double dDeltaMS ) const;
	bool GetIndexForTime( double dTimeMS, int & iIndex1, int & iIndex2, int & iFadeFactor ) const;

	// ** information about the presentation **
	int						GetDiaCount() const;
    int                     CountUUIDs( const string & sUUID ) const;
    int                     GetDiaIndexForUUID( const string & sUUID ) const;
    minHandle<DiaInfo>		GetDia( const string & sUUID ) const;
    minHandle<DiaInfo>		GetDiaAt( int iIndex ) const;
	minHandle<DiaInfo>		AddDiaAt( int iIndex, minHandle<DiaInfo> hNewDia );
	bool					RemoveDiaAt( int iIndex );
    double					GetDiaAbsStartDissolveTime( int iIndex ) const;             // time when dia is started to fade in
    double					GetDiaAbsStartShowTime( int iIndex ) const;                 // time when dia is visible with 100%
    double					GetDiaAbsFinishDissolveTime( int iIndex ) const;            // time when dia is not visible anymore --> finished fade out time
    bool					UpdateShowTimeForDia( int iIndex, double dDeltaTime );
	bool					UpdateDissolveTimeForDia( int iIndex, double dDeltaTime );

	// ** handling of the presentation (play-modus) **
    double					GetTotalTime() const;               // in seconds
	double					GetActPlayTime() /*const*/;
	double					GetActSoundPlayTime() /*const*/;
	double					GetCountDownTime() /*const*/;
	// ** for the iteration over all the slids **
	void					ResetPlay();
	bool					StartPlay( int iStartPos );
	void					StopPlay();
	void					PausePlay();
	bool					NextStep( double & dNextStepTimeOut );
	string					GetStepInfo() const;

	// ** connection to the projector and sound-player
	void					SetProjectorCom( RolleiCom * pProjectorCom );
	void					SetSoundPlayer( miniSound * pSoundPlayer);

	// ** some other informations **
	string					GetFullName() const;
    string      			GetName() const;
    void					SetName( const string & sNewName );

	string					GetComment() const;
	void					SetComment( const string & sComment );

	int						FindItemWithText( const string & sText, int iStartIndex = 0 ) const;

	void					MakeRelativePaths();
    void					MakeAbsolutePaths( const string & sDir );

	// ** to synchronize sound with slide-show, wait for first slide. Unit: seconds **
	double					GetOffsetForSound() const;

    ImageRatio              GetImageRatio() const;
    void                    SetImageRatio( ImageRatio value );

    void                    GetImageSize( unsigned long & iWidth, unsigned long & iHeight ) const;
    void                    SetImageSize( unsigned long iWidth, unsigned long iHeight );

	int						GetProjectorCount() const;
	minHandle<DiaProjector> GetProjectorPtr( int iNo ) const;

	SoundInfoContainer &	GetSoundInfoData();
	const SoundInfoContainer &	GetSoundInfoData() const;

	SoundCommentContainer &	GetSoundCommentData();

	PlotCommentContainer &	GetPlotCommentData();

	//DynGraphicOpContainer &	GetDynGraphicOpData();

    DynTextContainer &			GetDynGraphicData();
    const DynTextContainer &	GetDynGraphicData() const;

	ApplScriptEnvironment &	GetScriptEnvironment();

	// ** io methods **
	XmlTree					GetXMLTree() const;

	bool					Read( istream & aStream, bool bExecuteEventScript /*= true*/ );
	bool					Write( ostream & aStream ) const;

private:
	void	Init();
    void    UpdateAttachedDynamicTexts( int iIndex, double dDeltaTime );
	double	GetDissolveTimeOfSlide( int iSlideIndex ) const;
	void	MyExecuteScript( const string & sEvent );
	void	CheckScriptResult( const string & sEvent, bool bFoundScript, int iRet );
	bool	ExistsExternalDevice();

	// *** data ***
	string					m_sName;
	string					m_sPathInfo;				// temp
	string					m_sComment;

    DiaProjectorContainer	m_aProjectorContainer;      // for projectors (not used?)

    DiaInfoContainer		m_aDiaContainer;            // the container for the slides
    SoundInfoContainer		m_aSoundInfoContainer;      // the container for the sound files (*.mp3, *.wav)
    SoundCommentContainer	m_aSoundCommentContainer;   // the container for sound info texts
    PlotCommentContainer	m_aPlotCommentContainer;    // the container for plot info texts
    DynTextContainer		m_aDynTextContainer;        // the container for dynamic texts

    //DynGraphicOpContainer	m_aDynGraphicOpContainer;
    //TimerJobProcessor		m_aDynGraphicOpProcessor;

    ApplScriptEnvironment			m_aScriptEnv;
	minClientHandle<IGeneralDevice> m_hGenDev;

	// *** transient data ***
	ObjectChanged			m_aObjectChanged;

	// *** data for the presentation (play-modus) ***
	bool					m_bIsPlaying;
	bool					m_bIsPause;
	bool					m_bContinued;
	double					m_dAbsSoundStartTime;	// temp
	double					m_dPlayTime;			// in seconds
	double					m_dCountDownTime;
	double					m_dCountDownStartTime;
	int						m_iActPos;
	int						m_iActSubPos;
	int						m_iStartPos;
	string					m_sStepInfo;
	QTime					m_aCountDown;
	QTime 					m_aPlayTime;
    ImageRatio              m_aImageRatio;
    unsigned long           m_ulOutputWidth;
    unsigned long           m_ulOutputHeight;
	RolleiCom *				m_pProjectorCom;	// no owner !
	miniSound *				m_pSoundPlayer;		// no owner !
	DiaCallback *			m_pCallback;		// no owner !
	minLoggingInterface *	m_pLogging;			// no owner !
	//IDiaOutputWindowInternal *	m_pOutputWindowProxy;	// no owner !

public:
	// *** some public constants for this class ***
	static const int		ACT_FILE_VERSION;
	static const char *		MAGIC_FILE_STRING;
};

#endif

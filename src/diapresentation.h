/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/diapresentation.h,v $
 *
 *  $Revision: 1.5 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.4  2004/02/26 22:20:21  min
 *	Fixes to compile MinDia for the Zaurus.
 *	
 *	Revision 1.3  2004/01/18 23:51:23  min
 *	General device driver included.
 *	
 *	Revision 1.2  2003/10/26 17:36:05  min
 *	MakeRelativePaths() added.
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

#ifndef _DIAPRESENTATION_H
#define _DIAPRESENTATION_H

#include "minhandle.h"		// for the smart-ptr
#include "diainfo.h"
#include "iocontainer.h"
#include "diaprojector.h"
#include "soundinfo.h"
#include "dyngraphop.h"
#include "writexml.h"
#ifndef ZAURUS
#include "applscriptenv.h"
#endif
#include "igendev.h"

#include <qdatetime.h>		// for QTime
#include <qmap.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef QMap<QString,QImage>	QImageCache;

// ** pre-declarations
class RolleiCom;
class miniSound;
class MiniIniDB;
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

	DiaInfoContainer();

	bool IsChanged() const; 

	const_iterator FindItemWithText( const string & sText, const_iterator aStartPosIterator /*= begin()*/ ) const;
	void MakeRelativePaths();

	void SyncPositionInfos();

	virtual bool Read( istream & aStream );
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
	DiaPresentation( DiaCallback * pCallback, const string & sName = g_sDefPresentationFileName, MiniIniDB * pIniDB = 0, minLoggingInterface * pLogging = 0, IDiaOutputWindowInternal *	pOutputWindowProxy = 0 );

	void Clear();

	bool IsChanged() const;

	bool IsScriptEnabled() const;

	bool IsPlaying() const;
	bool IsPause() const;
	bool IsEdit() const;

	void PaintSlideForTime( const QImageCache & aImageCache, QPainter & aPainter, double dTimeMS ) const;
	bool IsNextSlideChanging( double dTimeMS, double dDeltaMS ) const;
	bool GetIndexForTime( double dTimeMS, int & iIndex1, int & iIndex2, int & iFadeFactor ) const;

	// ** information about the presentation **
	int						GetDiaCount() const;
	minHandle<DiaInfo>		GetDiaAt( int iIndex ) const;
	minHandle<DiaInfo>		AddDiaAt( int iIndex, minHandle<DiaInfo> hNewDia );
	bool					RemoveDiaAt( int iIndex );
	double					GetDiaAbsStartDissolveTime( int iIndex ) const;
	double					GetDiaAbsStartShowTime( int iIndex ) const;
	bool					UpdateShowTimeForDia( int iIndex, double dDeltaTime );
	bool					UpdateDissolveTimeForDia( int iIndex, double dDeltaTime );

	// ** handling of the presentation (play-modus) **
	double					GetTotalTime() const;
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
	const char *			GetName() const;
	void					SetName( const char * sNewName );

	string					GetComment() const;
	void					SetComment( const string & sComment );

	int						FindItemWithText( const string & sText, int iStartIndex = 0 ) const;

	void					MakeRelativePaths();

	// ** to synchronize sound with slide-show, wait for first slide. Unit: seconds **
	double					GetOffsetForSound() const;

	int						GetProjectorCount() const;
	minHandle<DiaProjector> GetProjectorPtr( int iNo ) const;

	SoundInfoContainer &	GetSoundInfoData();
	const SoundInfoContainer &	GetSoundInfoData() const;

	SoundCommentContainer &	GetSoundCommentData();

	PlotCommentContainer &	GetPlotCommentData();

	//DynGraphicOpContainer &	GetDynGraphicOpData();

	DynContainer &			GetDynGraphicData();
	const DynContainer &	GetDynGraphicData() const;

#ifndef ZAURUS
	ApplScriptEnvironment &	GetScriptEnvironment();
#endif

	// ** io methods **
	XmlTree					GetXMLTree() const;

	bool					Read( istream & aStream, bool bExecuteEventScript /*= true*/ );
	bool					Write( ostream & aStream ) const;

private:
	void	Init();
	double	GetDissolveTimeOfSlide( int iSlideIndex ) const;
	void	MyExecuteScript( const string & sEvent );
	void	CheckScriptResult( const string & sEvent, bool bFoundScript, int iRet );
	bool	ExistsExternalDevice();

	// *** data ***
	string					m_sName;
	string					m_sPathInfo;				// temp
	string					m_sComment;
	DiaProjectorContainer	m_aProjectorContainer;
	DiaInfoContainer		m_aDiaContainer;
	SoundInfoContainer		m_aSoundInfoContainer;
	SoundCommentContainer	m_aSoundCommentContainer;
	PlotCommentContainer	m_aPlotCommentContainer;

	//DynGraphicOpContainer	m_aDynGraphicOpContainer;
	//TimerJobProcessor		m_aDynGraphicOpProcessor;

	DynContainer			m_aDynItemContainer;

#ifndef ZAURUS
	ApplScriptEnvironment			m_aScriptEnv;
	minClientHandle<IGeneralDevice> m_hGenDev;
#endif

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

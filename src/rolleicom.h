/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/rolleicom.h,v $
 *
 *  $Revision: 1.1.1.1 $
 *
 *	$Log: not supported by cvs2svn $
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

#ifndef _ROLLEICOM_H
#define _ROLLEICOM_H

#include <string>

using namespace std;

class minLoggingInterface;
class minCmdProcessor;
class minSyncObject;

struct RolleiComHelperData;

class MiniIniDB;

// *******************************************************************
/** Access to the serial communication port (COM). */
class RolleiCom
{
public:
	RolleiCom( bool bIgnoreComSettings, bool bSimulation, int iProjectorType, minLoggingInterface * pLoggingChannel, MiniIniDB * pIniDB = 0, int iComPortNo = 1 /*, int iBaudrate = 1200, int iParityMode = EVEN, int iStopBits = TWO, int iDataBits = 7*/ );
	~RolleiCom();

	void	Start( int iComPortNo, int iBaudrate, int iParityMode, int iStopBits, int iDataBits, int iFlowMode );
	void	Stop();

	bool	IsOk() const;
	bool	IsTwinDigitalP() const;
	bool	IsMSC300P() const;
	void	SetProjectorType( int iType );
	bool	IsSimulation() const;
	void	SetSimulation( bool bSimulation );
	bool	IsLogging() const;
	void	SetLogging( bool bLogging );

	int		GetComPortNo() const;
	int		GetBaudrate() const;
	int		GetParityMode() const;
	int		GetStopBits() const;
	int		GetDataBits() const;
	int		GetFlowModus() const;

	void	SetLoggingChannel( minLoggingInterface * pLogChannel );

	// ** the functionality of the projector ***
	bool	DirectMode( bool bSync = true );
	bool	MemoryMode( bool bSync = true );
	bool	Reset( bool bSync = true );			// ** switch to keyboard-modus
	bool	SlideBack( bool bSync = true );
	bool	SlideForward( bool bSync = true );
	bool	StopGo( bool bSync = true );
	bool	SetAutofocus( bool bValue, bool bSync = true );
	bool	FocusStepForward( bool bSync = true );
	bool	FocusStepBackward( bool bSync = true );
	string	GetActSlideNoStrg( bool bSync = true );
	string	GetActLineNoStrg( bool bSync = true );
	bool	SetDissolveTime( int iTimeInTenthOfASecond, bool bSync = true );
	bool	SetLight( int iLightValue, bool bSync = true  );
	bool	GotoPosition( int iPosition );
	char	GetStatus( bool bSync = true );
	string	GetStatusStrg( char ch );

	// ** query state **
	bool	IsProjectorConnected();
	bool	IsKeyboardMode();
	bool	IsDirectMode();

	// ** helper methods for output **
	string	GetParityModeStrg( int iParityMode ) const;
	string	GetStopBitsStrg( int iStopBits ) const;
	bool	GetParityModeFromStrg( const string & s, int & iParityModeOut ) const;
	bool	GetStopBitsFromStrg( const string & s, int & iStopBitsOut ) const;

	// ** some constants ***

	// ** for parity mode **
	static const int NONE;
	static const int ODD;
	static const int EVEN;

	// ** for stop bits **
	static const int ONE;				// 1
	static const int ONE_POINT_FIVE;	// 1.5
	static const int TWO;				// 2

	// ** for flow control **
	static const int NO_FLOW;
	static const int XON_XOFF_FLOW;
	static const int HARDWARE_FLOW;

	static const int LIGHT_OFF;
	static const int LIGHT_80;
	static const int LIGHT_90;
	static const int LIGHT_100;
	static const int LIGHT_DISPLAY_LO;
	static const int LIGHT_DISPLAY_HIGH;

	static const int TWIN_DIGITAL_P;
	static const int MSC_300_P;

	bool	SendCmd( const string & sMsg, bool bExpectReturnValue = false );

protected:
	bool	SendCmdTwin( const string & sMsg, bool bExpectReturnValue );
	bool	SendCmdMSC( const string & sMsg, bool bExpectReturnValue );
	bool	SendMsg( const string & sMsg, bool bExpectReturnValue = false, bool bSync = false );
	string	GetMsg( int iCount = 4 );
	// ** methods for rollei msc projectors
	bool	ReceiveEcho( const string & sMsgCharacter, string & sEcho );
	bool	CheckReady();

private:
	void SetDefaultValues( int iProjectorType );

	// ** data **
	MiniIniDB *					m_pIniDB;				// no owner !
	minLoggingInterface *		m_pLoggingChannel;		// no owner !
	RolleiComHelperData *		m_pData;
	minCmdProcessor *			m_pCmdProcessor;
	bool						m_bIsSimulation;
	bool						m_bDoLogging;
	bool						m_bStopGoFlag;
	bool						m_bIgnoreComSettings;
	int							m_iComPortNo;
	int							m_iBaudrate;
	int							m_iParityMode;
	int							m_iStopBits;
	int							m_iDataBits;
	int							m_iFlowMode;			// new since 27.3.2002
	int							m_iProjectorType;		// new since 22.3.2002
	minSyncObject *				m_pComPortSync;			// new since 15.2.2003, to synchronize access to the Com-Port
};

#endif


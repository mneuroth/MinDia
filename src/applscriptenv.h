/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/applscriptenv.h,v $
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

#ifndef _SCRIPTENV_H
#define _SCRIPTENV_H

#include "iscript.h"
#include "minutils.h"
#include "writexml.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// *******************************************************************
/** Represents a script to be executed if an event occured.
  */
class ScriptEventItem
{
public:
	ScriptEventItem( const string & sLanguage = "", const string & sEvent = "", const string & sScript = "" );

#ifdef __BORLANDC__
	bool operator!=( const ScriptEventItem & ) const;
#endif

	string		GetLanguage() const;
	string		GetEvent() const;

	// ** important: return a copy of the data, because the script can replace 
	// ** the presentation-data and also the script-string !!!
	string 		GetScript() const;
	void		SetScript( const string & sScript );

	// ** io methods **
	XmlTree		GetXMLTree() const;

	bool		Read( istream & aStream );
	bool		Write( ostream & aStream ) const;

	// ** ScriptEvent-Item is equal, if the event-type is equal !
	bool		operator==( const ScriptEventItem & aOther ) const;

private:
	// ** data **
	string		m_sLanguage;
	string		m_sEvent;
	string		m_sScript;
};


// *******************************************************************
/** Handles all the script operations for an application.
  */
class ApplScriptEnvironment : public ObjectChanged
{
	typedef vector<ScriptEventItem>		ScriptEventContainer;
	typedef vector<string>				EventTypeContainer;

public:
	ApplScriptEnvironment( bool bEnabled = true );

	// ** is script execution enabled ?
	bool				IsEnabled() const;
	void				SetEnabled( bool bValue );

	int					GetKnownEventTypeCount() const;
	string				GetEventTypeWithIndex( int iIndex ) const;
	void				AddEventType( const string & sEventType );

	void				Clear();

	ScriptEventItem *	GetScriptEvent( const string & sEvent );

	bool				AddScriptEvent( const ScriptEventItem & aItem );

	bool				RemoveScriptEvent( const string & sEvent );
	
	bool				ExecuteScriptForEvent( const string & sEvent, bool & bScriptFound, int * pRetVal = 0 );

	// ** io methods **
	XmlTree				GetXMLTree() const;

	bool				Read( istream & aStream );
	bool				Write( ostream & aStream ) const;

private:
	// ** data **
	ScriptEventContainer						m_aEventContainer;

	// ** temp data **
	bool										m_bEnabled;

	EventTypeContainer							m_aKnownEventsContainer;

	minClientHandle<IGeneralScriptEnvironment>	m_hScriptEnv;
};

#endif

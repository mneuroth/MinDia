/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/applscriptenv.cpp,v $
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

#include "applscriptenv.h"

// *******************************************************************

ScriptEventItem::ScriptEventItem( const string & sLanguage, const string & sEvent, const string & sScript )
: m_sLanguage( sLanguage ),
  m_sEvent( sEvent ),
  m_sScript( sScript )
{
}

string ScriptEventItem::GetLanguage() const
{
	return m_sLanguage;
}

string ScriptEventItem::GetEvent() const
{
	return m_sEvent;
}

string ScriptEventItem::GetScript() const
{
	return m_sScript;
}

void ScriptEventItem::SetScript( const string & sScript )
{
	m_sScript = sScript;
}

XmlTree	ScriptEventItem::GetXMLTree() const
{
	XmlTree aTree( "scriptenvent" );

	aTree.PushTag( "language", m_sLanguage, XmlAttribList( "type", "string" ) );
	aTree.PushTag( "event", m_sEvent, XmlAttribList( "type", "string" ) );
	aTree.PushTag( "script", m_sScript, XmlAttribList( "type", "string" ) );

	return aTree;
}

bool ScriptEventItem::Read( istream & aStream )
{
	FileUtilityObj aFU;

	aFU.ReadStructBegin( aStream );
	ReadString( aStream, m_sLanguage );
	aFU.ReadSeparator( aStream );
	ReadString( aStream, m_sEvent );
	aFU.ReadSeparator( aStream );
	ReadString( aStream, m_sScript );
	//aFU.ReadSeparator( aStream );			// not needed, removed 15.3.2002
	aFU.ReadStructEnd( aStream );

	return aStream.good();
}

bool ScriptEventItem::Write( ostream & aStream ) const
{
	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	WriteString( aStream, m_sLanguage );
	aFU.WriteSeparator( aStream );
	aStream << endl;
	WriteString( aStream, m_sEvent );
	aFU.WriteSeparator( aStream );
	aStream << endl;
	WriteString( aStream, m_sScript );
	//aFU.WriteSeparator( aStream );		// not needed, removed 15.3.2002
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}

bool ScriptEventItem::operator==( const ScriptEventItem & aOther ) const
{
	return ( m_sEvent == aOther.m_sEvent );
}

// *******************************************************************

ApplScriptEnvironment::ApplScriptEnvironment( bool bEnabled )
: m_bEnabled( bEnabled ),
  m_hScriptEnv( g_IGeneralScriptEnvironmentID )
{
}

bool ApplScriptEnvironment::IsEnabled() const
{
	return m_bEnabled;
}

void ApplScriptEnvironment::SetEnabled( bool bValue )
{
	m_bEnabled = bValue;
}

void ApplScriptEnvironment::Clear()
{
	m_aEventContainer.erase( m_aEventContainer.begin(), m_aEventContainer.end() );
}

ScriptEventItem * ApplScriptEnvironment::GetScriptEvent( const string & sEvent )
{
	ScriptEventContainer::iterator aIter = m_aEventContainer.begin();

	while( aIter != m_aEventContainer.end() )
	{
		ScriptEventItem & aItem = *aIter;

		if( aItem.GetEvent() == sEvent )
		{
			return &aItem;
		}

		++aIter;
	}

	// ** object is changed !
	//SetChanged();
	// ** COMMENT: object changed for this object will be handled from user (here the gui)

	return 0;
}

bool ApplScriptEnvironment::AddScriptEvent( const ScriptEventItem & aItem )
{
	// ** exists allready a script for that event ?
	ScriptEventItem * pItem = GetScriptEvent( aItem.GetEvent() );

	if( pItem )
	{
		// ** yes --> overwrite it !
		*pItem = aItem;
	}
	else
	{
		// ** no --> add a new item
		m_aEventContainer.push_back( aItem );
	}

	// ** object is changed !
	SetChanged();

	return true;
}

bool ApplScriptEnvironment::RemoveScriptEvent( const string & sEvent )
{
	// ** exists a script for that event ?
	ScriptEventItem * pItem = GetScriptEvent( sEvent );

	if( pItem )
	{
		// ** yes --> remove it !

		remove( m_aEventContainer.begin(), m_aEventContainer.end(), ScriptEventItem( "", sEvent, "" ) );

		// ** object is changed !
		SetChanged();

		return true;
	}
	else
	{
		// ** not found --> signal an error
		return false;
	}
}

bool ApplScriptEnvironment::ExecuteScriptForEvent( const string & sEvent, bool & bScriptFound, int * pRetVal )
{
	bScriptFound = false;

	// ** exists a script for that event ?
	ScriptEventItem * pItem = GetScriptEvent( sEvent );

	if( pItem )
	{
		// ** yes --> execute it !

		bScriptFound = true;

		m_hScriptEnv.UpdateIfNotValid();
		if( m_hScriptEnv.IsValid() && m_hScriptEnv->IsOk() && IsEnabled() )
		{
			int iRet= m_hScriptEnv->ExecuteScript( pItem->GetScript(), m_hScriptEnv->GetLanguageFromStrg( pItem->GetLanguage() ) );

			if( pRetVal )
			{
				*pRetVal = iRet;
			}

			// ** script could be executed, maybe an error was occured (see return value) !
			return true;
		}
	}	

	// ** no script for that event found or no script engin for that language found
	return false;
}

XmlTree ApplScriptEnvironment::GetXMLTree() const
{
	XmlTree aTree( "scriptenv" );

	aTree.SetAsContainer( true );

	ScriptEventContainer::const_iterator aIter = m_aEventContainer.begin();
	while( aIter != m_aEventContainer.end() )
	{
		aTree.PushTag( (*aIter).GetXMLTree() );

		++aIter;
	}

	return aTree;
}

bool ApplScriptEnvironment::Read( istream & aStream )
{
	bool bRet = ReadContainer<ScriptEventContainer>( m_aEventContainer, aStream );

	// ** after read the object is unchanged !
	ClearChanged();

	return bRet;
}

bool ApplScriptEnvironment::Write( ostream & aStream ) const
{
	bool bRet = WriteContainer<ScriptEventContainer>( m_aEventContainer, aStream );

	if( bRet )
	{
		// ** after a sucessfull write the object is unchanged !
		((ApplScriptEnvironment *)this)->ClearChanged();		// because of const !
	}

	return bRet;
}

int ApplScriptEnvironment::GetKnownEventTypeCount() const
{
	return m_aKnownEventsContainer.size();
}

string ApplScriptEnvironment::GetEventTypeWithIndex( int iIndex ) const
{
	if( (iIndex>=0) && (iIndex<GetKnownEventTypeCount()) )
	{
		return m_aKnownEventsContainer[ iIndex ];
	}

	return "";
}

void ApplScriptEnvironment::AddEventType( const string & sEventType )
{
	m_aKnownEventsContainer.push_back( sEventType );
}

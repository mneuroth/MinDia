/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/eventmapdlgimpl.cpp,v $
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

#include "eventmapdlgimpl.h"

#include "applscriptenv.h"
#include "diapresentation.h"

#include <qcombobox.h> 
#include <qmultilineedit.h>

// *******************************************************************

EventMapDlgImpl::EventMapDlgImpl( ApplScriptEnvironment * pScriptEnv, QWidget* parent, const char* name, bool modal, WFlags fl )
: EventMapDlg( parent, name, modal, fl ),
  m_pScriptEnv( pScriptEnv )
{
	FillComboBox();

	// ** disable text-editing of the script source (use modify-button)
	m_pScript->setEnabled( false );

    connect( this, SIGNAL( sigDialogHelp(QWidget *, const QString &) ), parent, SLOT( sltShowModalHelp(QWidget *, const QString &) ) );
}

EventMapDlgImpl::~EventMapDlgImpl()
{
}

void EventMapDlgImpl::sltModifyScript()
{
	if( m_pScriptEnv )
	{
		QString sSelected = m_pEvents->currentText();
		const char * s = (const char *)sSelected;
		string sEvent = s ? s : "";

		ScriptEventItem * pItem = m_pScriptEnv->GetScriptEvent( sEvent ); 

		minClientHandle<IGeneralScriptEnvironment> hScriptEnv( g_IGeneralScriptEnvironmentID );

		if( hScriptEnv.IsValid() && hScriptEnv->IsOk() )
		{
			IGeneralScriptEnvironment::ScriptLanguage aLanguage = IGeneralScriptEnvironment::PYTHON;

			string sScript = pItem ? pItem->GetScript() : "# event: "+sEvent+"\n";
			
			bool bOk = hScriptEnv->ModifyScript( "event: "+sEvent, sScript, aLanguage );
			
			// ** the modify-dialog moves the input focus from this dialog
			setActiveWindow();

			if( bOk )
			{
				// ** exists a script event item
				if( pItem )
				{
					// ** yes --> replace it

					pItem->SetScript( sScript );
				}
				else
				{
					// ** no --> create a new one

					string sScriptType = hScriptEnv->GetLanguageStrg( aLanguage );

					ScriptEventItem aItem( sScriptType, sEvent, sScript );

					m_pScriptEnv->AddScriptEvent( aItem );
				}

				// ** new or replaced script source changes the state of the script-environment !
				m_pScriptEnv->SetChanged();

				// ** update the output of the event map dialog
				m_pScript->setText( sScript.c_str() );
			}
		}
	}
}

void EventMapDlgImpl::sltEventSelected( int /*iIndex*/ )
{
	if( m_pScriptEnv )
	{
		QString sSelected = m_pEvents->currentText();
		const char * s = (const char *)sSelected;
		string sTemp = s ? s : "";

		ScriptEventItem * pItem = m_pScriptEnv->GetScriptEvent( sTemp ); 

		if( pItem )
		{
			m_pScript->setText( pItem->GetScript().c_str() );
		}
		else
		{
			m_pScript->setText( s );
		}
	}
}

void EventMapDlgImpl::FillComboBox()
{
	if( m_pScriptEnv )
	{
		for( int i=0; i<m_pScriptEnv->GetKnownEventTypeCount(); i++ )
		{
			m_pEvents->insertItem( m_pScriptEnv->GetEventTypeWithIndex( i ).c_str() );
		}
	}

	m_pEvents->setCurrentItem( -1 );
}

void EventMapDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Key_F1) )
	{
		emit sigDialogHelp( this, "EventMapDialog" );
	}
	else
	{
		EventMapDlg::keyPressEvent( pEvent );
	}
}

/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/configplayerdlgimpl.cpp,v $
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

#include "configplayerdlgimpl.h"

#include <qcombobox.h>

#include <stdio.h>		// for: sprintf

// *******************************************************************
// *******************************************************************
// *******************************************************************

const char * c_sPlayerKey			= "mp3player";
const char * c_sPlayerOptionsKey	= "mp3options";

const char * c_sActPlayerKey		= "act_mp3";
const char * c_sActPlayerOptionsKey	= "act_mp3_opt";


ConfigPlayerDlgImpl::ConfigPlayerDlgImpl( MiniIniDB & aIniDB, QWidget* parent, const char* name, bool modal, WFlags fl )
: ConfigPlayerDlg( parent, name, modal, fl ),
  m_aIniDB( aIniDB )
{
	char sBuffer[256];
	sprintf( sBuffer, "%s.%d", c_sPlayerKey, 0 );
	string sPlayer = m_aIniDB.GetValue( sBuffer );

	// if no elements are found, insert some default values !
	if( sPlayer.length()==0 )
	{
		m_pPlayer->insertItem( "madplay" );
		m_pPlayer->insertItem( "mpg123" );

		m_pPlayerSettings->insertItem( "-q -v --no-tty-control" );
		m_pPlayerSettings->insertItem( "-q -v" );
	}
	else
	{
		ReadComboBoxes();
	}
}

ConfigPlayerDlgImpl::~ConfigPlayerDlgImpl()
{
}

void ConfigPlayerDlgImpl::sltCancel()
{
	emit reject();
}

void ConfigPlayerDlgImpl::sltOk()
{
	WriteComboBoxes();

	emit accept();
}

void ConfigPlayerDlgImpl::sltPlayerChanged(int)
{
}

void ConfigPlayerDlgImpl::WriteComboBoxes()
{
	char sBuffer[256];
	int i;

	m_aIniDB[ c_sActPlayerKey ] = m_pPlayer->currentText();
	m_aIniDB[ c_sActPlayerOptionsKey ] = m_pPlayerSettings->currentText();

	for( i=0; i<m_pPlayer->count(); i++ )
	{
		sprintf( sBuffer, "%s.%d", c_sPlayerKey, i );
		m_aIniDB[ sBuffer ] = m_pPlayer->text( i );
	}
	for( i=0; i<m_pPlayerSettings->count(); i++ )
	{
		sprintf( sBuffer, "%s.%d", c_sPlayerOptionsKey, i );
		m_aIniDB[ sBuffer ] = m_pPlayerSettings->text( i );
	}

	m_aIniDB.Save();
}

void ConfigPlayerDlgImpl::ReadComboBoxes()
{
	char	sBuffer[256];
	string	sActPlayer;
	string	sActPlayerOptions;
	bool	bFound = true;
	int		i = 0;
	int		iActPlayerPos = 0;
	int		iActPlayerOptionsPos = 0;

	MiniIniDB::const_iterator aIter = m_aIniDB.find( c_sActPlayerKey );
	if( aIter != m_aIniDB.end() )
	{
		sActPlayer = (*aIter).second.c_str();
	}
	aIter = m_aIniDB.find( c_sActPlayerOptionsKey );
	if( aIter != m_aIniDB.end() )
	{
		sActPlayerOptions = (*aIter).second.c_str();
	}

	do {
		sprintf( sBuffer, "%s.%d", c_sPlayerKey, i );

		MiniIniDB::const_iterator aIter = m_aIniDB.find( sBuffer );
		if( aIter != m_aIniDB.end() )
		{
			m_pPlayer->insertItem( (*aIter).second.c_str() );

			if( sActPlayer == (*aIter).second )
			{
				iActPlayerPos = i;
			}
		}
		else
		{
			bFound = false;
		}

		i++;
	} while( bFound );

	bFound = true;
	i = 0;
	do {
		sprintf( sBuffer, "%s.%d", c_sPlayerOptionsKey, i );

		MiniIniDB::const_iterator aIter = m_aIniDB.find( sBuffer );
		if( aIter != m_aIniDB.end() )
		{
			m_pPlayerSettings->insertItem( (*aIter).second.c_str() );

			if( sActPlayerOptions == (*aIter).second )
			{
				iActPlayerOptionsPos = i;
			}
		}
		else
		{
			bFound = false;
		}

		i++;
	} while( bFound );

	m_pPlayer->setCurrentItem( iActPlayerPos );
	m_pPlayerSettings->setCurrentItem( iActPlayerOptionsPos );
}

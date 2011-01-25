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

#include "configplayerdlgimpl.h"

#include <qcombobox.h>

#include <QSettings>

// *******************************************************************
// *******************************************************************
// *******************************************************************

ConfigPlayerDlgImpl::ConfigPlayerDlgImpl(QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
: ConfigPlayerDlg( parent, name, modal, fl )
{
#ifdef __APPLE__
    m_pPlayer->insertItem( "afplay" );
    m_pPlayer->insertItem( "/opt/local/bin/mpg123" );
#endif
    m_pPlayer->insertItem( "madplay" );
    m_pPlayer->insertItem( "mpg123" );

    m_pPlayerSettings->insertItem( "<none>" );
    m_pPlayerSettings->insertItem( "-q -v --no-tty-control" );
    m_pPlayerSettings->insertItem( "-q -v" );
    m_pPlayerSettings->insertItem( "-v" );
        
    RestoreSettings();
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
	SaveSettings();

	emit accept();
}

void ConfigPlayerDlgImpl::sltPlayerChanged(int)
{
}

void ConfigPlayerDlgImpl::RestoreSettings()
{
    QSettings aSettings;
    
    m_pPlayer->setCurrentIndex(m_pPlayer->findText(aSettings.value("Mp3/PlayerName",QString()).toString()));
    m_pPlayerSettings->setCurrentIndex(m_pPlayerSettings->findText(aSettings.value("Mp3/PlayerOptions",QString()).toString()));
}

void ConfigPlayerDlgImpl::SaveSettings()
{
    QSettings aSettings;
    
    aSettings.setValue("Mp3/PlayerName",m_pPlayer->currentText());
    aSettings.setValue("Mp3/PlayerOptions",m_pPlayerSettings->currentText());
}

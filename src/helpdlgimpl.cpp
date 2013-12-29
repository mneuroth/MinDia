/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2004 by Michael Neuroth
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

#include "helpdlgimpl.h"

#include <QMessageBox>

// *******************************************************************
// *******************************************************************
// *******************************************************************

HelpDlgImpl::HelpDlgImpl( QWidget* parent, Qt::WindowFlags fl )
: QDialog( parent, fl )
{
    setupUi(this);
}

HelpDlgImpl::~HelpDlgImpl()
{
}

void HelpDlgImpl::sltFindClicked()
{
	if( m_pSearchText->text().length()>0 )
	{
        if( !m_pTextBrowser->find( m_pSearchText->text() ) )
        {
            QMessageBox::warning( this, tr( "MinDia - Warning" ), tr( "Text not found !" ) );
        }
	}
}

void HelpDlgImpl::sltSearchTextChanged(const QString & /*sText*/)
{
    // reset the position for search ?
}

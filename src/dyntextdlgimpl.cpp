/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/dyntextdlgimpl.cpp,v $
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
 * Copyright (C) 2002-2003 by Michael Neuroth.							   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "dyntextdlgimpl.h"

#include <qcolordialog.h>
#include <qfontdialog.h>
#include <qpushbutton.h>
#include <qlineedit.h>

// *******************************************************************
// *******************************************************************
// *******************************************************************

DynamicTextDlgImpl::DynamicTextDlgImpl( const QFont & aInitFont, QWidget* parent, const char* name, bool modal, WFlags fl )
: DynamicTextDlg( parent, name, modal, fl ),
  m_aInitFont( aInitFont )
{
}

QFont DynamicTextDlgImpl::GetFont() const
{
	return m_aInitFont;
}

void DynamicTextDlgImpl::sltSelectFontcolor()
{
	QColor aColor = QColorDialog::getColor ( QColor(), this, "colordialog" );

	if( aColor.isValid() )
	{
		m_pSelectFontcolor->setBackgroundColor( aColor );
		m_pSelectFontcolor->setPalette( QPalette( aColor ) );
	}
}

void DynamicTextDlgImpl::sltSelectFont()
{
	bool bOk = false;

	QFont aFont = QFontDialog::getFont( &bOk, m_aInitFont );

	if( bOk )
	{
		QString sTemp;

		m_aInitFont = aFont;
		m_pFontName->setText( m_aInitFont.family() );
		sTemp = sTemp.setNum( m_aInitFont.pointSize() );
		m_pFontSize->setText( sTemp );
	}
}

void DynamicTextDlgImpl::sltDeleteText()
{
	setResult( 2 );
	hide();
}

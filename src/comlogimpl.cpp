/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/comlogimpl.cpp,v $
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

#include "comlogimpl.h"
#include "qtmtlock.h"

#include <qmultilineedit.h> 
#include <qevent.h>
#include <qapplication.h>

const int c_iCustomEvent_Logging = 12345;

// *******************************************************************
// *******************************************************************
// *******************************************************************

ComLoggingDialogImpl::ComLoggingDialogImpl( QWidget* parent, const char* name, bool modal, WFlags fl )
: ComLoggingDialog( parent, name, modal, fl )
{
    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltLoggingDialogClosed() ) );
    connect( this, SIGNAL( sigDialogHelp(const QString &) ), parent, SLOT( sltShowHelp(const QString &) ) );
}

ComLoggingDialogImpl::~ComLoggingDialogImpl()
{
}

void ComLoggingDialogImpl::LogMsg( const char * sMsg )
{
	QCustomEvent * pEvent = new QCustomEvent( c_iCustomEvent_Logging );
    pEvent->setData( new QString( sMsg ) );
	QApplication::postEvent( this, pEvent);
/*
	if( m_pOutput )
	{
		QtMTLock aMTLock;

		m_pOutput->insertLine( QString( sMsg ) );
		// ** update the visible area, so that the last output is visible
		m_pOutput->setCursorPosition( m_pOutput->numLines(), 1 );
	}
*/
}

void ComLoggingDialogImpl::sltCloseDialog()
{
	emit sigDialogClosed();

	emit accept();
}

void ComLoggingDialogImpl::closeEvent( QCloseEvent * pCloseEvent )
{
	emit sigDialogClosed();

	pCloseEvent->accept();
}

void ComLoggingDialogImpl::done( int iRet )
{
	emit sigDialogClosed();
	
	ComLoggingDialog::done( iRet );
}

void ComLoggingDialogImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Key_F1) )
	{
		emit sigDialogHelp( "LoggingDialog" );
	}
	else
	{
		ComLoggingDialog::keyPressEvent( pEvent );
	}
}

void ComLoggingDialogImpl::customEvent( QCustomEvent * pEvent )
{
    switch( pEvent->type() ) 
	{
		case c_iCustomEvent_Logging:
		{
			if( m_pOutput )
			{
				QtMTLock aMTLock;

			    QString * psMsg = (QString *)pEvent->data();

				m_pOutput->insertLine( *psMsg );
				// ** update the visible area, so that the last output is visible
				m_pOutput->setCursorPosition( m_pOutput->numLines(), 1 );

				delete psMsg;
			}
		} break;
		
		default:
		{
		}
	}
}


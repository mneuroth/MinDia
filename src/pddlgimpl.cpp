/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/pddlgimpl.cpp,v $
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

#include "pddlgimpl.h"

#include "diapresentation.h"

#include <qmultilineedit.h>


PresentationDataDlgImpl::PresentationDataDlgImpl( DiaPresentation * pData, QWidget* parent, const char* name, bool modal, WFlags fl )
: PresentationDataDlg( parent, name, modal, fl ),
  m_pData( pData )
{
    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltPresentationDataDialogClosed() ) );
    connect( this, SIGNAL( sigDocumentUpdate() ), parent, SLOT( sltDoDocumentStateUpdate() ) );
    connect( this, SIGNAL( sigDialogHelp(QWidget *, const QString &) ), parent, SLOT( sltShowModalHelp(QWidget *, const QString &) ) );
}

PresentationDataDlgImpl::~PresentationDataDlgImpl()
{
}

void PresentationDataDlgImpl::sltCloseDialog()
{
	TransferDataFromControl();

	emit accept();

	emit sigDialogClosed();
}

void PresentationDataDlgImpl::sltCancelDialog()
{
	emit reject();

	emit sigDialogClosed();
}

void PresentationDataDlgImpl::closeEvent( QCloseEvent * pCloseEvent )
{
	pCloseEvent->accept();

	emit sigDialogClosed();
}

void PresentationDataDlgImpl::showEvent( QShowEvent * /*pShowEvent*/ )
{
	TransferDataToControl();
}

void PresentationDataDlgImpl::TransferDataToControl()
{
	// ** init the data
	if( m_pComment && m_pData )
	{
		QString sComment = m_pData->GetComment().c_str();

		m_pComment->setText( sComment );
	}
}

void PresentationDataDlgImpl::TransferDataFromControl()
{
	if( m_pComment && m_pData )
	{
		QString sComment = m_pComment->text();

		// ** has text really changed ?
		string sOrg = m_pData->GetComment();
		if( QString( sOrg.c_str() ) != sComment )
		{
			m_pData->SetComment( (const char *)sComment );

			emit sigDocumentUpdate();
		}
	}
}

void PresentationDataDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Key_F1) )
	{
		emit sigDialogHelp( this, "PresentationDataDialog" );
	}
	else
	{
		PresentationDataDlg::keyPressEvent( pEvent );
	}
}

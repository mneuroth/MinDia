/**************************************************************************
 *ƒ
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
#include "misctools.h"

#include <QShowEvent>
#include <QKeyEvent>
#include <QCloseEvent>


PresentationDataDlgImpl::PresentationDataDlgImpl( DiaPresentation * pData, QWidget* parent, Qt::WFlags fl )
  : QDialog(parent, fl),
    m_pData( pData )
{
    setupUi(this);

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
        QString sComment = ToQString(m_pData->GetComment());

		m_pComment->setText( sComment );

        ImageRatio aImageRatio = m_pData->GetImageRatio();

        switch( aImageRatio )
        {
            case RATIO_16_9:
               m_prbt16_9->setChecked(true);
               break;
            case RATIO_3_2:
               m_prbt3_2->setChecked(true);
               break;
            case RATIO_4_3:
               m_prbt4_3->setChecked(true);
               break;
            case RATIO_IMAGE_RATIO:
               m_prbtImageRatio->setChecked(true);
               break;
            case RATIO_VARIABLE:
               m_prbtVariable->setChecked(true);
               break;
            case RATIO_USER:
               m_prbtUser->setChecked(true);
// TODO --> user daten fuer ration behandeln (not implemented yet)
               break;
        }
    }
}

void PresentationDataDlgImpl::TransferDataFromControl()
{
	if( m_pComment && m_pData )
	{
        bool bUpdate = false;
        QString sComment = m_pComment->document()->toPlainText();

		// ** has text really changed ?
		string sOrg = m_pData->GetComment();
        if( ToQString( sOrg ) != sComment )
		{
            m_pData->SetComment( ToStdString(sComment) );

            bUpdate = true;
		}

        ImageRatio aImageRatio = m_pData->GetImageRatio();
        ImageRatio aImageRatioNew = RATIO_UNDEFINED;

        if( m_prbt16_9->isChecked() )
        {
            aImageRatioNew = RATIO_16_9;
        }
        else if( m_prbt3_2->isChecked() )
        {
            aImageRatioNew = RATIO_3_2;
        }
        else if( m_prbt4_3->isChecked() )
        {
            aImageRatioNew = RATIO_4_3;
        }
        else if( m_prbtVariable->isChecked() )
        {
            aImageRatioNew = RATIO_VARIABLE;
        }
        else if( m_prbtImageRatio->isChecked() )
        {
            aImageRatioNew = RATIO_IMAGE_RATIO;
        }
        else if( m_prbtUser->isChecked() )
        {
            aImageRatioNew = RATIO_USER;
// TODO --> user daten fuer ration behandeln (not implemented yet)
        }

        if( aImageRatio != aImageRatioNew )
        {
// TODO --> ggf. message dialog anzeigen ob Aenderungen wirklich uebernommen werden sollen !?
            m_pData->SetImageRatio( aImageRatioNew );

            bUpdate = true;
        }

        if( bUpdate )
        {
            emit sigDocumentUpdate();
        }
	}
}

void PresentationDataDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Qt::Key_F1) )
	{
		emit sigDialogHelp( this, "PresentationDataDialog" );
	}
	else
	{
        QDialog::keyPressEvent( pEvent );
	}
}

/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/sndinfodlgimpl.cpp,v $
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

#include "sndinfodlgimpl.h"
#include "soundinfo.h"

#include <qtable.h>
#include <qlabel.h>
#include <qpushbutton.h>

// *******************************************************************
// *******************************************************************
// *******************************************************************

SoundInfoDlgImpl::SoundInfoDlgImpl( SoundInfoContainer * pSoundData, QWidget* parent, const char* name, bool modal, WFlags fl )
: SoundInfoDlg( parent, name, modal, fl ),
  m_pSoundData( pSoundData )
{
    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltSoundDataDialogClosed() ) );
    connect( this, SIGNAL( sigDocumentUpdate() ), parent, SLOT( sltDoDocumentStateUpdate() ) );
    connect( this, SIGNAL( sigUpdateViews() ), parent, SLOT( sltDoUpdateAllViews() ) );
    connect( this, SIGNAL( sigDataChanged() ), parent, SLOT( sltDoDataChanged() ) );
    connect( this, SIGNAL( sigDialogHelp(QWidget *, const QString &) ), parent, SLOT( sltShowModalHelp(QWidget *, const QString &) ) );

	// ** init table **
	m_pTable->setSelectionMode( QTable::Single );
	m_pTable->setNumCols( 11 );
	m_pTable->setNumRows( 1 );
	//m_pTable->setText( 0, 0, "Filenamexyz" );
	m_pTable->horizontalHeader()->setLabel( 0, tr( "wav-filename" ) );
	m_pTable->setColumnWidth( 0, 186 );
	m_pTable->horizontalHeader()->setLabel( 1, tr( "start-pos [ms]" ) );
	m_pTable->setColumnWidth( 1, 75 );
	m_pTable->horizontalHeader()->setLabel( 2, tr( "stop-pos [ms]" ) );
	m_pTable->setColumnWidth( 2, 75 );
	m_pTable->horizontalHeader()->setLabel( 3, tr( "delta [ms]" ) );
	m_pTable->setColumnWidth( 3, 75 );
	m_pTable->horizontalHeader()->setLabel( 4, tr( "delta [min:sec]" ) );
	m_pTable->setColumnWidth( 4, 75 );
	m_pTable->horizontalHeader()->setLabel( 5, tr( "total length [ms]" ) );
	m_pTable->setColumnWidth( 5, 75 );
	m_pTable->horizontalHeader()->setLabel( 6, tr( "total [min:sec]" ) );
	m_pTable->setColumnWidth( 6, 75 );

	m_pTable->horizontalHeader()->setLabel( 7, tr( "fade_in start [ms]" ) );
	m_pTable->setColumnWidth( 7, 75 );
	m_pTable->horizontalHeader()->setLabel( 8, tr( "fade_in length [ms]" ) );
	m_pTable->setColumnWidth( 8, 75 );
	m_pTable->horizontalHeader()->setLabel( 9, tr( "fade_out start [ms]" ) );
	m_pTable->setColumnWidth( 9, 75 );
	m_pTable->horizontalHeader()->setLabel( 10, tr( "fade_out length [ms]" ) );
	m_pTable->setColumnWidth( 10, 75 );

	// ** cancel button not supported yet
	buttonCancel->setEnabled( false );

	TransferData( true );
	UpdateCalculatedData();
}

SoundInfoDlgImpl::~SoundInfoDlgImpl()
{
}

void SoundInfoDlgImpl::sltCloseDialog()
{
	TransferData( false );

	emit sigDialogClosed();

	emit accept();
}

void SoundInfoDlgImpl::sltDialogCanceled()
{
	emit sigDialogClosed();

	emit reject();
}

void SoundInfoDlgImpl::sltNewRow()
{
	m_pTable->setNumRows( m_pTable->numRows()+1 );
}

void SoundInfoDlgImpl::sltDeleteRow()
{
	// ** move contents of act row to the end of the table
	if( m_pTable->currentSelection() != -1 )
	{
		// ** get the actual selected row, only one row can be selected !
		int iActRow = GetSelectedRow();

		for( int i=iActRow; i<m_pTable->numRows()-1; i++ )
		{
			m_pTable->swapRows( i, i+1 );
		}

		// ** and than delete the last row of the table
		m_pTable->setNumRows( m_pTable->numRows()-1 );

		// ** update the document data ***
		TransferData( false );
	}
}

void SoundInfoDlgImpl::sltRowUp()
{
	if( m_pTable->currentSelection() != -1 )
	{
		int iActRow = GetSelectedRow();

		if( iActRow>0 )
		{
			m_pTable->swapRows( iActRow, iActRow-1 );
			m_pTable->clearSelection();
			RepaintRow( iActRow );
			RepaintRow( iActRow-1 );

			QTableSelection aSelection;
			aSelection.init( iActRow-1, -1 );
			aSelection.expandTo( iActRow-1, m_pTable->numCols() );
			m_pTable->addSelection( aSelection );

			// ** update the document data ***
			TransferData( false );
		}
	}
}

void SoundInfoDlgImpl::sltRowDown()
{
	if( m_pTable->currentSelection() != -1 )
	{
		int iActRow = GetSelectedRow();

		if( iActRow<m_pTable->numRows()-1 )
		{
			m_pTable->swapRows( iActRow, iActRow+1 );
			m_pTable->clearSelection();
			RepaintRow( iActRow );
			RepaintRow( iActRow+1 );

			QTableSelection aSelection;
			aSelection.init( iActRow+1, -1 );
			aSelection.expandTo( iActRow+1, m_pTable->numCols() );
			m_pTable->addSelection( aSelection );

			// ** update the document data ***
			TransferData( false );
		}
	}
}

void SoundInfoDlgImpl::sltTableSelectionChanged()
{
	bool bSel = false;
	for( int i=0; i<m_pTable->numRows(); i++ )
	{
		bSel = bSel || m_pTable->isRowSelected( i, TRUE );
	}
	m_pDeleteLine->setEnabled( bSel );
	m_pLineUp->setEnabled( bSel );
	m_pLineDown->setEnabled( bSel );
}

void SoundInfoDlgImpl::sltValueChanged( int /*iRow*/, int /*iColumn*/ )
{
	TransferData( false );
	TransferData( true );
	UpdateCalculatedData();
}

void SoundInfoDlgImpl::closeEvent( QCloseEvent * pCloseEvent )
{
	emit sigDialogClosed();

	pCloseEvent->accept();
}

void SoundInfoDlgImpl::TransferData( bool bToTable )
{
	if( m_pSoundData )
	{
		if( bToTable )
		{
			// ** transfer data from data-container to gui
			m_pTable->setNumRows( m_pSoundData->size() );

			for( unsigned int i=0; i<m_pSoundData->size(); i++ )
			{
				QString s;
				int iPos;

				s = (*m_pSoundData)[i]->GetFileName().c_str();
				m_pTable->setText( i, 0, s );

				iPos = (*m_pSoundData)[i]->GetStartPos();
				s = s.setNum( iPos );
				m_pTable->setText( i, 1, s );
				iPos = (*m_pSoundData)[i]->GetStopPos();
				s = s.setNum( iPos );
				m_pTable->setText( i, 2, s );
				iPos = (*m_pSoundData)[i]->GetDelta();
				//QTableItem * pItem = m_pTable->item( 0, 3 );
				//pItem->EditType = QTableItem::None;
				s = s.setNum( iPos );
				m_pTable->setText( i, 3, s );
				s = SecondsInMinSec( iPos / 1000 ).c_str();
				m_pTable->setText( i, 4, s );
				iPos = (*m_pSoundData)[i]->GetTotalLength();
				s = s.setNum( iPos );
				m_pTable->setText( i, 5, s );
				s = SecondsInMinSec( iPos / 1000 ).c_str();
				m_pTable->setText( i, 6, s );

				iPos = (*m_pSoundData)[i]->GetFadeInStartPos();
				s = s.setNum( iPos );
				m_pTable->setText( i, 7, s );
				iPos = (*m_pSoundData)[i]->GetFadeInLength();
				s = s.setNum( iPos );
				m_pTable->setText( i, 8, s );
				iPos = (*m_pSoundData)[i]->GetFadeOutStartPos();
				s = s.setNum( iPos );
				m_pTable->setText( i, 9, s );
				iPos = (*m_pSoundData)[i]->GetFadeOutLength();
				s = s.setNum( iPos );
				m_pTable->setText( i, 10, s );
			}
		}
		else
		{
			m_pSoundData->erase( m_pSoundData->begin(), m_pSoundData->end() );

			for( int i=0; i<m_pTable->numRows(); i++ )
			{
				string	sFileName;
				int		iStartPos;
				int		iStopPos;
				int		iFadeInStart, iFadeInLength;
				int		iFadeOutStart, iFadeOutLength;
				QString s;
				bool	bOk;

				const char * sTemp =(const char *)m_pTable->text( i, 0 );
				sFileName = (sTemp ? sTemp : "" );
				sTemp = m_pTable->text( i, 1 );
				s = ( sTemp ? sTemp : "" );
				iStartPos = s.toInt( &bOk );
				if( !bOk )
				{
					iStartPos = -1;
				}
				sTemp = m_pTable->text( i, 2 );
				s = ( sTemp ? sTemp : "" );
				iStopPos = s.toInt( &bOk );
				if( !bOk || (iStartPos>iStopPos) )
				{
					iStopPos = -1;
				}

				sTemp = m_pTable->text( i, 7 );
				s = ( sTemp ? sTemp : "" );
				iFadeInStart = s.toInt( &bOk );
				sTemp = m_pTable->text( i, 8 );
				s = ( sTemp ? sTemp : "" );
				iFadeInLength = s.toInt( &bOk );
				sTemp = m_pTable->text( i, 9 );
				s = ( sTemp ? sTemp : "" );
				iFadeOutStart = s.toInt( &bOk );
				sTemp = m_pTable->text( i, 10 );
				s = ( sTemp ? sTemp : "" );
				iFadeOutLength = s.toInt( &bOk );

				minHandle<SoundInfo> hItem( new SoundInfo( sFileName, iStartPos, iStopPos ) );
				hItem->SetFadeInData( iFadeInStart, iFadeInLength );
				hItem->SetFadeOutData( iFadeOutStart, iFadeOutLength );
				m_pSoundData->push_back( hItem );
			}

			// min todo --> hier ggf. optimierte changed-Behandlung
			m_pSoundData->SetChanged();

			emit sigDocumentUpdate();
			emit sigUpdateViews();
		}
	}
}

void SoundInfoDlgImpl::UpdateCalculatedData()
{
	if( m_pSoundData )
	{
		int iTotal = m_pSoundData->GetTotalPlayLength();
		QString s = SecondsInMinSec( iTotal / 1000 ).c_str();
		m_pTotalPlayTime->setText( s );
	}
}

int SoundInfoDlgImpl::GetSelectedRow() const
{
	// ** get the actual selected row, only one row can be selected !
	int iActRow = m_pTable->numRows();	// ** means: no row selected
	for( int j=0; j<m_pTable->numRows(); j++ )
	{
		if( m_pTable->isRowSelected( j, TRUE ) )
		{
			iActRow = j;
			break;
		}
	}
	return iActRow;
}

void SoundInfoDlgImpl::RepaintRow( int iRow )
{
	for( int i=0; i<m_pTable->numCols(); i++ )
	{
		m_pTable->updateCell( iRow, i );
	}
}

void SoundInfoDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Key_F1) )
	{
		emit sigDialogHelp( this, "SoundInfoDialog" );
	}
	else
	{
		SoundInfoDlg::keyPressEvent( pEvent );
	}
}

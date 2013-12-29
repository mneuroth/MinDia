/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
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

#include "sndinfodlgimpl.h"
#include "soundinfo.h"
#include "misctools.h"

#include <QLabel>
#include <QPushButton>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QTableWidget>

// *******************************************************************
// *******************************************************************
// *******************************************************************

SoundInfoDlgImpl::SoundInfoDlgImpl( SoundInfoContainer * pSoundData, QWidget* parent, Qt::WindowFlags fl )
: QDialog( parent, fl ),
  m_pSoundData( pSoundData )
{
    setupUi(this);

    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltSoundDataDialogClosed() ) );
    connect( this, SIGNAL( sigDocumentUpdate() ), parent, SLOT( sltDoDocumentStateUpdate() ) );
    connect( this, SIGNAL( sigUpdateViews() ), parent, SLOT( sltDoUpdateAllViews() ) );
    connect( this, SIGNAL( sigDataChanged() ), parent, SLOT( sltDoDataChanged() ) );
    connect( this, SIGNAL( sigDialogHelp(QWidget *, const QString &) ), parent, SLOT( sltShowModalHelp(QWidget *, const QString &) ) );

	// ** init table **
    m_pTable->setSelectionMode( QAbstractItemView::SingleSelection );
    m_pTable->setColumnCount( 11 );
    m_pTable->setRowCount( 1 );
    m_pTable->setHorizontalHeaderItem(0,new QTableWidgetItem( tr( "sound-filename" ) ) );
	m_pTable->setColumnWidth( 0, 186 );
    m_pTable->setHorizontalHeaderItem(1,new QTableWidgetItem( tr( "start-pos [ms]" ) ) );
	m_pTable->setColumnWidth( 1, 75 );
    m_pTable->setHorizontalHeaderItem(2,new QTableWidgetItem( tr( "stop-pos [ms]" ) ) );
	m_pTable->setColumnWidth( 2, 75 );
    m_pTable->setHorizontalHeaderItem(3,new QTableWidgetItem( tr( "delta [ms]" ) ) );
	m_pTable->setColumnWidth( 3, 75 );
    m_pTable->setHorizontalHeaderItem(4,new QTableWidgetItem( tr( "delta [min:sec]" ) ) );
	m_pTable->setColumnWidth( 4, 75 );
    m_pTable->setHorizontalHeaderItem(5,new QTableWidgetItem( tr( "total length [ms]" ) ) );
	m_pTable->setColumnWidth( 5, 75 );
    m_pTable->setHorizontalHeaderItem(6,new QTableWidgetItem( tr( "total [min:sec]" ) ) );
	m_pTable->setColumnWidth( 6, 75 );

    m_pTable->setHorizontalHeaderItem(7,new QTableWidgetItem( tr( "fade_in start [ms]" ) ) );
	m_pTable->setColumnWidth( 7, 75 );
    m_pTable->setHorizontalHeaderItem(8,new QTableWidgetItem( tr( "fade_in length [ms]" ) ) );
	m_pTable->setColumnWidth( 8, 75 );
    m_pTable->setHorizontalHeaderItem(9,new QTableWidgetItem( tr( "fade_out start [ms]" ) ) );
	m_pTable->setColumnWidth( 9, 75 );
    m_pTable->setHorizontalHeaderItem(10,new QTableWidgetItem( tr( "fade_out length [ms]" ) ) );
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

    accept();
}

void SoundInfoDlgImpl::sltDialogCanceled()
{
	emit sigDialogClosed();

    reject();
}

void SoundInfoDlgImpl::sltNewRow()
{
    m_pTable->setRowCount( m_pTable->rowCount()+1 );
}

void SoundInfoDlgImpl::sltDeleteRow()
{
	// ** move contents of act row to the end of the table
    if( m_pTable->selectedItems().count()>0 )
	{
		// ** get the actual selected row, only one row can be selected !
		int iActRow = GetSelectedRow();
        m_pTable->removeRow(iActRow);

		// ** update the document data ***
		TransferData( false );
	}
}

void SoundInfoDlgImpl::SwapRows( int iRow1, int iRow2 )
{
    // simulate swapRows()
    for( int iCol=0; iCol<m_pTable->columnCount(); iCol++ )
    {
        QTableWidgetItem * pItem1 = m_pTable->takeItem( iRow1, iCol );
        QTableWidgetItem * pItem2 = m_pTable->takeItem( iRow2, iCol );

        m_pTable->setItem( iRow2, iCol, pItem1 );
        m_pTable->setItem( iRow1, iCol, pItem2 );
    }

}

void SoundInfoDlgImpl::sltRowUp()
{
    if( m_pTable->selectedItems().count()>0 )
	{
		int iActRow = GetSelectedRow();

		if( iActRow>0 )
		{
            SwapRows( iActRow, iActRow-1 );

            m_pTable->clearSelection();

            m_pTable->update(0,0,width(),height());

            // ** update the document data ***
			TransferData( false );
		}
	}
}

void SoundInfoDlgImpl::sltRowDown()
{
    if( m_pTable->selectedItems().count()>0 )
	{
		int iActRow = GetSelectedRow();

        if( iActRow<m_pTable->rowCount()-1 )
		{
            SwapRows( iActRow, iActRow+1 );

			m_pTable->clearSelection();

            m_pTable->update(0,0,width(),height());

            // ** update the document data ***
			TransferData( false );
		}
	}
}

void SoundInfoDlgImpl::sltTableSelectionChanged()
{
	bool bSel = false;
    for( int i=0; i<m_pTable->rowCount(); i++ )
	{
        bSel = bSel || GetSelectedRow()<m_pTable->rowCount();
	}
	m_pDeleteLine->setEnabled( bSel );
	m_pLineUp->setEnabled( bSel );
	m_pLineDown->setEnabled( bSel );
}

void SoundInfoDlgImpl::sltValueChanged( int /*iRow*/, int /*iColumn*/ )
{
    // wird nicht benoetigt, da beim schliessen des dialogs die uebertragung der daten erfolgt
    // waehrend dialog offen ist braucht man eigentlich keine aktualisierung !
    // Ansonsten ist dies hier eine rekursive endlos-schleife ! Event wird durch TransferData() ausgeloest !!!
    //TransferData( false );
    //TransferData( true );
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
            m_pTable->setRowCount( m_pSoundData->size() );

			for( unsigned int i=0; i<m_pSoundData->size(); i++ )
			{
				QString s;
				int iPos;

                s = ToQString( (*m_pSoundData)[i]->GetFileName() );
                m_pTable->setItem(i,0,new QTableWidgetItem( s ) );

				iPos = (*m_pSoundData)[i]->GetStartPos();
				s = s.setNum( iPos );
                m_pTable->setItem(i,1,new QTableWidgetItem( s ) );
                iPos = (*m_pSoundData)[i]->GetStopPos();
				s = s.setNum( iPos );
                m_pTable->setItem(i,2,new QTableWidgetItem( s ) );
                iPos = (*m_pSoundData)[i]->GetDelta();
				s = s.setNum( iPos );
                m_pTable->setItem(i,3,new QTableWidgetItem( s ) );
                s = ToQString( SecondsInMinSec( iPos / 1000 ) );
                m_pTable->setItem(i,4,new QTableWidgetItem( s ) );
                iPos = (*m_pSoundData)[i]->GetTotalLength();
				s = s.setNum( iPos );
                m_pTable->setItem(i,5,new QTableWidgetItem( s ) );
                s = ToQString( SecondsInMinSec( iPos / 1000 ) );
                m_pTable->setItem(i,6,new QTableWidgetItem( s ) );

				iPos = (*m_pSoundData)[i]->GetFadeInStartPos();
				s = s.setNum( iPos );
                m_pTable->setItem(i,7,new QTableWidgetItem( s ) );
                iPos = (*m_pSoundData)[i]->GetFadeInLength();
				s = s.setNum( iPos );
                m_pTable->setItem(i,8,new QTableWidgetItem( s ) );
                iPos = (*m_pSoundData)[i]->GetFadeOutStartPos();
				s = s.setNum( iPos );
                m_pTable->setItem(i,9,new QTableWidgetItem( s ) );
                iPos = (*m_pSoundData)[i]->GetFadeOutLength();
				s = s.setNum( iPos );
                m_pTable->setItem(i,10,new QTableWidgetItem( s ) );
            }
		}
		else
		{
			m_pSoundData->erase( m_pSoundData->begin(), m_pSoundData->end() );

            for( int i=0; i<m_pTable->rowCount(); i++ )
			{
				string	sFileName;
                int     iTotalLength;
				int		iStartPos;
				int		iStopPos;
				int		iFadeInStart, iFadeInLength;
				int		iFadeOutStart, iFadeOutLength;
				QString s;
				bool	bOk;

                sFileName = (m_pTable->item( i, 0 ) ? ToStdString(m_pTable->item( i, 0 )->text()) : "");
                s = ( m_pTable->item( i, 1 ) ? m_pTable->item( i, 1 )->text() : "" );
				iStartPos = s.toInt( &bOk );
				if( !bOk )
				{
					iStartPos = -1;
				}
                s = ( m_pTable->item( i, 2 ) ? m_pTable->item( i, 2 )->text() : "" );
				iStopPos = s.toInt( &bOk );
				if( !bOk || (iStartPos>iStopPos) )
				{
					iStopPos = -1;
				}
                s = ( m_pTable->item( i, 3 ) ? m_pTable->item( i, 3 )->text() : "" );
                iTotalLength = s.toInt( &bOk );
                if( !bOk )
                {
                    iTotalLength = 0;
                }

                s = ( m_pTable->item( i, 7 ) ? m_pTable->item( i, 7 )->text() : "" );
				iFadeInStart = s.toInt( &bOk );
                s = ( m_pTable->item( i, 8 ) ? m_pTable->item( i, 8 )->text() : "" );
				iFadeInLength = s.toInt( &bOk );
                s = ( m_pTable->item( i, 9 ) ? m_pTable->item( i, 9 )->text() : "" );
				iFadeOutStart = s.toInt( &bOk );
                s = ( m_pTable->item( i, 10 ) ? m_pTable->item( i, 10 )->text() : "" );
				iFadeOutLength = s.toInt( &bOk );

                minHandle<SoundInfo> hItem( new SoundInfo( sFileName, iTotalLength, iStartPos, iStopPos ) );
				hItem->SetFadeInData( iFadeInStart, iFadeInLength );
				hItem->SetFadeOutData( iFadeOutStart, iFadeOutLength );
				m_pSoundData->push_back( hItem );
			}

            // hier ggf. optimierte changed-Behandlung durchfuehren ?
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
        QString s = ToQString( SecondsInMinSec( iTotal / 1000 ) );
		m_pTotalPlayTime->setText( s );
	}
}

int SoundInfoDlgImpl::GetSelectedRow() const
{
	// ** get the actual selected row, only one row can be selected !
    int iActRow = m_pTable->rowCount();	// ** means: no row selected
    for( int j=0; j<m_pTable->rowCount(); j++ )
	{
        if( m_pTable->item(j,0) && m_pTable->item(j,0)->isSelected() )
        {
            iActRow = j;
            break;
        }
	}
	return iActRow;
}

void SoundInfoDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Qt::Key_F1) )
	{
		emit sigDialogHelp( this, "SoundInfoDialog" );
	}
	else
	{
        QDialog::keyPressEvent( pEvent );
	}
}

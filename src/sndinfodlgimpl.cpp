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
 *  $Revision: 1.2 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.1.1.1  2003/08/15 16:38:22  min
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

#include "sndinfodlgimpl.h"
#include "soundinfo.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QTableWidget>

// *******************************************************************
// *******************************************************************
// *******************************************************************

SoundInfoDlgImpl::SoundInfoDlgImpl( SoundInfoContainer * pSoundData, QWidget* parent, Qt::WFlags fl )
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
	//m_pTable->setText( 0, 0, "Filenamexyz" );
    m_pTable->setHorizontalHeaderItem(0,new QTableWidgetItem( tr( "wav-filename" ) ) );
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
#ifndef ZAURUS
	buttonCancel->setEnabled( false );
#endif

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

//        for( int i=iActRow; i<m_pTable->rowCount()-1; i++ )
//		{
//			m_pTable->swapRows( i, i+1 );
//		}
//
//		// ** and than delete the last row of the table
//        m_pTable->setRowCount( m_pTable->rowCount()-1 );

		// ** update the document data ***
		TransferData( false );
	}
}

void SoundInfoDlgImpl::sltRowUp()
{
    if( m_pTable->selectedItems().count()>0 )
	{
		int iActRow = GetSelectedRow();

		if( iActRow>0 )
		{
// TODO porting			m_pTable->swapRows( iActRow, iActRow-1 );
			m_pTable->clearSelection();
			RepaintRow( iActRow );
			RepaintRow( iActRow-1 );
/* TODO
			Q3TableSelection aSelection;
			aSelection.init( iActRow-1, -1 );
            aSelection.expandTo( iActRow-1, m_pTable->columnCount() );
			m_pTable->addSelection( aSelection );
*/
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
//TODO porting			m_pTable->swapRows( iActRow, iActRow+1 );
			m_pTable->clearSelection();
			RepaintRow( iActRow );
			RepaintRow( iActRow+1 );

  /* TODO
            Q3TableSelection aSelection;
			aSelection.init( iActRow+1, -1 );
            aSelection.expandTo( iActRow+1, m_pTable->columnCount() );
			m_pTable->addSelection( aSelection );
*/
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
        bSel = bSel || GetSelectedRow()<m_pTable->rowCount(); //m_pTable->isRowSelected( i, TRUE );
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
// TODO
//	TransferData( false );
//	TransferData( true );
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

				s = (*m_pSoundData)[i]->GetFileName().c_str();
                m_pTable->setItem(i,0,new QTableWidgetItem( s ) );

				iPos = (*m_pSoundData)[i]->GetStartPos();
				s = s.setNum( iPos );
                m_pTable->setItem(i,1,new QTableWidgetItem( s ) );
                iPos = (*m_pSoundData)[i]->GetStopPos();
				s = s.setNum( iPos );
                m_pTable->setItem(i,2,new QTableWidgetItem( s ) );
                iPos = (*m_pSoundData)[i]->GetDelta();
				//QTableItem * pItem = m_pTable->item( 0, 3 );
				//pItem->EditType = QTableItem::None;
				s = s.setNum( iPos );
                m_pTable->setItem(i,3,new QTableWidgetItem( s ) );
                s = SecondsInMinSec( iPos / 1000 ).c_str();
                m_pTable->setItem(i,4,new QTableWidgetItem( s ) );
                iPos = (*m_pSoundData)[i]->GetTotalLength();
				s = s.setNum( iPos );
                m_pTable->setItem(i,5,new QTableWidgetItem( s ) );
                s = SecondsInMinSec( iPos / 1000 ).c_str();
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

                const char * sTemp =(const char *)(m_pTable->item( i, 0 ) ? m_pTable->item( i, 0 )->text().toAscii() : "");
				sFileName = (sTemp ? sTemp : "" );
                sTemp = m_pTable->item( i, 1 ) ? m_pTable->item( i, 1 )->text().toAscii() : "";
				s = ( sTemp ? sTemp : "" );
				iStartPos = s.toInt( &bOk );
				if( !bOk )
				{
					iStartPos = -1;
				}
                sTemp = m_pTable->item( i, 2 ) ? m_pTable->item( i, 2 )->text().toAscii() : "";
				s = ( sTemp ? sTemp : "" );
				iStopPos = s.toInt( &bOk );
				if( !bOk || (iStartPos>iStopPos) )
				{
					iStopPos = -1;
				}
                sTemp = m_pTable->item( i, 3 ) ? m_pTable->item( i, 3 )->text().toAscii() : "";
                s = ( sTemp ? sTemp : "" );
                iTotalLength = s.toInt( &bOk );
                if( !bOk )
                {
                    iTotalLength = 0;
                }

                sTemp = m_pTable->item( i, 7 ) ? m_pTable->item( i, 7 )->text().toAscii() : "";
				s = ( sTemp ? sTemp : "" );
				iFadeInStart = s.toInt( &bOk );
                sTemp = m_pTable->item( i, 8 ) ? m_pTable->item( i, 8 )->text().toAscii() : "";
				s = ( sTemp ? sTemp : "" );
				iFadeInLength = s.toInt( &bOk );
                sTemp = m_pTable->item( i, 9 ) ? m_pTable->item( i, 9 )->text().toAscii() : "";
				s = ( sTemp ? sTemp : "" );
				iFadeOutStart = s.toInt( &bOk );
                sTemp = m_pTable->item( i, 10 ) ? m_pTable->item( i, 10 )->text().toAscii() : "";
				s = ( sTemp ? sTemp : "" );
				iFadeOutLength = s.toInt( &bOk );

                minHandle<SoundInfo> hItem( new SoundInfo( sFileName, iTotalLength, iStartPos, iStopPos ) );
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

void SoundInfoDlgImpl::RepaintRow( int /*iRow*/ )
{
    m_pTable->update(0,0,width(),height());
//    for( int i=0; i<m_pTable->columnCount(); i++ )
//	{
////TODO        m_pTable->updateCell( iRow, i );
//	}
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

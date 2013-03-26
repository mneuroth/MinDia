/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/commentdlgimpl.cpp,v $
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

#include "commentdlgimpl.h"
#include "soundinfo.h"
#include "misctools.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <QKeyEvent>
#include <QCloseEvent>

// *******************************************************************
// *******************************************************************
// *******************************************************************

CommentDlgImpl::CommentDlgImpl( GenericCommentContainer * pComments, QWidget* parent, Qt::WFlags fl )
: QDialog( parent, fl ),
  m_pComments( pComments )
{
    setupUi(this);

    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltSoundDataDialogClosed() ) );
    connect( this, SIGNAL( sigDocumentUpdate() ), parent, SLOT( sltDoDocumentStateUpdate() ) );
    connect( this, SIGNAL( sigUpdateViews() ), parent, SLOT( sltDoUpdateAllViews() ) );
    connect( this, SIGNAL( sigDataChanged() ), parent, SLOT( sltDoDataChanged() ) );
    connect( this, SIGNAL( sigDialogHelp(QWidget *, const QString &) ), parent, SLOT( sltShowModalHelp(QWidget *, const QString &) ) );

	// ** init table **
	m_pTable->setSelectionMode( QAbstractItemView::SingleSelection );
	m_pTable->/*setNumRows*/setRowCount( 1 );

	// ** init the title text for the columns
	// ** get one item to inspect the name for the columns
	// ** if no item exists, create a single item temporaray
	bool bItemCreated = false;
	if( m_pComments && m_pComments->size()==0 ) 
	{
		m_pComments->push_back_new_item();
		bItemCreated = true;
	}
	GenericDataInterface * pItem = m_pComments ? m_pComments->at( 0 ) : 0;
	if( pItem )
	{
		int iCount = pItem->GetDataCount();
		int iDefWidth = m_pTable->width() / (iCount+1);

		m_pTable->/*setNumCols*/setColumnCount( iCount );

        QStringList aLabels;
		for( int i=0; i<iCount; i++ )
		{
            aLabels.push_back( ToQString(pItem->GetDataName( i )) );
			// ** last column for the comment should be bigger than the other columns !
			if( i==iCount-1 )
			{
				m_pTable->setColumnWidth( i, iDefWidth*2 );
			}
			else
			{
				m_pTable->setColumnWidth( i, iDefWidth );
			}
		}
        m_pTable->setHorizontalHeaderLabels( aLabels );

		// ** update the title of the dialog
        setWindowTitle( windowTitle() + " (" + ToQString( pItem->GetName() ) + ")" );
	}
	// ** destroy the created item !
	if( m_pComments && bItemCreated )
	{
		m_pComments->clear();
	}

	// ** cancel button not supported yet
	buttonCancel->setEnabled( false );

	TransferData( true );
}

CommentDlgImpl::~CommentDlgImpl()
{
}

void CommentDlgImpl::sltCloseDialog()
{
	TransferData( false );

	emit sigDialogClosed();

	emit accept();
}

void CommentDlgImpl::sltDialogCanceled()
{
	emit sigDialogClosed();

	emit reject();
}

void CommentDlgImpl::sltNewRow()
{
    m_pTable->setRowCount( m_pTable->rowCount()+1 );
    for( int j=0; j<m_pTable->columnCount(); j++ )
    {
        m_pTable->setItem(m_pTable->rowCount()-1,j,new QTableWidgetItem( "" ) );
    }
}

void CommentDlgImpl::sltDeleteRow()
{
	// ** move contents of act row to the end of the table 
	if( /*m_pTable->currentSelection() != -1*/m_pTable->selectedItems().count()>0 )
	{
		// ** get the actual selected row, only one row can be selected !
		int iActRow = m_pTable->/*numRows*/rowCount();
		for( int j=0; j<m_pTable->/*numRows*/rowCount(); j++ )
		{
			//if( m_pTable->isRowSelected( j, TRUE ) )
			if( m_pTable->item(j,0)->isSelected() )
			{
				iActRow = j;
				break;
			}
		}
        m_pTable->removeRow(iActRow);

//		for( int i=iActRow; i<m_pTable->/*numRows*/rowCount()-1; i++ )
//		{
////TODO			m_pTable->swapRows( i, i+1 );
//		}
//
//		// ** and than delete the last row of the table 
//		m_pTable->/*setNumRows*/setRowCount( m_pTable->/*numRows*/rowCount()-1 );

		// ** update the document data ***
		TransferData( false );
	}
}

void CommentDlgImpl::sltTableSelectionChanged()
{
	bool bSel = false;
	for( int i=0; i<m_pTable->/*numRows*/rowCount(); i++ )
	{
        bSel = bSel || m_pTable->item(i,0)->isSelected();
		//bSel = bSel || m_pTable->isRowSelected( i, TRUE );
	}
	m_pDeleteLine->setEnabled( bSel );
}

void CommentDlgImpl::sltValueChanged( int /*iRow*/, int /*iColumn*/ )
{
// wird nicht benoetigt, da beim schliessen des dialogs die uebertragung der daten erfolgt
// waehrend dialog offen ist braucht man eigentlich keine aktualisierung !
// Ansonsten ist dies hier eine rekursive endlos-schleife ! Event wird durch TransferData() ausgeloest !!!
	// ** first: data from gui to container
// TODO ? --> crash !    
//    TransferData( false );
//    // ** than update view with new data from container
//    TransferData( true );
}

void CommentDlgImpl::sltSortTable()
{
	if( m_pComments )
	{
		m_pComments->SortData();
	}

	// ** after the data is sorted, update the gui
	TransferData( /*bToTable*/true );
}

void CommentDlgImpl::closeEvent( QCloseEvent * pCloseEvent )
{
	emit sigDialogClosed();

	pCloseEvent->accept();
}

void CommentDlgImpl::TransferData( bool bToTable )
{
	if( m_pComments )
	{
		if( bToTable )
		{
			// ** transfer data from data-container to gui
			m_pTable->/*setNumRows*/setRowCount( m_pComments->size() );
//m_pTable->/*setNumRows*/setColumnCount( 10 );

			for( int i=0; i<m_pComments->size(); i++ )
			{
				GenericDataInterface * pItem = m_pComments->at( i );

				for( int j=0; j<pItem->GetDataCount(); j++ )
				{
					string s = pItem->GetDataValue( j );
                    m_pTable->setItem(i,j,new QTableWidgetItem( ToQString(s) ) );
				}
			}
		}
		else
		{
			// ** erase contents of container
			m_pComments->clear();

			// ** and than fill the new data from the gui into the container
			for( int i=0; i<m_pTable->/*numRows*/rowCount(); i++ )
			{
				GenericDataInterface * pItem = m_pComments->push_back_new_item();

				for( int j=0; j<pItem->GetDataCount(); j++ )
				{
					QString sTemp = m_pTable->item(i,j) ? m_pTable->item(i,j)->text() : ""; //m_pTable->text( i, j );
                    string sStrg( ToStdString(sTemp) );

					if( (pItem->GetDataType( j )==GenericDataInterface::_STRING) ||
						(pItem->GetDataType( j )==GenericDataInterface::_INT) ||
						(pItem->GetDataType( j )==GenericDataInterface::_DOUBLE) )
					{
						pItem->SetDataValue( j, sStrg );
					}
				}
			}

			// min todo --> hier ggf. optimierte changed-Behandlung
			m_pComments->SetChanged();

			emit sigDocumentUpdate();
			emit sigUpdateViews();
		}
	}
}

void CommentDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Qt::Key_F1) )
	{
		emit sigDialogHelp( this, "CommentDialog" );
	}
	else
	{
		QDialog::keyPressEvent( pEvent );
	}
}

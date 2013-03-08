/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/hitemview.cpp,v $
 *
 *  $Revision: 1.4 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.3  2004/01/29 21:28:12  min
 *	Bugfix: disable drag & drop in run modus
 *	
 *	Revision 1.2  2004/01/18 23:48:07  min
 *	Changes for disabling compiler warnings.
 *	
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

#include "hitemview.h"

#include "diapresentation.h"

#include "misctools.h"

#include <stdio.h>

#include <QDropEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QScrollBar>
#include <QMenu>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QDrag>

// *******************************************************************
// *******************************************************************
// *******************************************************************

static int g_iIDCount = 0;

static string GetNextFreeID()
{
	char sBuffer[12];

	g_iIDCount++;

	sprintf( sBuffer, "id%d", g_iIDCount );

	return sBuffer;
}

// *******************************************************************

/** Helper class to restore the contents of a Qt-ScrollView */
class _RestoreContents
{
public:
    _RestoreContents( QAbstractScrollArea * pScrollView )
		: m_pScrollView( pScrollView )
	{
        m_x = m_pScrollView->horizontalScrollBar()->sliderPosition();
        m_y = m_pScrollView->verticalScrollBar()->sliderPosition();
	}
	~_RestoreContents()
	{
        m_pScrollView->horizontalScrollBar()->setSliderPosition(m_x);
        m_pScrollView->verticalScrollBar()->setSliderPosition(m_y);
    }

private:
    QAbstractScrollArea *   m_pScrollView;
    int                     m_x;
    int                     m_y;
};

// *******************************************************************
// *******************************************************************
// *******************************************************************

#define MODIFY_ENTRY	1

HItemView::HItemView( QWidget * pParent, int iWidth, int iHeight, QWidget * pMainWin, QObject * pControler, DiaPresentation * pDoc )
: QGraphicsView( 0, pParent )
{
    setAlignment(Qt::AlignLeft|Qt::AlignTop);
    setAcceptDrops(true);
//TODO porting	setDragAutoScroll( TRUE );
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_iDragTargetIndex = -1;

	m_pContextMenu		= new QMenu( this );
    
    QAction * pAction = new QAction( tr( "&Modify entry..." ), this );
	connect( pAction, SIGNAL( triggered() ), pMainWin, SLOT( sltShowModifyItem() ) );
    m_pContextMenu->addAction( pAction );
    
    m_pCanvas			= new QGraphicsScene( 0, 0, iWidth, iHeight );
	//m_pCanvas->setDoubleBuffering( true );
    setScene( m_pCanvas );

	// set default value for size hint
//	m_aSizeHint			= QSize( 600, 240 );

	m_aNextItemPos		= QPoint( 0, 0 );
	m_aItemShift		= QPoint( 180, 0 );
    m_aDefaultItemSize	= QRect( 0, 0, m_aItemShift.x(), 240/*m_aSizeHint.height()*/ );

	m_pDiaPres			= pDoc;

	setFocusPolicy( Qt::ClickFocus );

	// *** connect signals to slots ***
	if( pMainWin )
	{
    	connect( this, SIGNAL( sigItemSelected(int,HItem *,int,int) ), pMainWin, SLOT( sltItemSelected(int,HItem *,int,int) ) );
    	connect( this, SIGNAL( sigSelectItem(int,int) ), pMainWin, SLOT( sltSelectItem(int,int) ) );
    	connect( this, SIGNAL( sigViewDataChanged() ), pControler, SLOT( sltDataChanged() ) );
	    connect( this, SIGNAL( sigDialogHelp(const QString &) ), pMainWin, SLOT( sltShowHelp(const QString &) ) );
//    	connect( this, SIGNAL( sigModifySelectedEntry() ), pMainWin, SLOT( sltShowModifyItem() ) );
		connect( this, SIGNAL( sigLoadDoc(const QString &, bool) ), pMainWin, SLOT( sltLoadDoc(const QString &, bool) ) );
	}
}

HItemView::~HItemView()
{
	DeleteAllHItems();

	delete m_pCanvas;

	delete m_pContextMenu;
}

//QSize HItemView::sizeHint() const
//{
//    return m_aSizeHint;
//}
//
//void HItemView::setSizeHint( const QSize & aSize )
//{
//	m_aSizeHint = aSize;
//}

void HItemView::sltNewItem()
{
	// ** create a new dia item
	AddItemAt( minHandle<DiaInfo>( new DiaInfo( GetNextFreeID() ) ), /*means append*/-1, /*bInsert*/true );
}

void HItemView::sltUpdateView()
{
    m_pCanvas->update();
    // ** update view widget to show the new created item
    update( x(), y(), width(), height() );
}

void HItemView::sltUpdateSelected()
{
    m_pCanvas->update();
    // ** only repaint the selected border of all dias
    update( x(), y(), width(), height() );
}

void HItemView::AddItemAt( minHandle<DiaInfo> hDia, int iIndex, bool bInsert, bool bUpdateView, bool bDoResize )
{
	if( m_pDiaPres )
	{
		if( !bInsert )
		{
			hDia = m_pDiaPres->GetDiaAt( iIndex );
		}
		else
		{
			m_pDiaPres->AddDiaAt( iIndex, hDia );
		}

	/*	// ** here one can do an default initialisation of the new items
		// ** attention: with the SetData()-Call the item will be modified 
		// ** and so the document will be changed !
		if( hDia->IsEmpty() )
		{
		    // ** set default values for new item
			hDia->SetData( "", "", "" );
		}
	*/
	}

	HItem * pNewItem = 0;

	// ** insert or push_back in container depends on the index
	// ** index == -1 means, append at the end of the container
	if( (iIndex==-1) || (iIndex==(int)m_aItemContainer.size()) )
	{
		QRect aRect( m_aNextItemPos.x(), m_aNextItemPos.y(), m_aDefaultItemSize.width(), m_aDefaultItemSize.height() );

		pNewItem = new HItem( aRect, m_pCanvas, hDia );

	    // ** push new item to container
	    m_aItemContainer.push_back( pNewItem );
	}
	else
	{
		QPoint  aPos( GetPosOfItemWithIndex( iIndex ) );
		QRect   aRect( aPos.x(), aPos.y(), m_aDefaultItemSize.width(), m_aDefaultItemSize.height() );

		pNewItem = new HItem( aRect, m_pCanvas, hDia );

		// ** insert new item at position iIndex, shift all other items in view and container
        ShiftViewItemsFromIndex( iIndex );
		// ** insert item in the container
        m_aItemContainer.insert( m_aItemContainer.begin()+iIndex, pNewItem );
	}
	pNewItem->show();

	// ** update internal data for the next item
	m_aNextItemPos = m_aNextItemPos + m_aItemShift;

	// ** verifiy if the size for the canvas is big enough
	if( bDoResize && (m_pCanvas->width()<m_aNextItemPos.x()) )
	{
		// ** WARNING: the resize-method is expensive !!!
        m_pCanvas->setSceneRect(0,0,(qreal)m_aNextItemPos.x(),(qreal)m_pCanvas->height());
		// ** shift viewport, to ensure that the new item is visible
        ensureVisible( (qreal)m_aNextItemPos.x(), (qreal)m_aNextItemPos.y(), m_pCanvas->width(), m_pCanvas->height() );
	}

	if( bUpdateView )
	{
		// ** data changed, generate event for listener
		emit sigViewDataChanged();
		// ** update view widget to show the new created item
        //sltUpdateView();	// geschieht automatisch durch sigViewDataChanged()
	}

    // ** set the input focus on this window (for fast keyboard commands)
	setFocus();
}

void HItemView::RemoveItemAt( int iIndex, bool bDeleteData, bool bUpdateView )
{
	if( (iIndex>=0) && (iIndex<(int)m_aItemContainer.size()) )
	{
		HItem * pItem = m_aItemContainer[ iIndex ];

		// ** synchronize view with data model
		if( bDeleteData && m_pDiaPres )
		{
			m_pDiaPres->RemoveDiaAt( iIndex );
		}

		// ** container is owner of the pointer
		delete pItem;

		// ** delete pointer from the container
		m_aItemContainer.erase( m_aItemContainer.begin() + iIndex );

		if( bUpdateView )
		{
			// ** data changed, generate event for listener
			emit sigViewDataChanged();
			// ** update view widget to show the new created item
			//sltUpdateView();	// geschieht automatisch durch sigViewDataChanged()
		}

        // ** verifiy if the size for the canvas is too big and needs a resize (23.4.2011)
        if( /*bDoResize &&*/ (m_pCanvas->width()>m_aNextItemPos.x()) )
        {
            // ** WARNING: the resize-method is expensive !!!
            m_pCanvas->setSceneRect(0,0,(qreal)m_aNextItemPos.x(),(qreal)m_pCanvas->height());
            // ** shift viewport, to ensure that the new item is visible
            ensureVisible( (qreal)m_aNextItemPos.x(), (qreal)m_aNextItemPos.y(), m_pCanvas->width(), m_pCanvas->height() );
        }
    }
}

/*
DiaInfo * HItemView::GetItemAt( int iIndex ) const
{
	if( iIndex>=0 && iIndex<m_aItemContainer.size() )
	{
		HItem * pItem = m_aItemContainer[ iIndex ];

		return pItem;
	}
	return 0;
}
*/

void HItemView::mousePressEvent( QMouseEvent * pEvent )
{
	bool bIsOneSelected = false;
	HItem * pFirstSelectedItem = 0;

	if( (pEvent->button() == Qt::LeftButton) )
	{
		// ** disable mouse input in play-modus !
		if( m_pDiaPres && !m_pDiaPres->IsEdit() )
		{
			return;
		}

		// ** multi-selection only if shift button is pressed !
		// ** if no shift buttion is presed deselect all selected items
        if( !( (pEvent->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) )
		{
			sltSelectAllItems( false, false	);		// no update needed, because update will be triggert later in this method !
		}

		// ** find the selected item...
		int iFirstSelectedItemNo = -1;
		for( int i=0; i<(int)m_aItemContainer.size(); i++ )
		{
			HItem * pItem = m_aItemContainer[ i ];
			if( pItem && pItem->IsPointInItem( pEvent->x(), pEvent->y() ) )
			{
				bIsOneSelected = true;
				pItem->SetSelected( !pItem->GetSelected() );
				if( pFirstSelectedItem==0 )
				{
					pFirstSelectedItem = pItem;
					iFirstSelectedItemNo = i;
				}
			}
		}

		// ** if one item is clicked on, deselect all others...
		if( !bIsOneSelected )
		{
			sltSelectAllItems( false, true );
		}

		emit sigItemSelected( GetSelectedCount(), pFirstSelectedItem, iFirstSelectedItemNo, 0 );

		// ** update the view
        sltUpdateSelected();
	}
	else if( (pEvent->button() == Qt::RightButton) )
	{
		// ** handle popup menu...
		m_pContextMenu->exec( pEvent->globalPos() );
	}
}

bool HItemView::IsDragTargetNotDragSource( QMouseEvent * pEvent, int iActIndex )
{
	QPoint aPoint = pEvent->pos();

    int xx = aPoint.x() + x();
    int yy = aPoint.y() + y();

	// find the index for the selected image
	for( int i=0; i<(int)m_aItemContainer.size(); i++ )
	{
		HItem * pItem = m_aItemContainer[ i ];
        if( pItem && pItem->IsPointInItem( xx, yy ) )
		{
			return i != iActIndex;
		}
	}
	return false;
}

void HItemView::mouseMoveEvent( QMouseEvent * pEvent )
{
	// ** disable mouse input in play-modus !
	if( m_pDiaPres && !m_pDiaPres->IsEdit() )
	{
		return;
	}

	if( GetSelectedCount()>0 && IsDragTargetNotDragSource( pEvent, GetLastSelectedItemIndex() ) )
	{
		QString sDragString;
		MyIndexContainer aIndexContainer;

		if( GetActClipboardData( sDragString, &aIndexContainer ) )
		{
			QDrag *d = new QDrag( this );
            QMimeData * pMimeData = new QMimeData();
            pMimeData->setText(sDragString);
            d->setMimeData(pMimeData);            

            if( (pEvent->modifiers() & Qt::ControlModifier)==Qt::ControlModifier )
			{
                /*Qt::DropAction dropAction =*/ d->exec(Qt::CopyAction);
			}
			else
			{
                Qt::DropAction dropAction = d->exec(Qt::MoveAction);
                bool bOk = Qt::MoveAction==dropAction;

                // Bugfix 10.1.2011 --> for the MAC plattform the widget hirachy may be different for drag and drop
                if( bOk && (d->target()==this || d->target()->parent()==this ))
				{
					int iOffset = 0;

					// now delete all moved data (move == copy + delete)
					MyIndexContainer::reverse_iterator aIter = aIndexContainer.rbegin();
					while( aIter != aIndexContainer.rend() )
					{
						// is the target-index of the dragging operation before 
						// the source-index ?
						// if yes, than there have to be an offset !
						if( m_iDragTargetIndex<= *aIter )
						{
							iOffset = aIndexContainer.size();
						}
						else
						{
							iOffset = 0;
						}

						sltSelectItem( *aIter + iOffset, 0 );
						sltDeleteAllSlectedItems();
						++aIter;
					}

					sltSelectItem( m_iDragTargetIndex+iOffset-1, 0 );
				}
			}
		}
	}
}

/*
void HItemView::contentsMouseDoubleClickEvent( QMouseEvent * pEvent )
{
}
*/

void HItemView::keyPressEvent( QKeyEvent * pEvent )
{
	// ** disable keyboard input in play-modus !
	if( m_pDiaPres && !m_pDiaPres->IsEdit() )
	{
		return;
	}

	//if( pEvent->key() == Key_Delete )
    if( pEvent->text() == "d" )
	{
		sltDeleteAllSlectedItems();
	}
    else if( (pEvent->text() == "n") || (pEvent->key() == Qt::Key_Right) )
	{
		sltSelectNext();
	}
    else if( (pEvent->text() == "p") || (pEvent->key() == Qt::Key_Left) )
	{
		sltSelectPrev();
	}
	else if( (pEvent->key() == Qt::Key_Home) )
	{
		sltSelectItem( 0, 0 );
	}
	else if( (pEvent->key() == Qt::Key_End) )
	{
		sltSelectItem( m_aItemContainer.size()-1, 0 );
	}
	else if( (pEvent->key() == Qt::Key_Escape) )
	{
		sltSelectAllItems( false, true );
	}
	else if( (pEvent->key() == Qt::Key_F1) )
	{
		emit sigDialogHelp( "HItemView" );
	}
	else
	{
        QGraphicsView::keyPressEvent( pEvent );
	}
}

void HItemView::dragMoveEvent( QDragMoveEvent * /*pEvent*/ )
{
    // dummy method is needed so that drag and drop works !
}

void HItemView::dragEnterEvent( QDragEnterEvent * pEvent )
{
	if( m_pDiaPres->IsEdit() )
	{
		// allow only *.dia files and imager files to drop !

		QString sFileName;

		if( IsDiaDataFileDrag( pEvent, sFileName ) )
		{
			pEvent->accept();
		}
		else if( IsImageFileDrag( pEvent ) )
		{
            pEvent->accept();
		}
		else
		{
            if( pEvent->mimeData()->hasText() )
            {
                pEvent->accept();
            }
		}
    }
}

void HItemView::dropEvent( QDropEvent * pEvent )
{
	if( m_pDiaPres->IsEdit() )
	{
		QString sFileName;

		if( IsDiaDataFileDrag( pEvent, sFileName ) )
		{
			emit sigLoadDoc( sFileName, true );
		}
		else if( IsImageFileDrag( pEvent ) )
		{
			int iIndex = -1;

			QPoint aPoint = pEvent->pos();

            int xx = aPoint.x() + x();
            int yy = aPoint.y() + y();

			// find the index for the selected image ?
			for( int i=0; i<(int)m_aItemContainer.size(); i++ )
			{
				HItem * pItem = m_aItemContainer[ i ];
                if( pItem && pItem->IsPointInItem( xx, yy ) )
				{
					iIndex = i;
				}
			}

			// insert all droped image files at the found position 
            QList<QUrl> aLst = pEvent->mimeData()->urls();
			for( int i=0; i<(int)aLst.count(); i++ )
			{
                sFileName = aLst.at( i ).toLocalFile();
                AddItemAt( minHandle<DiaInfo>( new DiaInfo( GetNextFreeID(), sFileName.toStdString() ) ), iIndex+i, /*bInsert*/true );
			}

			sltSelectItem( iIndex, 0 );
		}
		else if( pEvent->mimeData()->hasText() ) 
		{
			int iIndex = -1;

			QPoint aPoint = pEvent->pos();

            int xx = aPoint.x() + x();
            int yy = aPoint.y() + y();

            sFileName = pEvent->mimeData()->text();
            
			// find the index for the selected image
			for( int i=0; i<(int)m_aItemContainer.size(); i++ )
			{
				HItem * pItem = m_aItemContainer[ i ];
                if( pItem && pItem->IsPointInItem( xx, yy ) )
				{
					iIndex = i;
				}
			}

			// min todo gulp: wenn iIndex == ActSelectedIndex --> ignorieren, nur wenn unterschiedlich draggen !

			//QDropEvent::Action aAction = pEvent->action();

			sltSelectItem( iIndex, 0 );
			SetFromClipboardData( sFileName );

			m_iDragTargetIndex = iIndex;

			sltSelectItem( iIndex, 0 );

            pEvent->accept();
		}
	}
}
	
void HItemView::sltDeleteAllSlectedItems()
{
	// ** restore the visible viewport after the operation
	_RestoreContents aContents( this );

	int iLastSelected = -1;
	for( int i=m_aItemContainer.size()-1; i>=0; i-- )
	{
		HItem * pItem = m_aItemContainer[ i ];
		if( pItem && pItem->GetSelected() )
		{
			// ** this is an user-event, data-object has to be deleted
			RemoveItemAt( i, true, /*bUpdateView*/false );
			iLastSelected = i;
		}
	}

	SyncViewWithData();

	emit sigViewDataChanged();

	// ** if items were selected, select the first item again and make it visible
	if( iLastSelected != -1 )
	{
		emit sigSelectItem( iLastSelected, 0 );

		// ** update the view
		sltUpdateSelected();
	}
}

void HItemView::sltSelectAllItems( bool bSelect, bool bUpdateView )
{
	for( int i=0; i<(int)m_aItemContainer.size(); i++ )
	{
		HItem * pItem = m_aItemContainer[ i ];
		if( pItem )
		{
			pItem->SetSelected( bSelect );
		}
	}

	// ** update the view (if needed)
	if( bUpdateView )
	{
		emit sigItemSelected( GetSelectedCount(), 0, -1, 0 );

		sltUpdateSelected();
	}
}

void HItemView::sltSelectNext()
{
	SelectItemDelta( 1 );
}

void HItemView::sltSelectPrev()
{
	SelectItemDelta( -1 );
}

void HItemView::sltSelectItem( int iNo, int iDissolveTimeInMS )
{
	SelectItem( iNo, iDissolveTimeInMS );
}

void HItemView::sltItemSelected( int /*iCount*/, int /*iFirstSelectedItemNo*/ )
{
	// not needed yet !
}

void HItemView::sltNewItemAfterSelected()
{
	// ** restore the visible viewport after the operation
	_RestoreContents aContents( this );

	int iIndex = GetLastSelectedItemIndex();
	
	AddItemAt( minHandle<DiaInfo>( new DiaInfo( GetNextFreeID() ) ), iIndex+1, /*bInsert*/true );

	// ** select the new created item 
    sltSelectItem( iIndex+1, 0 );
}

void HItemView::sltDeleteSelectedItem()
{
	// ** restore the visible viewport after the operation
	_RestoreContents aContents( this );

	int iIndex = GetLastSelectedItemIndex();
	
	RemoveItemAt( iIndex, true );

	SyncViewWithData();

	// ** select the new created item 
	if( iIndex>0 )
	{
		sltSelectItem( iIndex-1, 0 );
	}
}

bool HItemView::GetActClipboardData( QString & sDataOut, MyIndexContainer * pIndexContainer ) const
{
	QString s;

	if( pIndexContainer )
	{
		pIndexContainer->erase( pIndexContainer->begin(), pIndexContainer->end() );
	}

	for( int i=0; i<(int)m_aItemContainer.size(); i++ )
	{
		HItem * pItem = m_aItemContainer[ i ];
		if( pItem && pItem->GetSelected() )
		{
			if( pIndexContainer )
			{
				pIndexContainer->push_back( i );
			}
			s += pItem->GetData();
		}
	}

	if( !s.isNull() )
	{
		sDataOut = s;
		return true;
	}

	return false;
}

bool HItemView::GetActClipboardData( QString & sDataOut ) const
{
	return GetActClipboardData( sDataOut, 0 );
}

bool HItemView::SetFromClipboardData( const QString & sData )
{
	// ** restore the visible viewport after the operation
	_RestoreContents aContents( this );

	// ** insert data just before the last selected item

	// ** first: search for last selected item
	int i = 0;
	HItem * pSelectedItem = 0;
	for( i=m_aItemContainer.size()-1; i>=0; i-- )
	{
		HItem * pItem = m_aItemContainer[ i ];
		if( pItem && pItem->GetSelected() )
		{
			pSelectedItem = pItem;
			break;
		}
	}

	// ** if no item is selected, insert data at the end of the container
	if( i<0 )
	{
		i = m_aItemContainer.size();
	}

	if( !sData.isNull() )
	{
        string s = sData.toStdString();
		while( s.length()>0 )
		{
			// ** create a new dia item
			minHandle<DiaInfo> hDia( new DiaInfo(  GetNextFreeID() ) );

			if( hDia->SetFromData( s ) )
			{
				AddItemAt( hDia, i++, true );
			}
			else
			{
				// ** if an error occured, stop the loop
				s = "";
			}
		}
	}

	SyncViewWithData();

	emit sigViewDataChanged();

	// ** if a item was selected, select this item again and make it visible
	if( pSelectedItem != 0 )
	{
		emit sigSelectItem( i, 0 );

		// ** update the view
		sltUpdateSelected();
	}

	return true;
}

int HItemView::GetCountValidClipboardData( const QString & sData )
{
	int iCount = 0;
    string s = sData.toStdString();

	while( s.length()>0 )
	{
		DiaInfo aDia;

		if( aDia.SetFromData( s ) )
		{
			iCount++;
		}
		else
		{
			// ** if an error occured, stop the loop
			return iCount;
		}
	}

	return iCount;
}


void HItemView::SyncViewWithData()
{
    ResetView();
	
	// ** get new Dia-Object from the container
	if( m_pDiaPres )
	{
		for( int i=0; i<m_pDiaPres->GetDiaCount(); i++ )
		{
			minHandle<DiaInfo> hDia = m_pDiaPres->GetDiaAt( i );

			// ** Performace: do resize only for last item !
			AddItemAt( hDia, i, /*bInsert*/false, /*bUpdateView*/false, /*bDoResize*/(i==m_pDiaPres->GetDiaCount()-1) );
		}
	}

	sltUpdateView();
}

void HItemView::ResetView()
{
	m_aNextItemPos	= QPoint( 0, 0 );

	// ** reset scrollbars 
//    m_pCanvas->setSceneRect( 0, 0, m_aSizeHint.width(), m_aSizeHint.height() );

	DeleteAllHItems();
}

void HItemView::DeleteAllHItems()
{
	// ** important: delete only the View-objects, NOT the data-objects !!!
	for( int i=m_aItemContainer.size()-1; i>=0; i-- )
	{
		RemoveItemAt( i, /*bDeleteData*/false, /*bUpdateView*/false );
	}
}

int HItemView::GetSelectedCount() const
{
	int iSelectedCount = 0;

	for( int i=0; i<(int)m_aItemContainer.size(); i++ )
	{
		HItem * pItem = m_aItemContainer[ i ];
		if( pItem && pItem->GetSelected() )
		{
			iSelectedCount++;
		}
	}

	return iSelectedCount;
}

bool HItemView::IsValidIndex( int iIndex ) const
{
	if( (iIndex>=0) && (iIndex<(int)m_aItemContainer.size()) )
	{
		return true;
	}
	return false;
}


void HItemView::SelectItemDelta( int iDeltaIndex )
{
	if( GetSelectedCount()==1 )
	{
		// ** find the selected item...
		for( int i=0; i<(int)m_aItemContainer.size(); i++ )
		{
			HItem * pItem = m_aItemContainer[ i ];
			if( pItem && pItem->GetSelected() && IsValidIndex(i+iDeltaIndex) )
			{
				pItem->SetSelected( !pItem->GetSelected() );
				HItem * pItemNext = m_aItemContainer[ i+iDeltaIndex ];
				if( pItemNext )
				{
					pItemNext->SetSelected( !pItemNext->GetSelected() );
				}

				emit sigItemSelected( 1, pItemNext, i+iDeltaIndex, 0 );

				// ** shift scrollbar if needed
                QRectF aRect = pItemNext->rect();
                ensureVisible( aRect );

				break;
			}
		}

		// ** update the view
		sltUpdateSelected();
	}
	else
	{
		//beep
	}
}

bool HItemView::SelectItem( int iIndex, int iDissolveTimeInMS )
{
	if( (iIndex>=0) && (iIndex<(int)m_aItemContainer.size()) )
	{
		// ** select the the item with Index iIndex
		// ** and deselect all other items
		HItem * pItemSelected = 0;
		for( int i=0; i<(int)m_aItemContainer.size(); i++ )
		{
			HItem * pItem = m_aItemContainer[ i ];
			if( pItem )
			{
				if( i == iIndex )
				{
					pItem->SetSelected( true );
					pItemSelected = pItem;
				}
				else
				{
					pItem->SetSelected( false );
				}
			}
		}

		if( pItemSelected )
		{
			emit sigItemSelected( 1, pItemSelected, iIndex, iDissolveTimeInMS );
		}

		// ** shift scrollbar if needed
        QRectF aRect = pItemSelected->rect();
        ensureVisible( aRect );

		// ** update the view
		sltUpdateSelected();

		return true;
	}

	return false;
}

int HItemView::GetLastSelectedItemIndex() const
{
	for( int i=(int)m_aItemContainer.size()-1; i>=0; i-- )
	{
		HItem * pItem = m_aItemContainer[ i ];
		if( pItem )
		{
			if( pItem->GetSelected() )
			{
				return i;
			}
		}
	}

	// ** no item is selected, return last item-index
	return m_aItemContainer.size()-1;
}

bool HItemView::GetTwoSelectedItems( HItem * & pItem1, HItem * & pItem2 ) const
{
	pItem1 = 0;
	pItem2 = 0;

	for( int i=0; i<(int)m_aItemContainer.size(); i++ )
	{
		HItem * pItem = m_aItemContainer[ i ];
		if( pItem )
		{
			if( pItem->GetSelected() )
			{
				if( pItem1==0 )
				{
					pItem1 = pItem;
				}
				else if( pItem2==0 )
				{
					pItem2 = pItem;

					return true;
				}
			}
		}
	}

	return false;
}

void HItemView::ShiftViewItemsFromIndex( int iIndex )
{
    for( int i=iIndex; i<(int)m_aItemContainer.size(); i++ )
	{
		HItem * pItem = m_aItemContainer[ i ];
		if( pItem )
		{
            pItem->setRect( pItem->rect().x()+m_aItemShift.x(), pItem->rect().y()+m_aItemShift.y(), pItem->rect().width(), pItem->rect().height() );
		}
	}
}

QPoint HItemView::GetPosOfItemWithIndex( int iIndex )
{
	if( (iIndex>=0) && (iIndex<(int)m_aItemContainer.size()) )
	{
		HItem * pItem = m_aItemContainer[ iIndex ];
		if( pItem )
		{
            return QPoint( (int)pItem->rect().x(), (int)pItem->rect().y() );
	    }
	}
	return QPoint();
}

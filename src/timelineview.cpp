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

#include "timelineview.h"

#include "timelineitem.h"
#include "diapresentation.h"
#include "dyntextdlgimpl.h"

#include "misctools.h"

#include <qcursor.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qevent.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QToolTip>
#include <QMenu>
#include <QList>
#include <QUrl>
#include <QInputDialog>
#include <QGraphicsScene>

// *******************************************************************
// *******************************************************************
// *******************************************************************

const double g_dFactor = 10.0;		// one pixel is one tenth of a second !
const int g_iRampSize= 60;
const int g_iStartPosY = g_iRampSize + 20;
const int c_iDynOpOffset = 60;

TimeLineView::TimeLineView( QWidget * pParent, int iWidth, int iHeight, QWidget * pMainWin, QObject * pControler, DiaPresentation * pDoc )
    : QGraphicsView( 0, pParent )
{
    setAlignment(Qt::AlignLeft|Qt::AlignTop);
    setAcceptDrops(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	m_pContextMenu		= new QMenu( this );
    
    QAction * pAction = new QAction( tr( "Sound &data..." ), this );
	connect( pAction, SIGNAL( triggered() ), pMainWin, SLOT( sltShowSoundData() ) );
    m_pContextMenu->addAction( pAction );
    pAction = new QAction( tr( "Sound &comments..." ), this );
	connect( pAction, SIGNAL( triggered() ), pMainWin, SLOT( sltShowSoundComment() ) );
    m_pContextMenu->addAction( pAction );
    pAction = new QAction( tr( "&Plot comments..." ), this );
	connect( pAction, SIGNAL( triggered() ), pMainWin, SLOT( sltShowPlotComment() ) );
    m_pContextMenu->addAction( pAction );
    m_pContextMenu->addSeparator();
    pAction = new QAction( tr( "&Add dyn. text..." ), this );
    connect( pAction, SIGNAL(triggered()), this, SLOT(sltAddDynText()) );
    m_pContextMenu->addAction( pAction );
    pAction = new QAction( tr( "&Add attached dyn. text..." ), this );
    connect( pAction, SIGNAL(triggered()), this, SLOT(sltAddAttachedDynText()) );
    m_pContextMenu->addAction( pAction );
    m_pMenuDynTextEdit= new QAction( tr( "&Modify dyn. text..." ), this );
    connect( m_pMenuDynTextEdit, SIGNAL(triggered()), this, SLOT(sltEditDynText()) );
    m_pContextMenu->addAction( m_pMenuDynTextEdit );
    
    connect( this, SIGNAL(sigShowStatusMessage(QString)), pMainWin, SLOT(sltShowStatusBarMessage(QString)) );

    m_pCanvas			= new QGraphicsScene( 0, 0, iWidth, iHeight );
	//m_pCanvas->setDoubleBuffering( true );
    setScene( m_pCanvas );

	// set default value for size hint
	m_aSizeHint			= QSize( 600, 260 );

	m_pDiaPres			= pDoc;

	m_pParent			= pMainWin;

	m_iLastActPlayPos		= 0;
	m_iSelectedItemStartPos	= 0;
	m_iSelectedItemNo 		= -1;
	m_bDissolveSelected 	= false;
	m_bTotalTimeConstant 	= false;
	m_bMouseMovedWhilePressed = false;
	m_iSelectedDynTextIndex	= -1;

	setFocusPolicy( Qt::ClickFocus );
	viewport()->setMouseTracking( true );

	m_hTimeAxis = minHandle<TimeLineAxis>( new TimeLineAxis( m_pCanvas, g_dFactor, g_iStartPosY ) );

    m_hPlayMark = minHandle<QGraphicsLineItem>( new QGraphicsLineItem() );
    m_pCanvas->addItem(m_hPlayMark.GetPtr());
    m_hPlayMark->setPen( QColor( 255, 0, 0 ) );
    m_hPlayMark->setLine( 0, 0, 0, g_iStartPosY );
	m_hPlayMark->show();

	sltUpdateView();

   	connect( this, SIGNAL( sigViewDataChanged() ), pControler, SLOT( sltDataChanged() ) );
   	connect( this, SIGNAL( sigItemSelected(int,int) ), pControler, SLOT( sltSelectItem(int,int) ) );

	if( pMainWin )
	{
		connect( this, SIGNAL( sigLoadDoc(const QString &, bool) ), pMainWin, SLOT( sltLoadDoc(const QString &, bool) ) );
	}
}

TimeLineView::~TimeLineView()
{
	//let qt delete the object: delete m_pCanvas;

    delete m_pMenuDynTextEdit;
    
	delete m_pContextMenu;
}

QSize TimeLineView::sizeHint() const
{
	return m_aSizeHint;
}

void TimeLineView::setSizeHint( const QSize & aSize )
{
	m_aSizeHint = aSize;
}

void TimeLineView::SyncViewWithData()
{
	sltUpdateView();
}

void TimeLineView::sltAddDynText()
{
    DoAddDynText( false );
}

void TimeLineView::sltAddAttachedDynText()
{
    DoAddDynText( true );
}

void TimeLineView::DoAddDynText( bool bAsAttached )
{
    DynTextContainer & aDynGrOpContainer = m_pDiaPres->GetDynGraphicData();

    bool ok;
    QString sTxt = QInputDialog::getText(this,tr("Enter text"),tr("text:"),/*QLineEdit::EchoMode mode=*/QLineEdit::Normal,/*text=*/"",&ok,/*Qt::WindowFlags flags =*/0);
    if( ok && !sTxt.isEmpty() )
    {
        int x = m_aLastMousePos.x();
        double dStartTimeInMS = x*1000/g_dFactor-m_pDiaPres->GetOffsetForSound()*1000;

        // init default values for not attached dynamic text
        string sUUID;
        double dDiaStartTimeInMS = 0;
        if( bAsAttached )
        {
            int iIndex1, iIndex2, iFadeFactor;
            if( m_pDiaPres->GetIndexForTime( dStartTimeInMS, iIndex1, iIndex2, iFadeFactor ) )
            {
                if( iIndex1>=0 )
                {
                    sUUID = m_pDiaPres->GetDiaAt(iIndex1)->GetUUID();
                    dDiaStartTimeInMS = m_pDiaPres->GetDiaAbsStartDissolveTime(iIndex1)*1000.0;
                }
            }
        }

        aDynGrOpContainer.AddDefaultDynText( ToStdString(sTxt), dStartTimeInMS, 5000, sUUID, dDiaStartTimeInMS );

        emit sigViewDataChanged();
    }
}

void TimeLineView::sltEditDynText()
{
    int iSelectedDynTextIndex = m_pContextMenu->property("INDEX").toInt();
    ShowModifyDynObjectDialog( iSelectedDynTextIndex );
}

void TimeLineView::sltDoUpdateView( bool /*bErase*/ )
{
	if( m_pDiaPres )
	{
		double dTotalTime = m_pDiaPres->GetTotalTime();
		int iTotalLength = (int)(dTotalTime*g_dFactor);

		// ** update the size of the view **
		if( m_pCanvas->width()<iTotalLength )
		{
			// ** WARNING: the resize-method is expensive !!!
            m_pCanvas->setSceneRect( 0, 0, iTotalLength, m_pCanvas->height() );
		}

		// ** update the time axis **
		m_hTimeAxis->SetAxisLength( dTotalTime );

		// ** update the absolute time positions of the dias **
		m_aItemContainer.erase( m_aItemContainer.begin(), m_aItemContainer.end() );
		for( int i=0; i<m_pDiaPres->GetDiaCount(); i++ )
		{
			minHandle<TimeLineItem> hItem( new TimeLineItem( m_pCanvas, m_pDiaPres, i, g_dFactor, g_iRampSize, (m_iSelectedItemNo==i) ) );
			m_aItemContainer.push_back( hItem );

			if( hItem->IsSelected() )
			{
				// ** ensure that the selected item is visible
                ensureVisible( hItem->GetPositionX(), hItem->GetPositionY(), hItem->GetPositionX()+hItem->GetLength(), hItem->GetHeight() );
            }
		}

		// ** show infos about the music tracks **
		ShowMusicTracks();

		// ** and finally show the music and plot comments **
		ShowMusicComments();
		ShowPlotComments();
		// and the dynamic graphic operations
		ShowGraphicOperations();
	}

    update( x(), y(), /*contents*/width(), /*contents*/height()/*, bErase*/ );
}

void TimeLineView::sltUpdateView()
{
	sltDoUpdateView( /*bErase*/true );
}

void TimeLineView::sltUpdateSelected()
{
	// ** only selected item has changed, this means only repaint needed

	sltDoUpdateView( /*bErase*/false );
}

void TimeLineView::sltSelectItem( int iNo, int /*iDissolveTimeInMS*/ )
{
	// ** first: deselect all items
	int iCount = 0;
	MyItemContainer::iterator aIter = m_aItemContainer.begin();
	while( aIter != m_aItemContainer.end() )
	{
		minHandle<TimeLineItem> hItem = *aIter;

		if( iCount == iNo )
		{
			hItem->SetSelected( true );
		}
		else
		{
			hItem->SetSelected( false );
		}

		++iCount;
		++aIter;
	}

	m_iSelectedItemNo = iNo;

}

void TimeLineView::sltItemSelected( int iCount, int iFirstSelectedItemNo )
{
	if( iCount != 1 )
	{
		m_iSelectedItemNo = -1;
	}
	else
	{
		m_iSelectedItemNo = iFirstSelectedItemNo;
	}

	sltUpdateSelected();
}

void TimeLineView::SetPlayMark( double dActPlayTime )
{
    const int iDelta = 200;

	int iActPos = (int)(dActPlayTime*g_dFactor);

    m_hPlayMark->setPos( iActPos, 0 );

    // ** repaint only needed in play or pause modus
    if( iActPos >= 0 )
    {
        // Falls Probleme mit Repaint: ggf. komplette Timeline View aktualisieren...
        update( iActPos-iDelta, y(), 2*iDelta+1, /*contentsHeight()*/g_iStartPosY );
        // ** shift viewport, to ensure that the new item is visible
        //int iMargin = 50;
        //ensureVisible( iActPos+20, 0, width()-iMargin, height()-iMargin );
        ensureVisible( m_hPlayMark.GetPtr(), 0 );
    }

    // ** clear last play mark, after stop
    if( m_iLastActPlayPos-iActPos > 2 )
    {
        update( m_iLastActPlayPos-iDelta, y(), 2*iDelta+1, /*contents*/height() );
    }

	m_iLastActPlayPos = iActPos;
}

void TimeLineView::ShowPlotComments()
{
	const int c_iOffset = 30;
	const int c_iTextOffset = 5;

	m_aPlotCommentContainer.erase( m_aPlotCommentContainer.begin(), m_aPlotCommentContainer.end() );

	//QColor aColor( 255, 0, 255 );
	//QColor aColor( 110, 150, 50 );
	QColor aColor( 250, 140, 0 );

	const PlotCommentContainer & aCommentContainer = m_pDiaPres->GetPlotCommentData();
	int iSize = aCommentContainer.size();
	for( int i=0; i<iSize; i++ )
	{
		int iStartIndex = aCommentContainer[i]->GetStartIndex();
		int iStopIndex = aCommentContainer[i]->GetStopIndex();

		if( (m_pDiaPres->GetDiaCount()>=iStopIndex) && (m_pDiaPres->GetDiaCount()>=iStartIndex) )
		{
			int iPos = (int)(m_pDiaPres->GetDiaAbsStartDissolveTime( iStartIndex )*g_dFactor);
			// to position (exclusive) !
			int iPos2 = (int)(m_pDiaPres->GetDiaAbsStartDissolveTime( iStopIndex )*g_dFactor);

            QGraphicsLineItem * pLine1 = new QGraphicsLineItem();
            m_pCanvas->addItem(pLine1);
            pLine1->setLine( 0, 0, 0, 20 );
			//pLine1->setBrush( QBrush( aColor ) );
			pLine1->setPen( QPen( aColor ) );
            pLine1->setPos( iPos, g_iStartPosY+c_iOffset );
			pLine1->show();

            QGraphicsLineItem * pLine2 = new QGraphicsLineItem();
            m_pCanvas->addItem(pLine2);
            pLine2->setLine( 0, 0, 0, 20 );
			//pLine2->setBrush( QBrush( aColor ) );
            pLine2->setPen( QPen( aColor ) );
            pLine2->setPos( iPos2, g_iStartPosY+c_iOffset );
			pLine2->show();

            QGraphicsSimpleTextItem * pText = new QGraphicsSimpleTextItem();
            m_pCanvas->addItem(pText);
            QString sText = ToQString( aCommentContainer[i]->GetComment() );
			sText = "<-- " + sText + " -->";
			pText->setText( sText );
            pText->setBrush( aColor );
            pText->setPos( iPos, g_iStartPosY+c_iOffset+c_iTextOffset );
			pText->show();

			// ** shift the text into the middle of the two bars
            QRectF aRect = pText->boundingRect();
			if( aRect.width() < iPos2-iPos )
			{
                pText->setPos( iPos + ((iPos2-iPos-aRect.width())/2), g_iStartPosY+c_iOffset+c_iTextOffset );
			}

			m_aPlotCommentContainer.push_back( PlotCommentItem( 
                    pair< minHandle<QGraphicsLineItem>, minHandle<QGraphicsLineItem> >( minHandle<QGraphicsLineItem>( pLine1 ), minHandle<QGraphicsLineItem>( pLine2 ) ),
                    minHandle<QGraphicsSimpleTextItem>( pText ) ) );
		}
	}
}

void TimeLineView::ShowMusicComments()
{
	const int c_iOffset = 95;

	m_aMusicCommentContainer.erase( m_aMusicCommentContainer.begin(), m_aMusicCommentContainer.end() );

	QColor aColor( 60, 90, 160 );
	//QColor aColor( 255, 0, 0 );

	const SoundCommentContainer & aCommentContainer = m_pDiaPres->GetSoundCommentData();
	int iSize = aCommentContainer.size();
	int iOffset = (int)(m_pDiaPres->GetOffsetForSound()*g_dFactor);
	for( int i=0; i<iSize; i++ )
	{
		int iPos = (int)(aCommentContainer[i]->GetTimePos()*0.001*g_dFactor);
		int iLength = 10 * (i % 2);		// 3

		// ** connect the sound comments to the sound play time
		iPos += iOffset;

        QGraphicsLineItem * pLine = new QGraphicsLineItem();
        m_pCanvas->addItem(pLine);
        pLine->setLine( 0, 0, 0, 20+iLength );
		//pLine->setBrush( QBrush( aColor ) );
		pLine->setPen( QPen( aColor ) );
        pLine->setPos( iPos, g_iStartPosY+c_iOffset );
		pLine->show();

        QGraphicsSimpleTextItem * pText = new QGraphicsSimpleTextItem();
        m_pCanvas->addItem(pText);
        QString sText = ToQString( aCommentContainer[i]->GetComment() );
		pText->setText( sText );
        pText->setBrush( aColor );
        pText->setPos( iPos+2, g_iStartPosY+c_iOffset+10+iLength );
		pText->show(); 

        m_aMusicCommentContainer.push_back( CommentItem( CommentItemHelper( minHandle<QGraphicsLineItem>( pLine ), minHandle<QGraphicsSimpleTextItem>( pText ) ), ItemInfoHelper( sText, -1 ) ) );
	}
}

void TimeLineView::ShowMusicTracks()
{
	const int c_iTrackHeight = 20;	// 60
	const int c_iTrackOffset = 110;
	//const int c_iTextOffset  = c_iTrackHeight;

	m_aMusicContainer.erase( m_aMusicContainer.begin(), m_aMusicContainer.end() );
	m_aFadeContainer.erase( m_aFadeContainer.begin(), m_aFadeContainer.end() );

	QColor aColor1( 60, 90, 160 );
	//QColor aColor1( 150, 60, 180 );
	//QColor aColor1( 100, 65, 170 );
	QColor aColor2( 0, 0, 128 );

	int iOffset = (int)(m_pDiaPres->GetOffsetForSound()*g_dFactor);

	const SoundInfoContainer & aSoundContainer = m_pDiaPres->GetSoundInfoData();
	int iSize = aSoundContainer.size();
	for( int i=0; i<iSize; i++ )
	{
		int iStartPos = (int)(aSoundContainer.GetAbsPlayPos(i)*0.001*g_dFactor);
		int iDelta = (int)(aSoundContainer[i]->GetDelta()*0.001*g_dFactor);

        QGraphicsRectItem * pRect = new QGraphicsRectItem();
        m_pCanvas->addItem(pRect);
        pRect->setRect( 0, 0, iDelta, c_iTrackHeight );
		pRect->setBrush( QBrush( aColor1 ) );
        pRect->setPos( iOffset+iStartPos, g_iStartPosY+c_iTrackHeight+c_iTrackOffset /*80*/ );
		pRect->show();

        QGraphicsSimpleTextItem * pText = new QGraphicsSimpleTextItem();
        m_pCanvas->addItem(pText);
        QString sText = ToQString( aSoundContainer[i]->GetFileName() );
		pText->setText( sText );
        pText->setBrush( aColor2 );
        pText->setPos( iOffset+iStartPos, g_iStartPosY+c_iTrackHeight+c_iTrackOffset+c_iTrackHeight /*140*/ );
		pText->show();

		// ** show fade in / fade out line
		int iFadeInStart = (int)(aSoundContainer[i]->GetFadeInStartPos()*0.001*g_dFactor);
		int iFadeInLength = (int)(aSoundContainer[i]->GetFadeInLength()*0.001*g_dFactor);
		int iFadeOutStart = (int)(aSoundContainer[i]->GetFadeOutStartPos()*0.001*g_dFactor);
		int iFadeOutLength = (int)(aSoundContainer[i]->GetFadeOutLength()*0.001*g_dFactor);

        QGraphicsLineItem * pLineFadeIn = new QGraphicsLineItem();
        m_pCanvas->addItem(pLineFadeIn);
        pLineFadeIn->setLine( 0, c_iTrackHeight, iFadeInLength, 0 );
		//pLineFadeIn->setBrush( QBrush( aColor ) );
        pLineFadeIn->setPos( iOffset+iStartPos+iFadeInStart, g_iStartPosY+c_iTrackHeight+c_iTrackOffset );
		pLineFadeIn->show();
		pLineFadeIn->setSelected( true );
        //pLineFadeIn->setZ( 128 );

        QGraphicsLineItem * pLineFadeOut = new QGraphicsLineItem();
        m_pCanvas->addItem(pLineFadeOut);
        pLineFadeOut->setLine( 0, 0, iFadeOutLength, c_iTrackHeight );
		//pLineFadeOut->setBrush( QBrush( aColor ) );
        pLineFadeOut->setPos( iOffset+iStartPos+iFadeOutStart, g_iStartPosY+c_iTrackHeight+c_iTrackOffset );
		pLineFadeOut->show();
		pLineFadeOut->setSelected( true );
        //pLineFadeOut->setZ( 128 );

        m_aMusicContainer.push_back( MusicItem( minHandle<QGraphicsRectItem>( pRect ), minHandle<QGraphicsSimpleTextItem>( pText ) ) );
        m_aFadeContainer.push_back( FadeItem( minHandle<QGraphicsLineItem>( pLineFadeIn ), minHandle<QGraphicsLineItem>( pLineFadeOut ) ) );
	}
}

//static bool IsInGraphicOperationsArea( const QPoint & aPoint )
//{
//	if( (aPoint.y() > g_iStartPosY+c_iDynOpOffset) &&
//		(aPoint.y() < g_iStartPosY+c_iDynOpOffset+20) )
//	{
//		return true;
//	}
//	return false;
//}

void TimeLineView::ShowGraphicOperations()
{
	m_aDynGrapOpContainer.erase( m_aDynGrapOpContainer.begin(), m_aDynGrapOpContainer.end() );

	//QColor aColor( 128, 0, 0 );
	QColor aColor( 220, 0, 0 );
    QColor aConnectedColor( 0, 220, 0 );

	//const DynGraphicOpContainer & aDynGrOpContainer = m_pDiaPres->GetDynGraphicOpData();
	const DynTextContainer & aDynGrOpContainer = m_pDiaPres->GetDynGraphicData();
	int iSize = aDynGrOpContainer.size();
	int iOffset = (int)(m_pDiaPres->GetOffsetForSound()*g_dFactor);
	for( int i=0; i<iSize; i++ )
	{        
        double dStartTimeInMS, dShowTimeInMS;
        aDynGrOpContainer[i]->GetDefaultData( dStartTimeInMS, dShowTimeInMS );
        int iPos = (int)(dStartTimeInMS*0.001*g_dFactor);
        int iShowLength = (int)(dShowTimeInMS*0.001*g_dFactor);
		int iLength = 10 * (i % 2);

		// ** connect the sound comments to the sound play time
		iPos += iOffset;

        bool bIsAttached = aDynGrOpContainer[i]->IsAttachedToSlide();

        QGraphicsLineItem * pLine = new QGraphicsLineItem();
        m_pCanvas->addItem(pLine);
        pLine->setLine( 0, 0, 0, 20+iLength );
		//pLine->setBrush( QBrush( aColor ) );
        pLine->setPen( QPen( bIsAttached ? aConnectedColor : aColor ) );
        pLine->setPos( iPos, g_iStartPosY+c_iDynOpOffset );
		pLine->show();

        // Length of Text showing
        QGraphicsLineItem * pLineLength = new QGraphicsLineItem();
        m_pCanvas->addItem(pLineLength);
        pLineLength->setLine( 0, 20+iLength, iShowLength, 20+iLength );
        //pLine->setBrush( QBrush( aColor ) );
        pLineLength->setPen( QPen( bIsAttached ? aConnectedColor : aColor ) );
        pLineLength->setPos( iPos, g_iStartPosY+c_iDynOpOffset );
        pLineLength->show();

        QGraphicsSimpleTextItem * pText = new QGraphicsSimpleTextItem();
        m_pCanvas->addItem(pText);
        QString sText = ToQString( aDynGrOpContainer[i]->GetString() );
		QString sAddText;
		if( sText.length()>4 )
		{
			sAddText = "...";
		}
		pText->setText( sText.left( 4 )+sAddText );
        pText->setBrush( bIsAttached ? aConnectedColor : aColor );
        pText->setPos( iPos+2, g_iStartPosY+c_iDynOpOffset+10+iLength );
		pText->show(); 

        m_aDynGrapOpContainer.push_back( TextCommentItem( TextCommentItemHelper( DoubleLineItem( minHandle<QGraphicsLineItem>( pLine ), minHandle<QGraphicsLineItem>( pLineLength ) ), minHandle<QGraphicsSimpleTextItem>( pText ) ), ItemInfoHelper( sText, i ) ) );
	}
}

bool TimeLineView::IsDynTextSelected( const QPoint & aPoint, QString * psText, int * piIndex )
{
    TextCommentItemContainer::const_iterator aIter = m_aDynGrapOpContainer.begin();

	while( aIter != m_aDynGrapOpContainer.end() )
	{
        TextCommentItem aItem = *aIter;

		int x = aPoint.x();
		int y = aPoint.y();

		if( (x > aItem.first.second->x()) && (x < aItem.first.second->x()+30 ) &&
			(y > aItem.first.second->y()) && (y < aItem.first.second->y()+30 ) )
		{
			if( psText )
			{
				*psText = aItem.second.first;
			}
			if( piIndex )
			{
				*piIndex = aItem.second.second;
			}

            return true;
		}

		++aIter;
	}

    return false;
}

void TimeLineView::mousePressEvent( QMouseEvent * pEvent )
{
    QPointF pos = mapToScene(pEvent->x(),pEvent->y());
    int x = (int)pos.x();
    int y = (int)pos.y();
    QPoint point(x,y);

    if( (pEvent->button() == Qt::LeftButton) )
	{
        // ** allow modifiying of items only in edit-modus
		if( m_pDiaPres->IsEdit() )
		{
            // ** reset mousemove flag
			m_bMouseMovedWhilePressed = false;

			int iCount = 0;
			MyItemContainer::const_iterator aIter = m_aItemContainer.begin();

			while( aIter != m_aItemContainer.end() )
			{
				minHandle<TimeLineItem> hItem = *aIter;

				// ** change size of item ?
                if( hItem->IsStopBorderSelected( x, y ) )
				{
					// ** yes, change show time of selected dia
                    m_iSelectedItemStartPos = x;
					m_hSelectedItem = hItem;
					m_iSelectedItemNo = iCount;
					m_bDissolveSelected = false;
					m_bTotalTimeConstant = false;

					setCursor( Qt::SizeHorCursor );

					return;
				}
                else if( hItem->IsDissolveBorderSelected( x, y ) )
				{
					// ** yes, change dissolve time of selected dia

					// ** should total time hold constant (shift button pressed)
                    if( (pEvent->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier )
					{
						// ** no
						m_bTotalTimeConstant = false;
					}
					else
					{
						// ** yes
						m_bTotalTimeConstant = true;
					}

                    m_iSelectedItemStartPos = x;
					m_hSelectedItem = hItem;
					m_iSelectedItemNo = iCount;
					m_bDissolveSelected = true;

					setCursor( Qt::SizeHorCursor );

					return;
				}
                else if( hItem->IsSelected( x, y ) )
				{
					// ** no, select only item

					m_iSelectedItemNo = iCount;

					// ** emit the signal for selected item when mouse button is released

					return;
				}

				++iCount;
				++aIter;
			}

		}

		m_iSelectedItemStartPos = 0;
		m_hSelectedItem = minHandle<TimeLineItem>();

		// check for touched dynamic text objects
		int iIndexOut = 0;
		QString sText;
        bool bIsDynTextSelected = IsDynTextSelected( point, &sText, &iIndexOut );
        if( bIsDynTextSelected )
		{
            if( (pEvent->modifiers() & Qt::AltModifier) == Qt::AltModifier )
			{
				ShowModifyDynObjectDialog( iIndexOut );
			}
			else
			{
				m_iSelectedDynTextIndex = iIndexOut;
                m_iSelectedItemStartPos = x;

				setCursor( Qt::SizeHorCursor );
			}
		}
	}
	else if( (pEvent->button() == Qt::RightButton) )
	{
		// ** handle popup menu...
        m_aLastMousePos = point;

		// if mouse was clicked on a dynamic text,
		// enable the item to allow the modification of the dynamic text
        int iSelectedDynTextIndex = m_iSelectedDynTextIndex;
        bool bIsDynTextSelected = IsDynTextSelected( point, 0, &iSelectedDynTextIndex );
        m_pMenuDynTextEdit->setEnabled( bIsDynTextSelected );
				
        m_pContextMenu->setProperty("INDEX",iSelectedDynTextIndex);
		m_pContextMenu->exec( pEvent->globalPos() );
	}
}

void TimeLineView::mouseReleaseEvent( QMouseEvent * /*pEvent*/ )
{
	// ** allow modifiying of items only in edit-modus
	if( m_pDiaPres->IsEdit() )
	{

		m_iSelectedItemStartPos = 0;
		m_hSelectedItem = minHandle<TimeLineItem>();
		m_iSelectedDynTextIndex = -1;

		setCursor( Qt::ArrowCursor );

		if( m_iSelectedItemNo >= 0 )
		{
			// ** signal the controler, that the selected item has to be changed
			emit sigItemSelected( m_iSelectedItemNo, 0 );
		}
		else
		{
			// ** update view **
			sltUpdateView();
		}

		// ** update document only if data was changed with mouse move...
		if( m_bMouseMovedWhilePressed )
		{
			emit sigViewDataChanged();

			m_bMouseMovedWhilePressed = false;
		}
	}
}

void TimeLineView::mouseDoubleClickEvent ( QMouseEvent * pEvent )
{
    QPointF pos = mapToScene(pEvent->x(),pEvent->y());
    int x = (int)pos.x();
    int y = (int)pos.y();
    QPoint point(x,y);

    int iSelectedDynTextIndex = 0;
    bool bIsDynTextSelected = IsDynTextSelected( point, 0, &iSelectedDynTextIndex );
    if( bIsDynTextSelected )
    {
        ShowModifyDynObjectDialog( iSelectedDynTextIndex );
    }
}

// convert double time value in seconds
// into a integer value of 1/10 of a second
inline int SecTo10thSec(double dValue)
{
    return (int)(dValue*10.0);
}

void TimeLineView::mouseMoveEvent( QMouseEvent * pEvent )
{
	// ** allow modifiying of items only in edit-modus
	if( m_pDiaPres->IsEdit() )
	{
        QPointF pos = mapToScene(pEvent->x(),pEvent->y());
        int x = (int)pos.x();
        int y = (int)pos.y();
        QPoint point(x,y);
        double dTime = (double)x*g_dFactor*0.01;    // in seconds

		if( m_hSelectedItem.IsOk() )
		{
            // handle movement of dissiolve and show time of slide
            double dDelta = (double)(x-m_iSelectedItemStartPos);
            m_iSelectedItemStartPos = x;

			dDelta = dDelta / g_dFactor;

			if( m_bDissolveSelected )
			{
				m_hSelectedItem->ChangeDissolveTime( dDelta );

				if( m_bTotalTimeConstant )
				{
					// ** if dissolve time is increased,
					// ** the show time has to be decreased
                    m_hSelectedItem->ChangeShowTime( -dDelta );
				}
			}
			else
			{
				m_hSelectedItem->ChangeShowTime( dDelta );
			}

			// ** data was changed with mouse-move
			m_bMouseMovedWhilePressed = true;

			// ** update view **
			sltUpdateView();

			//emit sigViewDataChanged();
		}
		// handle the movement of dynamic text objects
		else if( m_iSelectedDynTextIndex>=0 )
		{
            bool bIsMoveAllowed = true;
            double dDelta = (double)(x-m_iSelectedItemStartPos);
            m_iSelectedItemStartPos = x;

			dDelta = dDelta / g_dFactor;

			DynTextContainer & aDynGrOpContainer = m_pDiaPres->GetDynGraphicData();

			minHandle<DynText> hItem = aDynGrOpContainer[ m_iSelectedDynTextIndex ];

            // if we have an attached dia, then check if we do not move out of the vaild range
            if( hItem->IsAttachedToSlide() )
            {
                int iIndex = m_pDiaPres->GetDiaIndexForUUID( hItem->GetAttachedSlideUUID() );
                if( iIndex>=0 )
                {
                    // do we reach the limits of the attached dia times ?
                    double dTextStartTime = hItem->GetStartTimeInMS()*0.001;    // ms --> sec
                    double dStart = m_pDiaPres->GetDiaAbsStartDissolveTime( iIndex );
                    double dFinish = m_pDiaPres->GetDiaAbsFinishDissolveTime( iIndex );

                    bIsMoveAllowed = SecTo10thSec(dTextStartTime+dDelta)>SecTo10thSec(dStart) && SecTo10thSec(dTextStartTime+dDelta)<SecTo10thSec(dFinish);
                }
            }

            if( bIsMoveAllowed )
            {
                hItem->Delta( dDelta*1000 );
                aDynGrOpContainer.SetChanged();
            }

			// ** data was changed with mouse-move
			m_bMouseMovedWhilePressed = true;

			// select the slide for this text item
            int iIndex = GetItemForPosX( x );
			if( iIndex>=0 )
			{
				sltSelectItem( iIndex, 0 );
			}

			// ** update view **
			sltUpdateView();
		}
		else
		{
			// ** update mouse cursor if one can shift dissolve or show time with mouse
            bool bCursorSet = false;
			MyItemContainer::const_iterator aIter = m_aItemContainer.begin();

			while( aIter != m_aItemContainer.end() )
			{
				minHandle<TimeLineItem> hItem = *aIter;

                if( hItem->IsStopBorderSelected( x, y ) )
				{
					setCursor( Qt::SizeHorCursor );
                    bCursorSet = true;
                    emit sigShowStatusMessage( tr("press mouse button and move to change show time of slide") );
                    break;
				}
                else if( hItem->IsDissolveBorderSelected( x, y ) )
				{
					setCursor( Qt::SizeHorCursor );
                    bCursorSet = true;
                    emit sigShowStatusMessage( tr("press mouse button and move to change dissolve time of slide") );
                    break;
				}

				++aIter;
			}

            if( !bCursorSet )
            {
                // check for touched dynamic text objects
                QString sText;
                bool bIsDynTextSelected = IsDynTextSelected( point, &sText );
                if( bIsDynTextSelected )
                {
                    setCursor( Qt::SizeHorCursor );
                    bCursorSet = true;
                    emit sigShowStatusMessage( tr("double mose click or press ALT and left click with mouse to modify item") );

                    // show the full text of the dynamic text as tooltip
                    QToolTip::showText( pEvent->globalPos(), sText, 0 );
                }

                if( !bCursorSet )
                {
                    setCursor( Qt::ArrowCursor );
                    emit sigShowStatusMessage( "" );
                }
            }

            // update the play info dialog with image of current play position moved by mouse cursor
            bool bShiftPressed = ((pEvent->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier);
            if( bShiftPressed )
            {
                SetPlayMark(dTime);
                emit sigPlayMarkChanged(dTime);
            }
		}
	}
}

void TimeLineView::dragEnterEvent( QDragEnterEvent * pEvent )
{
	if( m_pDiaPres->IsEdit() )
	{
		QString sFileName;

		if( IsDiaDataFileDrag( pEvent, sFileName ) )
		{
			pEvent->accept();
		}
        else if( IsSoundFileDrag( pEvent ) )
		{
			pEvent->accept();
		}
	}
}

void TimeLineView::dragMoveEvent ( QDragMoveEvent * pEvent )
{
	if( m_pDiaPres->IsEdit() )
	{
		QString sFileName;

		if( IsDiaDataFileDrag( pEvent, sFileName ) )
		{
			pEvent->accept();
		}
		else if( IsSoundFileDrag( pEvent ) )
		{
			pEvent->accept();
		}
		else
		{
			// don't accept text for graphic operations --> not implemented yet !

			//bool bAcceptText = IsInGraphicOperationsArea( pEvent->pos() );

			//pEvent->accept( (bAcceptText && QTextDrag::canDecode(pEvent) ) );
			////pEvent->setAction( QDropEvent::Copy );
		}
	}
}

void TimeLineView::dropEvent( QDropEvent * pEvent )
{
	if( m_pDiaPres->IsEdit() )
	{
		QString sFileName;

		if( IsDiaDataFileDrag( pEvent, sFileName ) )
		{
			emit sigLoadDoc( sFileName, true );
		}
		else if( IsSoundFileDrag( pEvent ) )
		{
            //SoundInfoContainer & aSoundContainer = m_pDiaPres->GetSoundInfoData();

			if( pEvent->mimeData()->hasUrls() )
			{
                QList<QUrl> aLst = pEvent->mimeData()->urls();
				for( int i=0; i<(int)aLst.count(); i++ )
				{
                    // get length of sound file asynchroniously !
                    // this is needed because otherwise the qt phonon framework does not deliver the correct length (unix)
                    QEvent * pEvent = new GetSoundLengthEvent(aLst.at(i).toLocalFile(),this);
                    QApplication::postEvent(m_pParent,pEvent);
                    // processing will be continued in customEvent() handling below
				}
			}            
		}
	}
}

void TimeLineView::customEvent(QEvent * pEvent)
{
    if( pEvent->type()==_USER_EVENT_GET_SOUND_LENGTH )
    {
        SoundInfoContainer & aSoundContainer = m_pDiaPres->GetSoundInfoData();
        GetSoundLengthEvent * pSoundEvent = (GetSoundLengthEvent *)pEvent;
        aSoundContainer.push_back( minHandle<SoundInfo>( new SoundInfo( ToStdString(pSoundEvent->GetFileName()), pSoundEvent->GetSoundLength() ) ) );
        aSoundContainer.SetChanged();
        //aSoundContainer.UpdateAllLengths();
        emit sigViewDataChanged();
    }
}

void TimeLineView::ShowModifyDynObjectDialog( int iIndexOut )
{
    // ** change data of the dynamic text with a dialog **
	DynTextContainer & aDynGrOpContainer = m_pDiaPres->GetDynGraphicData();

    if( iIndexOut>=0 && iIndexOut<(int)aDynGrOpContainer.size() )
    {        
    	minHandle<DynText> hItem = aDynGrOpContainer[ iIndexOut ];
    
        string sUUID1,sUUID2;
        double dStart, dDelta;
        hItem->GetDefaultData( dStart, dDelta );

        // get image index for time of dynamic text
        int iIndex1, iIndex2, iFadeFactor;
        if( m_pDiaPres->GetIndexForTime( dStart, iIndex1, iIndex2, iFadeFactor ) )
        {
            if( iIndex1>=0 )
            {
                sUUID1 = m_pDiaPres->GetDiaAt(iIndex1)->GetUUID();
            }
            if( iIndex2>=0 )
            {
                sUUID2 = m_pDiaPres->GetDiaAt(iIndex2)->GetUUID();
            }
        }

        DynamicTextDlgImpl aDlg( hItem, iIndex1>=0 ? iIndex1+1 : iIndex1, ToQString(sUUID1), iIndex2>=0 ? iIndex2+1 : iIndex2, ToQString(sUUID2), this, m_pParent );
        aDlg.setModal(true);
    
    	int iRet = aDlg.exec();
    
    	if( iRet == 2 )
    	{
    		// delete the selected item !
    		aDynGrOpContainer.erase( aDynGrOpContainer.begin()+iIndexOut );
    
    		aDynGrOpContainer.SetChanged();
    
    		emit sigViewDataChanged();
    	}
    	else if( iRet == 1 )
    	{
    		hItem->setText( aDlg.m_pText->text() );
    		hItem->setFont( aDlg.GetFont() );
    		if( aDlg.m_pRelPos->isChecked() )
    		{
    			hItem->SetRelativePos( aDlg.GetRelX(), aDlg.GetRelY() );
    		}
    		else
    		{
                hItem->setPos( aDlg.m_pPosX->text().toInt(),aDlg.m_pPosY->text().toInt() );
    			hItem->SetRelativePos( -1.0, -1.0 );
    		}
            QColor aColor = aDlg.m_pSelectFontcolor->palette().brush(QPalette::ButtonText).color();
            hItem->setBrush( aColor );
            hItem->font().setPointSize(aDlg.m_pFontSize->text().toInt());
            hItem->font().setFamily(aDlg.m_pFontName->text());

    		dStart = aDlg.m_pShowAtTime->text().toDouble();
    		dDelta = aDlg.m_pShowTime->text().toDouble();
    
    		hItem->ChangeDefaultData( dStart, dDelta );
    
    		if( aDlg.m_pAttributesForAll->isChecked() )
    		{
    			aDynGrOpContainer.SetAttributesForAllItems( hItem );
    		}

            if( aDlg.m_pChbAttachToImage1->isChecked() )
            {
                double dRelTime = hItem->GetStartTimeInMS() - m_pDiaPres->GetDiaAbsStartDissolveTime(iIndex1)*1000.0;
                hItem->SetAttachedSlideUUID( ToStdString(aDlg.m_pAttachedImageUUID1->text()), dRelTime );
            }
            else if( aDlg.m_pChbAttachToImage2->isChecked() )
            {
                double dRelTime = hItem->GetStartTimeInMS() - m_pDiaPres->GetDiaAbsStartDissolveTime(iIndex2);
                hItem->SetAttachedSlideUUID( ToStdString(aDlg.m_pAttachedImageUUID2->text()), dRelTime );
            }
            else
            {
                hItem->SetAttachedSlideUUID( "", 0.0 );
            }

    		aDynGrOpContainer.SetChanged();
    
    		emit sigViewDataChanged();
    	}
    }
}

int TimeLineView::GetItemForPosX( int x )
{
	int iCount = 0;
	MyItemContainer::const_iterator aIter = m_aItemContainer.begin();

	while( aIter != m_aItemContainer.end() )
	{
		minHandle<TimeLineItem> hItem = *aIter;

		if( hItem->IsSelected( x, hItem->GetPositionY() ) )
		{
			return iCount;
		}		

		++iCount;
		++aIter;
	}
	return -1;
}

// **************************************************************************

GetSoundLengthEvent::GetSoundLengthEvent( const QString & sFileName, QWidget * pRequester )
: QEvent( (Type) (_USER_EVENT_GET_SOUND_LENGTH) )
{
    m_sFileName = sFileName;
    m_pRequester = pRequester;
}

QString GetSoundLengthEvent::GetFileName() const
{
    return m_sFileName;
}

int GetSoundLengthEvent::GetSoundLength() const
{
    return m_iSoundLength;
}

void GetSoundLengthEvent::SetSoundLength( int val )
{
    m_iSoundLength = val;
}

QWidget * GetSoundLengthEvent::GetRequester() const
{
    return m_pRequester;
}

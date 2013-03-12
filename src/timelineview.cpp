/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/timelineview.cpp,v $
 *
 *  $Revision: 1.3 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.2  2004/01/18 23:36:10  min
 *	Bugfixes and dynamic text handling improved (new menu item).
 *	
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
/* TODO Qt4
class MyDynamicToolTip : public QToolTip
{
public:
    MyDynamicToolTip( QWidget * pParent );

protected:
    virtual void maybeTip( const QPoint & aPoint );
};


MyDynamicToolTip::MyDynamicToolTip( QWidget * pParent )
: QToolTip( pParent )
{
}

void MyDynamicToolTip::maybeTip( const QPoint &pos )
{
    if( !parentWidget()->inherits( "TimeLineView" ) )
	{
		return;
	}

	QString sText;
    QRect aRect( ((TimeLineView*)parentWidget())->GetTipRect( pos, &sText ) );
    if( !aRect.isValid() )
	{
		return;
	}

    //QString s;
    //s.sprintf( "position: %d,%d", r.center().x(), r.center().y() );
    tip( aRect, sText );
}
*/
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
//TODO porting	setDragAutoScroll( TRUE );
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
    m_pMenuDynTextEdit= new QAction( tr( "&Modify dyn. text..." ), this );
    connect( m_pMenuDynTextEdit, SIGNAL(triggered()), this, SLOT(sltEditDynText()) );
    m_pContextMenu->addAction( m_pMenuDynTextEdit );
    
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
//   		connect( this, SIGNAL( sigModifySoundData() ), pMainWin, SLOT( sltShowSoundData() ) );
//   		connect( this, SIGNAL( sigModifySoundComment() ), pMainWin, SLOT( sltShowSoundComment() ) );
//   		connect( this, SIGNAL( sigModifyPlotComment() ), pMainWin, SLOT( sltShowPlotComment() ) );
		connect( this, SIGNAL( sigLoadDoc(const QString &, bool) ), pMainWin, SLOT( sltLoadDoc(const QString &, bool) ) );
	}

//TODO Qt4	m_pToolTip = new MyDynamicToolTip( this );
    m_pToolTip = 0;
}

TimeLineView::~TimeLineView()
{
	//let qt delete the object: delete m_pCanvas;

//TODO Qt4	delete m_pToolTip;

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
	DynContainer & aDynGrOpContainer = m_pDiaPres->GetDynGraphicData();

    bool ok;
    QString sTxt = QInputDialog::getText(this,tr("Enter text"),tr("text:"),/*QLineEdit::EchoMode mode=*/QLineEdit::Normal,/*text=*/"",&ok,/*Qt::WindowFlags flags =*/0);
    if( ok && !sTxt.isEmpty() )    
	{
		int x = m_aLastMousePos.x();	

        aDynGrOpContainer.AddDefaultDynText( sTxt.toStdString(), x*1000/g_dFactor-m_pDiaPres->GetOffsetForSound()*1000, 5000 );

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
                ensureVisible( hItem->GetPositionX(), hItem->GetPositionY(), m_pCanvas->width(), m_pCanvas->height() );
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
// TODO --> ist dies noch notwendig ?
//#ifndef __linux__
	const int iDelta = 4;
//#endif

	int iActPos = (int)(dActPlayTime*g_dFactor);

    m_hPlayMark->setPos( iActPos, 0 );

	// ** repaint only needet in play or pause modus
	if( iActPos >= 0 )
	{
// TODO --> ist dies noch notwendig ?
//#ifdef __linux__
//		repaintContents( contentsX(), contentsY(), /*contents*/visibleWidth(), g_iStartPosY );
//#else
        update( iActPos-iDelta, y(), 2*iDelta+1, /*contentsHeight()*/g_iStartPosY );
//#endif
		// ** shift viewport, to ensure that the new item is visible
        ensureVisible( iActPos+20, 0, width(), height() );
	}

	// ** clear last play mark, after stop
	if( m_iLastActPlayPos-iActPos > 2 )
	{
//#ifdef __linux__
//		repaintContents( contentsX(), contentsY(), /*contents*/visibleWidth(), /*contents*/visibleHeight() );
//#else
        update( m_iLastActPlayPos-iDelta, y(), 2*iDelta+1, /*contents*/height() );
//#endif
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
			QString sText = aCommentContainer[i]->GetComment().c_str();
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
		QString sText = aCommentContainer[i]->GetComment().c_str();
		pText->setText( sText );
        pText->setBrush( aColor );
        pText->setPos( iPos+2, g_iStartPosY+c_iOffset+10+iLength );
		pText->show(); 

        m_aMusicCommentContainer.push_back( MusicCommentItem( MusicCommentItemHelper( minHandle<QGraphicsLineItem>( pLine ), minHandle<QGraphicsSimpleTextItem>( pText ) ), ItemInfoHelper( sText, -1 ) ) );
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
        QString sText = aSoundContainer[i]->GetFileName().c_str();
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
	//const DynGraphicOpContainer & aDynGrOpContainer = m_pDiaPres->GetDynGraphicOpData();
	const DynContainer & aDynGrOpContainer = m_pDiaPres->GetDynGraphicData();
	int iSize = aDynGrOpContainer.size();
	int iOffset = (int)(m_pDiaPres->GetOffsetForSound()*g_dFactor);
	for( int i=0; i<iSize; i++ )
	{
		int iPos = (int)(aDynGrOpContainer[i]->GetStartTime()*0.001*g_dFactor);
		int iLength = 10 * (i % 2);

		// ** connect the sound comments to the sound play time
		iPos += iOffset;

        QGraphicsLineItem * pLine = new QGraphicsLineItem();
        m_pCanvas->addItem(pLine);
        pLine->setLine( 0, 0, 0, 20+iLength );
		//pLine->setBrush( QBrush( aColor ) );
		pLine->setPen( QPen( aColor ) );
        pLine->setPos( iPos, g_iStartPosY+c_iDynOpOffset );
		pLine->show();

        QGraphicsSimpleTextItem * pText = new QGraphicsSimpleTextItem();
        m_pCanvas->addItem(pText);
		QString sText = aDynGrOpContainer[i]->GetString().c_str();
		QString sAddText;
		if( sText.length()>4 )
		{
			sAddText = "...";
		}
		pText->setText( sText.left( 4 )+sAddText );
        pText->setBrush( aColor );
        pText->setPos( iPos+2, g_iStartPosY+c_iDynOpOffset+10+iLength );
		pText->show(); 

        m_aDynGrapOpContainer.push_back( MusicCommentItem( MusicCommentItemHelper( minHandle<QGraphicsLineItem>( pLine ), minHandle<QGraphicsSimpleTextItem>( pText ) ), ItemInfoHelper( sText, i ) ) );
	}
}

QRect TimeLineView::GetTipRect( const QPoint & aPoint, QString * psText, int * piIndex )
{
	MusicCommentItemContainer::const_iterator aIter = m_aDynGrapOpContainer.begin();

	while( aIter != m_aDynGrapOpContainer.end() )
	{
		MusicCommentItem aItem = *aIter;

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

			return QRect( aPoint.x(), aPoint.y(), 20, 30 );
		}

		++aIter;
	}

	return QRect( 0, 0, -1, -1 );
}

void TimeLineView::mousePressEvent( QMouseEvent * pEvent )
{
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
				if( hItem->IsStopBorderSelected( pEvent->x(), pEvent->y() ) )
				{
					// ** yes, change show time of selected dia
					m_iSelectedItemStartPos = pEvent->x();
					m_hSelectedItem = hItem;
					m_iSelectedItemNo = iCount;
					m_bDissolveSelected = false;
					m_bTotalTimeConstant = false;

					setCursor( Qt::SizeHorCursor );

					return;
				}
				else if( hItem->IsDissolveBorderSelected( pEvent->x(), pEvent->y() ) )
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

					m_iSelectedItemStartPos = pEvent->x();
					m_hSelectedItem = hItem;
					m_iSelectedItemNo = iCount;
					m_bDissolveSelected = true;

					setCursor( Qt::SizeHorCursor );

					return;
				}
				else if( hItem->IsSelected( pEvent->x(), pEvent->y() ) )
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
		QRect aRect = GetTipRect( pEvent->pos(), &sText, &iIndexOut );
	    if( aRect.isValid() )
		{
            if( (pEvent->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier )
			{
				ShowModifyDynObjectDialog( iIndexOut );
			}
			else
			{
				m_iSelectedDynTextIndex = iIndexOut;
				m_iSelectedItemStartPos = pEvent->x();

				setCursor( Qt::SizeHorCursor );
			}
		}
	}
	else if( (pEvent->button() == Qt::RightButton) )
	{
		// ** handle popup menu...
		m_aLastMousePos = pEvent->pos();

		// if mouse was clicked on a dynamic text,
		// enable the item to allow the modification of the dynamic text
        int iSelectedDynTextIndex = m_iSelectedDynTextIndex;
        QRect aRect = GetTipRect( pEvent->pos(), 0, &iSelectedDynTextIndex );
        m_pMenuDynTextEdit->setEnabled( aRect.isValid() );
				
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

void TimeLineView::mouseMoveEvent( QMouseEvent * pEvent )
{
	// ** allow modifiying of items only in edit-modus
	if( m_pDiaPres->IsEdit() )
	{
		if( m_hSelectedItem.IsOk() )
		{
			double dDelta = (double)(pEvent->x()-m_iSelectedItemStartPos);
			m_iSelectedItemStartPos = pEvent->x();

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
			double dDelta = (double)(pEvent->x()-m_iSelectedItemStartPos);
			m_iSelectedItemStartPos = pEvent->x();

			dDelta = dDelta / g_dFactor;

			DynContainer & aDynGrOpContainer = m_pDiaPres->GetDynGraphicData();

			minHandle<DynText> hItem = aDynGrOpContainer[ m_iSelectedDynTextIndex ];
			hItem->Delta( dDelta*1000 );

			aDynGrOpContainer.SetChanged();

			// ** data was changed with mouse-move
			m_bMouseMovedWhilePressed = true;

			// select the slide for this text item
			int iIndex = GetItemForPosX( pEvent->x() );
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
			
			MyItemContainer::const_iterator aIter = m_aItemContainer.begin();

			while( aIter != m_aItemContainer.end() )
			{
				minHandle<TimeLineItem> hItem = *aIter;

				if( hItem->IsStopBorderSelected( pEvent->x(), pEvent->y() ) )
				{
					setCursor( Qt::SizeHorCursor );

					return;
				}
				else if( hItem->IsDissolveBorderSelected( pEvent->x(), pEvent->y() ) )
				{
					setCursor( Qt::SizeHorCursor );

					return;
				}

				++aIter;
			}

			// check for touched dynamic text objects
			QRect aRect = GetTipRect( pEvent->pos() );
			if( aRect.isValid() )
			{
				setCursor( Qt::SizeHorCursor );

				return;
			}

			setCursor( Qt::ArrowCursor );
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

		QPoint aPos = pEvent->pos();

		if( IsDiaDataFileDrag( pEvent, sFileName ) )
		{
			emit sigLoadDoc( sFileName, true );
		}
		else if( IsSoundFileDrag( pEvent ) )
		{
			SoundInfoContainer & aSoundContainer = m_pDiaPres->GetSoundInfoData();

			if( pEvent->mimeData()->hasUrls() )
			{
                QList<QUrl> aLst = pEvent->mimeData()->urls();
				for( int i=0; i<(int)aLst.count(); i++ )
				{
                    QEvent * pEvent = new GetSoundLengthEvent(aLst.at(i).toLocalFile(),this);
                    QApplication::postEvent(m_pParent,pEvent);
// TODO gulp --> hier die laenge der sound datei schon mitgeben... --> asynchrone behandlung ! ==> laenge der sound datei in datei speichern !
// erzeuge asynchron eine liste der SoundInfo() Objekte mit korrekten zeiten --> setze sound file am player und warte auf signal totalTimeChanged
                    //aSoundContainer.push_back( minHandle<SoundInfo>( new SoundInfo( (const char *)aLst.at(i).toLocalFile().toAscii(), /*TODO*/1000 ) ) );
                    //aSoundContainer.SetChanged();
				}
                //aSoundContainer.UpdateAllLengths();
                //emit sigViewDataChanged();
			}
            
//			const char * s;
//			QString sFileName;
//
//			if( Q3UriDrag::decode( pEvent, aStrList ) )
//			{
//				for( int i=0; i<(int)aStrList.count(); i++ )
//				{
//					const QString sTest = aStrList.at(i);
//					s = (const char *)sTest;
//					sFileName = Q3UriDrag::uriToLocalFile( s );
//					s = (const char *)sFileName;
//
//					aSoundContainer.push_back( minHandle<SoundInfo>( new SoundInfo( s ) ) );
//					aSoundContainer.SetChanged();
//				}
//				emit sigViewDataChanged();
//			}
		}
	/*	else if( QUriDrag::decode( pEvent, aStrList ) )
		{
			const char * s;
			QString sFileName;

			for( int i=0; i<aStrList.count(); i++ )
			{
				const QString sTest = aStrList.at(i);
				s = (const char *)sTest;
				sFileName = QUriDrag::uriToLocalFile( s );
				s = (const char *)sFileName;

				if( IsDiaDataFile( s ) )
				{
					emit sigLoadDoc( sFileName, true );
				}
			}

			// min todo --> ggf. auf *.dia pruefen

			// otherwise it is maybe an image ?

			bool bOk = ReadQImage( (const char *)sFileName, aImage );
			//bool bOk = aImage.load( s1 );
		}
	*/	/*else if( Q3TextDrag::decode( pEvent, sText ) )
		{
			const char * s = (const char *)sText;

			Q3Url aUrl( sText );

			bool bOk = aUrl.isValid();
			bOk = aUrl.isLocalFile();

			QString sName = aUrl.fileName();
			QString sPath = aUrl.dirPath();

			s = (const char *)sName;
			s = (const char *)sPath;

			QString sFullName = sPath + sName;
			sFullName = sFullName.left( sFullName.length()-2 );
			s = (const char *)sFullName;
			bOk = aImage.load( sFullName );
		}*/
	}
}

void TimeLineView::customEvent(QEvent * pEvent)
{
    if( pEvent->type()==_USER_EVENT_GET_SOUND_LENGTH )
    {
        SoundInfoContainer & aSoundContainer = m_pDiaPres->GetSoundInfoData();
        GetSoundLengthEvent * pSoundEvent = (GetSoundLengthEvent *)pEvent;
        aSoundContainer.push_back( minHandle<SoundInfo>( new SoundInfo( pSoundEvent->GetFileName().toStdString(), pSoundEvent->GetSoundLength() ) ) );
        aSoundContainer.SetChanged();
        //aSoundContainer.UpdateAllLengths();
        emit sigViewDataChanged();
    }
}

void TimeLineView::ShowModifyDynObjectDialog( int iIndexOut )
{
    // ** change data of the dynamic text with a dialog **
	DynContainer & aDynGrOpContainer = m_pDiaPres->GetDynGraphicData();

    if( iIndexOut>=0 && iIndexOut<(int)aDynGrOpContainer.size() )
    {
        
    	minHandle<DynText> hItem = aDynGrOpContainer[ iIndexOut ];
    
        DynamicTextDlgImpl aDlg( hItem, this, m_pParent );
        aDlg.setModal(true);
//        aDlg.show();
    //remove vector
//    	aDlg.m_pText->setText( QString( hItem->text() ) );
//    	aDlg.m_pText->setFocus();
//    	aDlg.m_pFontName->setText( hItem->font().family() );
//    	QString sTemp;
//        sTemp = sTemp.setNum( hItem->font().pointSize() );
//    	aDlg.m_pFontSize->setText( sTemp );
////    	sTemp = sTemp.setNum( hItem->x() );
////    	aDlg.m_pPosX->setText( sTemp );
////    	sTemp = sTemp.setNum( hItem->y() );
////    	aDlg.m_pPosY->setText( sTemp );
//        QColor aColor = hItem->brush().color();
//    	aDlg.m_pSelectFontcolor->setPalette( QPalette( aColor ) );
//    	double xRel,yRel;
//    	if( hItem->GetRelativePos( xRel, yRel ) )
//    	{
//    		aDlg.SetRelPos( xRel, yRel );
//    	}
    
        double dStart, dDelta;
        hItem->GetDefaultData( dStart, dDelta );
    
//    	sTemp = sTemp.setNum( /*hItem->GetStartTime()*/dStart );
//    	aDlg.m_pShowAtTime->setText( sTemp );
//    	sTemp = sTemp.setNum( dDelta );
//    	aDlg.m_pShowTime->setText( sTemp );
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
            QColor aColor = aDlg.m_pSelectFontcolor->palette().background().color(); //backgroundColor();
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

/*  gulp working
 *
 *   music_file.mp3 --> (music_file.mp3,length_in_ms) -->
 *
 **/

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

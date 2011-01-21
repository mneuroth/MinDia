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
#include "EnterValueDlg.h"
#include "dyntextdlgimpl.h"

#include "misctools.h"

#include <qcursor.h>
#include <q3popupmenu.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qevent.h>
#include <q3dragobject.h>
#include <q3url.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlineedit.h>
//Added by qt3to4:
#include <Q3StrList>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QToolTip>

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

#define MODIFY_SOUNDDATA		1
#define MODIFY_SOUNDCOMMENTS	2
#define MODIFY_PLOTCOMMENTS		3
#define MODIFY_ADDDYNTEXT		4
#define MODIFY_MODIFYDYNTEXT	5

const double g_dFactor = 10.0;		// one pixel is one tenth of a second !
const int g_iRampSize= 60;
const int g_iStartPosY = g_iRampSize + 20;
const int c_iDynOpOffset = 60;

TimeLineView::TimeLineView( QWidget * pParent, int iWidth, int iHeight, QWidget * pMainWin, QObject * pControler, DiaPresentation * pDoc )
: Q3CanvasView( 0, pParent )
{
	setAcceptDrops( TRUE );
	setDragAutoScroll( TRUE );

	m_pContextMenu		= new Q3PopupMenu( this );
	m_pContextMenu->insertItem( tr( "Sound &data..." ), MODIFY_SOUNDDATA );
	m_pContextMenu->insertItem( tr( "Sound &comments..." ), MODIFY_SOUNDCOMMENTS );
	m_pContextMenu->insertItem( tr( "&Plot comments..." ), MODIFY_PLOTCOMMENTS );
	m_pContextMenu->insertSeparator();
	m_pContextMenu->insertItem( tr( "&Add dyn. text..." ), MODIFY_ADDDYNTEXT );
	m_pContextMenu->insertItem( tr( "&Modify dyn. text..." ), MODIFY_MODIFYDYNTEXT );
	//connect( m_pContextMenu, SIGNAL( aboutToShow() ), this, SLOT( sltShowContextMenu() ) );
	connect( m_pContextMenu, SIGNAL( activated(int) ), this, SLOT( sltContextMenuActivated(int) ) );

	m_pCanvas			= new Q3Canvas( iWidth, iHeight );
	//m_pCanvas->setDoubleBuffering( true );
	setCanvas( m_pCanvas );

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

	m_hPlayMark = minHandle<Q3CanvasLine>( new Q3CanvasLine( m_pCanvas ) );
	m_hPlayMark->setBrush( QColor( 255, 0, 0 ) );
	m_hPlayMark->setPoints( 0, 0, 0, g_iStartPosY );
	m_hPlayMark->show();

	sltUpdateView();

   	connect( this, SIGNAL( sigViewDataChanged() ), pControler, SLOT( sltDataChanged() ) );
   	connect( this, SIGNAL( sigItemSelected(int,int) ), pControler, SLOT( sltSelectItem(int,int) ) );

	if( pMainWin )
	{
   		connect( this, SIGNAL( sigModifySoundData() ), pMainWin, SLOT( sltShowSoundData() ) );
   		connect( this, SIGNAL( sigModifySoundComment() ), pMainWin, SLOT( sltShowSoundComment() ) );
   		connect( this, SIGNAL( sigModifyPlotComment() ), pMainWin, SLOT( sltShowPlotComment() ) );
		connect( this, SIGNAL( sigLoadDoc(const QString &, bool) ), pMainWin, SLOT( sltLoadDoc(const QString &, bool) ) );
	}

//TODO Qt4	m_pToolTip = new MyDynamicToolTip( this );
    m_pToolTip = 0;
}

TimeLineView::~TimeLineView()
{
	//let qt delete the object: delete m_pCanvas;

	delete m_pToolTip;

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

void TimeLineView::sltDoUpdateView( bool bErase )
{
	if( m_pDiaPres )
	{
		double dTotalTime = m_pDiaPres->GetTotalTime();
		int iTotalLength = (int)(dTotalTime*g_dFactor);

		// ** update the size of the view **
		if( m_pCanvas->width()<iTotalLength )
		{
			// ** WARNING: the resize-method is expensive !!!
			m_pCanvas->resize( iTotalLength, m_pCanvas->height() );
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
				ensureVisible( hItem->GetPositionX()+hItem->GetLength(), hItem->GetPositionY() );
				ensureVisible( hItem->GetPositionX(), hItem->GetPositionY() );
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

	repaintContents( contentsX(), contentsY(), /*contents*/visibleWidth(), /*contents*/visibleHeight(), bErase );
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

void TimeLineView::sltContextMenuActivated( int iMenuIndex )
{
	switch( iMenuIndex )
	{
		case MODIFY_SOUNDDATA:
			emit sigModifySoundData();
			break;
		case MODIFY_SOUNDCOMMENTS:
			emit sigModifySoundComment();
			break;
		case MODIFY_PLOTCOMMENTS:
			emit sigModifyPlotComment();
			break;
		case MODIFY_ADDDYNTEXT:
			{
				DynContainer & aDynGrOpContainer = m_pDiaPres->GetDynGraphicData();

				EnterValueDlg aEnterDlg( this, "enter_text", /*modal*/TRUE );

				aEnterDlg.setCaption( tr( "Enter text" ) );
				aEnterDlg.m_pTextLabel->setText( tr( "text:" ) );
				aEnterDlg.m_pEnterValue->setFocus();

				int iRet = aEnterDlg.exec();

				if( iRet == 1 )
				{
					QString s = aEnterDlg.m_pEnterValue->text();

					int x = m_aLastMousePos.x();	

					aDynGrOpContainer.AddDefaultDynText( (const char *)s, x*1000/g_dFactor-m_pDiaPres->GetOffsetForSound()*1000, 5000 );

					emit sigViewDataChanged();
				}
			}
			break;
		case MODIFY_MODIFYDYNTEXT:
			{
				ShowModifyDynObjectDialog( m_iSelectedDynTextIndex );
			}
			break;
	}
}

void TimeLineView::SetPlayMark( double dActPlayTime )
{
// TODO --> ist dies noch notwendig ?
#ifndef __linux__
	const int iDelta = 4;
#endif

	int iActPos = (int)(dActPlayTime*g_dFactor);

	m_hPlayMark->move( iActPos, 0 );

	// ** repaint only needet in play or pause modus
	if( iActPos >= 0 )
	{
// TODO --> ist dies noch notwendig ?
#ifdef __linux__
		repaintContents( contentsX(), contentsY(), /*contents*/visibleWidth(), g_iStartPosY );
#else
		repaintContents( iActPos-iDelta, contentsY(), 2*iDelta+1, /*contentsHeight()*/g_iStartPosY );
#endif
		// ** shift viewport, to ensure that the new item is visible
		ensureVisible( iActPos+20, 0 );
	}

	// ** clear last play mark, after stop
	if( m_iLastActPlayPos-iActPos > 2 )
	{
#ifdef __linux__
		repaintContents( contentsX(), contentsY(), /*contents*/visibleWidth(), /*contents*/visibleHeight() );
#else
		repaintContents( m_iLastActPlayPos-iDelta, contentsY(), 2*iDelta+1, /*contents*/visibleHeight() );
#endif
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

			Q3CanvasLine * pLine1 = new Q3CanvasLine( m_pCanvas );
			pLine1->setPoints( 0, 0, 0, 20 );
			//pLine1->setBrush( QBrush( aColor ) );
			pLine1->setPen( QPen( aColor ) );
			pLine1->move( iPos, g_iStartPosY+c_iOffset );
			pLine1->show();

			Q3CanvasLine * pLine2 = new Q3CanvasLine( m_pCanvas );
			pLine2->setPoints( 0, 0, 0, 20 );
			//pLine2->setBrush( QBrush( aColor ) );
			pLine2->setPen( QPen( aColor ) );
			pLine2->move( iPos2, g_iStartPosY+c_iOffset );
			pLine2->show();

			Q3CanvasText * pText = new Q3CanvasText( m_pCanvas );
			QString sText = aCommentContainer[i]->GetComment().c_str();
			sText = "<-- " + sText + " -->";
			pText->setText( sText );
			pText->setColor( aColor );
			pText->move( iPos, g_iStartPosY+c_iOffset+c_iTextOffset );
			pText->show();

			// ** shift the text into the middle of the two bars
			QRect aRect = pText->boundingRect();
			if( aRect.width() < iPos2-iPos )
			{
				pText->move( iPos + ((iPos2-iPos-aRect.width())/2), g_iStartPosY+c_iOffset+c_iTextOffset );
			}

			m_aPlotCommentContainer.push_back( PlotCommentItem( 
					pair< minHandle<Q3CanvasLine>, minHandle<Q3CanvasLine> >( minHandle<Q3CanvasLine>( pLine1 ), minHandle<Q3CanvasLine>( pLine2 ) ),
					minHandle<Q3CanvasText>( pText ) ) );
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

		Q3CanvasLine * pLine = new Q3CanvasLine( m_pCanvas );
		pLine->setPoints( 0, 0, 0, 20+iLength );
		//pLine->setBrush( QBrush( aColor ) );
		pLine->setPen( QPen( aColor ) );
		pLine->move( iPos, g_iStartPosY+c_iOffset );
		pLine->show();

		Q3CanvasText * pText = new Q3CanvasText( m_pCanvas );
		QString sText = aCommentContainer[i]->GetComment().c_str();
		pText->setText( sText );
		pText->setColor( aColor );
		pText->move( iPos+2, g_iStartPosY+c_iOffset+10+iLength );
		pText->show(); 

		m_aMusicCommentContainer.push_back( MusicCommentItem( MusicCommentItemHelper( minHandle<Q3CanvasLine>( pLine ), minHandle<Q3CanvasText>( pText ) ), ItemInfoHelper( sText, -1 ) ) );
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

		Q3CanvasRectangle * pRect = new Q3CanvasRectangle( m_pCanvas );
		pRect->setSize( iDelta, c_iTrackHeight );
		pRect->setBrush( QBrush( aColor1 ) );
		pRect->move( iOffset+iStartPos, g_iStartPosY+c_iTrackHeight+c_iTrackOffset /*80*/ );
		pRect->show();

		Q3CanvasText * pText = new Q3CanvasText( m_pCanvas );
		QString sText = aSoundContainer[i]->GetFileName().c_str();
		pText->setText( sText );
		pText->setColor( aColor2 );
		pText->move( iOffset+iStartPos, g_iStartPosY+c_iTrackHeight+c_iTrackOffset+c_iTrackHeight /*140*/ );
		pText->show();

		// ** show fade in / fade out line
		int iFadeInStart = (int)(aSoundContainer[i]->GetFadeInStartPos()*0.001*g_dFactor);
		int iFadeInLength = (int)(aSoundContainer[i]->GetFadeInLength()*0.001*g_dFactor);
		int iFadeOutStart = (int)(aSoundContainer[i]->GetFadeOutStartPos()*0.001*g_dFactor);
		int iFadeOutLength = (int)(aSoundContainer[i]->GetFadeOutLength()*0.001*g_dFactor);

		Q3CanvasLine * pLineFadeIn = new Q3CanvasLine( m_pCanvas );
		pLineFadeIn->setPoints( 0, c_iTrackHeight, iFadeInLength, 0 );
		//pLineFadeIn->setBrush( QBrush( aColor ) );
		pLineFadeIn->move( iOffset+iStartPos+iFadeInStart, g_iStartPosY+c_iTrackHeight+c_iTrackOffset );
		pLineFadeIn->show();
		pLineFadeIn->setSelected( true );
		pLineFadeIn->setZ( 128 );

		Q3CanvasLine * pLineFadeOut = new Q3CanvasLine( m_pCanvas );
		pLineFadeOut->setPoints( 0, 0, iFadeOutLength, c_iTrackHeight );
		//pLineFadeOut->setBrush( QBrush( aColor ) );
		pLineFadeOut->move( iOffset+iStartPos+iFadeOutStart, g_iStartPosY+c_iTrackHeight+c_iTrackOffset );
		pLineFadeOut->show();
		pLineFadeOut->setSelected( true );
		pLineFadeOut->setZ( 128 );

		m_aMusicContainer.push_back( MusicItem( minHandle<Q3CanvasRectangle>( pRect ), minHandle<Q3CanvasText>( pText ) ) );
		m_aFadeContainer.push_back( FadeItem( minHandle<Q3CanvasLine>( pLineFadeIn ), minHandle<Q3CanvasLine>( pLineFadeOut ) ) );
	}
}

static bool IsInGraphicOperationsArea( const QPoint & aPoint )
{
	if( (aPoint.y() > g_iStartPosY+c_iDynOpOffset) &&
		(aPoint.y() < g_iStartPosY+c_iDynOpOffset+20) )
	{
		return true;
	}
	return false;
}

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

		Q3CanvasLine * pLine = new Q3CanvasLine( m_pCanvas );
		pLine->setPoints( 0, 0, 0, 20+iLength );
		//pLine->setBrush( QBrush( aColor ) );
		pLine->setPen( QPen( aColor ) );
		pLine->move( iPos, g_iStartPosY+c_iDynOpOffset );
		pLine->show();

		Q3CanvasText * pText = new Q3CanvasText( m_pCanvas );
		QString sText = aDynGrOpContainer[i]->GetString().c_str();
		QString sAddText;
		if( sText.length()>4 )
		{
			sAddText = "...";
		}
		pText->setText( sText.left( 4 )+sAddText );
		pText->setColor( aColor );
		pText->move( iPos+2, g_iStartPosY+c_iDynOpOffset+10+iLength );
		pText->show(); 

		m_aDynGrapOpContainer.push_back( MusicCommentItem( MusicCommentItemHelper( minHandle<Q3CanvasLine>( pLine ), minHandle<Q3CanvasText>( pText ) ), ItemInfoHelper( sText, i ) ) );
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

void TimeLineView::contentsMousePressEvent( QMouseEvent * pEvent )
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
					if( (pEvent->state() & Qt::ShiftModifier) == Qt::ShiftModifier )
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
			if( (pEvent->state() & Qt::ShiftModifier) == Qt::ShiftModifier )
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
		int iTemp = m_iSelectedDynTextIndex;	// save info of the selected item
		QRect aRect = GetTipRect( pEvent->pos(), 0, &m_iSelectedDynTextIndex );
		m_pContextMenu->setItemEnabled( MODIFY_MODIFYDYNTEXT, aRect.isValid() );
				
		m_pContextMenu->exec( pEvent->globalPos() );

		// restore info
		m_iSelectedDynTextIndex = iTemp;
	}
}

void TimeLineView::contentsMouseReleaseEvent( QMouseEvent * /*pEvent*/ )
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

void TimeLineView::contentsMouseMoveEvent( QMouseEvent * pEvent )
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
			pEvent->accept( Q3UriDrag::canDecode( pEvent ) );
		}
//        QTextDrag::canDecode(pEvent) ||
	}
}

void TimeLineView::dragMoveEvent ( QDragMoveEvent * pEvent )
{
	if( m_pDiaPres->IsEdit() )
	{
		QString sFileName;

		if( IsDiaDataFileDrag( pEvent, sFileName ) )
		{
			pEvent->accept( Q3UriDrag::canDecode( pEvent ) );
		}
		else if( IsSoundFileDrag( pEvent ) )
		{
			pEvent->accept( Q3UriDrag::canDecode( pEvent ) );
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
		QString sText;
		Q3StrList aStrList;
		QImage aImage;

		QPoint aPos = pEvent->pos();

		if( Q3ImageDrag::decode( pEvent, aImage ) )
		{
			int i = 0;
			i = 2;
		}
		else if( IsDiaDataFileDrag( pEvent, sFileName ) )
		{
			emit sigLoadDoc( sFileName, true );
		}
		else if( IsSoundFileDrag( pEvent ) )
		{
			SoundInfoContainer & aSoundContainer = m_pDiaPres->GetSoundInfoData();

			const char * s;
			QString sFileName;

			if( Q3UriDrag::decode( pEvent, aStrList ) )
			{
				for( int i=0; i<(int)aStrList.count(); i++ )
				{
					const QString sTest = aStrList.at(i);
					s = (const char *)sTest;
					sFileName = Q3UriDrag::uriToLocalFile( s );
					s = (const char *)sFileName;

					aSoundContainer.push_back( minHandle<SoundInfo>( new SoundInfo( s ) ) );
					aSoundContainer.SetChanged();
				}
				emit sigViewDataChanged();
			}
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
	*/	else if( Q3TextDrag::decode( pEvent, sText ) )
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
		}
	}
}

void TimeLineView::ShowModifyDynObjectDialog( int iIndexOut )
{
	// ** change data of the dynamic text with a dialog **
	DynContainer & aDynGrOpContainer = m_pDiaPres->GetDynGraphicData();

	minHandle<DynText> hItem = aDynGrOpContainer[ iIndexOut ];

	DynamicTextDlgImpl aDlg( hItem, this, m_pParent, "enter_dynamic_text", /*modal*/TRUE );
//remove vector
	aDlg.m_pText->setText( QString( hItem->text() ) );
	aDlg.m_pText->setFocus();
	aDlg.m_pFontName->setText( hItem->font().family() );
	QString sTemp;
	sTemp = sTemp.setNum( hItem->font().pointSize() );
	aDlg.m_pFontSize->setText( sTemp );
	sTemp = sTemp.setNum( hItem->x() );
	aDlg.m_pPosX->setText( sTemp );
	sTemp = sTemp.setNum( hItem->y() );
	aDlg.m_pPosY->setText( sTemp );
	QColor aColor = hItem->color();
	aDlg.m_pSelectFontcolor->setPalette( QPalette( aColor ) );
	double xRel,yRel;
	if( hItem->GetRelativePos( xRel, yRel ) )
	{
		aDlg.SetRelPos( xRel, yRel );
	}

	double dStart, dDelta;
	hItem->GetDefaultData( dStart, dDelta );

	sTemp = sTemp.setNum( /*hItem->GetStartTime()*/dStart );
	aDlg.m_pShowAtTime->setText( sTemp );
	sTemp = sTemp.setNum( dDelta );
	aDlg.m_pShowTime->setText( sTemp );

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
			hItem->setX( aDlg.m_pPosX->text().toInt() );
			hItem->setY( aDlg.m_pPosY->text().toInt() );
			hItem->SetRelativePos( -1.0, -1.0 );
		}
		QColor aColor = aDlg.m_pSelectFontcolor->backgroundColor();
		hItem->setColor( aColor );

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


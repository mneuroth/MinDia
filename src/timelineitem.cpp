/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/timelineitem.cpp,v $
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

#include "timelineitem.h"

#include "diapresentation.h"

#include <q3canvas.h>
#include <qbrush.h>
//Added by qt3to4:
#include <Q3PointArray>

// *******************************************************************
// *******************************************************************
// *******************************************************************

const int g_iMouseAreaWidth = 5;

TimeLineItem::TimeLineItem( Q3Canvas * pCanvas, DiaPresentation * pDiaPres, int iSlideNo, double dFactor, int iRampSize, bool bIsSelected )
: m_pCanvas( pCanvas ),
  m_pDissolveRamp( 0 ),
  m_pSlideLabel( 0 ),
  m_pDiaPres( pDiaPres ), 
  m_iSlideNo( iSlideNo ),
  m_dFactor( dFactor ),
  m_iRampSize( iRampSize ),
  m_iRampDelta( 0 ),
  m_bIsSelected( bIsSelected )
{
	UpdateItem();
}

TimeLineItem::~TimeLineItem()
{
	CleanUp();
}

bool TimeLineItem::IsSelected() const
{
	return m_bIsSelected;
}

void TimeLineItem::SetSelected( bool bValue )
{
	m_bIsSelected = bValue;
}

void TimeLineItem::CleanUp()
{
	delete m_pDissolveRamp;
	delete m_pSlideLabel;
}

void TimeLineItem::UpdateItem()
{
	if( m_pDiaPres )
	{
		CleanUp();

		int iPosition = GetPositionX();
		int iShowPos = (int)(m_pDiaPres->GetDiaAbsStartShowTime( m_iSlideNo )*m_dFactor);
		int iDelta = iShowPos-iPosition;
		int iDelta2 = GetLength();

		m_iRampDelta = iDelta;

		m_pDissolveRamp = new Q3CanvasPolygon( m_pCanvas );

		Q3PointArray aPoints( 7 );
		aPoints[0] = QPoint( 0, m_iRampSize );
		aPoints[1] = QPoint( iDelta, m_iRampSize );
		aPoints[2] = QPoint( iDelta2, m_iRampSize );	// *
		aPoints[3] = QPoint( iDelta2, 0 );			// *
		aPoints[4] = QPoint( iDelta, 0 );			// *
		aPoints[5] = QPoint( iDelta, 0 );
		aPoints[6] = QPoint( 0, m_iRampSize );
		m_pDissolveRamp->setPoints( aPoints );
		QBrush aBrush( Qt::SolidPattern/*Dense4Pattern*/ );
		if( IsSelected() )
		{
			aBrush.setColor( QColor( 128, 0, 0 ) );
		}
		else if( m_iSlideNo % 2 == 1 )
		{
			aBrush.setColor( QColor( 0, 128, 0 ) );
		}
		else
		{
			aBrush.setColor( QColor( 0, 0, 128 ) );
		}
		m_pDissolveRamp->setBrush( aBrush );
		m_pDissolveRamp->move( iPosition, GetPositionY() );
		m_pDissolveRamp->show();

		m_pSlideLabel = new Q3CanvasText( m_pCanvas );
		QString sLabel;
		sLabel.setNum( m_iSlideNo + 1 );
		m_pSlideLabel->setText( sLabel );
		m_pSlideLabel->move( iPosition, GetPositionY()+m_iRampSize );
		m_pSlideLabel->show();
	}
}

bool TimeLineItem::IsSelected( int x, int y ) const
{
	QRect aRect = m_pDissolveRamp->boundingRect();

	if( (x >= aRect.left()) && (x <= aRect.right()) &&
		(y >= aRect.top()) && (y <= aRect.bottom()) )
	{
		return true;
	}

	return false;
}

bool TimeLineItem::IsStopBorderSelected( int x, int y ) const
{
	QRect aRect = m_pDissolveRamp->boundingRect();

	if( (x >= aRect.right()-g_iMouseAreaWidth) && (x <= aRect.right()) &&
		(y >= aRect.top()) && (y <= aRect.bottom()) )
	{
		return true;
	}

	return false;
}

bool TimeLineItem::IsDissolveBorderSelected( int x, int y ) const
{
	QRect aRect = m_pDissolveRamp->boundingRect();

	if( (x >= aRect.left()+m_iRampDelta-g_iMouseAreaWidth) && 
		(x <= aRect.left()+m_iRampDelta+g_iMouseAreaWidth) && 
		(y >= aRect.top()) && (y <= aRect.bottom()) )
	{
		return true;
	}

	return false;
}

void TimeLineItem::ChangeShowTime( double dDeltaTime )
{
	m_pDiaPres->UpdateShowTimeForDia( m_iSlideNo, dDeltaTime );
	
	UpdateItem();
}

void TimeLineItem::ChangeDissolveTime( double dDeltaTime )
{
	m_pDiaPres->UpdateDissolveTimeForDia( m_iSlideNo, dDeltaTime );
	
	UpdateItem();
}

int TimeLineItem::GetPositionX() const
{
	int iPosition = (int)(m_pDiaPres->GetDiaAbsStartDissolveTime( m_iSlideNo )*m_dFactor);
	return iPosition;
}

int TimeLineItem::GetPositionY() const
{
	const int iOffsetY = 5;

	return iOffsetY;
}

int TimeLineItem::GetLength() const
{
	int iNextPosition = (int)(m_pDiaPres->GetDiaAbsStartDissolveTime( m_iSlideNo+1 )*m_dFactor);
	int iDelta2 = iNextPosition-GetPositionX();

	return iDelta2;
}

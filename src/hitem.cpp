/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/hitem.cpp,v $
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

#include "hitem.h"

#include "diainfo.h"

#include "misctools.h"

#include <qobject.h>
#include <qpainter.h>
#include <qimage.h>

#include <stdio.h>

// *******************************************************************

const int c_iOffset			= 75;
const int c_iDY				= 12;
const int c_iSlideWidth		= 148;	//145;
const int c_iSlideHeight	= 92;	//90;
const int c_iSlideOffsX		= 14;	//15;
const int c_iSlideOffsY		= 44;	//45;
const int c_iSlideOffsY2	= 39;	//40;

// *******************************************************************
// *******************************************************************
// *******************************************************************

#ifdef _with_canvas_items

/** class which draws a slide in a canvas.
 */
class SlideItem : public Q3CanvasRectangle
{
public:
	SlideItem( const QRect & aRect, Q3Canvas * canvas, const QString & sImageFileName, bool bHorizontalSlide = true );

protected:
	virtual void drawShape( QPainter & aPainter ); 

private:
	bool		m_bHorizontalSlide;
	QString		m_sImageFileName;
}

// *******************************************************************

SlideItem::SlideItem( const QRect & aRect, Q3Canvas * canvas, const QString & sImageFileName, bool bHorizontalSlide )
: Q3CanvasRectangle( aRect, canvas ), 
  m_bHorizontalSlide( bHorizontalSlide ),
  m_sImageFileName( sImageFileName )
{
}

void SlideItem::drawShape( QPainter & aPainter )
{
	QRect	aRect( rect() );
	QColor	aGrey( 188, 188, 188 );
	QColor	aWhite( 255, 255, 255 );
	QColor	aBlack( 0, 0, 0 );
	QPen	aPen( aGrey );

	aPen.setWidth( 10 );
	aPainter.setPen( aPen );
	int iRound = 5;
	aPainter.drawRoundRect( aRect.x()+iRound, aRect.y()+iRound, aRect.width()-2*iRound, aRect.height()-c_iOffset, iRound, iRound );
	iRound = 10;

	QBrush aBrush1( aGrey );
	aPainter.fillRect( aRect.x()+iRound, aRect.y()+iRound, aRect.width()-2*iRound, aRect.height()-c_iOffset, aBrush1 );
	QBrush aBrush2( aWhite );
	QPoint aSlideStartPoint;
	QRect aSlideRect;
	if( m_bHorizontalSlide )
	{
		aSlideStartPoint.setX( aRect.x()+c_iSlideOffsX );
		aSlideStartPoint.setY( aRect.y()+c_iSlideOffsY2 );
		aSlideRect.setRect( aSlideStartPoint.x(), aSlideStartPoint.y(), c_iSlideWidth, c_iSlideHeight );
	}
	else
	{
		aSlideStartPoint.setX( aRect.x()+c_iSlideOffsY );
		aSlideStartPoint.setY( aRect.y()+c_iSlideOffsX );
		aSlideRect.setRect( aSlideStartPoint.x(), aSlideStartPoint.y(), c_iSlideHeight, c_iSlideWidth );
	}

	// ** show the image, if available
	QImage aImage;
	QImage aImageOrg;

	bool bOk = ReadQImage( m_sImageFileName.s_str(), aImageOrg );

	if( bOk && !aImageOrg.isNull() )
	{
		aImage = aImageOrg.smoothScale( c_iSlideWidth, c_iSlideHeight );
	}
	else
	{
		aImage = aImageOrg;
	}
	if( m_aImage.isNull() )
	{
		aPainter.fillRect( aSlideRect, aBrush2 );
	}
	else
	{
		aPainter.drawImage( aSlideRect.x(), aSlideRect.y(), m_aImage );
	}
}

#endif

// *******************************************************************
// *******************************************************************
// *******************************************************************

HItem::HItem( const QRect & aRect, Q3Canvas * pCanvas, minHandle<DiaInfo> hData  )
: Q3CanvasRectangle( aRect, pCanvas ),
  m_pImageCache( 0 )
{
	m_hData = hData;
	m_bIsSelected = false;

	// ** do not draw the rectangle of this item 
	setPen( QPen( QColor( 255, 255, 255 ) ) );
	setZ( 50 );

#ifdef _with_canvas_items
	CreateElements( pCanvas );
	UpdateElements();
#endif
}

HItem::~HItem()
{
#ifdef _with_canvas_items
	DeleteElements();
#endif
	if( m_pImageCache )
	{
		delete m_pImageCache;
	}
}

void HItem::drawShape( QPainter & aPainter )
{
	/*
	// ** paint a cross to mark the view area
	QPoint aPoint( m_aPos.x()+m_aSize.width(), m_aPos.y()+m_aSize.height() );
	aPainter.drawLine( m_aPos, aPoint );
	aPainter.drawLine( m_aPos.x(), m_aPos.y()+m_aSize.height(), m_aPos.x()+m_aSize.width(), m_aPos.y() );
	*/

	// ** paint a slide frame
	QRect	aRect( rect() );
	QColor	aGrey( 188, 188, 188 );
	QColor	aWhite( 255, 255, 255 );
	QColor	aBlack( 0, 0, 0 );
	QPen	aPen( aGrey );

	aPen.setWidth( 10 );
	aPainter.setPen( aPen );
	int iRound = 5;
	aPainter.drawRoundRect( aRect.x()+iRound, aRect.y()+iRound, aRect.width()-2*iRound, aRect.height()-c_iOffset, iRound, iRound );
	iRound = 10;

	QBrush aBrush1( aGrey );
	aPainter.fillRect( aRect.x()+iRound, aRect.y()+iRound, aRect.width()-2*iRound, aRect.height()-c_iOffset, aBrush1 );
	QPoint aSlideStartPoint;
	QRect aSlideRect;
	if( m_hData->IsHorizontalFormat() )
	{
		aSlideStartPoint.setX( aRect.x()+c_iSlideOffsX );
		aSlideStartPoint.setY( aRect.y()+c_iSlideOffsY2 );
		aSlideRect.setRect( aSlideStartPoint.x(), aSlideStartPoint.y(), c_iSlideWidth, c_iSlideHeight );
	}
	else
	{
		aSlideStartPoint.setX( aRect.x()+c_iSlideOffsY );
		aSlideStartPoint.setY( aRect.y()+c_iSlideOffsX );
		aSlideRect.setRect( aSlideStartPoint.x(), aSlideStartPoint.y(), c_iSlideHeight, c_iSlideWidth );
	}

	// has the image changed ? yes --> update cache !
	if( m_sImageFileNameCache != m_hData->GetImageFile() )
	{
		delete m_pImageCache;
		m_pImageCache = 0;
	}
	// ** show the image, if available
	if( !m_pImageCache )
	{
		QImage aImage;
		QImage aImageOrg;

		bool bOk = ReadQImage( m_hData->GetImageFile(), aImageOrg );
		m_sImageFileNameCache = m_hData->GetImageFile();

		if( bOk && (!aImageOrg.isNull()) )
		{
			if( m_hData->IsHorizontalFormat() )
			{
				aImage = aImageOrg.smoothScale( c_iSlideWidth, c_iSlideHeight );
			}
			else
			{
				aImage = aImageOrg.smoothScale( c_iSlideHeight, c_iSlideWidth );
			}
		}
		else
		{
			aImage = aImageOrg;
		}

		m_pImageCache = new QImage( aImage );
	}
	if( m_pImageCache->isNull() )
	{
		QBrush aBrush( aWhite );

		aPainter.fillRect( aSlideRect, aBrush );
	}
	else
	{
		aPainter.drawImage( aSlideRect.x(), aSlideRect.y(), *m_pImageCache );
	}
	/* ehemals wurden Skripte mit gelben Hintergrund hinterlegt
	else
	{
		QBrush aBrush( QColor( 255, 255, 196 ) );

		aPainter.fillRect( aSlideRect, aBrush );
	}
	*/

	// ** show text-data (id, comment, dissolve, timer, etc.)
	QPoint aTextStartPoint( aRect.x()+2, aRect.y()+aRect.height()-c_iOffset+c_iDY );
	aPainter.setPen( aBlack );
	if( m_hData.IsOk() )
	{
		int iMaxWidth = aRect.width();
		int iMaxHeight = c_iDY+5;

		char sBuffer[255];
		sprintf( sBuffer, "pos=%d", m_hData->GetPosition()+1 );
		aPainter.drawText( aTextStartPoint.x(), aTextStartPoint.y(),
						   iMaxWidth, iMaxHeight, Qt::AlignLeft | Qt::AlignTop,
						   sBuffer );
		sprintf( sBuffer, "id=%s", m_hData->GetId() );
		aPainter.drawText( aTextStartPoint.x(), aTextStartPoint.y()+c_iDY,
						   iMaxWidth, iMaxHeight, Qt::AlignLeft | Qt::AlignTop,
						   sBuffer );
		sprintf( sBuffer, "comment=%s", m_hData->GetComment() );
		aPainter.drawText( aTextStartPoint.x(), aTextStartPoint.y()+2*c_iDY,
						   iMaxWidth, iMaxHeight, Qt::AlignLeft | Qt::AlignTop,
						   sBuffer );

		QString sTemp = m_hData->GetImageFile();
		if( m_hData->HasScript() )
		{
			sTemp += " ";
			sTemp += QObject::tr( "<script>" );
		}
		aPainter.drawText( aSlideStartPoint.x(), aSlideStartPoint.y() /*+c_iDY*/,
						   c_iSlideWidth, iMaxHeight, Qt::AlignLeft | Qt::AlignTop,
						   sTemp );

		int iOffset = 3*c_iDY;
		for( int i=0; i<m_hData->GetOperationCount(); i++ )
		{
			TimeOperation aOp = m_hData->GetOperation( i );

			if( aOp.GetOperationType() == TimeOperation::DISSOLVE_IN )
			{
				sprintf( sBuffer, "dissolve=%4.1f s", aOp.GetOperationTime() );
			}
			if( aOp.GetOperationType() == TimeOperation::SHOW )
			{
				sprintf( sBuffer, "timer=%4.1f s", aOp.GetOperationTime() );
			}

			aPainter.drawText( aTextStartPoint.x(), aTextStartPoint.y()+iOffset,
							   iMaxWidth, iMaxHeight, Qt::AlignLeft | Qt::AlignTop,
							   sBuffer );
			iOffset += c_iDY;
		}
	}

	// ** show selection-frame, if needed
	if( m_bIsSelected )
	{
		aPainter.setPen( QColor( 255, 0, 0 ) );
	}
	else
	{
		aPainter.setPen( QColor( 255, 255, 255 ) );
	}
	aPainter.drawRect( aRect.x(), aRect.y(), aRect.width(), aRect.height() );
}

void HItem::SetSelected( bool bSelect )
{
	m_bIsSelected = bSelect;
}

bool HItem::GetSelected() const
{
	return m_bIsSelected;
}

QString HItem::GetData() const
{
	return QString( m_hData->GetData().c_str() );
}

minHandle<DiaInfo> HItem::GetInfoData() const
{
	return m_hData;
}

bool HItem::IsPointInItem( int x, int y ) const
{
	if( (this->x()<=x) && (this->x()+this->width()>=x) &&
		(this->y()<=y) && (this->y()+this->height()>=y) )
	{
		return true;
	}
	return false;
}

#ifdef _with_canvas_items

void HItem::CreateElements( Q3Canvas * pCanvas )
{
	m_pPosText = new Q3CanvasText( pCanvas );
	m_pPosText->show();
	m_pIdText = new Q3CanvasText( pCanvas );
	m_pIdText->show();
	m_pCommentText = new Q3CanvasText( pCanvas );
	m_pCommentText->show();
	m_pFileNameText = new Q3CanvasText( pCanvas );
	m_pFileNameText->show();
	m_pSlideItem = new SlideItem( QRect( 0, 0, width(), height() ), pCanvas, m_hData->GetImageFile() );
	m_pSlideItem->show();
}

void HItem::DeleteElements()
{
	delete m_pPosText;
	delete m_pIdText;
	delete m_pCommentText;
	delete m_pFileNameText;
	delete m_pSlideItem;
}

void HItem::UpdateElements()
{
	char sBuffer[255];

	QPoint aTextStartPoint( x()+2, y()+height()-c_iOffset+c_iDY );

	sprintf( sBuffer, "pos=%d", m_hData->GetPosition()+1 );
	m_pPosText->setText( sBuffer );
	m_pPosText->move( aTextStartPoint.x(), aTextStartPoint.y() );

	sprintf( sBuffer, "id=%s", m_hData->GetId() );
	m_pIdText->setText( sBuffer );
	m_pIdText->move( aTextStartPoint.x(), aTextStartPoint.y()+c_iDY );

	sprintf( sBuffer, "comment=%s", m_hData->GetComment() );
	m_pCommentText->setText( sBuffer );
	m_pCommentText->move( aTextStartPoint.x(), aTextStartPoint.y()+2*c_iDY );

	m_pFileNameText;

	m_pSlideItem->move( x(), y() );

	if( m_bIsSelected )
	{
		setPen( QPen( QColor( 255, 0, 0 ) ) );
	}
	else
	{
		setPen( QPen( QColor( 255, 255, 255 ) ) );
	}
}

#endif

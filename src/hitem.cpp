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

HItem::HItem( const QRect & aRect, QGraphicsScene * pCanvas, minHandle<DiaInfo> hData  )
: QGraphicsRectItem(QRectF(aRect.x(),aRect.y(),aRect.width(),aRect.height())),
  m_pImageCache( 0 )
{
	m_hData = hData;
	m_bIsSelected = false;

	// ** do not draw the rectangle of this item 
	setPen( QPen( QColor( 255, 255, 255 ) ) );
    //old: setZ( 50 );

    pCanvas->addItem(this);
}

HItem::~HItem()
{
	if( m_pImageCache )
	{
		delete m_pImageCache;
	}
}

void HItem::paint( QPainter * pPainter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/ )
{
    /*
    // ** paint a cross to mark the view area
    QPoint aPoint( m_aPos.x()+m_aSize.width(), m_aPos.y()+m_aSize.height() );
    aPainter.drawLine( m_aPos, aPoint );
    aPainter.drawLine( m_aPos.x(), m_aPos.y()+m_aSize.height(), m_aPos.x()+m_aSize.width(), m_aPos.y() );
    */

    QRectF	aRect( rect() );
    QColor	aGrey( 188, 188, 188 );
    QColor	aWhite( 255, 255, 255 );
    QColor	aBlack( 0, 0, 0 );
    QPen	aPen( aGrey );

    aPen.setWidth( 10 );
    pPainter->setPen( aPen );
    int iRound = 5;
    pPainter->drawRoundRect( aRect.x()+iRound, aRect.y()+iRound, aRect.width()-2*iRound, aRect.height()-c_iOffset, iRound, iRound );
    iRound = 10;

    QBrush aBrush1( aGrey );
    pPainter->fillRect( aRect.x()+iRound, aRect.y()+iRound, aRect.width()-2*iRound, aRect.height()-c_iOffset, aBrush1 );
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

    QString sImageFileName = ToQString( m_hData->GetImageFile() );
    // has the image changed ? yes --> update cache !
    if( m_sImageFileNameCache != sImageFileName )
    {
        delete m_pImageCache;
        m_pImageCache = 0;
    }
    // ** show the image, if available
    if( !m_pImageCache )
    {
        QImage aImage;
        QImage aImageOrg;

        bool bOk = ReadQImage( sImageFileName, aImageOrg );
        m_sImageFileNameCache = sImageFileName;

        if( bOk && (!aImageOrg.isNull()) )
        {
            if( m_hData->IsHorizontalFormat() )
            {
                aImage = aImageOrg.scaled( c_iSlideWidth, c_iSlideHeight );
            }
            else
            {
                aImage = aImageOrg.scaled( c_iSlideHeight, c_iSlideWidth );
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

        pPainter->fillRect( aSlideRect, aBrush );
    }
    else
    {
        pPainter->drawImage( aSlideRect.x(), aSlideRect.y(), *m_pImageCache );
    }
    /* ehemals wurden Skripte mit gelben Hintergrund hinterlegt
    else
    {
        QBrush aBrush( QColor( 255, 255, 196 ) );

        pPainter->fillRect( aSlideRect, aBrush );
    }
    */

    // ** show text-data (id, comment, dissolve, timer, etc.)
    QPoint aTextStartPoint( aRect.x()+2, aRect.y()+aRect.height()-c_iOffset+c_iDY );
    pPainter->setPen( aBlack );
    if( m_hData.IsOk() )
    {
        int iMaxWidth = aRect.width();
        int iMaxHeight = c_iDY+5;

        QString sBuf;
        sBuf = QString("pos=%1").arg( m_hData->GetPosition()+1 );
        pPainter->drawText( aTextStartPoint.x(), aTextStartPoint.y(),
                           iMaxWidth, iMaxHeight, Qt::AlignLeft | Qt::AlignTop,
                           sBuf );
        sBuf = QString("id=%1").arg( ToQString( m_hData->GetId() ) );
        pPainter->drawText( aTextStartPoint.x(), aTextStartPoint.y()+c_iDY,
                           iMaxWidth, iMaxHeight, Qt::AlignLeft | Qt::AlignTop,
                           sBuf );
        sBuf = QString("comment=%1").arg( ToQString( m_hData->GetComment() ) );
        pPainter->drawText( aTextStartPoint.x(), aTextStartPoint.y()+2*c_iDY,
                           iMaxWidth, iMaxHeight, Qt::AlignLeft | Qt::AlignTop,
                           sBuf );

        QString sTemp = ToQString( m_hData->GetImageFile() );
        if( m_hData->HasScript() )
        {
            sTemp += " ";
            sTemp += QObject::tr( "<script>" );
        }
        pPainter->drawText( aSlideStartPoint.x(), aSlideStartPoint.y() /*+c_iDY*/,
                           c_iSlideWidth, iMaxHeight, Qt::AlignLeft | Qt::AlignTop,
                           sTemp );

        int iOffset = 3*c_iDY;
        for( int i=0; i<m_hData->GetOperationCount(); i++ )
        {
            TimeOperation aOp = m_hData->GetOperation( i );

            if( aOp.GetOperationType() == TimeOperation::DISSOLVE_IN )
            {
                sBuf = QString("dissolve=%2 s").arg( aOp.GetOperationTime(), 4 );
            }
            if( aOp.GetOperationType() == TimeOperation::SHOW )
            {
                sBuf = QString("timer=%2 s").arg( aOp.GetOperationTime(), 4 );
            }

            pPainter->drawText( aTextStartPoint.x(), aTextStartPoint.y()+iOffset,
                               iMaxWidth, iMaxHeight, Qt::AlignLeft | Qt::AlignTop,
                               sBuf);
            iOffset += c_iDY;
        }
    }

    // ** show selection-frame, if needed
    if( m_bIsSelected )
    {
        pPainter->setPen( QColor( 255, 0, 0 ) );
    }
    else
    {
        pPainter->setPen( QColor( 255, 255, 255 ) );
    }
    pPainter->drawRect( aRect.x(), aRect.y(), aRect.width()-1, aRect.height()-1 );
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
    return QString( ToQString(m_hData->GetData()) );
}

minHandle<DiaInfo> HItem::GetInfoData() const
{
	return m_hData;
}

bool HItem::IsPointInItem( int x, int y ) const
{
    if( (rect().x()<=x) && (rect().x()+rect().width()>=x) &&
        (rect().y()<=y) && (rect().y()+rect().height()>=y) )
	{
		return true;
	}
	return false;
}

/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2013 by Michael Neuroth
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

#include "graphicsitemresizeablerect.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>

#include <math.h>

inline bool IsInRange( double value, double pos, double delta )
{
    return fabs(value-pos)<=delta;
}

inline int Calc( double factor, int value )
{
    return (int)(factor*(double)value);
}

GraphicsItemResizeableRect::GraphicsItemResizeableRect( GraphicsItemChangedCallback * pCallback )
    : m_Ratio( RATIO_UNDEFINED ),
      m_relX( 0.0 ),
      m_relY( 0.0 ),
      m_relDX( 1.0 ),
      m_relDY( 1.0 ),
      m_pCallback( pCallback ),
      m_aResizeState( NONE )
{
    setAcceptHoverEvents( true );
    setFlag( QGraphicsItem::ItemIsMovable );
    setFlag( QGraphicsItem::ItemSendsGeometryChanges );
}

void GraphicsItemResizeableRect::Rescale( QSize aImageSize )
{
    m_aImageSize = aImageSize;

    QSize aRatioSize = GetRatioSize();
    int dx = aRatioSize.width();
    int dy = aRatioSize.height();

    setRect( 0, 0, Calc(m_relDX,dx), Calc(m_relDY,dy) );
    setPos( Calc(m_relX,dx), Calc(m_relY,dy) );
}

void GraphicsItemResizeableRect::SetClippingData( ImageRatio ratio, double relX, double relY, double relDX, double relDY )
{
    m_Ratio = ratio;
    m_relX = relX;
    m_relY = relY;
    m_relDX = relDX;
    m_relDY = relDY;
}

void GraphicsItemResizeableRect::GetClippingData( double & relX, double & relY, double & relDX, double & relDY )
{
    relX = m_relX;
    relY = m_relY;
    relDX = m_relDX;
    relDY = m_relDY;
}

void GraphicsItemResizeableRect::CheckRectForClipping()
{
    const double MIN_REL_SIZE = 0.05;

    QSize aRatioSize = GetRatioSize();
    double dFacX = (double)m_aImageSize.width()/(double)aRatioSize.width();
    double dFacY = (double)m_aImageSize.height()/(double)aRatioSize.height();

    if( m_relX<0.0 )
    {
        m_relX = 0.0;
    }
    if( m_relY<0.0 )
    {
        m_relY = 0.0;
    }
    if( m_relDX>1.0 )
    {
        m_relDX = 1.0;
    }
    if( m_relDX<MIN_REL_SIZE )
    {
        m_relDX = MIN_REL_SIZE;
    }
    if( m_relDY<MIN_REL_SIZE )
    {
        m_relDY = MIN_REL_SIZE;
    }
    if( m_relDY>1.0 )
    {
        m_relDY = 1.0;
    }
    if( m_relX+m_relDX>dFacX )
    {
        m_relX = dFacX-m_relDX;
    }
    if( m_relY+m_relDY>dFacY )
    {
        m_relY = dFacY-m_relDY;
    }
}

void GraphicsItemResizeableRect::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
    int deltaX = event->pos().x()-event->lastPos().x();     // -1 <--     +1 -->
    int deltaY = event->pos().y()-event->lastPos().y();
    double dx = (double)deltaX/(double)m_aImageSize.width();
    double dy = (double)deltaY/(double)m_aImageSize.height();

    if( m_aResizeState==WEST )
    {
        m_relDX += -dx;
        m_relX += dx;
        m_relDY = m_relDX;
    }
    else if( m_aResizeState==EAST )
    {
        m_relDX += dx;
        m_relDY = m_relDX;
    }
    else if( m_aResizeState==NORTH )
    {
        m_relDY += -dy;
        m_relY += dy;
        m_relDX = m_relDY;
    }
    else if( m_aResizeState==SOUTH )
    {
        m_relDY += dy;
        m_relDX = m_relDY;
    }
    else
    {
        // handle move
        m_relX += dx;
        m_relY += dy;
    }

    CheckRectForClipping();
    Rescale(m_aImageSize);  // move item myself and do not let the library move the item
                            // in the later case the logical and physical coodinates will
                            // differ because of rounding errors !!!

    if( m_pCallback )
    {
        m_pCallback->ItemModified( this );
    }

    //not used: QGraphicsRectItem::mouseMoveEvent( event );
}

void GraphicsItemResizeableRect::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
    const double DELTA = 5.0;

    QPointF pos = event->pos();
    bool bWest = IsInRange( pos.x(), 0, DELTA );
    bool bNorth = IsInRange( pos.y(), 0, DELTA );
    if(  bWest || IsInRange( pos.x(), rect().width(), DELTA ) )
    {
        m_aResizeState = bWest ? WEST : EAST;
        setCursor( Qt::SizeHorCursor );
    }
    else if( bNorth || IsInRange( pos.y(), rect().height(), DELTA ) )
    {
        m_aResizeState = bNorth ? NORTH : SOUTH;
        setCursor( Qt::SizeVerCursor );
    }
    else
    {
        m_aResizeState = NONE;
        setCursor( Qt::ArrowCursor );
    }
    QGraphicsRectItem::hoverMoveEvent( event );
}

/*
QVariant GraphicsItemResizeableRect::itemChange( GraphicsItemChange change, const QVariant & value )
{
    if( change==QGraphicsItem::ItemPositionChange )
    {
    }
    return QGraphicsRectItem::itemChange( change, value );
}
*/

QSize GraphicsItemResizeableRect::GetRatioSize() const
{
    return GetRatioSizeForAvailableSize( m_aImageSize, m_Ratio );
}


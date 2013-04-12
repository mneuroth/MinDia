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
#include <QGraphicsWidget>
#include <QCursor>

#include <math.h>

#include <iostream>
using namespace std;

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
    QSize aMaxSize = QSize( aImageSize.width(), aImageSize.height() );
    QSize aSize = GetRatioSizeForAvailableSize( aMaxSize, m_Ratio );
    cout << "RESCALE max: " << aMaxSize.width() << " " << aMaxSize.height() << " ratio: " << aSize.width() << " " << aSize.height() << endl;
    int dx = aSize.width();
    int dy = aSize.height();

// TODO --> hier noch das korrekte Ratio behandeln, falls image-ratio von desired-ratio abweicht verschiebt sich das Rechteck !
//          ==> wird von ausserhalb geprueft !

//cout << "RESCALE data " << m_relX << " " << m_relY << " " << m_relDX << " " << m_relDY << " size " << dx << " " << dy << endl;
    setRect( 0, 0, Calc(m_relDX,dx), Calc(m_relDY,dy) );
//cout << "--> " << Calc(m_relX,dx) << " " << Calc(m_relY,dy) << " " << Calc(m_relDX,dx) << " " << Calc(m_relDY,dy) << " sum=" << Calc(m_relX,dx)+Calc(m_relDX,dx) << endl ;
//cout << "--> rect pos= " << pos().x() << " " << pos().y() << endl;
    setPos( Calc(m_relX,dx), Calc(m_relY,dy) ); // triggert ggf. entsprechenden itemChange() Aufruf !
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
    QSize aMaxSize = QSize( m_aImageSize.width(), m_aImageSize.height() );
    QSize aSize = GetRatioSizeForAvailableSize( aMaxSize, m_Ratio );
    double dFacX = ((double)aMaxSize.width()/(double)aSize.width());
    int dx = aSize.width();
    int dy = aSize.height();
    int x0 = Calc(m_relX,dx);
    int y0 = Calc(m_relY,dy);
    int dx0 = Calc(m_relDX,dx);
    int dy0 = Calc(m_relDY,dy);
    bool bContained = x0>=0.0 && x0+dx0<=aMaxSize.width() && y0>=0.0 && y0+dy0<=aMaxSize.height();

    if( m_relX<0.0 )
    {
        m_relX = 0.0;
    }
//    if( m_relX+m_relDX>1.0 )
    if( x0+dx0>aMaxSize.width() )
    {
        if( dFacX>1.0 )
        {
            m_relX = dFacX-m_relDX;
        }
        else
        {
            m_relDX = 1.0;
            m_relDY = m_relDX;
        }
        //m_relDX = 1.0-m_relX;
//        m_relDX = ((double)aSize.width()/(double)aMaxSize.width())-m_relX;
        m_relDX = 1.0;
//        m_relDY = ((double)aMaxSize.width()/(double)aSize.width())-1.0;
        m_relDY = m_relDX;
    }
    if( m_relY<0.0 )
    {
        m_relY = 0.0;
    }
    if( m_relY+m_relDY>1.0 )
    {
        m_relDY = 1.0-m_relY;
        m_relDX = m_relDY;
    }
// TODO gulp: wenn original bild anderes Ratio hat liegt rect nicht nur im Bereich 0 < x+dx < 1.0 !
}

void GraphicsItemResizeableRect::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
    int deltaX = event->pos().x()-event->lastPos().x();     // -1 <--     +1 -->
    int deltaY = event->pos().y()-event->lastPos().y();
    double dx = (double)deltaX/(double)m_aImageSize.width();
    double dy = (double)deltaY/(double)m_aImageSize.height();
//    double dFactor = (double)m_aImageSize.width()/(double)m_aImageSize.height();

//    cout << "MOVE " << deltaX << " " << deltaY << " state: " << (int)m_aResizeState << " " << dx << " " << dy << endl;
//    cout << " was " << m_relX << " " << m_relY << " " << m_relDX << " " << m_relDY << endl;

    bool bHandled = false;
    if( cursor().shape()==Qt::SizeHorCursor )
    {
        if( m_aResizeState==WEST )
        {
            m_relDX += -dx;
            m_relX += dx;
            m_relDY = m_relDX;
        }
        else
        {
            m_relDX += dx;
            //m_relX += dx;
            m_relDY = m_relDX;
        }
        CheckRectForClipping();
        Rescale(m_aImageSize);
        bHandled = true;
    }
    else if( cursor().shape()==Qt::SizeVerCursor )
    {
        if( m_aResizeState==NORTH )
        {
            m_relDY += -dy;
            m_relY += dy;
            m_relDX = m_relDY;
        }
        else
        {
            m_relDY += dy;
            //m_relY += dy;
            m_relDX = m_relDY;
        }
        CheckRectForClipping();
        Rescale(m_aImageSize);
        bHandled = true;
    }
    else
    {
        // handle move
        m_relX += dx;
        m_relY += dy;
    }

    if( m_pCallback )
    {
        m_pCallback->ItemModified( this );
    }

//    cout << " now " << m_relX << " " << m_relY << " " << m_relDX << " " << m_relDY << endl;

    if( !bHandled )
    {
        QGraphicsRectItem::mouseMoveEvent( event );
    }
}

void GraphicsItemResizeableRect::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
    //cout << "hover " << event->pos().x() << " " << event->pos().y() << " scene: " << event->scenePos().x() << " " << event->scenePos().y() << endl;
    QPointF pos = event->pos();
    double delta = 5;
    bool bWest = IsInRange( pos.x(), 0, delta );
    bool bNorth = IsInRange( pos.y(), 0, delta );
    if(  bWest || IsInRange( pos.x(), rect().width(), delta ) )
    {
        m_aResizeState = bWest ? WEST : EAST;
        setCursor( Qt::SizeHorCursor );
    }
    else if( bNorth || IsInRange( pos.y(), rect().height(), delta ) )
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

QVariant GraphicsItemResizeableRect::itemChange( GraphicsItemChange change, const QVariant & value )
{
    //cout << "==> ITEM CHANGE " << change << endl;
    if( change==QGraphicsItem::ItemPositionChange )
    {
        QPointF newPos = value.toPointF();

        //cout << "pos change" << endl;
        //cout << "--- " << scenePos().x() << " " << scenePos().y() << endl;
        //cout << "*** POS CHANGE " << newPos.x() << " " << newPos.y() << " old: " << pos().x() << " " << pos().y() << " scene: " << rect.x() << " " << rect.y() << " " << rect.width() << " " << rect.height() << " contains=" << rect.contains(value.toPointF()) << endl;

        bool bOk = true;
        double x = (double)(newPos.x())/(double)m_aImageSize.width();
        if( x<0.0 || x+m_relDX>1.0)
        {
            if( x<0.0 )
            {
                newPos.setX(0);
                m_relX = 0.0;
            }
            else
            {
                newPos.setX((1.0-m_relDX)*(double)m_aImageSize.width());
                m_relX = 1.0-m_relDX;
            }
            bOk = false;
        }
        double y = (double)(newPos.y())/(double)m_aImageSize.height();
        if( y<0.0 || y+m_relDY>1.0)
        {
            if( y<0.0 )
            {
                newPos.setY(0);
                m_relY = 0.0;
            }
            else
            {
                newPos.setY((1.0-m_relDY)*(double)m_aImageSize.height());
                m_relY = 1.0-m_relDY;
            }
            bOk = false;
        }

        if( !bOk )
        {
           return newPos;
        }
    }
    return QGraphicsRectItem::itemChange( change, value );
}

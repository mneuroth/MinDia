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
    : m_Ratio(RATIO_UNDEFINED),
      m_relX(0.0),
      m_relY(0.0),
      m_relDX(1.0),
      m_relDY(1.0),
      m_pCallback( pCallback )
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
    int dx = aSize.width();
    int dy = aSize.height();

// TODO --> hier noch das korrekte Ratio behandeln, falls image-ratio von desired-ratio abweicht verschiebt sich das Rechteck !

cout << "RESCALE data " << m_relX << " " << m_relY << " " << m_relDX << " " << m_relDY << " size " << dx << " " << dy << endl;
    setRect( Calc(m_relX,dx)/*+Calc(m_relDX,dx)/2*/, Calc(m_relY,dy)/*+Calc(m_relDY,dy)/2*/, Calc(m_relDX,dx), Calc(m_relDY,dy) );
cout << "--> " << Calc(m_relX,dx) << " " << Calc(m_relY,dy) << " " << Calc(m_relDX,dx) << " " << Calc(m_relDY,dy) << endl ;
    setPos(0,0);
cout << "--> rect pos= " << pos().x() << " " << pos().y() << endl;
}

void GraphicsItemResizeableRect::SetClippingData( ImageRatio ratio, double relX, double relY, double relDX, double relDY )
{
    m_Ratio = ratio;
    m_relX = relX;
    m_relY = relY;
    m_relDX = relDX;
    m_relDY = relDY;
cout << "*** set CLIP data " << m_relX << " " << m_relY << " " << m_relDX << " " << m_relDY << endl;
}

void GraphicsItemResizeableRect::GetClippingData( double & relX, double & relY, double & relDX, double & relDY )
{
    relX = m_relX;
    relY = m_relY;
    relDX = m_relDX;
    relDY = m_relDY;
cout << "+++ get CLIP data " << m_relX << " " << m_relY << " " << m_relDX << " " << m_relDY << endl;
}

void GraphicsItemResizeableRect::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
    cout << "MOVE " << event->pos().x() << " " << event->pos().y() << " scene: " << event->lastPos().x() << " " << event->lastPos().y() << endl;
    //cout << "glob MOVE " << event->scenePos().x() << " " << event->scenePos().y() << " scene: " << event->lastScenePos().x() << " " << event->lastScenePos().y() << endl;
    QPointF p = mapToItem( this, event->pos() );
    //cout << " xx " << p.x() << " " << p.y() << " " << (int)GetCurrentImageRatio() << endl;
    //scene()->activeWindow()->setCursor( Qt::SizeHorCursor );
    cout << " rect pos= " << pos().x() << " " << pos().y() << endl;

   // m_relX = 0.2;

    int deltaX = event->pos().x()-event->lastPos().x();
    int deltaY = event->pos().y()-event->lastPos().y();
    double dx = (double)deltaX/(double)m_aImageSize.width();
    double dy = (double)deltaY/(double)m_aImageSize.height();
    m_relX += dx;
    m_relY += dy;
    cout << "DELTA MOVE " << dx << " " << dy << endl;
/*
    m_relX = (double)pos().x()/(double)m_aImageSize.width();
    m_relY = (double)pos().y()/(double)m_aImageSize.height();
    m_relDX = (double)rect().width()/(double)m_aImageSize.width();
    m_relDY = (double)rect().height()/(double)m_aImageSize.height();
*/

    QGraphicsRectItem::mouseMoveEvent( event );
}

void GraphicsItemResizeableRect::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
    //cout << "hover " << event->pos().x() << " " << event->pos().y() << " scene: " << event->scenePos().x() << " " << event->scenePos().y() << endl;
    QPointF pos = event->pos();
    double delta = 5;
    if( IsInRange( pos.x(), 0, delta ) || IsInRange( pos.x(), rect().width(), delta ) )
    {
        setCursor( Qt::SizeHorCursor );
    }
    else if( IsInRange( pos.y(), 0, delta ) || IsInRange( pos.y(), rect().height(), delta ) )
    {
        setCursor( Qt::SizeVerCursor );
    }
    else
    {
        setCursor( Qt::ArrowCursor );
    }
    QGraphicsRectItem::hoverMoveEvent( event );
}

QVariant GraphicsItemResizeableRect::itemChange( GraphicsItemChange change, const QVariant & value )
{
    //cout << "==> ITEM CHANGE " << change << endl;
    if( change==QGraphicsItem::ItemPositionChange )
    {
        // update the new rectangle data
        /*
        m_relX = (double)pos().x()/(double)m_aImageSize.width();
        m_relY = (double)pos().y()/(double)m_aImageSize.height();
        m_relDX = (double)rect().width()/(double)m_aImageSize.width();
        m_relDY = (double)rect().height()/(double)m_aImageSize.height();
        */
cout << "ITEM change " << m_relX << " " << m_relY << " " << m_relDX << " " << m_relDY << endl;
        if( m_pCallback )
        {
            m_pCallback->ItemModified( this );
        }
    }
    return QGraphicsRectItem::itemChange( change, value );
}

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

#ifndef GRAPHICSITEMRESIZEABLERECT_H
#define GRAPHICSITEMRESIZEABLERECT_H

#include <QGraphicsRectItem>

#include "misctools.h"

enum ResizeState
{
    NONE,
    NORTH,
    SOUTH,
    WEST,
    EAST
};

class GraphicsItemChangedCallback
{
public:
    virtual void ItemModified( QGraphicsItem * pItem ) = 0;
};

class GraphicsItemResizeableRect : public QGraphicsRectItem
{
public:
    GraphicsItemResizeableRect( GraphicsItemChangedCallback * pCallback = 0 );

    void Rescale( QSize aImageSize );
    void SetClippingData( ImageRatio ratio, double relX, double relY, double relDX, double relDY );
    void GetClippingData( double & relX, double & relY, double & relDX, double & relDY );

    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
    virtual void hoverMoveEvent( QGraphicsSceneHoverEvent * event );

    virtual QVariant itemChange( GraphicsItemChange change, const QVariant & value );

private:
    void CheckRectForClipping();

    ImageRatio                      m_Ratio;
    double                          m_relX;
    double                          m_relY;
    double                          m_relDX;
    double                          m_relDY;
    GraphicsItemChangedCallback *   m_pCallback;
    QSize                           m_aImageSize;
    ResizeState                     m_aResizeState;
};

#endif // GRAPHICSITEMRESIZEABLERECT_H

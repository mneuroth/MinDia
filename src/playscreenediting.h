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

#ifndef PLAYSCREENEDITING_H
#define PLAYSCREENEDITING_H

#include <QGraphicsScene>

#include "graphicsitemresizeablerect.h"

class PlayScreenEditing : public QGraphicsScene, public GraphicsItemChangedCallback
{
    Q_OBJECT

public:
    explicit PlayScreenEditing(QObject *parent = 0);
    
    void SetBackgroundImage( const QString & sImageFileName );

    void SetClippingData( double relX, double relY, double relDX, double relDY );
    void GetClippingData( double & relX, double & relY, double & relDX, double & relDY );

    // implements GraphicsItemChangedCallback
    virtual void ItemModified( QGraphicsItem * pItem );

signals:
    void sigDataChanged();
    
public slots:
    void sltSceneRectChanged( const QRectF & rect );

private:
    void Rescale();

    QString                       m_sImageFileName;
    QGraphicsPixmapItem *         m_pBackgroundImage;
    GraphicsItemResizeableRect *  m_pClipRange;
};

#endif // PLAYSCREENEDITING_H

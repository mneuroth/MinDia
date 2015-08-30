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

#include "playscreenediting.h"

#include "misctools.h"

#include <QGraphicsSimpleTextItem>

PlayScreenEditing::PlayScreenEditing(QObject *parent) :
    QGraphicsScene(parent),
    m_pBackgroundImage( 0 )
{
    m_pClipRange = new GraphicsItemResizeableRect( this );
    m_pClipRange->SetClippingData( GetCurrentImageRatio(), 0, 0, 1, 1 );    // default values
    m_pClipRange->hide();
    addItem(m_pClipRange);

    m_pClipRangeEnd = new GraphicsItemResizeableRect( this );
    m_pClipRangeEnd->SetClippingData( GetCurrentImageRatio(), 0, 0, 1, 1 );    // default values
    m_pClipRangeEnd->setPen(QPen(QColor("yellow")));
    m_pClipRangeEnd->hide();
    addItem(m_pClipRangeEnd);

    connect(this,SIGNAL(sceneRectChanged(QRectF)),this,SLOT(sltSceneRectChanged(QRectF)));
}

void PlayScreenEditing::ItemModified( QGraphicsItem * /*pItem*/ )
{
    emit sigDataChanged();
}

void PlayScreenEditing::sltSceneRectChanged( const QRectF & /*rect*/ )
{
    Rescale();
}

void PlayScreenEditing::sltSetDarkRectangle( bool bValue )
{
    QColor aNewColor;
    if( bValue )
    {
        aNewColor = QColor(0,0,0);
    }
    else
    {
        aNewColor = QColor(255,255,255);
    }
    m_pClipRange->setPen( QPen( aNewColor ) );
    //m_pClipRangeEnd->setPen( QPen( aNewColor ) );

    update();
}

void PlayScreenEditing::SetBackgroundImage( const QString & sImageFileName )
{
    m_sImageFileName = sImageFileName;
    Rescale();
}

void PlayScreenEditing::SetClippingData( double relX, double relY, double relDX, double relDY )
{
    m_pClipRange->SetClippingData( GetCurrentImageRatio(), relX, relY, relDX, relDY );
    m_pClipRange->show();
    Rescale();
}

void PlayScreenEditing::SetClippingEndData( double relX, double relY, double relDX, double relDY )
{
    m_pClipRangeEnd->SetClippingData( GetCurrentImageRatio(), relX, relY, relDX, relDY );
    if( relX<0 && relY<0 )
    {
        m_pClipRangeEnd->hide();
    }
    else
    {
        m_pClipRangeEnd->show();
    }
    Rescale();
}

void PlayScreenEditing::GetClippingData( double & relX, double & relY, double & relDX, double & relDY )
{
    m_pClipRange->GetClippingData( relX, relY, relDX, relDY );
}

void PlayScreenEditing::GetClippingEndData( double & relX, double & relY, double & relDX, double & relDY )
{
    m_pClipRangeEnd->GetClippingData( relX, relY, relDX, relDY );
}

void PlayScreenEditing::Rescale()
{
// TODO --> noch mal bild gelesen !
    QImage aImage = ReadQImageOrEmpty( m_sImageFileName, this->width(), this->height() );
    QPixmap aPixmap;
    aPixmap.convertFromImage( aImage );
    if( m_pBackgroundImage!=0 )
    {
        removeItem( m_pBackgroundImage );
        delete m_pBackgroundImage;
    }
    m_pBackgroundImage = addPixmap( aPixmap.scaled( this->width(), this->height(), Qt::KeepAspectRatio ) );
    m_pBackgroundImage->show();
    m_pBackgroundImage->stackBefore( m_pClipRange );
    // update rectangle
    int dx = m_pBackgroundImage->boundingRect().size().width();
    int dy = m_pBackgroundImage->boundingRect().size().height();
    m_pClipRange->Rescale(QSize(dx,dy));
    m_pClipRangeEnd->Rescale(QSize(dx,dy));
    update();
}

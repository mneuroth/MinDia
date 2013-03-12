#include "dynamictextitem.h"

#include <QGraphicsScene>
#include <QGraphicsWidget>

#include <iostream>

using namespace std;

DynamicTextItem::DynamicTextItem( DynamicItemChangedCallback * pCallback )
    : m_pCallback(pCallback)
{
}

DynamicTextItem::~DynamicTextItem()
{
}

void DynamicTextItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
    //cout << "move " << /*event->pos().*/scenePos().x() << "/" << x() << " " << /*event->pos().*/scenePos().y() << "/" << y() << endl;
    //cout << "scene " << this->scene()->width() << " " << this->scene()->height() << endl;
    QGraphicsSimpleTextItem::mouseMoveEvent(event);
    if( m_pCallback )
    {
        m_pCallback->updated();
    }
}

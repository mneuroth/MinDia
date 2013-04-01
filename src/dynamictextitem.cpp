#include "dynamictextitem.h"

#include <QGraphicsScene>
#include <QGraphicsWidget>

DynamicTextItem::DynamicTextItem( DynamicItemChangedCallback * pCallback )
    : m_pCallback(pCallback)
{
}

DynamicTextItem::~DynamicTextItem()
{
}

void DynamicTextItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
    QGraphicsSimpleTextItem::mouseMoveEvent(event);
    if( m_pCallback )
    {
        m_pCallback->updated();
    }
}

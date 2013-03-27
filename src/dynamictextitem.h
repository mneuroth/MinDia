#ifndef DYNAMICTEXTITEM_H
#define DYNAMICTEXTITEM_H

#include <QGraphicsSimpleTextItem>

class DynamicItemChangedCallback
{
public:
    virtual ~DynamicItemChangedCallback() {}

    virtual void updated() = 0;
};

class DynamicTextItem : public QGraphicsSimpleTextItem
{
public:
    DynamicTextItem( DynamicItemChangedCallback * pCallback );
    virtual ~DynamicTextItem();

    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * event );

private:
    DynamicItemChangedCallback *    m_pCallback;
};

#endif // DYNAMICTEXTITEM_H

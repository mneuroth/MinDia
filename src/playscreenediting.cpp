#include "playscreenediting.h"

#include "misctools.h"

#include <QGraphicsSimpleTextItem>

PlayScreenEditing::PlayScreenEditing(QObject *parent) :
    QGraphicsScene(parent),
    m_pBackgroundImage( 0 )
{
    m_pClipRange = new GraphicsItemResizeableRect( this );
    m_pClipRange->SetClippingData( GetCurrentImageRatio(), 0, 0, 1, 1 );    // default values
    m_pClipRange->show();
    addItem(m_pClipRange);

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

void PlayScreenEditing::GetClippingData( double & relX, double & relY, double & relDX, double & relDY )
{
    m_pClipRange->GetClippingData( relX, relY, relDX, relDY );
}

void PlayScreenEditing::Rescale()
{
    QImage aImage;
    if( !ReadQImage( m_sImageFileName, aImage ) )
    {
        aImage = CreateWhiteImage();
    }
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
    update();
}

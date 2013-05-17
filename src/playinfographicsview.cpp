
#include "playinfographicsview.h"

#include <QMenu>
#include <QList>
#include <QByteArray>
#include <QPicture>
#include <QImageWriter>
#include <QContextMenuEvent>

#include "playinfodlgimpl.h"

// *******************************************************************

PlayInfoGraphicsView::PlayInfoGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
}

void PlayInfoGraphicsView::SetDlg(PlayInfoDlgImpl * pDlg)
{
    m_pDlg = pDlg;
}

void PlayInfoGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    PlayInfoContextMenu menu(this,m_pDlg);
    menu.exec(event->globalPos());
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

PlayInfoContextMenu::PlayInfoContextMenu( QWidget * parent, PlayInfoDlgImpl * pMyDialog )
: QMenu( parent ),
  m_pMyDialog( pMyDialog )
{
    m_pImageFormats = new QMenu( tr( "Save &image..." ), 0 );

    // ** get all supported image formates and fill the menu with this formats
    QList<QByteArray> aList = QImageWriter::supportedImageFormats();
    for( int i=0; i<aList.count(); i++ )
    {
        m_pImageFormats->addAction( new QAction( QString(aList.at( i )), 0 ) );
    }
    connect( m_pImageFormats, SIGNAL( triggered(QAction*) ), this, SLOT( sltImageFormatTriggered(QAction*) ) );

    QAction * pAction = new QAction( tr( "Dissolve &Start/Stop" ), 0 );
    addAction( pAction );
    connect( pAction, SIGNAL(triggered()), this, SLOT(sltStartStopFade()) );

    addMenu( m_pImageFormats);

    addSeparator();

    m_pActionFullScreen = new QAction( tr( "&Full screen" ), 0 );       // will be changed in sltShowMenu()
    addAction( m_pActionFullScreen );
    connect( m_pActionFullScreen, SIGNAL(triggered()), this, SLOT(sltToggleFullScreen()) );

    addSeparator();

    pAction = new QAction( tr( "Sta&rt" ), 0 );
    addAction( pAction );
    connect( pAction, SIGNAL(triggered()), m_pMyDialog, SLOT(sltPlay()) );
    pAction = new QAction( tr( "P&ause" ), 0 );
    addAction( pAction );
    connect( pAction, SIGNAL(triggered()), m_pMyDialog, SLOT(sltPause()) );
    pAction = new QAction( tr( "S&top" ), 0 );
    addAction( pAction );
    connect( pAction, SIGNAL(triggered()), m_pMyDialog, SLOT(sltStop()) );

    addSeparator();

    pAction = new QAction( tr( "&Close" ), 0 );
    addAction( pAction );
    connect( pAction, SIGNAL(triggered()), this, SLOT(sltClose()) );

    connect( this, SIGNAL( aboutToShow() ), this, SLOT( sltShowMenu() ) );
}

PlayInfoContextMenu::~PlayInfoContextMenu()
{
    delete m_pImageFormats;
    delete m_pActionFullScreen;
}

void PlayInfoContextMenu::sltImageFormatTriggered( QAction * pAction )
{
    m_pMyDialog->sltSaveActImage( pAction->text() );
}

void PlayInfoContextMenu::sltStartStopFade()
{
    m_pMyDialog->StartStopFade();
}

void PlayInfoContextMenu::sltToggleFullScreen()
{
    if( !m_pMyDialog->IsFullScreen() )
    {
        //m_pMyDialog->showFullScreen();
        m_pMyDialog->FullScreen();
    }
    else
    {
        //m_pMyDialog->showNormal();
        m_pMyDialog->RestoreSize();
    }
}

void PlayInfoContextMenu::sltClose()
{
    m_pMyDialog->close();
}

void PlayInfoContextMenu::sltShowMenu()
{
    if( m_pMyDialog )
    {
// TODO --> IsFullScreen() funktioniert anscheinend nicht korrekt ! --> Windows ? 4.7.3 ?, OK fuer Mac 4.8.1
        if( m_pMyDialog->IsFullScreen() )
        {
            m_pActionFullScreen->setText( tr("&Normal") );
        }
        else
        {
            m_pActionFullScreen->setText( tr("&Full screen") );
        }
    }
}


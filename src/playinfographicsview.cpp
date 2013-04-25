
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
// TODO --> funktioniert das ?
    connect( m_pImageFormats, SIGNAL( activated(int) ), this, SLOT( sltImageFormatActivated(int) ) );

    QAction * pAction = new QAction( tr( "Dissolve &Start/Stop" ), 0 );
    addAction( pAction );
    connect( pAction, SIGNAL(triggered()), this, SLOT(sltStartStopFade()) );

    addMenu( m_pImageFormats);

    addSeparator();

    m_pActionFullScreen = new QAction( tr( "&Full screen" ), 0 );
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

    //insertItem( tr( "Dissolve &Start/Stop" ), STOP_START_ID ) );
    //insertItem( tr( "Save &image..." ), m_pImageFormats );
    //insertSeparator();
    //insertItem( tr( "&Full screen" ), MAXIMIZE_ID );
    //insertSeparator();
    //insertItem( tr( "&Close" ), CLOSE_ID );

    connect( this, SIGNAL( aboutToShow() ), this, SLOT( sltShowMenu() ) );
    //connect( this, SIGNAL( activated(int) ), this, SLOT( sltActivated(int) ) );
}

PlayInfoContextMenu::~PlayInfoContextMenu()
{
    delete m_pImageFormats;
    delete m_pActionFullScreen;
// TODO ---> andere actions auch zerstoreren ?
}

void PlayInfoContextMenu::sltImageFormatActivated( int iIndex )
{
    QStringList aList = QPicture::outputFormatList();

    if( m_pMyDialog && (iIndex>=0) && (iIndex<(int)aList.count()) )
    {
        m_pMyDialog->sltSaveActImage( aList.at( iIndex ) );
    }
}

//void PlayInfoContextMenu::sltActivated( int iIndex )
//{
//	if( m_pMyDialog )
//	{
//		switch( iIndex )
//		{
//			case STOP_START_ID:
//				m_pMyDialog->StartStopFade();
//				break;
//			case MAXIMIZE_ID :
//				if( !m_pMyDialog->IsFullScreen() )
//				{
//					m_pMyDialog->FullScreen();
//				}
//				else
//				{
//					m_pMyDialog->RestoreSize();
//				}
//				break;
//			case CLOSE_ID :
//				m_pMyDialog->close();
//				break;
//		}
//	}
//}

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


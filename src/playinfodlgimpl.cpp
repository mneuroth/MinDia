/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/playinfodlgimpl.cpp,v $
 *
 *  $Revision: 1.8 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.7  2005/12/26 16:11:40  Michael
 *	added new command line option -expand
 *	
 *	Revision 1.6  2004/04/09 15:43:10  min
 *	Optimizations for Zaurus port.
 *	
 *	Revision 1.5  2004/04/09 15:20:54  min
 *	removed inline for _FadeImage()
 *
 *	Revision 1.4  2004/02/20 20:11:07  min
 *	Bugfixes and fullscreen button implemented.
 *	
 *	Revision 1.3  2004/02/16 19:44:51  min
 *	Fixes for Borland C++
 *	
 *	Revision 1.2  2003/10/26 17:32:39  min
 *	Directory for images added.
 *	
 *	Revision 1.1.1.1  2003/08/15 16:38:22  min
 *	Initial checkin of MinDia Ver. 0.97.1
 *	
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2002 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "playinfodlgimpl.h"

#include "appconfig.h"

#include "qtmtlock.h"
#include <QKeyEvent>
#include <QPixmap>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QShowEvent>
#include <QCloseEvent>
#include <QImageWriter>
#include <QImage>
#include <QPicture>

#include "misctools.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>
#include <qcheckbox.h> 
#include <qradiobutton.h> 
#include <qpainter.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qdatetime.h>

#include <QFileDialog>

#include "icons.xpm"

//#define _ORIGINAL_FADE

#define MAX_FADE_DELAY  1   // in ms
#define MAX_FADE_FACTOR 255

static int g_iTimerDelay = MAX_FADE_DELAY;


// help function to create a image with a white background
static QImage CreateWhiteImage()
{
	const int c_iWidth = 3;
	const int c_iHeight = 3;

	QImage aImage( c_iWidth, c_iHeight, /*32*/QImage::Format_RGB32 );

	for( int y=0; y<c_iHeight; y++ )
	{
		uchar * pLine = aImage.scanLine( y );

		for( int x=0; x<c_iWidth*4; x=x+4 )
		{
			pLine[x] = 255;
			pLine[x+1] = 255;
			pLine[x+2] = 255;
			pLine[x+3] = 0;
		}
	}

	return aImage;
}

//// *******************************************************************
//// *******************************************************************
//// *******************************************************************

//PlayInfoContextMenu::PlayInfoContextMenu( QWidget * parent, PlayInfoDlgImpl * pMyDialog )
//: QMenu( parent ),
//  m_pMyDialog( pMyDialog )
//{
//	m_pImageFormats = new QMenu( tr( "Save &image..." ), 0 );

//	// ** get all supported image formates and fill the menu with this formats
//    QList<QByteArray> aList = QImageWriter::supportedImageFormats();
//	for( int i=0; i<aList.count(); i++ )
//	{
//		m_pImageFormats->addAction( new QAction( QString(aList.at( i )), 0 ) );
//	}
//// TODO --> funktioniert das ?
//	connect( m_pImageFormats, SIGNAL( activated(int) ), this, SLOT( sltImageFormatActivated(int) ) );

//    QAction * pAction = new QAction( tr( "Dissolve &Start/Stop" ), 0 );
//	addAction( pAction );
//    connect( pAction, SIGNAL(activated()), this, SLOT(sltStartStopFade()) );

//    addMenu( m_pImageFormats);
    
//    addSeparator();
    
//    m_pActionFullScreen = new QAction( tr( "&Full screen" ), 0 );
//	addAction( m_pActionFullScreen );
//    connect( m_pActionFullScreen, SIGNAL(activated()), this, SLOT(sltToggleFullScreen()) );

//    addSeparator();

//    pAction = new QAction( tr( "&Close" ), 0 );
//	addAction( pAction );
//    connect( pAction, SIGNAL(activated()), this, SLOT(sltClose()) );
    
//    //insertItem( tr( "Dissolve &Start/Stop" ), STOP_START_ID ) );
//	//insertItem( tr( "Save &image..." ), m_pImageFormats );
//	//insertSeparator();
//	//insertItem( tr( "&Full screen" ), MAXIMIZE_ID );
//	//insertSeparator();
//	//insertItem( tr( "&Close" ), CLOSE_ID );

//	connect( this, SIGNAL( aboutToShow() ), this, SLOT( sltShowMenu() ) );
//	//connect( this, SIGNAL( activated(int) ), this, SLOT( sltActivated(int) ) );
//}

//PlayInfoContextMenu::~PlayInfoContextMenu()
//{
//	delete m_pImageFormats;
//    delete m_pActionFullScreen;
//// TODO ---> andere actions auch zerstoreren ?
//}

//void PlayInfoContextMenu::sltImageFormatActivated( int iIndex )
//{
//	QStringList aList = QPicture::outputFormatList();

//	if( m_pMyDialog && (iIndex>=0) && (iIndex<(int)aList.count()) )
//	{
//		m_pMyDialog->sltSaveActImage( aList.at( iIndex ) );
//	}
//}

////void PlayInfoContextMenu::sltActivated( int iIndex )
////{
////	if( m_pMyDialog )
////	{
////		switch( iIndex )
////		{
////			case STOP_START_ID:
////				m_pMyDialog->StartStopFade();
////				break;
////			case MAXIMIZE_ID :
////				if( !m_pMyDialog->IsFullScreen() )
////				{
////					m_pMyDialog->FullScreen();
////				}
////				else
////				{
////					m_pMyDialog->RestoreSize();
////				}
////				break;
////			case CLOSE_ID :
////				m_pMyDialog->close();
////				break;
////		}
////	}
////}

//void PlayInfoContextMenu::sltStartStopFade()
//{
//	m_pMyDialog->StartStopFade();
//}

//void PlayInfoContextMenu::sltToggleFullScreen()
//{
//	if( !m_pMyDialog->IsFullScreen() )
//    {
//		m_pMyDialog->FullScreen();
//	}
//	else
//	{
//		m_pMyDialog->RestoreSize();
//	}
//}

//void PlayInfoContextMenu::sltClose()
//{
//	m_pMyDialog->close();
//}

//void PlayInfoContextMenu::sltShowMenu()
//{
//	if( m_pMyDialog )
//	{
//		if( m_pMyDialog->IsFullScreen() )
//		{
//            m_pActionFullScreen->setText( tr("&Normal") );
//		}
//		else
//		{
//            m_pActionFullScreen->setText( tr("&Full screen") );
//		}
//	}
//}

// *******************************************************************
// *******************************************************************
// *******************************************************************

//class MenuCanvasView : public QGraphicsView // Q3CanvasView
//{
//public:
//    MenuCanvasView( /*Q3Canvas*/QGraphicsScene * viewing=0, QWidget * parent=0, const char * name=0, Qt::WFlags f=0 );
//	~MenuCanvasView();

//    void SetImagePtr( QImage * pImage )
//    {
//        m_pImage = pImage;
//    }

//    // ** WARNING: takes the ownership of the given pointer !!!
//	void SetPopupMenu( QMenu * pMenu );

//protected:
//	virtual void contentsMousePressEvent( QMouseEvent * pEvent );

//protected:
//    virtual void drawBackground( QPainter &, const QRect & area );

//private:
//    QMenu *         m_pPopupMenu;
//    QImage *        m_pImage;
//};

//MenuCanvasView::MenuCanvasView( /*Q3Canvas*/QGraphicsScene * viewing, QWidget * parent, const char * name, Qt::WFlags f )
//: /*Q3CanvasView*/QGraphicsView( viewing, parent/*, name, f*/ ),
//  m_pPopupMenu( 0 ),
//  m_pImage( 0 )

//{
//}

//MenuCanvasView::~MenuCanvasView()
//{
//	delete m_pPopupMenu;
//}

//void MenuCanvasView::SetPopupMenu( QMenu * pMenu )
//{
//	m_pPopupMenu = pMenu;
//}

//void MenuCanvasView::contentsMousePressEvent( QMouseEvent * pEvent )
//{
//	if( (pEvent->button() == Qt::RightButton) && m_pPopupMenu )
//	{
//		m_pPopupMenu->exec( pEvent->globalPos() );
//	}
//	else
//	{
////TODO        /*Q3CanvasView*/QGraphicsView::contentsMousePressEvent( pEvent );
//	}
//}

//void MenuCanvasView::drawBackground( QPainter & aPainter, const QRect & area )
//{
//    // ** OPTIMIZE HERE: erase is not needed all the time !?
//    aPainter.eraseRect( area );

//    aPainter.drawImage( 0, 0, *m_pImage );
//}

// *******************************************************************
// *******************************************************************
// *******************************************************************

void _FadeImage( QPainter * pPainter, const QRectF & area, int iFadeFactor, const QImage & aImagePrevious, const QImage & aImage )
{
    double dFactor = (double)(iFadeFactor)/(double)MAX_FADE_FACTOR;
    double currentOpacity = pPainter->opacity();

    pPainter->setRenderHint(QPainter::Antialiasing);

    pPainter->setOpacity(1.0-dFactor);

// TODO --> klaeren ob hier image erst scaliert werden sollen
    pPainter->drawImage(area,aImagePrevious);
    pPainter->setOpacity(dFactor);
    pPainter->drawImage(area,aImage);

    pPainter->setOpacity(currentOpacity);
}

class SimpleBitmapCanvas : public QGraphicsScene
{
public:
    SimpleBitmapCanvas( QObject * parent = 0 )
    : QGraphicsScene( parent ),
      m_iFadeFactor( 0 ),
      m_pImage( 0 ),
      m_pImagePrevious( 0 )
	{
	}

	void SetImagePtr( QImage * pImage )
	{
		m_pImage = pImage;
	}

    void SetImagePreviousPtr( QImage * pImage )
    {
        m_pImagePrevious = pImage;
    }

    void SetFadeFactor( int iFadeFactor )
    {
        m_iFadeFactor = iFadeFactor;
    }

protected:
    virtual void drawBackground( QPainter *, const QRectF & area );

private:
    int             m_iFadeFactor;
	QImage *		m_pImage;
    QImage *        m_pImagePrevious;
};

#include <QPaintEngine>

void SimpleBitmapCanvas::drawBackground( QPainter * pPainter, const QRectF & area )
{
//    cout << "DRAW paintEngine " << pPainter->paintEngine() << " feat=" << (int) pPainter->paintEngine()->type() << endl;
    _FadeImage(pPainter,area,m_iFadeFactor,*m_pImagePrevious,*m_pImage);
}

// *******************************************************************
// *******************************************************************
// *******************************************************************
#ifdef xx
typedef int myint;

/* for lookup table tests:

typedef int byte;

byte oben[65536];
byte unten[65536];

int init_cache()
{
    for (int mask = 0; mask < 256; mask++) {
      for (int farbe = 0; farbe < 256; farbe++) {
        oben[(mask << 8) + farbe]  = (farbe * (mask+1)) >> 8;
        unten[(mask << 8) + farbe] = (farbe * (256-mask)) >> 8;
      }
    }
    return 0;
}

static int g_iTest = init_cache();
*/

/** factor == 0..255 --> Image1..Image2
  * IMPORTANT: both images MUST have the same size ! */
QImage _FadeImage( const QImage & aImage1, const QImage & aImage2, int iFactor )
{
    int iWidth = aImage1.width();
    int iHeight = aImage1.height();
    //int iColors = aImage1.numColors();	// nur falls palette verwendet wird !
    //int iDeep = aImage1.depth();
    int iBytes = aImage1.numBytes();
    //int iLinie = aImage1.bytesPerLine();
    //QImage::Endian aVal = aImage1.bitOrder();
    //bool bHasAlphaBuff = aImage1.hasAlphaBuffer();

    //QImage aRet( iWidth, iHeight, iDeep, 0, aVal );
    QImage aRet( iWidth, iHeight, aImage1.format() );
    uchar * pResultBuffer = aRet.bits();

    const uchar * pBuffer1 = aImage1.bits();

    //QImage aImg2 = aImage2.smoothScale( iWidth, iHeight );
    const uchar * pBuffer2 = aImage2.bits();

    if( pBuffer1 && pBuffer2 )
    {
        myint iFactor1 = iFactor;
        myint iFactor2 = MAX_FADE_FACTOR-iFactor;

        /* this algorithm seems to have the best performance ! */
        for( int i=0; i<iBytes; i=i+4 )
        {
            int i2 = i+1;
            int i3 = i+2;
            //int i4 = i+3;
            pResultBuffer[i]  = (uchar)(( ((myint)pBuffer1[i] )*iFactor1 + ((myint)pBuffer2[i] )*iFactor2 ) >> 8);
            pResultBuffer[i2] = (uchar)(( ((myint)pBuffer1[i2])*iFactor1 + ((myint)pBuffer2[i2])*iFactor2 ) >> 8);
            pResultBuffer[i3] = (uchar)(( ((myint)pBuffer1[i3])*iFactor1 + ((myint)pBuffer2[i3])*iFactor2 ) >> 8);
            //pResultBuffer[i4] = (uchar)(( ((myint)pBuffer1[i4])*iFactor1 + ((myint)pBuffer2[i4])*iFactor2 ) >> 8);
        }

// ****************************************************************
/*
        // Performance remark: fading 548x360 images takes ca. 60-70ms
        // (best !) nur 3 bytes behandeln (mit >> 8): 15-20 ms
        //                                --> d.h. ca 81 ms fuer 1024x768 step fade time
        //								  scalen dauert ca. 110 ms, d.h. erst scalen und dann faden !
        // lookup-tabelle: 20-25 ms

        for( int i=0; i<iBytes;  )
        {
            //int i2 = i+1;
            //int i3 = i+2;
            //int i4 = i+3;
            pResultBuffer[i] = (uchar)(( ((myint)pBuffer1[i])*iFactor1 + ((myint)pBuffer2[i])*iFactor2 ) >> 8);
            i++;
            pResultBuffer[i] = (uchar)(( ((myint)pBuffer1[i])*iFactor1 + ((myint)pBuffer2[i])*iFactor2 ) >> 8);
            i++;
            pResultBuffer[i] = (uchar)(( ((myint)pBuffer1[i])*iFactor1 + ((myint)pBuffer2[i])*iFactor2 ) >> 8);
            i++;
            //pResultBuffer[i] = (uchar)(( ((myint)pBuffer1[i])*iFactor1 + ((myint)pBuffer2[i])*iFactor2 ) >> 8);
            i++;
        }
*/
/*
        iFactor1 = iFactor << 8;
        for( int i=0; i<iBytes; i=i+4 )
        {
            int i2 = i+1;
            int i3 = i+2;
            //int i4 = i+3;
            pResultBuffer[i] = (uchar)(oben[ iFactor1 + pBuffer1[i] ] + unten[ iFactor1 + pBuffer2[i] ]);
            pResultBuffer[i2] = (uchar)(oben[ iFactor1 + pBuffer1[i2] ] + unten[ iFactor1 + pBuffer2[i2] ]);
            pResultBuffer[i3] = (uchar)(oben[ iFactor1 + pBuffer1[i3] ] + unten[ iFactor1 + pBuffer2[i3] ]);
            //pResultBuffer[i4] = (uchar)(oben[ iFactor1 + pBuffer1[i4] ] + unten[ iFactor1 + pBuffer2[i4] ]);
        }
*/
/*
        // Performance remark: fading 548x360 images takes ca. 50-60ms
        // normal:				46-51 ms
        // >> 8 anstatt / 255:	22-30 ms
        // short anstatt int:	24-30 ms
        // lookup table:		23-30 ms
        //iFactor1 = iFactor << 8;
        for( int i=0; i<iBytes; i++ )
        {
            //pResultBuffer[i] = (uchar)(( ((myint)pBuffer1[i])*iFactor1 + ((myint)pBuffer2[i])*iFactor2 ) / 255);
            pResultBuffer[i] = (uchar)(( ((myint)pBuffer1[i])*iFactor1 + ((myint)pBuffer2[i])*iFactor2 ) >> 8);
            //pResultBuffer[i] = oben[ iFactor1 + pBuffer1[i] ] + unten[ iFactor1 + pBuffer2[i] ];
        }
*/
// ****************************************************************

        return aRet;
    }
    else
    {
        if( pBuffer1 )
        {
            return aImage1;
        }
        else
        {
            return aImage2;
        }
    }
}
#endif

// *******************************************************************

PlayInfoDlgImpl::PlayInfoDlgImpl( QObject * pShowControler, QWidget * parent, Qt::WFlags fl )
: QDialog( parent, fl /*| Qt::WStyle_Maximize*/ ),      // TODO style for maximize ?
  m_pParent( parent ),
  m_iFadeInTimeInMS( 0 ),
  m_aFadeTime( 1 ),
  m_pFadeInTimer( 0 )
{
    setupUi(this);

	QPixmap aPauseIcon( pausescript );
	QPixmap aStopIcon( stopscript );
	QPixmap aRunIcon( runscript );
	QPixmap aFullscreenIcon( fullscreen );
	m_pRun->setIcon( aRunIcon );	
	m_pStop->setIcon( aStopIcon );
	m_pPause->setIcon( aPauseIcon );
	m_pFullScreen->setIcon( aFullscreenIcon );

	m_pFadeInTimer = new QTimer( this );
	connect( m_pFadeInTimer, SIGNAL( timeout() ), this, SLOT( sltFadeInTimer() ) );

    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltPlayInfoDialogClosed() ) );
    connect( this, SIGNAL( sigDocumentUpdate() ), parent, SLOT( sltDoDocumentStateUpdate() ) );
    connect( this, SIGNAL( sigDialogHelp(const QString &) ), parent, SLOT( sltShowHelp(const QString &) ) );

    connect( this, SIGNAL( sigDoPlay() ), pShowControler, SLOT( sltPlayPresentation() ) );
    connect( this, SIGNAL( sigDoPause() ), pShowControler, SLOT( sltPausePresentation() ) );
    connect( this, SIGNAL( sigDoStop() ), pShowControler, SLOT( sltStopPresentation() ) );

    m_pCanvas = new SimpleBitmapCanvas( /*m_pFrame*/0 );

    QRect aFrameRect = m_pCanvasView->frameRect();
    // ** update size of the canvas
    m_pCanvas->setSceneRect(0,0,aFrameRect.width(), aFrameRect.height());

//	m_pCanvasView->SetPopupMenu( new PlayInfoContextMenu( this, this ) );
// TODO --> context Menu am Canvas/Graphics Scene setzen !
    m_pCanvasView->setScene(m_pCanvas);
    m_pCanvasView->show();

	m_pDisplayImage->setChecked( true );
	//m_pScaleExpand->setChecked( true );		// neu seit 15.2.2003
    //m_pScaleOriginal->setChecked( true );

    m_pCanvas->SetImagePtr( &m_aScaledImage );
    m_pCanvas->SetImagePreviousPtr( &m_aScaledImagePrevious );

    m_pCanvasView->SetDlg(this);
}

PlayInfoDlgImpl::~PlayInfoDlgImpl()
{
	m_pFadeInTimer->stop();
	delete m_pFadeInTimer;

	// ** erase all canvas items before deleting the canvas --> dangling references !
	m_aItemContainer.erase( m_aItemContainer.begin(), m_aItemContainer.end() );

	// ** will be done from Qt !
	//delete m_pCanvasView;
	//delete m_pCanvas;
}

void PlayInfoDlgImpl::UpdateStatus( bool bIsPlaying, bool bIsPause )
{
	m_pRun->setEnabled( !bIsPlaying );
	m_pPause->setEnabled( bIsPlaying && !bIsPause );
	m_pStop->setEnabled( bIsPlaying || bIsPause );
}

bool PlayInfoDlgImpl::IsFullScreen() const
{
    return isFullScreen();
}

// TODO improve handling --> dies ist temporaer !!!
//static QByteArray g_test;

void PlayInfoDlgImpl::FullScreen()
{
//    g_test = this->saveGeometry();
    //if( !IsFullScreen() )
	{
		// ** for full screen modus remove the controls in the control area
//TODO		PlayInfoDialogLayout->setSpacing( 0 );
//TODO		PlayInfoDialogLayout->setMargin( 0 );

		m_pDisplayImage->hide();
		m_pRun->hide();
		m_pPause->hide();
		m_pStop->hide();
		m_pFullScreen->hide();
		m_pScaleImageGroup->hide();
		m_pClose->hide();
        m_pOperationGroup->hide();

        this->layout()->setContentsMargins(0,0,0,0);
//        this->layout()->setSpacing(0);
        m_aScreenGroup->setContentsMargins(0,0,0,0);
        m_aScreenGroup->layout()->setContentsMargins(0,0,0,0);
//        m_aScreenGroup->layout()->setSpacing(0);
//TODO		delete m_pButtonLayout;
//TODO		m_pButtonLayout = 0;

		//m_pCanvasView->setLineWidth( 0 );
//		m_pCanvasView->setFrameShadow( QFrame::Plain );
		//m_pCanvasView->setFrameShape( QFrame::NoFrame );
		//m_pCanvasView->setFrameStyle( QFrame::NoFrame );

		// ** save flags and position before fullscreen modus
		// ** this values have to be restored later !
//TODO		m_flLastFlags = getWFlags();
		m_aLastPos = pos();

        showFullScreen();
//        this->repaint();
        this->resize(this->width(),this->height()+1);
//        m_pCanvasView->showFullScreen();
	}
}

void PlayInfoDlgImpl::RestoreSize()
{
    //if( IsFullScreen() )
	{
////TODO		m_pButtonLayout = new Q3HBoxLayout; 
//		// ** see PlayInfoDlg.cpp for the values **
////TODO		m_pButtonLayout->setSpacing( 4 );	// 6
////TODO		m_pButtonLayout->setMargin( 0 );
//
//		// anscheinend werden auch Sub-Layouts zerstoert
//		// wenn der Eltern-Layout-Container zerstoert wird !
//// TODO !!!
//		m_pLeftContainer = new Q3VBoxLayout( 0, 0, 6, "m_pLeftContainer");
//		m_pButtonContainer = new Q3HBoxLayout( 0, 1, 1, "m_pButtonContainer");
//		m_pButtonContainer->addWidget( m_pRun );
//		m_pButtonContainer->addWidget( m_pPause );
//		m_pButtonContainer->addWidget( m_pStop );
//		m_pButtonContainer->addWidget( m_pFullScreen );
//		m_pLeftContainer->addLayout( m_pButtonContainer );
//		m_pLeftContainer->addWidget( m_pDisplayImage );
//
//		m_pButtonLayout->addLayout( m_pLeftContainer );
//	    QSpacerItem* spacer1 = new QSpacerItem( 19, 15, QSizePolicy::Expanding, QSizePolicy::Minimum );
//	    m_pButtonLayout->addItem( spacer1 );
//		m_pButtonLayout->addWidget( m_pScaleImageGroup );
//	    QSpacerItem* spacer2 = new QSpacerItem( 16, 16, QSizePolicy::Expanding, QSizePolicy::Minimum );
//	    m_pButtonLayout->addItem( spacer2 );
//	    m_pButtonLayout->addWidget( m_pClose );
//		// ** see PlayInfoDlg.cpp for the values **
//	    PlayInfoDialogLayout->setSpacing( 2 );	// 6
//		PlayInfoDialogLayout->setMargin( 2 );	// 11
//	    PlayInfoDialogLayout->addLayout( m_pButtonLayout );
//
		m_pDisplayImage->show();
		m_pRun->show();
		m_pPause->show();
		m_pStop->show();
		m_pFullScreen->show();
		m_pScaleImageGroup->show();
		m_pClose->show();
        m_pOperationGroup->show();


//        m_aScreenGroup->layout()->setSpacing(6);
        m_aScreenGroup->layout()->setContentsMargins(3,3,3,3);
        m_aScreenGroup->setContentsMargins(6,6,6,6);
        this->layout()->setContentsMargins(6,6,6,6);
//        this->layout()->setSpacing(6);

        //m_pCanvasView->setLineWidth( 2 );
//		m_pCanvasView->setFrameShadow( Q3Frame::Sunken );

        showNormal();

//        this->restoreGeometry(g_test);

//        this->repaint();
        this->resize(this->width(),this->height()+1);

       // resize();
//        m_pCanvasView->showNormal();

		// ** the method showFullScreen changes the parent and the flags
		// ** so this values should be restored now !
//		reparent( m_pParent, m_flLastFlags, m_aLastPos, TRUE );
	}
}

bool PlayInfoDlgImpl::IsFading() const
{
	return m_pFadeInTimer->isActive();
}

void PlayInfoDlgImpl::StartStopFade()
{
	if( m_pFadeInTimer->isActive() )
	{
		m_pFadeInTimer->stop();
	}
	else
	{
		m_pFadeInTimer->start( 1 );
	}
}

bool PlayInfoDlgImpl::IsValid() const
{
	return true;
}

bool PlayInfoDlgImpl::Show()
{
	return isVisible();
}

bool PlayInfoDlgImpl::Hide()
{
	return !isVisible();
}

bool PlayInfoDlgImpl::Maximize()
{
	QtMTLock aMTLock;

	FullScreen();

	return true;
}

bool PlayInfoDlgImpl::Restore()
{
	QtMTLock aMTLock;

	RestoreSize();

	return true;
}

int PlayInfoDlgImpl::GetWidth() const
{
	QtMTLock aMTLock;

	return width();
}

int PlayInfoDlgImpl::GetHeight() const
{
	QtMTLock aMTLock;

	return height();
}

bool PlayInfoDlgImpl::SetSize( int iWidth, int iHeight )
{
	QtMTLock aMTLock;

	resize( iWidth, iHeight );

	return true;
}

bool PlayInfoDlgImpl::SetPos( int iXPos, int iYPos )
{
	QtMTLock aMTLock;

	move( iXPos, iYPos );

	return true;
}

int PlayInfoDlgImpl::GetDrawWidth() const
{
	QtMTLock aMTLock;

    return m_pCanvasView->width();
}

int PlayInfoDlgImpl::GetDrawHeight() const
{
	QtMTLock aMTLock;

    return m_pCanvasView->height();
}

bool PlayInfoDlgImpl::Clear()
{
	m_aItemContainer.erase( m_aItemContainer.begin(), m_aItemContainer.end() );

	{
		QtMTLock aMTLock;

		m_pCanvas->update();
	}

	return true;
}

bool PlayInfoDlgImpl::SetColor( int iRed, int iGreen, int iBlue )
{
	m_aActColor = QColor( iRed, iGreen, iBlue );

	return true;
}

bool PlayInfoDlgImpl::SetFont( const string & sFontName )
{
	QtMTLock aMTLock;

    m_aActFont.setFamily( sFontName.c_str() );

	return true;
}

bool PlayInfoDlgImpl::SetFontSize( int iSizeInPixel, bool bBold, bool bItalic )
{
	QtMTLock aMTLock;

	m_aActFont.setPixelSize( iSizeInPixel );
	m_aActFont.setBold( bBold );
	m_aActFont.setItalic( bItalic );

	return true;
}

int PlayInfoDlgImpl::SetTextXY( int x, int y, const string & sText )
{
	QtMTLock aMTLock;

    QGraphicsSimpleTextItem * pText = new QGraphicsSimpleTextItem();
    pText->setText(sText.c_str());
    m_pCanvas->addItem(pText);

	pText->setFont( m_aActFont );
    pText->setBrush( QBrush( m_aActColor ) );
    pText->setPos( x, y );
//	pText->setZ( 100 );
	pText->show();

	m_aItemContainer.push_back( CanvasItem( pText ) );

	m_pCanvas->update();

	return m_aItemContainer.size()-1;
}

int PlayInfoDlgImpl::GetTextCount() const
{
	return m_aItemContainer.size();
}

bool PlayInfoDlgImpl::MoveText( int iTextID, int x, int y )
{
	if( IsIndexOk( iTextID ) )
	{
		QtMTLock aMTLock;

		CanvasItem aItem = m_aItemContainer[ iTextID ];

        aItem->setPos( x, y );

		m_pCanvas->update();

		return true;
	}
	return false;
}

int PlayInfoDlgImpl::GetTextX( int iTextID ) const
{
	if( IsIndexOk( iTextID ) )
	{
		QtMTLock aMTLock;

		CanvasItem aItem = m_aItemContainer[ iTextID ];

		return (int)aItem->x();
	}
	return 0;
}

int PlayInfoDlgImpl::GetTextY( int iTextID ) const
{
	if( IsIndexOk( iTextID ) )
	{
		QtMTLock aMTLock;

		CanvasItem aItem = m_aItemContainer[ iTextID ];

		return (int)aItem->y();
	}
	return 0;
}

int PlayInfoDlgImpl::GetTextWidth( int iTextID ) const
{
	if( IsIndexOk( iTextID ) )
	{
		QtMTLock aMTLock;

		CanvasItem aItem = m_aItemContainer[ iTextID ];

        QRectF aRect = aItem->boundingRect();

		return aRect.width();
	}
	return 0;
}

int PlayInfoDlgImpl::GetTextHeight( int iTextID ) const
{
	if( IsIndexOk( iTextID ) )
	{
		QtMTLock aMTLock;

		CanvasItem aItem = m_aItemContainer[ iTextID ];

        QRectF aRect = aItem->boundingRect();

		return aRect.height();
	}
	return 0;
}

bool PlayInfoDlgImpl::SetTextColor( int iTextID, int iRed, int iGreen, int iBlue )
{
	if( IsIndexOk( iTextID ) )
	{
		QtMTLock aMTLock;

		CanvasItem aItem = m_aItemContainer[ iTextID ];

        QGraphicsSimpleTextItem * pText = (QGraphicsSimpleTextItem *)aItem.GetPtr();

		if( pText )
		{
            pText->setBrush( QColor( iRed, iGreen, iBlue ) );

			m_pCanvas->update();

			return true;
		}
	}
	return false;
}

IColor PlayInfoDlgImpl::GetTextColor( int iTextID ) const
{
	if( IsIndexOk( iTextID ) )
	{
		QtMTLock aMTLock;

		CanvasItem aItem = m_aItemContainer[ iTextID ];

        QGraphicsSimpleTextItem * pText = (QGraphicsSimpleTextItem *)aItem.GetPtr();

		if( pText )
		{
            QColor aQtColor = pText->brush().color();

			IColor aColor;
			aColor.iRed = aQtColor.red();
			aColor.iGreen = aQtColor.green();
			aColor.iBlue = aQtColor.blue();
			return aColor;
		}
	}
	return IColor();
}

bool PlayInfoDlgImpl::DeleteText( int iTextID )
{
	if( IsIndexOk( iTextID ) )
	{
		{
			CanvasItem aItem = m_aItemContainer[ iTextID ];

			// ** check for an error...
			if( m_aItemContainer.erase( m_aItemContainer.begin()+(iTextID) ) != m_aItemContainer.end() )
			{
				return false;
			}
		}

		{
			QtMTLock aMTLock;

			// ** everything is ok, update the view and return
			m_pCanvas->update();
		}

		return true;

		// CanvasItem will be destroyed after the block is left,
		// because it is not hold in the container any more !
	}
	return false;
}

QGraphicsScene * PlayInfoDlgImpl::GetCanvas()
{
    return m_pCanvas;
}

void PlayInfoDlgImpl::sltCloseDialog()
{
	TransferDataFromControl();

	emit accept();

	emit sigDialogClosed();
}

void PlayInfoDlgImpl::sltCancelDialog()
{
	emit reject();

	emit sigDialogClosed();
}

void PlayInfoDlgImpl::sltSetImage( const QString & sImageFileName, bool bIsPlaying, int iDissolveTimeInMS )
{
    if( !sImageFileName.isEmpty() )
	{
		QImage aImage;

        /*bool bOk =*/ ReadQImage( sImageFileName, aImage );

		if( bIsPlaying )
		{
			// ** because of performace: first scaling (ca. 110ms), than fading (ca. 80ms) !!!
			// ** values for 1024x768 images on 700MHz PIII.
			// ** ATTENTION: comment the next line out, if first fading and than scaling is needed !
            aImage = DoScaleImage( aImage );

			sltFadeInImage( aImage, iDissolveTimeInMS );
		}
		else
		{
            m_aPreviousImage = aImage;
            sltSetImage( aImage );
		}
	}
	else
	{
		QImage aImage = CreateWhiteImage();

		if( bIsPlaying )
		{
			// ** ATTENTION: comment the next line out, if first fading and than scaling is needed !
			aImage = DoScaleImage( aImage );

			sltFadeInImage( aImage, iDissolveTimeInMS );
		}
		else
		{
            m_aPreviousImage = aImage;
            sltSetImage( aImage );
		}
	}
}

void PlayInfoDlgImpl::sltSetImage( const QImage & aImage, bool bForceSet )
{
	m_aActImage = aImage;

    m_aScaledImage = DoScaleImage( m_aActImage );    
    m_aScaledImagePrevious = DoScaleImage( bForceSet ? m_aActImage : m_aPreviousImage );

    //m_pCanvas->SetFadeFactor(0.5);

    m_pCanvas->update();
}

void PlayInfoDlgImpl::sltFadeInImage( const QImage & aNewImage, int iFadeInTimeInMS )
{
	m_pFadeInTimer->stop();

    QImage aFadeInImage = aNewImage;
    if( aFadeInImage.isNull() )
	{
        aFadeInImage = QImage( m_aActImage.width(), m_aActImage.height(), m_aActImage.format() );
        aFadeInImage.fill( 0 );
	}
	// ** for fade int we need true color, 
	// ** fade in is difficult for images with different color paletts !
    if( aFadeInImage.depth()<32 )
	{
        aFadeInImage = aFadeInImage.convertToFormat( QImage::Format_RGB32 );
	}

	// ** both images should have the same size !!!
    m_aPreviousImage = m_aActImage.scaled( aFadeInImage.width(), aFadeInImage.height() );;
	if( m_aPreviousImage.isNull() )
	{
        m_aPreviousImage = QImage( aFadeInImage.width(), aFadeInImage.height(), aFadeInImage.format() );
		m_aPreviousImage.fill( 0 );
	}
	if( m_aPreviousImage.depth()<32 )
	{
        m_aPreviousImage = m_aPreviousImage.convertToFormat( QImage::Format_RGB32 );
	}

	// ** update the fade in data
	if( m_pFadeInTimer->isActive() )
	{
		cout << "ERROR: Timer is active !" << endl;
	}

	m_iFadeInTimeInMS	= iFadeInTimeInMS;
	m_aFadeTime.Reset();
    m_aFadeTime.Start();

	// ** do we need a fade in ?
    int iFadeInFactor = 0;
    if( m_iFadeInTimeInMS<1 )
	{
		// no, time to short !
        iFadeInFactor = MAX_FADE_FACTOR;
	}

    sltSetImage( aFadeInImage );
    m_pCanvas->SetFadeFactor(iFadeInFactor);
    m_pCanvas->update();

    // ** and start the fade in process...
    m_pFadeInTimer->start( 1 );
}

void PlayInfoDlgImpl::sltFadeInTimer()
{
	m_pFadeInTimer->stop();
    // process the next fade in step...

    // ** do fade in process only if dialog is visible !
    if( isVisible() )
    {
        int deltaSinceStart = m_aFadeTime.CurrentDelta();
        int iFadeInFactor = (int)((double)MAX_FADE_FACTOR*(double)deltaSinceStart/(double)m_iFadeInTimeInMS);
        if( iFadeInFactor > MAX_FADE_FACTOR )
        {
            // fade in is finished !
            m_aFadeTime.Stop();

            iFadeInFactor = 0;

            m_pCanvas->SetFadeFactor(iFadeInFactor);

            m_aPreviousImage = m_aActImage;

            //sltSetImage( m_aFadeInImage );
            sltSetImage( m_aActImage );
        }
        else
        {
            m_pCanvas->SetFadeFactor(iFadeInFactor);
        }
        m_pCanvas->update();

        m_pFadeInTimer->start( g_iTimerDelay );
	}
}

void PlayInfoDlgImpl::sltSaveActImage( const QString & sImageFormat )
{
	QString sExt( "*." );
	sExt += sImageFormat;

    QString sFileName = QFileDialog::getSaveFileName( this, tr( "Save as" ), /*QString::null*/GetImagePath().c_str(), sExt );

    if( !sFileName.isEmpty() )
	{
        /*bool bOk =*/ m_aActImage.save( sFileName, sImageFormat.toAscii().constData() );
	}
}

void PlayInfoDlgImpl::sltDispImageToggled( bool /*bState*/ )
{
	// ** update the background image with the new size
	sltSetImage( m_aActImage );
}

void PlayInfoDlgImpl::sltScaleImageToggled( bool /*bState*/ )
{
	// ** update the background image with the new size
	sltSetImage( m_aActImage );
}

void PlayInfoDlgImpl::sltStop()
{
	emit sigDoStop();
}

void PlayInfoDlgImpl::sltPause()
{
	emit sigDoPause();
}

void PlayInfoDlgImpl::sltPlay()
{
	emit sigDoPlay();
}

void PlayInfoDlgImpl::sltFullScreen()
{
	Maximize();
}

#include <math.h>

static bool is_approx(double value1, double value2)
{
    const double EPS = 0.02;
    double val = fabs(value1-value2);
    return val<EPS;
}

void PlayInfoDlgImpl::sltRatioChanged( const QString & sItem )
{
    int iHeightScreen = m_pCanvasView->height();
    int iWidthScreen = m_pCanvasView->width();
    int iHeight = height();
    int iWidth = width();
    int iNewWidthScreen = iWidthScreen;
    QString s = sItem;
    if( sItem==tr("image ratio"))
    {
        double dDx = (double)m_aActImage.width();
        double dDy = (double)m_aActImage.height();
        if( is_approx(dDx/dDy,1.3333333) )
        {
            s = tr("4:3");
        }
        else if( is_approx(dDx/dDy,1.5) )
        {
            s = tr("3:2");
        }
        else if( is_approx(dDx/dDy,1.7777777) )
        {
            s = tr("16:9");
        }
        m_pScreenRatio->setCurrentIndex(m_pScreenRatio->findText(s));
    }
    if( s==tr("3:2"))
    {
        iNewWidthScreen = iHeightScreen*3/2;
    }
    else if( s==tr("4:3"))
    {
        iNewWidthScreen = iHeightScreen*4/3;
    }
    else if( s==tr("16:9"))
    {
        iNewWidthScreen = iHeightScreen*16/9;
    }
    this->resize(iWidth+iNewWidthScreen-iWidthScreen,iHeight);
}

void PlayInfoDlgImpl::closeEvent( QCloseEvent * pCloseEvent )
{
	pCloseEvent->accept();

	emit sigDialogClosed();
}

void PlayInfoDlgImpl::showEvent( QShowEvent * /*pShowEvent*/ )
{
	TransferDataToControl();
}

void PlayInfoDlgImpl::TransferDataToControl()
{
}

void PlayInfoDlgImpl::TransferDataFromControl()
{
}

QImage PlayInfoDlgImpl::DoScaleImage( const QImage & aImage )
{
	QImage aScaledImage;

	if( m_pDisplayImage->isChecked() )
	{
		//QRect aFrameRect = m_pFrame->frameRect();
		QRect aFrameRect = m_pCanvasView->frameRect();

//		m_aScaleTime.Start();
		if( !aImage.isNull() )
		{
			if( m_pScaleOriginal->isChecked() )
			{
				aScaledImage = aImage;
			}
			else if( m_pScaleFillX->isChecked() )
			{
				int iCalcHeight = aFrameRect.width() * aImage.height() / aImage.width();
				aScaledImage = aImage.scaled( aFrameRect.width(), iCalcHeight );
			}
			else if( m_pScaleFillY->isChecked() )
			{
				int iCalcWidth = aFrameRect.height() * aImage.width() / aImage.height();
				aScaledImage = aImage.scaled( iCalcWidth, aFrameRect.height() );
			}
			else if( m_pScaleExpand->isChecked() )
			{
				aScaledImage = aImage.scaled( aFrameRect.width(), aFrameRect.height() );
				// Performance remark: 548x360 --> 1024x768 takes ca. 110ms
			}
            else if( m_pScaleOptimal->isChecked() )
            {
                int screenDx = m_pCanvasView->width();
                int screenDy = m_pCanvasView->height();
                int imgDx = aImage.width();
                int imgDy = aImage.height();
                double screenRatio = (double)screenDx/(double)screenDy;
                double imgRatio = (double)imgDx/(double)imgDy;
                if( imgRatio>screenRatio )
                {
                    // use screen dx for image dx and use image ratio to calculate new image dy
                    aScaledImage = aImage.scaled( screenDx, (int)((double)screenDx/imgRatio) );
                }
                else
                {
                    aScaledImage = aImage.scaled( (int)((double)screenDy*imgRatio), screenDy );
                }
            }
		}
		else
		{
			//aScaledImage = QImage();
		}
//		m_aScaleTime.Stop();
		// ** for performance tests...
		//cout << "scale= " << m_aScaleTime.GetAverageTimeInMS() << endl;
	}
	else
	{
		//aScaledImage = QImage();
	}

	return aScaledImage;
}

bool PlayInfoDlgImpl::IsIndexOk( int iTextID ) const
{
	return (iTextID>=0) && (iTextID<(int)m_aItemContainer.size());
}

void PlayInfoDlgImpl::done( int iRet )
{
	emit sigDialogClosed();
	
	QDialog::done( iRet );
}

void PlayInfoDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Qt::Key_F1) )
	{
		emit sigDialogHelp( "PlayInfoDialog" );
	}
	else if( (pEvent->text() == "m") )
	{
		FullScreen();
	}
	else if( (pEvent->text() == "n") || (IsFullScreen() && (pEvent->key() == Qt::Key_Escape /*ESC*//*27*/)) )
	{
		RestoreSize();
	}
	else if( (pEvent->text() == "s") )
	{
		StartStopFade();
	}
	else if( /*((pEvent->state() & ControlButton)==ControlButton) &&*/ (pEvent->text() == "r") )
	{
		if( m_pRun->isEnabled() )
		{
			emit sigDoPlay();
		}
	}
	else if( (pEvent->text() == "p") )
	{
		if( m_pPause->isEnabled() )
		{
			emit sigDoPause();
		}
	}
	else if( (pEvent->text() == "t") )
	{
		if( m_pStop->isEnabled() )
		{
			emit sigDoStop();
		}
	}
	else
	{
		QDialog::keyPressEvent( pEvent );
	}
}

void PlayInfoDlgImpl::resizeEvent( QResizeEvent * pEvent )
{
cout << "ResizeEvent " << isFullScreen() << endl;
// TODO gulp --> if fullscreen and controls not hidden --> hide them (workaround for restoring full screen)
if( m_pRun->isVisible() )
{
//    FullScreen();
}

    QRect aFrameRect = m_pCanvasView->frameRect();

	// ** update size of the canvas
	m_pCanvasView->resize( aFrameRect.width(), aFrameRect.height() );
    m_pCanvas->setSceneRect(0,0,aFrameRect.width()-5, aFrameRect.height()-5);

	// ** update the background image with the new size
	sltSetImage( m_aActImage );

	QDialog::resizeEvent( pEvent );
}

void PlayInfoDlgImpl::SetExpandImage( bool bExpand )
{
	if( bExpand )
	{
		m_pScaleExpand->setChecked( TRUE );
	}
	else
	{
		m_pScaleOriginal->setChecked( TRUE );
	}
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

TimeMeasurement::TimeMeasurement( int iDefaultTimeInMS )
: m_iMeasurementCount( 0 ),
  m_iSum( 0 ),
  m_iDefaultTimeInMS( iDefaultTimeInMS )
{
}

void TimeMeasurement::Reset()
{
	m_iMeasurementCount = 0;
	m_iSum = 0;
}

void TimeMeasurement::Start()
{
	m_aStartTime = QTime::currentTime();
}

void TimeMeasurement::Stop()
{
	m_aStopTime = QTime::currentTime();

	Update();
}

int TimeMeasurement::CurrentDelta()
{
    return m_aStartTime.elapsed();
}

bool TimeMeasurement::IsValid() const
{
	return ( m_iMeasurementCount > 0 );
}

int	TimeMeasurement::GetAverageTimeInMS() const
{
	if( m_iMeasurementCount > 0 )
	{
		return m_iSum / m_iMeasurementCount;
	}
	return m_iDefaultTimeInMS;
}

void TimeMeasurement::Update()
{
	m_iMeasurementCount++;
	m_iSum += m_aStartTime.msecsTo( m_aStopTime );
}

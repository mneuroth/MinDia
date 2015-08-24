/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
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

#include "playinfodlgimpl.h"

#include "appconfig.h"
#include "misctools.h"
#include "qtmtlock.h"

#include <math.h>

#include <QKeyEvent>
#include <QPixmap>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QShowEvent>
#include <QCloseEvent>
#include <QImageWriter>
#include <QImage>
#include <QPicture>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>
#include <QCheckBox>
#include <QRadioButton>
#include <QPainter>
#include <QLayout>
#include <QPushButton>
#include <QToolButton>
#include <QDateTime>
#include <QFileDialog>
#include <QPaintEngine>


#define MAX_FADE_DELAY  1   // in ms
#define MAX_FADE_FACTOR 255

static int g_iTimerDelay = MAX_FADE_DELAY;

// *******************************************************************
// *******************************************************************
// *******************************************************************

void _FadeImage( QPainter * pPainter, int iFadeFactor, const QImage & aImagePrevious, const QImage & aImage )
{
    double dFactor = (double)(iFadeFactor)/(double)MAX_FADE_FACTOR;
    double currentOpacity = pPainter->opacity();

    pPainter->setRenderHint(QPainter::Antialiasing);

    pPainter->setOpacity(1.0/*-dFactor*/);

    pPainter->drawImage(QPoint(0,0),aImagePrevious);
    pPainter->setOpacity(dFactor);
    pPainter->drawImage(QPoint(0,0),aImage);

    pPainter->setOpacity(currentOpacity);
}

// *******************************************************************
class SimpleBitmapScene : public QGraphicsScene
{
public:
    SimpleBitmapScene( QObject * parent = 0 )
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

void SimpleBitmapScene::drawBackground( QPainter * pPainter, const QRectF & /*area*/ )
{
    _FadeImage(pPainter,m_iFadeFactor,*m_pImagePrevious,*m_pImage);
}

// *******************************************************************
// *******************************************************************
// *******************************************************************
#ifdef _old_fade_implementation

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

#endif //_old_fade_implementation

// *******************************************************************

PlayInfoDlgImpl::PlayInfoDlgImpl( QObject * pShowControler, QWidget * parent, Qt::WindowFlags fl )
: QDialog( parent, fl /*| Qt::WStyle_Maximize*/ ),
  m_pParent( parent ),
  m_iFadeInTimeInMS( 0 ),
  m_aFadeTime( 1 ),
  m_pFadeInTimer( 0 )
{
    setupUi(this);

    QPixmap aPauseIcon( ":/icons/icons/pause.png" );
    QPixmap aStopIcon( ":/icons/icons/stop.png" );
    QPixmap aRunIcon( ":/icons/icons/run.png" );
    QPixmap aFullscreenIcon( ":/icons/icons/fullscreen.png" );
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

    m_pScene = new SimpleBitmapScene( /*m_pFrame*/0 );

    m_pCanvasView->setScene(m_pScene);
    m_pCanvasView->show();

	m_pDisplayImage->setChecked( true );
	//m_pScaleExpand->setChecked( true );		// neu seit 15.2.2003
    //m_pScaleOriginal->setChecked( true );

    m_pScene->SetImagePtr( &m_aScaledActImage );
    m_pScene->SetImagePreviousPtr( &m_aScaledImagePrevious );

    m_pCanvasView->SetDlg(this);

    SetCurrentImage( m_aActImage );

    connect( m_pScene, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(sltSceneRectChanged(QRectF)) );
}

void PlayInfoDlgImpl::sltSceneRectChanged( const QRectF & /*rect*/ )
{
}

PlayInfoDlgImpl::~PlayInfoDlgImpl()
{
	m_pFadeInTimer->stop();
	delete m_pFadeInTimer;

	// ** erase all canvas items before deleting the canvas --> dangling references !
	m_aItemContainer.erase( m_aItemContainer.begin(), m_aItemContainer.end() );

	// ** will be done from Qt !
    //delete m_pCanvasView;
    //delete m_pScene;
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

void PlayInfoDlgImpl::FullScreen()
{
    //if( !IsFullScreen() )
	{
		// ** for full screen modus remove the controls in the control area
		m_pDisplayImage->hide();
		m_pRun->hide();
		m_pPause->hide();
		m_pStop->hide();
		m_pFullScreen->hide();
		m_pScaleImageGroup->hide();
		m_pClose->hide();
        m_pOperationGroup->hide();

        this->layout()->setContentsMargins(0,0,0,0);
        m_aScreenGroup->setContentsMargins(0,0,0,0);
        m_aScreenGroup->layout()->setContentsMargins(0,0,0,0);

        // ** save position before fullscreen modus
		// ** this values have to be restored later !
		m_aLastPos = pos();

        showFullScreen();

        this->resize(this->width(),this->height()+1);
	}
}

void PlayInfoDlgImpl::RestoreSize()
{
    //if( IsFullScreen() )
	{
		m_pDisplayImage->show();
		m_pRun->show();
		m_pPause->show();
		m_pStop->show();
		m_pFullScreen->show();
		m_pScaleImageGroup->show();
		m_pClose->show();
        m_pOperationGroup->show();


        m_aScreenGroup->layout()->setContentsMargins(3,3,3,3);
        m_aScreenGroup->setContentsMargins(6,6,6,6);
        this->layout()->setContentsMargins(6,6,6,6);

        showNormal();

        this->resize(this->width(),this->height()+1);
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

        m_pScene->update();
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

    m_aActFont.setFamily( ToQString(sFontName) );

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
    pText->setText( ToQString(sText) );
    m_pScene->addItem(pText);

    pText->setFont( m_aActFont );
    pText->setBrush( QBrush( m_aActColor ) );
    pText->setPos( x, y );
	pText->show();

	m_aItemContainer.push_back( CanvasItem( pText ) );

    m_pScene->update();

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

        m_pScene->update();

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

            m_pScene->update();

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
            m_pScene->update();
		}

		return true;

		// CanvasItem will be destroyed after the block is left,
		// because it is not hold in the container any more !
	}
	return false;
}

QGraphicsScene * PlayInfoDlgImpl::GetCanvas()
{
    return m_pScene;
}

void PlayInfoDlgImpl::sltCloseDialog()
{
	TransferDataFromControl();

    accept();

	emit sigDialogClosed();
}

void PlayInfoDlgImpl::sltCancelDialog()
{
    reject();

	emit sigDialogClosed();
}

void PlayInfoDlgImpl::sltSetImage( const QImage & aImage, bool bIsPlaying, int iDissolveTimeInMS )
{
    if( bIsPlaying )
    {
        // ** because of performace: first scaling (ca. 110ms), than fading (ca. 80ms) !!!
        // ** values for 1024x768 images on 700MHz PIII.
        // ** ATTENTION: comment the next line out, if first fading and than scaling is needed !
        sltFadeInImage( DoScaleImage( aImage ), iDissolveTimeInMS );
    }
    else
    {
        m_aPreviousImage = aImage;
        SetCurrentImage( aImage );
    }
}

void PlayInfoDlgImpl::Refresh()
{
    SetCurrentImage( m_aActImage );
}

void PlayInfoDlgImpl::SetCurrentImage( const QImage & aImage, bool bForceSet )
{
	m_aActImage = aImage;

    m_aScaledActImage = DoScaleImage( m_aActImage );
    m_aScaledImagePrevious = DoScaleImage( bForceSet ? m_aActImage : m_aPreviousImage );

    //m_pScene->SetFadeFactor(0.5);

    m_pScene->update();
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
    if( m_iFadeInTimeInMS<=0 )
    {
        m_iFadeInTimeInMS = 1;      // Bugfix for 0 dissiove time for white image at show start
        // use a dissolve time > 0 for slide show, use at leas 1 ms
    }
	m_aFadeTime.Reset();
    m_aFadeTime.Start();

	// ** do we need a fade in ?
    int iFadeInFactor = 0;
    if( m_iFadeInTimeInMS<1 )
	{
		// no, time to short !
        iFadeInFactor = MAX_FADE_FACTOR;
	}

    SetCurrentImage( aFadeInImage );
    m_pScene->SetFadeFactor(iFadeInFactor);
    m_pScene->update();

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
        bool bStartTimer = true;
        int deltaSinceStart = m_aFadeTime.CurrentDelta();
        int iFadeInFactor = (int)((double)MAX_FADE_FACTOR*(double)deltaSinceStart/(double)m_iFadeInTimeInMS);
        if( iFadeInFactor > MAX_FADE_FACTOR )
        {
            // fade in is finished !
            m_aFadeTime.Stop();
            bStartTimer = false;

            iFadeInFactor = 0;

            m_pScene->SetFadeFactor(iFadeInFactor);

            m_aPreviousImage = m_aActImage;

            //sltSetImage( m_aFadeInImage );
            SetCurrentImage( m_aActImage );
        }
        else
        {
            m_pScene->SetFadeFactor(iFadeInFactor);
        }
        m_pScene->update();

        if( bStartTimer )   // bugfix: 9.11.2014 --> stop hight CPU load after stopping of slide show
        {
            m_pFadeInTimer->start( g_iTimerDelay );
        }
	}
}

void PlayInfoDlgImpl::sltSaveActImage( const QString & sImageFormat )
{
	QString sExt( "*." );
	sExt += sImageFormat;

    QString sFileName = QFileDialog::getSaveFileName( this, tr( "Save as" ), ToQString(GetImagePath()), sExt );

    if( !sFileName.isEmpty() )
	{
        /*bool bOk =*/ m_aActImage.save( sFileName, sImageFormat.toLatin1().constData() );
	}
}

void PlayInfoDlgImpl::sltDispImageToggled( bool /*bState*/ )
{
	// ** update the background image with the new size
    SetCurrentImage( m_aActImage );
}

void PlayInfoDlgImpl::sltScaleImageToggled( bool /*bState*/ )
{
	// ** update the background image with the new size
    SetCurrentImage( m_aActImage );
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

const int BORDER = 0;

QImage PlayInfoDlgImpl::DoScaleImage( const QImage & aImage )
{
	QImage aScaledImage;

	if( m_pDisplayImage->isChecked() )
	{
        QRect aFrameRect = m_pCanvasView->geometry();   // frameRect()

		if( !aImage.isNull() )
		{
			if( m_pScaleOriginal->isChecked() )
			{
                m_pCanvasView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);       // anscheinend ok fuer mac und windows !
                m_pCanvasView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                aScaledImage = aImage;
                m_pScene->setSceneRect(0,0,aScaledImage.width(),aScaledImage.height());
            }
			else if( m_pScaleFillX->isChecked() )
			{
                m_pCanvasView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                m_pCanvasView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                int SCROLLBAR = m_pCanvasView->verticalScrollBar()->frameSize().width();
                int iCalcHeight = (aFrameRect.width()-SCROLLBAR) * aImage.height() / aImage.width();
                aScaledImage = aImage.scaled( aFrameRect.width()-SCROLLBAR, iCalcHeight, Qt::KeepAspectRatio );
                m_pScene->setSceneRect(0.0,0.0,(double)aScaledImage.width(),(double)(iCalcHeight>aFrameRect.height() ? iCalcHeight : aFrameRect.height())-SCROLLBAR);
            }
			else if( m_pScaleFillY->isChecked() )
			{
                m_pCanvasView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                m_pCanvasView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                int SCROLLBAR = m_pCanvasView->horizontalScrollBar()->frameSize().height();
                int iCalcWidth = (aFrameRect.height()-SCROLLBAR) * aImage.width() / aImage.height();
                aScaledImage = aImage.scaled( iCalcWidth, aFrameRect.height()-SCROLLBAR, Qt::KeepAspectRatio );
                m_pScene->setSceneRect(0.0,0.0,(double)(iCalcWidth>aFrameRect.width() ? iCalcWidth : aFrameRect.width())-SCROLLBAR,(double)aScaledImage.height());
            }
			else if( m_pScaleExpand->isChecked() )
			{
                m_pCanvasView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                m_pCanvasView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                aScaledImage = aImage.scaled( aFrameRect.width()-BORDER, aFrameRect.height()-BORDER );
                // Performance remark: 548x360 --> 1024x768 takes ca. 110ms
                m_pScene->setSceneRect(0.0,0.0,(double)(aFrameRect.width()-BORDER), (double)(aFrameRect.height()-BORDER));
            }
            else if( m_pScaleOptimal->isChecked() )
            {
                m_pCanvasView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                m_pCanvasView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                int screenDx = m_pCanvasView->width()-BORDER;
                int screenDy = m_pCanvasView->height()-BORDER;
                int imgDx = aImage.width();
                int imgDy = aImage.height();
                double screenRatio = (double)screenDx/(double)screenDy;
                double imgRatio = (double)imgDx/(double)imgDy;
                double dx,dy;
                if( imgRatio>screenRatio )
                {
                    // use screen dx for image dx and use image ratio to calculate new image dy
                    dx = (double)screenDx;
                    dy = (double)((double)screenDx/imgRatio);
                }
                else
                {
                    dx = (double)((double)screenDy*imgRatio);
                    dy = (double)screenDy;
                }
                // Bugfix: update scene rect because otherwise the text will not be positioned correctly
                aScaledImage = aImage.scaled( (int)dx, (int)dy );
                m_pScene->setSceneRect(0.0,0.0,dx,dy);
            }
		}
		else
		{
			//aScaledImage = QImage();
		}
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
    QDialog::resizeEvent( pEvent );
    // refresh the image...
    SetCurrentImage( m_aActImage, true );   // 29.4.2013 bugfix: support resize of play mark images with dynamic texts
}

void PlayInfoDlgImpl::SetExpandImage( bool bExpand )
{
	if( bExpand )
	{
        m_pScaleExpand->setChecked( true );
	}
	else
	{
        m_pScaleOriginal->setChecked( true );
	}
}

void PlayInfoDlgImpl::SetImageRatio( ImageRatio ratio )
{
    QString s;

    switch( ratio )
    {
        case RATIO_16_9:
            s = tr("16:9");
            break;
        case RATIO_3_2:
            s = tr("3:2");
            break;
        case RATIO_4_3:
            s = tr("4:3");
            break;
        case RATIO_IMAGE_RATIO:
            s = tr("image ratio");
            break;
        case RATIO_VARIABLE:
            s = tr("variable");
            break;
        case RATIO_USER:
            // TODO
            break;

        case RATIO_UNDEFINED:
        default:
            ; // ignore
    }
    m_pScreenRatio->setCurrentIndex(m_pScreenRatio->findText(s));
}

void PlayInfoDlgImpl::customEvent(QEvent * pEvent)
{
    if( pEvent->type()==c_iCustomEvent_PostImage )
    {
        MyCustomEvent<ImageReaderData> * pCustomEvent = (MyCustomEvent<ImageReaderData> *)pEvent;
        ImageReaderData aData = pCustomEvent->data();
        if( aData.m_pImage )
        {
            sltSetImage( *(aData.m_pImage), aData.m_bIsPlaying, aData.m_iDissolveTimeInMS );
            delete aData.m_pImage;
            aData.m_pImage = 0;
        }
        pCustomEvent->setData( aData );
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

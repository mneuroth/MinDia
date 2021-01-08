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

#ifndef _PLAYINFODLGIMPL_H
#define _PLAYINFODLGIMPL_H

#include "ui_PlayInfoDlg4.h"

#include "iscript.h"
#include "minhandle.h"
#include "misctools.h"

#include <QGraphicsScene>
#include <QImage>
#include <QTimer>
#include <QDateTime>
#include <QCloseEvent>
#include <QShowEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QDialog>
#include <QElapsedTimer>

class QGraphicsScene;
class QAction;

class MenuCanvasView;
class SimpleBitmapScene;
class PlayInfoDlgImpl;

// *******************************************************************
/** A helper class to measure the time needed for a operation.
  */
class TimeMeasurement
{
public:
	TimeMeasurement( int iDefaultTimeInMS = -1 );

	void	Reset();

	void	Start();
	void	Stop();
    int     CurrentDelta();

	bool	IsValid() const;
	int		GetAverageTimeInMS() const;

private:
	void	Update();

	// ** data **
    QElapsedTimer	m_aStartTime;
    QElapsedTimer	m_aStopTime;
    int             m_iMeasurementCount;
    int             m_iSum;
    int             m_iDefaultTimeInMS;
};

// *******************************************************************
/** Implementation of the Play-Info-Dialog
  */
class PlayInfoDlgImpl : public QDialog, Ui_PlayInfoDialog, public IDiaOutputWindowInternal
{
	Q_OBJECT

    typedef minHandle< QGraphicsItem >	CanvasItem;
	typedef vector< CanvasItem >		CanvasItemContainer;

public:
    PlayInfoDlgImpl( QObject * pShowControler, QWidget * parent, Qt::WindowFlags fl = 0 );
	virtual ~PlayInfoDlgImpl();

	void UpdateStatus( bool bIsPlaying, bool bIsPause );

	bool IsFullScreen() const;
	void FullScreen();
	void RestoreSize();

	bool IsFading() const;
	void StartStopFade();

	void SetExpandImage( bool bExpand );

    void SetImageRatio( ImageRatio ratio );

    void SetCurrentImage( const QImage & aImage, bool bForceSet = false );

    void Refresh();

	// ** implements the IDiaOutputWindow-(Script)Interface **
	virtual bool	IsValid() const;
	virtual bool	Show();
	virtual bool	Hide();
	virtual bool	Maximize();
	virtual bool	Restore();
	virtual int		GetWidth() const;
	virtual int		GetHeight() const;
	virtual bool	SetSize( int iWidth, int iHeight );
	virtual bool	SetPos( int iXPos, int iYPos );
	virtual int		GetDrawWidth() const;
	virtual int		GetDrawHeight() const;
	virtual bool	Clear();
	virtual bool	SetColor( int iRed, int iGreen, int iBlue );
    virtual bool	SetFont( const string & sFontName );
	virtual bool	SetFontSize( int iSizeInPixel, bool bBold, bool bItalic );
    virtual int 	SetTextXY( int x, int y, const string & sText );
	virtual int		GetTextCount() const;
	virtual bool	MoveText( int iTextID, int x, int y );
	virtual int		GetTextX( int iTextID ) const;
	virtual int		GetTextY( int iTextID ) const;
	virtual int		GetTextWidth( int iTextID ) const;
	virtual int		GetTextHeight( int iTextID ) const;
	virtual bool	SetTextColor( int iTextID, int iRed, int iGreen, int iBlue );
	virtual IColor	GetTextColor( int iTextID ) const;
	virtual bool	DeleteText( int iTextID );

    virtual QGraphicsScene *	GetCanvas();

    virtual void customEvent(QEvent * pEvent);

public slots:
	virtual void sltCloseDialog();
	virtual void sltCancelDialog();

    virtual void sltDispImageToggled( bool bState );
    virtual void sltScaleImageToggled( bool bState );

    virtual void sltStop();
    virtual void sltPause();
    virtual void sltPlay();
    virtual void sltFullScreen();

    void sltSetImage( const QImage & aImage, bool bIsPlaying, int iDissolveTimeInMS, minHandle<DiaInfo> hDia );
    void sltFadeInImage( const QImage & aNewImage, int iFadeInTimeInMS );

	void sltFadeInTimer();
    void sltKenBurnsTimer();

	void sltSaveActImage( const QString & sImageFormat );

    void sltRatioChanged( const QString & sItem );

    void sltSceneRectChanged( const QRectF & rect );

signals:
	void sigDialogClosed();
	void sigDocumentUpdate();
    void sigDialogHelp( const QString & sHelpTag );
	void sigDoPlay();
	void sigDoPause();
	void sigDoStop();

protected:
	virtual void closeEvent( QCloseEvent * pCloseEvent );
	virtual void showEvent( QShowEvent * pShowEvent ); 
	virtual void done( int iRet );
	virtual void keyPressEvent( QKeyEvent * pEvent ); 
    virtual void resizeEvent( QResizeEvent * pEvent );

private:
    // ** help methods **
	void TransferDataToControl();
	void TransferDataFromControl();
    QImage DoScaleImage( const QImage & aImage );
	bool IsIndexOk( int iTextID ) const;

    SimpleBitmapScene *     m_pScene;
	QWidget *				m_pParent;			// no owner !
    Qt::WindowFlags			m_flLastFlags;
	QPoint					m_aLastPos;

    QImage					m_aActImage;            // the actual image !
    QImage					m_aPreviousImage;       // the previous image, needed to dissolve the image
    QImage					m_aScaledActImage;		// the scaled image for the real output
    QImage					m_aScaledImagePrevious; // the scaled previous image for fading

	int 					m_iFadeInTimeInMS;

	CanvasItemContainer		m_aItemContainer;
	QColor					m_aActColor;
	QFont					m_aActFont;

	TimeMeasurement			m_aFadeTime;		// to measure the time needed for one fade in step

    minHandle<DiaInfo>      m_hDia;             // the current dia
    QElapsedTimer           m_aKenBurnsStartTime;
    int                     m_iKenBurnsEffectTimeInMS;

	QTimer *				m_pFadeInTimer;
    QTimer *                m_pKenBurnsTimer;
};	

#endif


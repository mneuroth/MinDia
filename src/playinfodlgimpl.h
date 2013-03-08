/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/playinfodlgimpl.h,v $
 *
 *  $Revision: 1.4 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.3  2004/04/09 15:43:28  min
 *	Optimizations for Zaurus port.
 *	
 *	Revision 1.2  2004/02/20 20:11:07  min
 *	Bugfixes and fullscreen button implemented.
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

#ifndef _PLAYINFODLGIMPL_H
#define _PLAYINFODLGIMPL_H

#include "ui_PlayInfoDlg4.h"

#include "iscript.h"
#include "minhandle.h"

#include <QGraphicsScene>
#include <qimage.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <QCloseEvent>
#include <QShowEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QDialog>

class QGraphicsScene;

class QAction;

class MenuCanvasView;
class SimpleBitmapCanvas;
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
	QTime	m_aStartTime;
	QTime	m_aStopTime;
	int		m_iMeasurementCount;
	int		m_iSum;
	int		m_iDefaultTimeInMS;
};

//// *******************************************************************
///** The context menu for the canvas view
//  */
//class PlayInfoContextMenu : public QMenu
//{
//	Q_OBJECT

//public:
//	PlayInfoContextMenu( QWidget * parent, PlayInfoDlgImpl * pMyDialog );
//	~PlayInfoContextMenu();

//public slots:
//    void sltStartStopFade();
//    void sltToggleFullScreen();
//    void sltClose();

//	void sltImageFormatActivated( int iIndex );
//	//void sltActivated( int iIndex );
//	void sltShowMenu();

//private:
//	PlayInfoDlgImpl *	m_pMyDialog;
//	QMenu *		        m_pImageFormats;
//    QAction *           m_pActionFullScreen;
//};

// *******************************************************************
/** Implementation of the Play-Info-Dialog
  */
class PlayInfoDlgImpl : public QDialog, Ui_PlayInfoDialog, public IDiaOutputWindowInternal
{
	Q_OBJECT

    typedef minHandle< QGraphicsItem >	CanvasItem;
	typedef vector< CanvasItem >		CanvasItemContainer;

public:
    PlayInfoDlgImpl( QObject * pShowControler, QWidget * parent, Qt::WFlags fl = 0 );
	virtual ~PlayInfoDlgImpl();

	void UpdateStatus( bool bIsPlaying, bool bIsPause );

	bool IsFullScreen() const;
	void FullScreen();
	void RestoreSize();

	bool IsFading() const;
	void StartStopFade();

	void SetExpandImage( bool bExpand );

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

public slots:
	virtual void sltCloseDialog();
	virtual void sltCancelDialog();

    virtual void sltDispImageToggled( bool bState );
    virtual void sltScaleImageToggled( bool bState );

    virtual void sltStop();
    virtual void sltPause();
    virtual void sltPlay();
    virtual void sltFullScreen();

	void sltSetImage( const QString & sImageFileName, bool bIsPlaying, int iDissolveTimeInMS );
	void sltSetImage( const QImage & aImage );
	void sltFadeInImage( const QImage & aNewImage, int iFadeInTimeInMS );

	void sltFadeInTimer();

	void sltSaveActImage( const QString & sImageFormat );

    void sltRatioChanged( const QString & sItem );

signals:
	void sigDialogClosed();
	void sigDocumentUpdate();
	void sigDialogHelp( const QString & sHelpTag);
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

	SimpleBitmapCanvas *	m_pCanvas;
	QWidget *				m_pParent;			// no owner !
	Qt::WFlags				m_flLastFlags;
	QPoint					m_aLastPos;

	QImage					m_aActImage;		// the actual image !
	QImage					m_aPreviousImage;	// the previous image, needed to dissolve the image 
	QImage					m_aScaledImage;		// the scaled image for the real output
    QImage					m_aScaledImagePrevious; // the scaled previous image for fading

	int 					m_iFadeInTimeInMS;

	CanvasItemContainer		m_aItemContainer;
	QColor					m_aActColor;
	QFont					m_aActFont;

	TimeMeasurement			m_aFadeTime;		// to measure the time needed for one fade in step

	QTimer *				m_pFadeInTimer;
};	

#endif


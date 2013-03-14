/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/timelineview.h,v $
 *
 *  $Revision: 1.2 $
 *
 *	$Log: not supported by cvs2svn $
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

#ifndef _TIMELINEVIEW_H
#define _TIMELINEVIEW_H

#include "minhandle.h"

// ** gui
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QDragMoveEvent>
#include <QGraphicsView>

#include <vector>

using namespace std;

#include "timelineitem.h"
#include "timelineaxis.h"

class DiaPresentation;
class QMenu;
class MyDynamicToolTip;

#define _USER_EVENT_GET_SOUND_LENGTH			 QEvent::User+4

// *******************************************************************
/**
 * User event to get the length of a sound file (asynchroniously)
 */
class GetSoundLengthEvent : public QEvent
{
public:
    GetSoundLengthEvent( const QString & sFileName, QWidget * pRequester );

    QString GetFileName() const;
    int     GetSoundLength() const;
    void    SetSoundLength( int val );
    QWidget * GetRequester() const;

private:
    QString     m_sFileName;
    int         m_iSoundLength;
    QWidget *   m_pRequester;
};

// *******************************************************************
/** Handels a timeline view for the dia presentation.
  */
class TimeLineView : public QGraphicsView
{
	Q_OBJECT

	typedef vector< minHandle<TimeLineItem> >	MyItemContainer;

    typedef pair< pair< minHandle<QGraphicsLineItem>, minHandle<QGraphicsLineItem> >, minHandle<QGraphicsSimpleTextItem> > PlotCommentItem;
	typedef vector< PlotCommentItem >	PlotCommentItemContainer;

    typedef pair< minHandle<QGraphicsLineItem>, minHandle<QGraphicsSimpleTextItem> > MusicCommentItemHelper;
	typedef pair< QString, int > ItemInfoHelper;
	typedef pair< MusicCommentItemHelper, ItemInfoHelper > MusicCommentItem;
	typedef vector< MusicCommentItem >	MusicCommentItemContainer;

    typedef pair< minHandle<QGraphicsLineItem>, minHandle<QGraphicsLineItem> > FadeItem;
    typedef pair< minHandle<QGraphicsRectItem>, minHandle<QGraphicsSimpleTextItem> > MusicItem;
	typedef vector< MusicItem >	MusicItemContainer;
	typedef vector< FadeItem >	FadeItemContainer;

public:
	TimeLineView( QWidget * pParent, int iWidth, int iHeight, QWidget * pMainWin = 0, QObject * pControler = 0, DiaPresentation * pDoc = 0 );
	virtual ~TimeLineView();

	// *** overload inherited methods ***
	virtual QSize sizeHint() const;

	// *** handle signals
    virtual void mousePressEvent( QMouseEvent * pEvent );
    virtual void mouseReleaseEvent( QMouseEvent * pEvent );
    virtual void mouseMoveEvent( QMouseEvent * pEvent );
	virtual void dragEnterEvent( QDragEnterEvent * pEvent );
	virtual void dragMoveEvent ( QDragMoveEvent * pEvent );
	virtual void dropEvent( QDropEvent * pEvent );
    virtual void customEvent(QEvent * pEvent);

	// *** my own interface ***
	void setSizeHint( const QSize & aSize );

	void SyncViewWithData();

	void SetPlayMark( double dActPlayTime );

    QRect GetTipRect( const QPoint & aPoint, QString * psText = 0, int * piIndex = 0 );

public slots:
	// ** helper methods **
	void sltDoUpdateView( bool bErase );
	void sltUpdateView();
	void sltUpdateSelected();
	void sltSelectItem( int iNo, int iDissolveTimeInMS );
	void sltItemSelected( int iCount, int iFirstSelectedItemNo );
    void sltAddDynText();
    void sltEditDynText();

signals:
	void sigViewDataChanged();
	void sigItemSelected( int iNo, int iDissolveTimeInMS );
	void sigLoadDoc( const QString & sFileName, bool bExecuteEvent );
    void sigPlayMarkChanged( double dTimePosInSec );

private:
	// ** helper methods **
	void ShowPlotComments();
	void ShowMusicComments();
	void ShowMusicTracks();
	void ShowGraphicOperations();
	void ShowModifyDynObjectDialog( int iIndexOut );
	int  GetItemForPosX( int x );

	// ** data **
	QMenu *				        m_pContextMenu;
    QAction *                   m_pMenuDynTextEdit;
    QGraphicsScene *			m_pCanvas;
	QSize						m_aSizeHint;
	MyDynamicToolTip *			m_pToolTip;
	QPoint						m_aLastMousePos;			// temp

    minHandle<QGraphicsLineItem>	m_hPlayMark;

	minHandle<TimeLineAxis>		m_hTimeAxis;

	MyItemContainer				m_aItemContainer;			// container for slides
	MusicCommentItemContainer	m_aMusicCommentContainer;	// container for the (music) comment items
	MusicItemContainer			m_aMusicContainer;			// container for music/wav files
	FadeItemContainer			m_aFadeContainer;			// conatiner for music fade/in/out infos
	PlotCommentItemContainer	m_aPlotCommentContainer;	// container for the plot comment items
	MusicCommentItemContainer	m_aDynGrapOpContainer;		// container for the dynamic graphic operations items

	// ** reference to data **
	DiaPresentation *			m_pDiaPres;					// !!! NO OWNER !!!

	// ** temp data **
	QWidget	*					m_pParent;
	int							m_iLastActPlayPos;
	int							m_iSelectedItemStartPos;
	bool						m_bDissolveSelected;
	bool						m_bTotalTimeConstant;
	minHandle<TimeLineItem>		m_hSelectedItem;
	int							m_iSelectedItemNo;
	bool						m_bMouseMovedWhilePressed;
	int							m_iSelectedDynTextIndex;	// temp info for shift and context menu
};

#endif

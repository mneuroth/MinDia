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
#include <q3canvas.h>
//Added by qt3to4:
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QDragMoveEvent>
#include <Q3PopupMenu>

#include <vector>

using namespace std;

#include "timelineitem.h"
#include "timelineaxis.h"

class DiaPresentation;
class Q3PopupMenu;
class MyDynamicToolTip;

// *******************************************************************
/** Handels a timeline view for the dia presentation.
  */
class TimeLineView : public Q3CanvasView
{
	Q_OBJECT

	typedef vector< minHandle<TimeLineItem> >	MyItemContainer;

	typedef pair< pair< minHandle<Q3CanvasLine>, minHandle<Q3CanvasLine> >, minHandle<Q3CanvasText> > PlotCommentItem;
	typedef vector< PlotCommentItem >	PlotCommentItemContainer;

	typedef pair< minHandle<Q3CanvasLine>, minHandle<Q3CanvasText> > MusicCommentItemHelper;
	typedef pair< QString, int > ItemInfoHelper;
	typedef pair< MusicCommentItemHelper, ItemInfoHelper > MusicCommentItem;
	typedef vector< MusicCommentItem >	MusicCommentItemContainer;

	typedef pair< minHandle<Q3CanvasLine>, minHandle<Q3CanvasLine> > FadeItem;
	typedef pair< minHandle<Q3CanvasRectangle>, minHandle<Q3CanvasText> > MusicItem;
	typedef vector< MusicItem >	MusicItemContainer;
	typedef vector< FadeItem >	FadeItemContainer;

public:
	TimeLineView( QWidget * pParent, int iWidth, int iHeight, QWidget * pMainWin = 0, QObject * pControler = 0, DiaPresentation * pDoc = 0 );
	virtual ~TimeLineView();

	// *** overload inherited methods ***
	virtual QSize sizeHint() const;

	// *** handle signals
	virtual void contentsMousePressEvent( QMouseEvent * pEvent );
	virtual void contentsMouseReleaseEvent( QMouseEvent * pEvent );
	virtual void contentsMouseMoveEvent( QMouseEvent * pEvent );
	virtual void dragEnterEvent( QDragEnterEvent * pEvent );
	virtual void dragMoveEvent ( QDragMoveEvent * pEvent );
	virtual void dropEvent( QDropEvent * pEvent );

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
	void sltContextMenuActivated( int iMenuIndex );

signals:
	void sigViewDataChanged();
	void sigItemSelected( int iNo, int iDissolveTimeInMS );
	void sigModifySoundData();
	void sigModifySoundComment();
	void sigModifyPlotComment();
	void sigLoadDoc( const QString & sFileName, bool bExecuteEvent );

private:
	// ** helper methods **
	void ShowPlotComments();
	void ShowMusicComments();
	void ShowMusicTracks();
	void ShowGraphicOperations();
	void ShowModifyDynObjectDialog( int iIndexOut );
	int  GetItemForPosX( int x );

	// ** data **
	Q3PopupMenu *				m_pContextMenu;
	Q3Canvas *					m_pCanvas;
	QSize						m_aSizeHint;
	MyDynamicToolTip *			m_pToolTip;
	QPoint						m_aLastMousePos;			// temp

	minHandle<Q3CanvasLine>		m_hPlayMark;

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

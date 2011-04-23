/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/hitemview.h,v $
 *
 *  $Revision: 1.2 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.1.1.1  2003/08/15 16:38:21  min
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

#ifndef _HITEMVIEW_H
#define _HITEMVIEW_H

// ** gui
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGraphicsView>

#include "hitem.h"
#include "diainfo.h"

// ** data structures
#include <vector>

using namespace std;

// *******************************************************************

class DiaPresentation;
class QMenu;
    
/*
class VectorHItem : public vector<HItem *>
{
public:
	int count() const	{ return size(); }
};
*/

// *******************************************************************
/** Handels a container view for viewable
  * items. 
  */
class HItemView : public QGraphicsView
{
	Q_OBJECT

	typedef vector<HItem *>	MyItemContainer;
	typedef vector<int>		MyIndexContainer;

public:
	HItemView( QWidget * pParent, int iWidth, int iHeight, QWidget * pMainWin = 0, QObject * pControler = 0, DiaPresentation * pDoc = 0 );
	virtual ~HItemView();

    // *** overload inherited methods ***
//    virtual QSize sizeHint() const;

	// *** my own interface ***
//	void setSizeHint( const QSize & aSize );

	// *** handle signals
    virtual void mousePressEvent( QMouseEvent * pEvent );
    virtual void mouseMoveEvent( QMouseEvent * pEvent );
	//virtual void contentsMouseDoubleClickEvent( QMouseEvent * pEvent );
	virtual void keyPressEvent( QKeyEvent * pEvent ); 
	virtual void dragEnterEvent( QDragEnterEvent * pEvent );
	virtual void dropEvent( QDropEvent * pEvent );
    virtual void dragMoveEvent( QDragMoveEvent * pEvent );

	int  GetSelectedCount() const;
	int  GetLastSelectedItemIndex() const;
	bool GetTwoSelectedItems( HItem * & aItem1, HItem * & aItem2 ) const;	// needed for dissolve tests

	bool GetActClipboardData( QString & sDataOut ) const;
	bool SetFromClipboardData( const QString & sData );
	int  GetCountValidClipboardData( const QString & sData );

	void SyncViewWithData();

public slots:
	void sltNewItem();
	void sltUpdateView();
	void sltUpdateSelected();
	void sltDeleteAllSlectedItems();
	void sltSelectAllItems( bool bSelect, bool bUpdateView );
	void sltSelectNext();
	void sltSelectPrev();
	void sltSelectItem( int iNo, int iDissolveTimeInMS );
	void sltItemSelected( int iCount, int iFirstSelectedItemNo );
	void sltNewItemAfterSelected();
	void sltDeleteSelectedItem();

signals:
	void sigItemSelected( int iCount, HItem * pFirstSelectedItem, int iFirstSelectedItemNo, int iDissolveTimeInMS );
	void sigSelectItem( int iIndex, int iDissolveTimeInMS );
	void sigViewDataChanged();
	void sigDialogHelp( const QString & sHelpTag);
	void sigLoadDoc( const QString & sFileName, bool bExecuteEvent );

private:
	// *** helper methods ***
	void ResetView();
	void DeleteAllHItems();
	bool IsValidIndex( int iIndex ) const;
	void SelectItemDelta( int iDeltaIndex );
	bool SelectItem( int iIndex, int iDissolveTimeInMS );
	void ShiftViewItemsFromIndex( int iIndex );
	QPoint GetPosOfItemWithIndex( int iIndex );
	// *** operations on the data ***
	void AddItemAt( minHandle<DiaInfo> hDiaInfo, int iIndex = -1, bool bInsert = false, bool bUpdateView = true, bool bDoResize = true );
	void RemoveItemAt( int iIndex, bool bDeleteData = false, bool bUpdateView = true );
	//DiaInfo * GetItemAt( int iIndex ) const;
	bool GetActClipboardData( QString & sDataOut, MyIndexContainer * pIndexContainer ) const;
	bool IsDragTargetNotDragSource( QMouseEvent * pEvent, int iActIndex );

	// *** data ***
	QMenu *		        m_pContextMenu;
    QGraphicsScene *	m_pCanvas;
//	QSize				m_aSizeHint;

	QPoint				m_aNextItemPos;
	QPoint				m_aItemShift;
	QRect				m_aDefaultItemSize;

	int					m_iDragTargetIndex;		// temp

	MyItemContainer		m_aItemContainer;

	// ** reference to data ***
	DiaPresentation *	m_pDiaPres;				// !!! NO OWNER !!!
};

#endif


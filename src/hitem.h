/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/hitem.h,v $
 *
 *  $Revision: 1.1.1.1 $
 *
 *	$Log: not supported by cvs2svn $
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

#ifndef _HTIEM_H
#define _HTIEM_H

// ** gui
#include <qcanvas.h> 

#include "minhandle.h"		// for the smart-ptr

// *******************************************************************

class DiaInfo;
class SlideItem;
class QImage;

#undef _with_canvas_items

// *******************************************************************
/** Represents an Item in an ItemView.
  */
class HItem : public QCanvasRectangle
{
public:
	HItem( const QRect & aRect, QCanvas * pCanvas, minHandle<DiaInfo> hData );
	~HItem();

	void SetSelected( bool bSelect );
	bool GetSelected() const;

	QString GetData() const;

	minHandle<DiaInfo> GetInfoData() const;

	bool IsPointInItem( int x, int y ) const;

protected:

#ifndef _with_canvas_items
    virtual void drawShape( QPainter & aPainter );
#endif

private:

#ifdef _with_canvas_items
	// ** helper methods **
	void CreateElements( QCanvas * pCanvas );
	void DeleteElements();
	void UpdateElements();

	// ** the element data **
	QCanvasText *		m_pPosText;
	QCanvasText *		m_pIdText;
	QCanvasText *		m_pCommentText;
	QCanvasText *		m_pFileNameText;
	SlideItem *			m_pSlideItem;
#else
	QString				m_sImageFileNameCache;
	QImage *			m_pImageCache;
#endif

	// ** data **
	bool				m_bIsSelected;

	minHandle<DiaInfo>	m_hData;
};

#endif

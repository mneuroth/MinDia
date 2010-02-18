/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/dyntextdlgimpl.h,v $
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
 * Copyright (C) 2002-2003 by Michael Neuroth.							   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#ifndef _DYNTEXTDLGIMPL_H
#define _DYNTEXTDLGIMPL_H

#include "DynTextDlg.h"

#include "dyngraphop.h"

#include <qfont.h>
#include <q3canvas.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QKeyEvent>


class DrawingArea : public Q3CanvasView
{
	Q_OBJECT

public:
	DrawingArea( QWidget * pParent, QWidget * pSignalClient );
	virtual ~DrawingArea();

	virtual void contentsMousePressEvent( QMouseEvent * pEvent );
	virtual void contentsMouseMoveEvent( QMouseEvent * pEvent );

signals:
	void sigTextMoved();

private:
	Q3CanvasItem *	m_pMovingItem;
	QPoint			m_aMovingStart;
};

class DynamicTextDlgImpl : public DynamicTextDlg
{
	Q_OBJECT

public:
    DynamicTextDlgImpl( minHandle<DynText> hItem, QWidget * parent = 0, QWidget * pMain = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
	virtual ~DynamicTextDlgImpl();

	double GetRelX() const;
	double GetRelY() const;
	void SetRelPos( double xRel, double yRel );

	QFont GetFont() const;

public slots:
    virtual void sltRelPosToggled(bool bValue);
	virtual void sltDeleteText();
	virtual void sltSelectFont();
    virtual void sltSelectFontcolor();
	
	void sltUpdateData();

signals:
	void sigDialogHelp( QWidget * pParent, const QString & sHelpTag );

protected:
	virtual void keyPressEvent( QKeyEvent * pEvent ); 

private:
	minHandle<DynText>	m_hItem;
	QFont				m_aInitFont;
	DrawingArea *		m_pDrawingAreaCanvas;
	Q3Canvas *			m_pCanvas;
	Q3CanvasText *		m_pCanvasText;
};

#endif

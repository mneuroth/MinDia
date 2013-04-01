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
 * Copyright (C) 2002-2013 by Michael Neuroth.							   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#ifndef _DYNTEXTDLGIMPL_H
#define _DYNTEXTDLGIMPL_H

#include "ui_DynTextDlg4.h"

#include "dyngraphop.h"
#include "dynamictextitem.h"

#include <QFont>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>

class DynamicTextDlgImpl : public QDialog, public Ui_DynamicTextDlg, public DynamicItemChangedCallback
{
	Q_OBJECT

public:
    DynamicTextDlgImpl( minHandle<DynText> hItem, int iIndex1, const QString & sUUID1, int iIndex2, const QString & sUUID2, QWidget * parent = 0, QWidget * pMain = 0, Qt::WFlags fl = 0 );
	virtual ~DynamicTextDlgImpl();

	double GetRelX() const;
	double GetRelY() const;
	void SetRelPos( double xRel, double yRel );

	QFont GetFont() const;

    virtual void updated();

public slots:
    void sltRelPosToggled(bool bValue);
    void sltDeleteText();
    void sltSelectFont();
    void sltSelectFontcolor();
    void sltToggleImage1(bool bValue);
    void sltToggleImage2(bool bValue);

	void sltUpdateData();

signals:
	void sigDialogHelp( QWidget * pParent, const QString & sHelpTag );

protected:
	virtual void keyPressEvent( QKeyEvent * pEvent ); 
    virtual void resizeEvent( QResizeEvent * event );

    void UpdateTextPosition();
    void UpdateTextData();

private:
    minHandle<DynText>         m_hItem;
    QFont                      m_aInitFont;
    QGraphicsScene *           m_pCanvas;
    DynamicTextItem *          m_pCanvasText;
};

#endif

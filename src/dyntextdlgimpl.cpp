/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/dyntextdlgimpl.cpp,v $
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

#include "dyntextdlgimpl.h"

#include <qcolordialog.h>
#include <qfontdialog.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qlayout.h>

#include <stdio.h>

// *******************************************************************
// *******************************************************************
// *******************************************************************

DrawingArea::DrawingArea( QWidget * pParent, QWidget * pSignalClient )
: QCanvasView( 0, pParent )
{
    connect( this, SIGNAL( sigTextMoved() ), pSignalClient, SLOT( sltUpdateData() ) );
}

DrawingArea::~DrawingArea()
{
}

void DrawingArea::contentsMousePressEvent( QMouseEvent * pEvent )
{
    QCanvasItemList l = canvas()->collisions( pEvent->pos() );

    for( QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it ) 
	{
		QCanvasText * item = (QCanvasText *)(*it);

		// is the text hit by the mouse ?
		int ix = pEvent->pos().x();
		int iy = pEvent->pos().y();
		QRect r = item->boundingRect();
		if( r.left()>=ix && ix<=r.right() &&
			r.bottom()>=iy && iy<=r.top() )
		{
			// no
			continue;
		}

		m_pMovingItem = *it;
		m_aMovingStart = pEvent->pos();
		return;
    }
    m_pMovingItem = 0;
}

void DrawingArea::contentsMouseMoveEvent( QMouseEvent * pEvent )
{
    if( m_pMovingItem ) 
	{
		m_pMovingItem->moveBy(pEvent->pos().x() - m_aMovingStart.x(),
				       pEvent->pos().y() - m_aMovingStart.y());
		m_aMovingStart = pEvent->pos();
		canvas()->update();

		emit sigTextMoved();
    }
}

// *******************************************************************

DynamicTextDlgImpl::DynamicTextDlgImpl( minHandle<DynText> hItem, QWidget * parent, QWidget * pMain, const char* name, bool modal, WFlags fl )
: DynamicTextDlg( parent, name, modal, fl ),
  m_hItem( hItem ),
  m_aInitFont( hItem->font() )
{
	int x = m_pDrawingArea->width()-5;
	int y = m_pDrawingArea->height()-5;
	m_pCanvas = new QCanvas(x,y);

	QBoxLayout * l = new QVBoxLayout( m_pDrawingArea );

	m_pDrawingAreaCanvas = new DrawingArea( m_pDrawingArea, this );
	m_pDrawingAreaCanvas->setCanvas( m_pCanvas );

    l->addWidget( m_pDrawingAreaCanvas );

	m_pCanvasText = new QCanvasText( m_hItem->GetString().c_str(), m_pCanvas );
	m_pCanvasText->move(100,100);
	m_pCanvasText->show();

	sltRelPosToggled( m_pRelPos->isChecked() );

    connect( this, SIGNAL( sigDialogHelp(QWidget *, const QString &) ), pMain, SLOT( sltShowModalHelp(QWidget *, const QString &) ) );

	sltUpdateData();
}

DynamicTextDlgImpl::~DynamicTextDlgImpl()
{
	delete m_pDrawingAreaCanvas;
}

double DynamicTextDlgImpl::GetRelX() const
{
	return (double)m_pCanvasText->x()/(double)m_pDrawingArea->width();
}

double DynamicTextDlgImpl::GetRelY() const
{
	return (double)m_pCanvasText->y()/(double)m_pDrawingArea->height();
}

void DynamicTextDlgImpl::SetRelPos( double xRel, double yRel )
{
	if( xRel>=0 && yRel>=0 )
	{
		m_pCanvasText->setX( xRel*m_pDrawingArea->width() );
		m_pCanvasText->setY( yRel*m_pDrawingArea->height() );
		m_pRelPos->setChecked( true );
	}
	else
	{
		m_pRelPos->setChecked( false );
	}
}

QFont DynamicTextDlgImpl::GetFont() const
{
	return m_aInitFont;
}

void DynamicTextDlgImpl::sltRelPosToggled(bool bValue)
{
	m_pPosX->setEnabled( !bValue );
	m_pPosY->setEnabled( !bValue );
	m_pPosRelX->setEnabled( bValue );
	m_pPosRelY->setEnabled( bValue );
	m_pDrawingArea->setEnabled( bValue );
	m_pDrawingAreaCanvas->setEnabled( bValue );
}

void DynamicTextDlgImpl::sltSelectFontcolor()
{
	QColor aColor = QColorDialog::getColor ( QColor(), this, "colordialog" );

	if( aColor.isValid() )
	{
		m_pSelectFontcolor->setBackgroundColor( aColor );
		m_pSelectFontcolor->setPalette( QPalette( aColor ) );
	}
}

void DynamicTextDlgImpl::sltSelectFont()
{
	bool bOk = false;

	QFont aFont = QFontDialog::getFont( &bOk, m_aInitFont );

	if( bOk )
	{
		QString sTemp;

		m_aInitFont = aFont;
		m_pFontName->setText( m_aInitFont.family() );
		sTemp = sTemp.setNum( m_aInitFont.pointSize() );
		m_pFontSize->setText( sTemp );
	}
}

void DynamicTextDlgImpl::sltDeleteText()
{
	setResult( 2 );
	hide();
}

void DynamicTextDlgImpl::sltUpdateData()
{
	char sBuffer[128];

	sprintf( sBuffer, "%5.3lf", GetRelX() );
	m_pPosRelX->setText( sBuffer );
	sprintf( sBuffer, "%5.3lf", GetRelY() );
	m_pPosRelY->setText( sBuffer );
}

void DynamicTextDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Key_F1) )
	{
		emit sigDialogHelp( this, "DynGraphOpDlg" );
	}
	else
	{
		DynamicTextDlg::keyPressEvent( pEvent );
	}
}

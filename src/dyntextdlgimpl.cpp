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
#include <QBoxLayout>
#include <QMouseEvent>
#include <QKeyEvent>

#include <stdio.h>

// *******************************************************************
// *******************************************************************
// *******************************************************************

//DrawingArea::DrawingArea( QWidget * pParent, QWidget * pSignalClient )
//: QGraphicsView( 0, pParent )
//{
//    connect( this, SIGNAL( sigTextMoved() ), pSignalClient, SLOT( sltUpdateData() ) );
//}

//DrawingArea::~DrawingArea()
//{
//}

//void DrawingArea::contentsMousePressEvent( QMouseEvent * pEvent )
//{
//// TODO Qt4
////    Q3CanvasItemList l = canvas()->collisions( pEvent->pos() );

////    for( Q3CanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it )
////	{
////		Q3CanvasText * item = (Q3CanvasText *)(*it);

////		// is the text hit by the mouse ?
////		int ix = pEvent->pos().x();
////		int iy = pEvent->pos().y();
////		QRect r = item->boundingRect();
////		if( r.left()>=ix && ix<=r.right() &&
////			r.bottom()>=iy && iy<=r.top() )
////		{
////			// no
////			continue;
////		}

////		m_pMovingItem = *it;
////		m_aMovingStart = pEvent->pos();
////		return;
////    }
//    m_pMovingItem = 0;
//}

//void DrawingArea::contentsMouseMoveEvent( QMouseEvent * pEvent )
//{
//    if( m_pMovingItem )
//	{
//		m_pMovingItem->moveBy(pEvent->pos().x() - m_aMovingStart.x(),
//				       pEvent->pos().y() - m_aMovingStart.y());
//		m_aMovingStart = pEvent->pos();
////TODO Qt4		canvas()->update();

//		emit sigTextMoved();
//    }
//}

// *******************************************************************

DynamicTextDlgImpl::DynamicTextDlgImpl( minHandle<DynText> hItem, QWidget * parent, QWidget * pMain, Qt::WFlags fl )
: QDialog( parent, fl ),
  m_hItem( hItem ),
  m_aInitFont( hItem->font() )
{
    setupUi(this);

//	int x = m_pDrawingArea->width()-5;
//	int y = m_pDrawingArea->height()-5;
    m_pCanvas = new QGraphicsScene();

	QBoxLayout * l = new QVBoxLayout( m_pDrawingArea );
    l->setContentsMargins(0,0,0,0);

//	m_pDrawingAreaCanvas = new DrawingArea( m_pDrawingArea, this );
//	m_pDrawingAreaCanvas->setCanvas( m_pCanvas );

//    l->addWidget( m_pDrawingAreaCanvas );

    m_pCanvasText = new DynamicTextItem(this);
    m_pCanvasText->setText( m_hItem->GetString().c_str());
    m_pCanvasText->setFlag(QGraphicsItem::ItemIsMovable);
    m_pCanvas->addItem(m_pCanvasText);

// TODO Qt4 --> bewegen des Textes realisieren...

//    m_pCanvasText = new Q3CanvasText( m_hItem->GetString().c_str(), m_pCanvas );
//	sltRelPosToggled( m_pRelPos->isChecked() );

    connect( this, SIGNAL( sigDialogHelp(QWidget *, const QString &) ), pMain, SLOT( sltShowModalHelp(QWidget *, const QString &) ) );

//    connect( m_pCanvas, SIGNAL( changed(const QList<QRectF> & region) ), this, SLOT(sltUpdateData()) );
//    connect( m_pCanvas, SIGNAL( sceneRectChanged( const QRectF & rect ) ), this, SLOT(sltUpdateData()) );

    m_pDrawingArea->setScene(m_pCanvas);
    m_pDrawingArea->show();

    m_pCanvas->setSceneRect(0,0,this->m_pDrawingArea->width(),this->m_pDrawingArea->height());

    show();

    m_pText->setText( QString( hItem->text() ) );
    m_pText->setFocus();
    m_pFontName->setText( hItem->font().family() );
    QString sTemp;
    sTemp = sTemp.setNum( hItem->font().pointSize() );
    m_pFontSize->setText( sTemp );
//    	sTemp = sTemp.setNum( hItem->x() );
//    	aDlg.m_pPosX->setText( sTemp );
//    	sTemp = sTemp.setNum( hItem->y() );
//    	aDlg.m_pPosY->setText( sTemp );
    QColor aColor = hItem->brush().color();
    m_pSelectFontcolor->setPalette( QPalette( aColor ) );
    UpdateTextPosition();
    m_pCanvasText->show();

    double dStart, dDelta;
    hItem->GetDefaultData( dStart, dDelta );

    sTemp = sTemp.setNum( /*hItem->GetStartTime()*/dStart );
    m_pShowAtTime->setText( sTemp );
    sTemp = sTemp.setNum( dDelta );
    m_pShowTime->setText( sTemp );

    sltUpdateData();
}

DynamicTextDlgImpl::~DynamicTextDlgImpl()
{
//	delete m_pDrawingAreaCanvas;
    delete m_pCanvas;
}

void DynamicTextDlgImpl::updated()
{
    sltUpdateData();
    UpdateTextData();
}

void DynamicTextDlgImpl::UpdateTextPosition()
{
    double xRel,yRel;
    if( m_hItem->GetRelativePos( xRel, yRel ) && xRel>=0 && yRel>=0 )
    {
        SetRelPos( xRel, yRel );
    }
    else
    {
        m_pCanvasText->setPos(m_hItem->x(),m_hItem->y());
    }
}

void DynamicTextDlgImpl::UpdateTextData()
{
// TODO --> klaeren ob ueberhaupt item aktualisiert werden soll, bevor ok gedrueckt wurde --> abbrechen funktioniert dann naemlich nicht mehr !
    if( m_pRelPos->isChecked() )
    {
        m_hItem->SetRelativePos( GetRelX(), GetRelY() );
    }
    else
    {
        m_hItem->setPos( m_pCanvasText->x(), m_pCanvasText->y() );
        m_hItem->SetRelativePos( -1.0, -1.0 );
    }
}

double DynamicTextDlgImpl::GetRelX() const
{
    return (double)m_pCanvasText->x()/(double)m_pCanvas->width();
}

double DynamicTextDlgImpl::GetRelY() const
{
    return (double)m_pCanvasText->y()/(double)m_pCanvas->height();
}

void DynamicTextDlgImpl::SetRelPos( double xRel, double yRel )
{
	if( xRel>=0 && yRel>=0 )
	{
        double x = xRel*m_pCanvas->width();
        double y = yRel*m_pCanvas->height();
        m_pCanvasText->setPos( x, y );
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
    if( !bValue )
    {
        m_hItem->SetRelativePos(-1.0,-1.0);
    }
    else
    {
        m_hItem->SetRelativePos(GetRelX(),GetRelY());
    }
//	m_pDrawingAreaCanvas->setEnabled( bValue );
}

void DynamicTextDlgImpl::sltSelectFontcolor()
{
	QColor aColor = QColorDialog::getColor ( QColor(), this, "colordialog" );

	if( aColor.isValid() )
	{
        QPalette aPalette;
        aPalette.setColor(backgroundRole(),aColor);
        m_pSelectFontcolor->setPalette( aPalette );
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
    m_pPosRelX->setText( QString("%1").arg(GetRelX()*100.0,5,'f',2) );
    m_pPosRelY->setText( QString("%1").arg(GetRelY()*100.0,5,'f',2)  );

    m_pPosX->setText( QString("%1").arg(m_pCanvasText->x(),5,'f',0) );
    m_pPosY->setText( QString("%1").arg(m_pCanvasText->y(),5,'f',0) );
}

void DynamicTextDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Qt::Key_F1) )
	{
		emit sigDialogHelp( this, "DynGraphOpDlg" );
	}
	else
	{
        QDialog::keyPressEvent( pEvent );
	}
}

void DynamicTextDlgImpl::resizeEvent( QResizeEvent * event )
{
    QDialog::resizeEvent(event);
    m_pCanvas->setSceneRect(0,0,this->m_pDrawingArea->width()-2,this->m_pDrawingArea->height()-2);
    //cout << "resize " << this->m_pDrawingArea->width() << " " << this->m_pDrawingArea->height() << endl;
    //cout << "--> " << GetRelX() << " " << GetRelY() << " --> " << m_pCanvasText->x() << " " << m_pCanvasText->y() << endl;
    sltUpdateData();
    // update the relative positions after resize
    UpdateTextPosition();
}


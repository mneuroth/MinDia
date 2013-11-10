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

#include "dyntextdlgimpl.h"

#include "misctools.h"

#include <QColorDialog>
#include <QFontDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QBoxLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGraphicsRectItem>

// *******************************************************************
// *******************************************************************
// *******************************************************************

DynamicTextDlgImpl::DynamicTextDlgImpl( minHandle<DynText> hItem, int iIndex1, const QString & sUUID1, int iIndex2, const QString & sUUID2, QWidget * parent, QWidget * pMain, Qt::WFlags fl )
: QDialog( parent, fl ),
  m_hItem( hItem ),
  m_aInitFont( hItem->orgFont() )
{
    setupUi(this);

//	int x = m_pDrawingArea->width()-5;
//	int y = m_pDrawingArea->height()-5;
    m_pCanvas = new QGraphicsScene();

	QBoxLayout * l = new QVBoxLayout( m_pDrawingArea );
    l->setContentsMargins(0,0,0,0);

    m_pCanvasText = new DynamicTextItem(this);
    m_pCanvasText->setText( ToQString(m_hItem->GetString()) );
    m_pCanvasText->setFlag( QGraphicsItem::ItemIsMovable );

    m_pClippingArea = new QGraphicsRectItem();

    m_pCanvas->addItem(m_pCanvasText);
    m_pCanvas->addItem(m_pClippingArea);

    connect( this, SIGNAL( sigDialogHelp(QWidget *, const QString &) ), pMain, SLOT( sltShowModalHelp(QWidget *, const QString &) ) );

// TODO --> datenaustausch wie bei anderen Dialoge realisieren !
//    connect( m_pCanvas, SIGNAL( changed(const QList<QRectF> & region) ), this, SLOT(sltUpdateData()) );
//    connect( m_pCanvas, SIGNAL( sceneRectChanged( const QRectF & rect ) ), this, SLOT(sltUpdateData()) );

    m_pDrawingArea->setScene(m_pCanvas);
    m_pDrawingArea->show();

//    m_pCanvas->setSceneRect( 0, 0, this->m_pDrawingArea->width(), this->m_pDrawingArea->height());

    show();

    QFont aFont = hItem->orgFont();

    m_pText->setText( QString( hItem->text() ) );
    m_pText->setFocus();
    m_pFontName->setText( aFont.family() );
    QString sTemp;
    sTemp = sTemp.setNum( aFont.pointSize() );
    m_pFontSize->setText( sTemp );

    SetTextFont( aFont );

    QColor aColor = hItem->brush().color();
    SetTextColor( aColor );

    UpdateTextPosition();
    m_pCanvasText->show();

    double dStart, dDelta;
    hItem->GetDefaultData( dStart, dDelta );

    sTemp = sTemp.setNum( dStart );
    m_pShowAtTime->setText( sTemp );
    sTemp = sTemp.setNum( dDelta );
    m_pShowTime->setText( sTemp );

    if( hItem->IsAttachedToSlide() )
    {
        string sUUID = hItem->GetAttachedSlideUUID();
        if( sUUID==ToStdString(sUUID1) )
        {
            m_pChbAttachToImage1->setChecked(true);
        }
        else if( sUUID==ToStdString(sUUID2) )
        {
            m_pChbAttachToImage2->setChecked(true);
        }
    }

    if( !sUUID1.isEmpty() )
    {
        m_pAttachedImageUUID1->setText( sUUID1 );
        m_pAttachedImageIndex1->setText( QString("#%1").arg(iIndex1) );
    }

    if( !sUUID2.isEmpty() )
    {
        m_pAttachedImageUUID2->setText( sUUID2 );
        m_pAttachedImageIndex2->setText( QString("#%1").arg(iIndex2) );
    }
    else
    {
        // do not use setVisible, because size of cliping area is than not calculated correctly !
        m_pChbAttachToImage2->setEnabled(false);
        m_pAttachedImageUUID2->setEnabled(false);
        m_pAttachedImageIndex2->setEnabled(false);
    }

    sltUpdateData();
}

DynamicTextDlgImpl::~DynamicTextDlgImpl()
{
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
    return (double)m_pCanvasText->x()/(double)m_pClippingArea->rect().width();
}

double DynamicTextDlgImpl::GetRelY() const
{
    return (double)m_pCanvasText->y()/(double)m_pClippingArea->rect().height();
}

void DynamicTextDlgImpl::SetRelPos( double xRel, double yRel )
{
	if( xRel>=0 && yRel>=0 )
	{
        double x = xRel*m_pClippingArea->rect().width();
        double y = yRel*m_pClippingArea->rect().height();
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
}

void DynamicTextDlgImpl::sltSelectFontcolor()
{
	QColor aColor = QColorDialog::getColor ( QColor(), this, "colordialog" );

	if( aColor.isValid() )
	{
        SetTextColor( aColor );
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

        SetTextFont( aFont );
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

void DynamicTextDlgImpl::sltToggleImage1(bool bValue)
{
    if( bValue )
    {
        m_pChbAttachToImage2->setChecked(false);
    }
}

void DynamicTextDlgImpl::sltToggleImage2(bool bValue)
{
    if( bValue )
    {
        m_pChbAttachToImage1->setChecked(false);
    }
}

void DynamicTextDlgImpl::sltTextChanged( const QString & sNewText )
{
    m_pCanvasText->setText( sNewText );
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

    m_pCanvas->setSceneRect(0,0,this->m_pDrawingArea->width()-4,this->m_pDrawingArea->height()-4);
    sltUpdateData();
    // update the relative positions after resize
    UpdateTextPosition();
    // update clipping area size
    QSize aClippingAreaSize = GetRatioSizeForAvailableSize( m_pDrawingArea->size(), GetCurrentImageRatio() );
    m_pClippingArea->setRect(0,0,aClippingAreaSize.width()-4,aClippingAreaSize.height()-4);
}

void DynamicTextDlgImpl::SetTextColor( const QColor & aColor )
{
    QPalette aPalette = palette();
    aPalette.setBrush( QPalette::ButtonText, aColor);
    m_pSelectFontcolor->setPalette( aPalette );
    m_pCanvasText->setBrush( QBrush( aColor ) );
}

void DynamicTextDlgImpl::SetTextFont( const QFont & aFont )
{
    // calculate scaling factor for output of dynamic text
    QSize aClippingAreaSize = GetRatioSizeForAvailableSize( m_pDrawingArea->size(), GetCurrentImageRatio() );
    QSize aOutputAreaSize = GetRatioSizeForAvailableSize( GetCurrentOutputSize(), GetCurrentImageRatio() );
    double dScaleX = (double)aClippingAreaSize.width()/(double)aOutputAreaSize.width();
    //double dScaleY = (double)aClippingAreaSize.height()/(double)aOutputAreaSize.height();

    // use default size for button text
    QFont aTempFont = aFont;
    aTempFont.setPointSize( m_pSelectFont->font().pointSize() );
    m_pSelectFont->setFont( aTempFont );

    // scale dynamic text for output image size
    aTempFont = aFont;
    int iScaledFontSize = (int)((double)aFont.pointSize()*dScaleX);
    aTempFont.setPointSize( iScaledFontSize );
    m_pCanvasText->setFont( aTempFont );
}

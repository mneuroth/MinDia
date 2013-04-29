/**************************************************************************
 *ƒ
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2013 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "pddlgimpl.h"

#include "diapresentation.h"
#include "misctools.h"

#include <QShowEvent>
#include <QKeyEvent>
#include <QCloseEvent>

/*
const QString g_sDefaultSize1 = QObject::tr("400:304");
const QString g_sDefaultSize2 = QObject::tr("576:400");
// 640x480   // NTSC (VGA)      4:3
// 800x600
// 1024x768
// 720x480   // NTSC (DV)       3:2
// 768x576   // PAL (DVB)       4:3
// 1024x576  // PAL-wide (DVB)  16:9
// 704x576   // PAL (DVD)
// 720x576   // PAL (DV)
// 960x720   // HD720 HDTV      4:3
// 1440x1080 // HD1080 HDTV     4:3
const QString g_sDefaultSize3 = QObject::tr("1280:720");        // 720p:  hd ready
const QString g_sDefaultSize4 = QObject::tr("1920:1080");       // 1080p: full hd
const QString g_sSizeOfFirstImage = QObject::tr("size of first image");
const QString g_sUserValue = QObject::tr("user value");

http://de.wikipedia.org/wiki/Bildaufl%C3%B6sung
http://de.wikipedia.org/wiki/DVD-Video
http://de.wikipedia.org/wiki/Vorlage:Monitoraufl%C3%B6sungen

16:9
----
7680x4320   UHDV
2048x1152   QWXGA
1920x1080   1080p, HDTV, Blu-ray        *
1280x720    720p                        *
1024x576    PAL widescreen              *
960x540     PAL optimal                 *

3:2
---
1920x1280                               *
720x480     NTSC (DV)                   *
480x320

4:3
---
4096x3072   4K
2048x1536   2K
1920x1440                               *
1440x1080   HDTV                        *
800x600                                 *
640x480     VGA                         *
720x576     DVB (PAL), DVD-Video (PAL)  *
533x400     S-VHS                       *
533x400     Hi8
320x240     VHS                         *

16:9            3:2             4:3                 ID  Enum
------------------------------------------------------------------
1920x1080 *     1920x1280       1920:1440          10   RESX_1920
1440x810        1440x960        1440x1080           9   RESX_1440
1280x720  *     1280x853        128x960             8   ...
1024x576  *     1024x682        1024x768    XGA     7
960x540   *     960x640         960x720             6
800x450         800x533         800x600     SVGA    5
720x405         720x480  *      720x540     *       4
640x360         640x426         640x480     VGA     3
533x299         533x355         533x400     *       2
320x180         320x213         320x240     *       1
                                                    0
*/

PresentationDataDlgImpl::PresentationDataDlgImpl( DiaPresentation * pData, QWidget* parent, Qt::WFlags fl )
  : QDialog(parent, fl),
    m_pData( pData )
{
    setupUi(this);

    m_pOutputSize->addItem("user");
    m_pOutputSize->addItem("1920:1080");
    m_pOutputSize->addItem("1280:720");

    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltPresentationDataDialogClosed() ) );
    connect( this, SIGNAL( sigDocumentUpdate() ), parent, SLOT( sltDoDocumentStateUpdate() ) );
    connect( this, SIGNAL( sigDialogHelp(QWidget *, const QString &) ), parent, SLOT( sltShowModalHelp(QWidget *, const QString &) ) );
}

PresentationDataDlgImpl::~PresentationDataDlgImpl()
{
}

void PresentationDataDlgImpl::sltCloseDialog()
{
	TransferDataFromControl();

	emit accept();

	emit sigDialogClosed();
}

void PresentationDataDlgImpl::sltCancelDialog()
{
	emit reject();

	emit sigDialogClosed();
}

void PresentationDataDlgImpl::sltCurrentOutputSizeChanged( const QString & sValue )
{
    if( sValue=="user" )
    {
        m_pOutputWidth->setEnabled( true );
        m_pOutputHeight->setEnabled( true );
    }
    else
    {
        m_pOutputWidth->setEnabled( false );
        m_pOutputHeight->setEnabled( false );
    }
}

void PresentationDataDlgImpl::sltUserWidthChanged( const QString & sValue )
{
    int iValue = sValue.toInt();
    double dFactor = GetFactorForImageRatio( GetCurrentImageRatio() );
    m_pOutputHeight->setText( QString("%1").arg((int)((double)iValue/dFactor)) );
}

void PresentationDataDlgImpl::sltImageRatioChanged()
{
    sltUserWidthChanged( m_pOutputWidth->text() );
}

void PresentationDataDlgImpl::closeEvent( QCloseEvent * pCloseEvent )
{
	pCloseEvent->accept();

	emit sigDialogClosed();
}

void PresentationDataDlgImpl::showEvent( QShowEvent * /*pShowEvent*/ )
{
	TransferDataToControl();
}

void PresentationDataDlgImpl::TransferDataToControl()
{
	// ** init the data
	if( m_pComment && m_pData )
	{
        QString sComment = ToQString(m_pData->GetComment());

		m_pComment->setText( sComment );

        ImageRatio aImageRatio = m_pData->GetImageRatio();

        switch( aImageRatio )
        {
            case RATIO_16_9:
               m_prbt16_9->setChecked(true);
               break;
            case RATIO_3_2:
               m_prbt3_2->setChecked(true);
               break;
            case RATIO_4_3:
               m_prbt4_3->setChecked(true);
               break;
            case RATIO_IMAGE_RATIO:
               m_prbtImageRatio->setChecked(true);
               break;
            case RATIO_VARIABLE:
               m_prbtVariable->setChecked(true);
               break;
            case RATIO_USER:
               m_prbtUser->setChecked(true);
// TODO --> user daten fuer ration behandeln (not implemented yet)
               break;
            case RATIO_UNDEFINED:
            default:
                // ignore
                ;
        }
    }
}

void PresentationDataDlgImpl::TransferDataFromControl()
{
	if( m_pComment && m_pData )
	{
        bool bUpdate = false;
        QString sComment = m_pComment->document()->toPlainText();

		// ** has text really changed ?
		string sOrg = m_pData->GetComment();
        if( ToQString( sOrg ) != sComment )
		{
            m_pData->SetComment( ToStdString(sComment) );

            bUpdate = true;
		}

        ImageRatio aImageRatio = m_pData->GetImageRatio();
        ImageRatio aImageRatioNew = GetCurrentImageRatio();

        if( aImageRatio != aImageRatioNew )
        {
// TODO --> ggf. message dialog anzeigen ob Aenderungen wirklich uebernommen werden sollen !?
            m_pData->SetImageRatio( aImageRatioNew );

            bUpdate = true;
        }

// TODO gulp --> image output size setzen ...

        if( bUpdate )
        {
            emit sigDocumentUpdate();
        }
	}
}

void PresentationDataDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Qt::Key_F1) )
	{
		emit sigDialogHelp( this, "PresentationDataDialog" );
	}
	else
	{
        QDialog::keyPressEvent( pEvent );
	}
}

ImageRatio PresentationDataDlgImpl::GetCurrentImageRatio() const
{
    ImageRatio aImageRatioNew = RATIO_UNDEFINED;

    if( m_prbt16_9->isChecked() )
    {
        aImageRatioNew = RATIO_16_9;
    }
    else if( m_prbt3_2->isChecked() )
    {
        aImageRatioNew = RATIO_3_2;
    }
    else if( m_prbt4_3->isChecked() )
    {
        aImageRatioNew = RATIO_4_3;
    }
    else if( m_prbtVariable->isChecked() )
    {
        aImageRatioNew = RATIO_VARIABLE;
    }
    else if( m_prbtImageRatio->isChecked() )
    {
        aImageRatioNew = RATIO_IMAGE_RATIO;
    }
    else if( m_prbtUser->isChecked() )
    {
        aImageRatioNew = RATIO_USER;
// TODO --> user daten fuer ration behandeln (not implemented yet)
    }

    return aImageRatioNew;
}

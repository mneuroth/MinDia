/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/createmoviedlgimpl.cpp,v $
 *
 *  $Revision: 1.1 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.2  2004/02/16 19:49:52  min
 *	Cast problem with QString solved.
 *	
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

#include "createmoviedlgimpl.h"

#include <qcombobox.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qfiledialog.h>

#include <stdio.h>		// for: sprintf

// *******************************************************************
// *******************************************************************
// *******************************************************************

const QString g_sDefaultSize1 = QObject::tr("400:304");
const QString g_sSizeOfFirstImage = QObject::tr("size of first image");
const QString g_sUserValue = QObject::tr("user value");

CreateMovieDlgImpl::CreateMovieDlgImpl( double dTotalTimeMS, QWidget* parent, const char* name, bool modal, WFlags fl )
: CreateMovieDlg( parent, name, modal, fl )
{
	m_dTotalTimeMS = dTotalTimeMS;

	m_pImagesPerSecond->setMinValue(1);
	m_pImagesPerSecond->setMaxValue(100);
	m_pImagesPerSecond->setValue(10);

	m_pImageRatio->insertItem( g_sDefaultSize1 );
	m_pImageRatio->insertItem( g_sUserValue );
	m_pImageRatio->insertItem( g_sSizeOfFirstImage );
	m_pImageRatio->setCurrentItem( 0 );
		sltImageRatioSelected( g_sDefaultSize1 );

	m_pImageNameOffset->setText( "image" );

	m_pDirectoryName->setText( "movie" );

	m_pGeneratorCmd->setText( "jpeg2yuv -I p -f 10 -j movie\\image%05d.jpg | yuv2lav -o movie\\movie.avi" );
}

CreateMovieDlgImpl::~CreateMovieDlgImpl()
{
}

void CreateMovieDlgImpl::sltSelectOutputDirectory()
{
	QString sDir = QFileDialog::getExistingDirectory( m_pDirectoryName->text() );

	if( !sDir.isEmpty() )
	{
		m_pDirectoryName->setText( sDir );
	}
}

void CreateMovieDlgImpl::sltImageRatioSelected( const QString & sValue )
{
	if( sValue==g_sDefaultSize1 )
	{
		m_pImageWidth->setText( "400" );
		m_pImageHeight->setText( "304" );
		m_pImageWidth->setEnabled( false );
		m_pImageHeight->setEnabled( false );
	}
	else if( sValue==g_sUserValue )
	{
		m_pImageWidth->setText( "" );
		m_pImageHeight->setText( "" );
		m_pImageWidth->setEnabled( true );
		m_pImageHeight->setEnabled( true );
	}
	else if( sValue==g_sSizeOfFirstImage )
	{
		m_pImageWidth->setText( "?" );
		m_pImageHeight->setText( "?" );
		m_pImageWidth->setEnabled( false );
		m_pImageHeight->setEnabled( false );
	}
}


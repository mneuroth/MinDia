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
 *  $Revision: 1.2 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.1  2004/04/09 19:27:27  min
 *	Dialog for movie support
 *	
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

#include "doccontroler.h"

#include <qcombobox.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qfiledialog.h>
#include <qprocess.h>

#include <stdio.h>		// for: sprintf

// *******************************************************************
// *******************************************************************
// *******************************************************************

const QString g_sDefaultSize1 = QObject::tr("400:304");
const QString g_sSizeOfFirstImage = QObject::tr("size of first image");
const QString g_sUserValue = QObject::tr("user value");

CreateMovieDlgImpl::CreateMovieDlgImpl( DocumentAndControler * pDocControler, double dTotalTimeMS, QWidget* parent, const char* name, bool modal, WFlags fl )
: CreateMovieDlg( parent, name, modal, fl ),
  m_pProcess( 0 )
{
	m_dTotalTimeMS = dTotalTimeMS;
	m_pDocControler = pDocControler;

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

	UpdateCmds();
}

CreateMovieDlgImpl::~CreateMovieDlgImpl()
{
	delete m_pProcess;
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

void CreateMovieDlgImpl::sltCreateImages()
{
	QString sName = m_pImageNameOffset->text();
	QString sDir = m_pDirectoryName->text();
	int iImagesPerSecond = m_pImagesPerSecond->text().toInt();
	int iWidth = m_pImageWidth->text().toInt();
	int iHeight = m_pImageHeight->text().toInt();

	double dDeltaInMS = 1/((double)iImagesPerSecond)*1000.0;

	int iCount = m_pDocControler->CreateImagesForMovie( 
			(const char *)sDir, (const char *)sName, 
			iWidth, iHeight,
			0, m_pDocControler->GetPresentation().GetTotalTime()*1000, dDeltaInMS );

	QString sOutput;
	sOutput.sprintf( tr("Created %d images"), iCount );
	m_pOutput->append( sOutput );
}

void CreateMovieDlgImpl::sltCreateAVI()
{
	QString sCmd = m_pGeneratorCmd->text();

	QStringList aCmdList = QStringList::split( " ", sCmd );

	if( m_pProcess )
	{
		delete m_pProcess;
		m_pProcess = 0;
	}

	m_pProcess = new QProcess( this );
	//m_pProcess->setCommunication(0);

	for( unsigned int i=0; i<aCmdList.size(); i++ )
	{
		m_pProcess->addArgument( *aCmdList.at(i) );
		m_pOutput->append( *aCmdList.at(i) );
	}

	connect( m_pProcess, SIGNAL(readyReadStdout()), this, SLOT(sltReadFromStdout()) );
     
	m_pProcess->start();

//	int iRet = system( (const char *)sCmd );

//_ spawn

//	QString sOutput;
//	sOutput.sprintf( tr("Created subprocess ret=%d"), iRet );
//	m_pOutput->append( sOutput );
}

void CreateMovieDlgImpl::sltAddSound()
{
	if( m_pProcess )
	{
		delete m_pProcess;
		m_pProcess = 0;
	}

	m_pProcess = new QProcess( this );

	m_pProcess->addArgument("cmd.exe");
	m_pProcess->addArgument("/k");
	m_pProcess->addArgument("dir");
	m_pProcess->addArgument("/od");
	m_pProcess->addArgument("|");
	m_pProcess->addArgument("grep");
	m_pProcess->addArgument("INSTALL");

	connect( m_pProcess, SIGNAL(readyReadStdout()), this, SLOT(sltReadFromStdout()) );
     
	m_pProcess->start();
}

void CreateMovieDlgImpl::sltCreateVCD()
{
}

void CreateMovieDlgImpl::sltMakeShow()
{
}

void CreateMovieDlgImpl::sltDeleteTempFiles()
{
}

void CreateMovieDlgImpl::sltUpdateCmds()
{
//	UpdateCmds();
}

void CreateMovieDlgImpl::sltReadFromStdout()
{
	QString sOutput = m_pProcess->readLineStdout();
	
	while( !sOutput.isNull() )
	{
		m_pOutput->append( sOutput );

		sOutput = m_pProcess->readLineStdout();
	}
}

void CreateMovieDlgImpl::sltReadFromStderr()
{
}

void CreateMovieDlgImpl::UpdateCmds()
{
	QString sCmd;
	//sCmd.sprintf( "jpeg2yuv -I p -f %d -j movie\\image%05d.jpg | yuv2lav -o movie\\movie.avi", iImagesPerSecond );

	m_pGeneratorCmd->setText( "jpeg2yuv -I p -f 10 -j movie\\image%05d.jpg | yuv2lav -o movie\\movie.avi" );
	m_pSoundGeneratorCmd->setText( "lavaddwav movie\\movie.avi movie.wav movie\\movie.avi" );
	m_pVCDGeneratorCmd->setText( "lav2yuv movie\\movie.avi | yuvscaler -O VCD | mpeg2enc -f 1 -r 16 -o movie\\movie.mpg" );
}

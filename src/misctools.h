/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2003 by Michael Neuroth
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

#ifndef _MISCTOOLS_H
#define _MISCTOOLS_H

#include <QImage>
#include <QString>
#include <QSize>

#include <string>

using namespace std;

class QDropEvent;

// *******************************************************************
enum ImageRatio {
    RATIO_UNDEFINED,
    RATIO_16_9,
    RATIO_3_2,
    RATIO_4_3,
    RATIO_IMAGE_RATIO,
    RATIO_VARIABLE,
    RATIO_USER
};

// *******************************************************************
enum ImageSize {
    SIZE_UNDEFINED,
    SIZE_1920_1080,
    SIZE_1280_720
};

// *******************************************************************

QImage CreateWhiteImage( int iWidth = 1024, int iHeight = 768 );

QImage GetImageFromFileName( const QString & sImageFileName );

QImage CopyImageArea( const QImage & aImage, double relX, double relY, double relDX, double relDY );

bool ReadQImage( const QString & sFileName, QImage & aImageOut );

void ReadQImageOrEmpty( const QString & sFileName, QImage & aImageOut );

bool IsDiaDataFile( const QString & sFileName );

bool IsDiaDataFileDrag( const QDropEvent * pEvent, QString & sFileNameOut );

bool IsImageFileDrag( const QDropEvent * pEvent );

bool IsSoundFileDrag( const QDropEvent * pEvent );

string ToStdString( const QString & sStrg );

QString ToQString( const string & sStrg );

bool IsImageFile( const QString & sFileName );

QString ConvertToRelPath( const QString & sPath );
string ConvertToRelPath( const string & sPath );

QString ConvertToAbsPath( const QString & sPath, const QString & sDir );
string ConvertToAbsPath( const string & sPath, const string & sDir );

double GetFactorForImageRatio( ImageRatio ratio );
ImageRatio GetCurrentImageRatio();
QSize GetCurrentOutputSize();

// return a size object for the given ratio
// which fits perfectly into the given availabale size
QSize GetRatioSizeForAvailableSize( QSize aAvailableSize, ImageRatio ratio );

#endif

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
    SIZEX_UNDEFINED = 0,
    SIZEX_320 = 320,
    SIZEX_533 = 533,
    SIZEX_640 = 640,
    SIZEX_720 = 720,
    SIZEX_800 = 800,
    SIZEX_960 = 960,
    SIZEX_1024 = 1024,
    SIZEX_1280 = 1280,
    SIZEX_1440 = 1440,
    SIZEX_1920 = 1920
};
/*
16:9            3:2             4:3                 ID  Enum
------------------------------------------------------------------
1920x1080 *     1920x1280       1920:1440          10   SIZEX_1920
1440x810        1440x960        1440x1080           9   SIZEX_1440
1280x720  *     1280x853        128x960             8   ...
1024x576  *     1024x682        1024x768    XGA     7
960x540   *     960x640         960x720             6
800x450         800x533         800x600     SVGA    5
720x405         720x480  *      720x540     *       4
640x360         640x426         640x480     VGA     3
533x299         533x355         533x400     *       2
320x180         320x213         320x240     *       1
*/

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
QSize GetSizeForSizeX( ImageSize aImageSize );
QStringList GetSizeStrings( ImageRatio ratio );
ImageSize GetImageSizeTypeFromSize( const QSize & aSize );

// return a size object for the given ratio
// which fits perfectly into the given availabale size
QSize GetRatioSizeForAvailableSize( QSize aAvailableSize, ImageRatio ratio );

double GetScaleFactorFor( int iWidth, int iHeight );

#endif

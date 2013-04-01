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

#include <string>

using namespace std;

class QDropEvent;

QImage CreateWhiteImage();

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

#endif

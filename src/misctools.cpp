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
 * Copyright (C) 2013 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "misctools.h"

#include "mindiawindow.h"

#include <QImage>
#include <QImageReader>
#include <QList>
#include <QUrl>
#include <QMimeData>
#include <QMimeSource>
#include <QDropEvent>
#include <QString>
#include <QMap>
#include <QPair>
#include <QDir>

#include <stdio.h>
#include <ctype.h>

#include <string>
//#include <fstream>
#include <locale.h>
#include <algorithm>

using namespace std;

// *******************************************************************

class QImageCache
{
public:
    QImageCache(int iMaxWidth, int iMaxHeight, int iMaxItems);

    const QImage & Get( const QString & sImageFileName );

private:
    void CheckCacheSpace();

    int                                         m_iMaxItems;
    int                                         m_iMaxWidth, m_iMaxHeight;
    QMap<QString,QPair<QImage,unsigned long> >  m_aMap;
    QImage                                      m_aEmptyImage;
};

QImageCache::QImageCache(int iMaxWidth = 1920, int iMaxHeight = 1080, int iMaxItems = 20)
    : m_iMaxItems(iMaxItems),
      m_iMaxWidth(iMaxWidth),
      m_iMaxHeight(iMaxHeight)
{
}

void QImageCache::CheckCacheSpace()
{
    QString sFoundKey;
    unsigned long ulCount = 0xFFFF;
    if( m_aMap.size()>=m_iMaxItems )
    {
        // find image with lowest usage count and remove this item from cache
        foreach (const QString & sKey, m_aMap.keys())
        {
            if( ulCount>m_aMap[sKey].second )
            {
                ulCount = m_aMap[sKey].second;
                sFoundKey = sKey;
            }
        }
        if( sFoundKey.length()>0 )
        {
            m_aMap.remove(sFoundKey);
        }
    }
}

const QImage & QImageCache::Get( const QString & sImageFileName )
{
    // exists an entry for the requested image ?
    if( !m_aMap.contains(sImageFileName) )
    {
        // NO
        CheckCacheSpace();
        // add a new image to the cache
        QImage aImage(sImageFileName);
        // scale to maximum image size (if needed)
        if( aImage.width()>m_iMaxWidth || aImage.height()>m_iMaxHeight )
        {
            aImage = aImage.scaled(m_iMaxWidth,m_iMaxHeight,Qt::KeepAspectRatio);
        }
        if( !aImage.isNull() )
        {
            m_aMap[sImageFileName] = QPair<QImage,unsigned long>(aImage,1);
        }
        return m_aMap[sImageFileName].first;
    }
    else
    {
        // YES
        // increment access counter
        m_aMap[sImageFileName].second += 1;
        return m_aMap[sImageFileName].first;
    }
    return m_aEmptyImage;
}

QImageCache g_aImageCache;

// *******************************************************************

// help function to create a image with a white background
QImage CreateWhiteImage()
{
    const int c_iWidth = 3;
    const int c_iHeight = 3;

    QImage aImage( c_iWidth, c_iHeight, /*32*/QImage::Format_RGB32 );

    for( int y=0; y<c_iHeight; y++ )
    {
        uchar * pLine = aImage.scanLine( y );

        for( int x=0; x<c_iWidth*4; x=x+4 )
        {
            pLine[x] = 255;
            pLine[x+1] = 255;
            pLine[x+2] = 255;
            pLine[x+3] = 0;
        }
    }

    return aImage;
}

QImage GetImageFromFileName( const QString & sImageFileName )
{
    QImage aImage;

    if( !sImageFileName.isEmpty() )
    {
        /*bool bOk =*/ ReadQImage( sImageFileName, aImage );
    }
    else
    {
        aImage = CreateWhiteImage();
    }

    return aImage;
}

QRect GetArea( const QSize & aImageSize, double relX, double relY, double relDX, double relDY )
{
    QRect rect;

    rect.setX( (int)(((double)aImageSize.width())*relX) );
    rect.setY( (int)(((double)aImageSize.height())*relY) );
    rect.setWidth( (int)(((double)aImageSize.width())*relDX) );
    rect.setHeight( (int)(((double)aImageSize.height())*relDY) );

    return rect;
}

QImage GetImageArea( const QImage & aImage, double relX, double relY, double relDX, double relDY )
{
    return aImage.copy( GetArea( aImage.size(), relX, relY, relDX, relDY ) );
}

bool ReadQImage( const QString & sFileName, QImage & aImageOut )
{
    aImageOut = g_aImageCache.Get(sFileName);
	return !aImageOut.isNull();
}

void ReadQImageOrEmpty( const QString & sFileName, QImage & aImageOut )
{
    if( !ReadQImage(sFileName,aImageOut) )
    {
        aImageOut = CreateWhiteImage();
    }
}

bool IsDiaDataFile( const QString & sFileName )
{
	// check if the given file name has .dia as extension
    if( !sFileName.isNull() && sFileName.length()>0 )
	{
        QString sTemp = sFileName.toLower();

        if( (sTemp.indexOf(".dia")>=0) )
		{
			return true;
		}
	}
	return false;
}

bool IsDiaDataFileDrag( const QDropEvent * pEvent, QString & sFileNameOut )
{
	if( pEvent->mimeData()->hasUrls() )
	{
        QList<QUrl> aLst = pEvent->mimeData()->urls();
        
        if( aLst.count() == 1 ) 
        {
			const QString sTest = aLst.at( 0 ).toLocalFile();

            if( IsDiaDataFile( sTest ) )
			{
                sFileNameOut = sTest;
				return true;
			}
        }        
	}

	return false;
}

bool IsImageFileDrag( const QDropEvent * pEvent )
{
	if( pEvent->mimeData()->hasUrls() )
	{
        QList<QUrl> aLst = pEvent->mimeData()->urls();

		// all droped file-names have to be images with known format !!!
		for( int i=0; i<(int)aLst.count(); i++ )
		{
			const QString sTest = aLst.at( i ).toLocalFile();
    
            if( QImage(sTest).isNull() )
			{
                return false;
			}

////TODO Qt4:
////            if( (aReader.imageFormat( sFileName )==/*0*/QImage::Format_Invalid) && !IsJPEG( s ) )
////			if( !IsJPEG( s ) )
//  //          QImageReader aReader(sFileName);
//  //          if( !aReader.canRead() )
//            if( QImage(sFileName ).isNull() )
//			{
//                return false;
//			}
        }
        
		return true;
	}

	return false;
}

static bool IsWAVorMP3( const QString & sFileName )
{
	// check if the given file name has .jpg or .jpeg as extension
    if( !sFileName.isNull() && sFileName.length()>0 )
	{
        QString sTemp = sFileName.toLower();
		
        if( (sTemp.indexOf(".wav")>=0) || (sTemp.indexOf(".mp3")>=0) )
		{
			return true;
		}
	}
	return false;
}

bool IsSoundFileDrag( const QDropEvent * pEvent )
{
	if( pEvent->mimeData()->hasUrls() )
	{        
        QList<QUrl> aLst = pEvent->mimeData()->urls();
        
		// all droped file-names have to be images with known format !!!
		for( int i=0; i<(int)aLst.count(); i++ )
		{
			const QString sTest = aLst.at( i ).toLocalFile();
    
            if( !IsWAVorMP3( sTest ) )
			{
				return false;
			}
		}
        
		return true;
	}

	return false;
}

string ToStdString( const QString & sStrg )
{
    return sStrg.toLocal8Bit().constData();
}

QString ToQString( const string & sStrg )
{
    return QString::fromLocal8Bit( sStrg.c_str() );
}

bool IsImageFile( const QString & sFileName )
{
    bool ok = true;
    QStringList lstFileNames = sFileName.split("\n");
    foreach( const QString & s, lstFileNames )
    {
        QImage aImage( s );
        ok = ok && !aImage.isNull();
    }
    return ok;
}

QString ConvertToRelPath( const QString & sPath )
{
    QDir aCurrDir;  // == currentPath()
    return aCurrDir.relativeFilePath( sPath );
    // original: FileUtilityObj::ConvertToRelPath()
}

string ConvertToRelPath( const string & sPath )
{
    return ToStdString( ConvertToRelPath( ToQString( sPath ) ) );
}

QString ConvertToAbsPath( const QString & sPath, const QString & sDir )
{
    QDir aCurrDir(sDir);
    return aCurrDir.absoluteFilePath( sPath );
}

string ConvertToAbsPath( const string & sPath, const string & sDir )
{
    return ToStdString( ConvertToAbsPath( ToQString( sPath ), ToQString( sDir ) ) );
}

ImageRatio GetCurrentImageRatio()
{
    return GetCurrentPresentation()->GetImageRatio();
}

QSize GetSizeFor( ImageRatio ratio, QSize aSize = QSize(1920,1920) )
{
    const int dxRef = 1920;

    switch( ratio )
    {
        case RATIO_16_9:
            aSize.setWidth(dxRef);
            aSize.setHeight(dxRef*9/16);
            break;
        case RATIO_3_2:
            aSize.setWidth(dxRef);
            aSize.setHeight(dxRef*2/3);
            break;
        case RATIO_4_3:
            aSize.setWidth(dxRef);
            aSize.setHeight(dxRef*3/4);
            break;
        case RATIO_IMAGE_RATIO:
            // return unmodified size
            break;
        case RATIO_VARIABLE:
            break;
        case RATIO_USER:
            // TODO
            break;

        case RATIO_UNDEFINED:
        default:
            ; // ignore
    }

    return aSize;
}

QSize GetMaximumSizeFor( QSize aSize, ImageRatio ratio )
{
    QSize aRect = GetSizeFor( ratio );

    aRect.scale( aSize, Qt::KeepAspectRatio );

    return aRect;
}

QSize GetRatioSizeForAvailableSize( QSize aAvailableSize, ImageRatio ratio )
{
    QSize aRet = GetSizeFor( ratio, aAvailableSize );
    aRet.scale( aAvailableSize, Qt::KeepAspectRatio );
    return aRet;
}

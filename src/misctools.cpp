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

#include <math.h>

#include <QImage>
#include <QImageReader>
#include <QList>
#include <QUrl>
#include <QDropEvent>
#include <QString>
#include <QMap>
#include <QPair>
#include <QDir>
#include <QMimeData>

// *******************************************************************

class QImageCache
{
public:
    QImageCache(int iMaxWidth = 1920, int iMaxHeight = 1080, int iMaxItems = 20);

    const QImage & Get( const QString & sImageFileName );

    unsigned long GetCacheSize() const;

private:
    void CheckCacheSpace();

    int                                         m_iMaxItems;
    int                                         m_iMaxWidth, m_iMaxHeight;
    QMap<QString,QPair<QImage,unsigned long> >  m_aMap;
    QImage                                      m_aEmptyImage;
};

QImageCache::QImageCache(int iMaxWidth, int iMaxHeight, int iMaxItems)
    : m_iMaxItems(iMaxItems),
      m_iMaxWidth(iMaxWidth),
      m_iMaxHeight(iMaxHeight)
{
}

unsigned long QImageCache::GetCacheSize() const
{
    unsigned long ulSize = 0;
    foreach (const QString & sKey, m_aMap.keys())
    {
        ulSize += m_aMap[sKey].first.byteCount();
    }
    return ulSize;
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
// TODO: disable cache to improve quality ! --> necessary for zoom into images !
        // limit size of image in cach --> otherwise 20 x 12MPixel images == 1GByte !
        if( aImage.width()>m_iMaxWidth || aImage.height()>m_iMaxHeight )
        {
            aImage = aImage.scaled(m_iMaxWidth,m_iMaxHeight,Qt::KeepAspectRatio);
        }
        //if( !aImage.isNull() )
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
QImage CreateWhiteImage( int iWidth, int iHeight )
{
    return CreateColorImage( QColor("white"), iWidth, iHeight );
}

QImage CreateColorImage( const QColor & aColor, int iWidth, int iHeight )
{
    QImage aImage( iWidth, iHeight, /*32*/QImage::Format_RGB32 );
    aImage.fill( aColor );
    return aImage;
}

static QSize GetSizeFor( ImageRatio ratio, QSize aSize = QSize(1920,1920) )
{
    const int dxRef = aSize.width();

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

/*
static QSize GetMaximumSizeFor( QSize aSize, ImageRatio ratio )
{
    QSize aRect = GetSizeFor( ratio );

    aRect.scale( aSize, Qt::KeepAspectRatioByExpanding );

    return aRect;
}
*/

QSize GetRatioSizeForAvailableSize( QSize aAvailableSize, ImageRatio ratio )
{
    QSize aRet = GetSizeFor( ratio );

    aRet.scale( aAvailableSize, Qt::KeepAspectRatio );

    return aRet;
}

static QRect GetArea( const QSize & aSrcImageSize, double relX, double relY, double relDX, double relDY, ImageRatio targetRatio )
{
    QRect rect;

    QSize aRatioSize = GetRatioSizeForAvailableSize( aSrcImageSize, targetRatio );

    double dSrcFactor = (double)aSrcImageSize.width()/(double)aSrcImageSize.height();
    double dTargetFactor = (double)aRatioSize.width()/(double)aRatioSize.height();
    double dFactorX = dTargetFactor/dSrcFactor;
    double dFactorY = dSrcFactor/dTargetFactor;

    if( dSrcFactor<dTargetFactor )
    {
        dFactorX = 1.0;
    }
    else
    {
        dFactorY = 1.0;
    }

    rect.setX( (int)(((double)aSrcImageSize.width())*relX*dFactorX) );
    rect.setY( (int)(((double)aSrcImageSize.height())*relY*dFactorY) );
    rect.setWidth( (int)(((double)aSrcImageSize.width())*relDX*dFactorX) );
    rect.setHeight( (int)(((double)aSrcImageSize.height())*relDY*dFactorY) );

    return rect;
}

const double EPSILON = 0.00001;

inline bool IsInEpsilon( double value, double pos )
{
    return fabs(value-pos)<=EPSILON;
}

/*
static bool IsFullArea( double relX, double relY, double relDX, double relDY )
{
    return IsInEpsilon( relX, 0.0 ) && IsInEpsilon( relY, 0.0 ) && IsInEpsilon( relDX, 1.0 ) && IsInEpsilon( relDY, 1.0 );
}
*/

QImage CopyImageArea( const QImage & aImage, double relX, double relY, double relDX, double relDY )
{
    return aImage.copy( GetArea( aImage.size(), relX, relY, relDX, relDY, GetCurrentImageRatio() ) );
}

QImage ReadQImage( const QString & sFileName, int /*maxWidth*/, int /*maxHeight*/ )
{
    QImage aImageOut;
// TODO --> Performance Optimierung: ggf. inklusive image area cachen !? --> siehe CopyImageArea !
    if( QColor::isValidColor(sFileName) )
    {
        aImageOut = CreateColorImage( QColor(sFileName) );
    }
    else
    {
        aImageOut = g_aImageCache.Get(sFileName);
    }
    return aImageOut;
}

QImage ReadQImageOrEmpty( const QString & sFileName, int maxWidth, int maxHeight )
{
    QImage aImageOut = ReadQImage(sFileName, maxWidth, maxHeight );
    if( aImageOut.isNull() )
    {
        aImageOut = CreateWhiteImage( maxWidth, maxHeight );
    }
    return aImageOut;
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

QSize GetCurrentOutputSize()
{
    unsigned long ulWidth, ulHeight;
    GetCurrentPresentation()->GetImageSize( ulWidth, ulHeight );
    return QSize( ulWidth, ulHeight );
}

QSize GetSizeForSizeX( ImageSize aImageSize, ImageRatio aImageRatio )
{
    int iWidth = (int)aImageSize;
    return QSize( iWidth, (int)((double)iWidth/GetFactorForImageRatio(aImageRatio)) );
}

QSize GetSizeForSizeX( ImageSize aImageSize )
{
    return GetSizeForSizeX(aImageSize,GetCurrentImageRatio());
}

double GetFactorForImageRatio( ImageRatio ratio )
{
    QSize aSize = GetRatioSizeForAvailableSize( QSize(1920,1920), ratio );
    return (double)aSize.width()/(double)aSize.height();
}

/*
static ImageRatio GetImageRatio( const QSize & aSize )
{
    double dFactor = (double)aSize.width()/(double)aSize.height();

    if( IsInEpsilon( dFactor-1.333333333,EPSILON ) )
    {
        return RATIO_4_3;
    }
    else if( IsInEpsilon( dFactor-1.5,EPSILON ) )
    {
        return RATIO_3_2;
    }
    else if( IsInEpsilon( dFactor-1.777777777,EPSILON ) )
    {
        return RATIO_16_9;
    }
    else
    {
        return RATIO_USER;
    }
}
*/

QString GetSizeString( const QSize & aSize )
{
    if( aSize.width()==0 )
    {
        return QString("user");
    }
    return QString("%1:%2").arg(aSize.width()).arg(aSize.height());
}

QStringList GetSizeStrings( ImageRatio ratio )
{
    QStringList lstStrings;

    lstStrings.append( GetSizeString( GetSizeForSizeX( SIZEX_UNDEFINED, ratio ) ) );
    lstStrings.append( GetSizeString( GetSizeForSizeX( SIZEX_1920, ratio ) ) );
    lstStrings.append( GetSizeString( GetSizeForSizeX( SIZEX_1440, ratio ) ) );
    lstStrings.append( GetSizeString( GetSizeForSizeX( SIZEX_1280, ratio ) ) );
    lstStrings.append( GetSizeString( GetSizeForSizeX( SIZEX_1024, ratio ) ) );
    lstStrings.append( GetSizeString( GetSizeForSizeX( SIZEX_960, ratio ) ) );
    lstStrings.append( GetSizeString( GetSizeForSizeX( SIZEX_800, ratio ) ) );
    lstStrings.append( GetSizeString( GetSizeForSizeX( SIZEX_720, ratio ) ) );
    lstStrings.append( GetSizeString( GetSizeForSizeX( SIZEX_640, ratio ) ) );
    lstStrings.append( GetSizeString( GetSizeForSizeX( SIZEX_533, ratio ) ) );
    lstStrings.append( GetSizeString( GetSizeForSizeX( SIZEX_320, ratio ) ) );

    return lstStrings;
}

ImageSize GetImageSizeTypeFromSize( const QSize & aSize )
{
    return (ImageSize)aSize.width();
}

double GetScaleFactorFor( int iWidth, int iHeight )
{
    QSize aClippingAreaSize = GetRatioSizeForAvailableSize( QSize(iWidth,iHeight), GetCurrentImageRatio() );
    QSize aOutputAreaSize = GetRatioSizeForAvailableSize( GetCurrentOutputSize(), GetCurrentImageRatio() );
    double dScaleX = (double)aClippingAreaSize.width()/(double)aOutputAreaSize.width();
    return dScaleX;
}

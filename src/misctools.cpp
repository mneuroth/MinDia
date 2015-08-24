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
#include <QApplication>
#include <QElapsedTimer>

// *******************************************************************

// global_cache <-- show_cache
// TODO: image cache fuer dia show auf platte speichern: *.dia.cache
// TODO: image cache im hintergrund erzeugen, falls noch nicht da
// TODO: image cache fuer kleine Bitmaps anlegen, falls grosse bmps benoetigt werden diese asynchron nachladen...
// TODO: flush cache implementieren !!!

static QImageCache g_aImageCache(256,256);

static QAsyncImageReaderThread * g_pAsyncImageReaderThread = 0;

// *******************************************************************

QImageCache::QImageCache(int iMaxWidth, int iMaxHeight, int iMaxItems)
    : m_iMaxItems(iMaxItems),
      m_iMaxWidth(iMaxWidth),
      m_iMaxHeight(iMaxHeight),
      m_pTargetForMessages(0),
      m_bStopThread(false),
      m_bModified(false),
      m_aLock(QMutex::Recursive)
{
}

QImageCache::~QImageCache()
{
}

bool QImageCache::IsModified() const
{
    return m_bModified;
}

void QImageCache::Clean()
{
    m_aLock.lock/*ForWrite*/();
    if( !m_aMap.isEmpty() )
    {
        m_aMap.clear();
        m_bModified = true;
    }
    m_aLock.unlock();
}

void QImageCache::DoStop()
{
    if( isRunning() )
    {
        // stop the worker thread and wait for finished
        m_bStopThread = true;
        while( isRunning() )
        {
            msleep(10);
        }
    }
}

// TODO: ggf. create image cach trennen von image cach zugriff !!!
void QImageCache::run()
{
    QImage aImage;
    foreach( const QString sFileName, m_lstImageFileNames )
    {
        PostMessage( tr("reading ")+sFileName );
        Get(sFileName,m_iMaxWidth,m_iMaxHeight,aImage);
        if( m_bStopThread )
        {
            break;
        }
    }
    //unsigned long sz = GetCacheSizeInBytes();
    PostMessage( tr("image cache successfully created !") );
}

void QImageCache::InitCacheInBackground( const QStringList & lstImageFileNames, QObject * pTargetForMessages )
{
    m_pTargetForMessages = pTargetForMessages;
    DoStop();
    m_lstImageFileNames = lstImageFileNames;
    m_bStopThread = false;
    Clean();
    start();
}

void QImageCache::RemoveUnusedItems( const QStringList & lstImageFileNames, QObject * pTargetForMessages )
{
    m_pTargetForMessages = pTargetForMessages;
    QStringList lstRemoveKeys;
    m_aLock.lock();
    // first remove all unused images from current cache
    foreach (const QString & sKey, m_aMap.keys())
    {
        if( !lstImageFileNames.contains(sKey) )
        {
            lstRemoveKeys.append( sKey );
        }
    }
    foreach (const QString & sFileName, lstRemoveKeys)
    {
        m_aMap.remove( sFileName );
        PostMessage( tr("Removed file from cache: ")+sFileName );
    }
    m_aLock.unlock();
    // second add all new image files to cache
    QImage aImage;
    foreach (const QString & sFileName, lstImageFileNames)
    {
        if( !Get( sFileName,m_iMaxWidth,m_iMaxHeight,aImage) )
        {
            PostMessage( tr("Aded file to cache: ")+sFileName );
        }
    }
}

unsigned long QImageCache::GetCacheSizeInBytes()
{
    unsigned long ulSize = 0;
    m_aLock.lock/*ForRead*/();
    foreach (const QString & sKey, m_aMap.keys())
    {
        ulSize += m_aMap[sKey].first.byteCount();
    }
    m_aLock.unlock();
    return ulSize;
}

bool QImageCache::WriteCache( const QString & sCacheFileName )
{
    QElapsedTimer aTimer;
    aTimer.start();

    bool ret = false;
    QFile aFile( sCacheFileName );
    if( aFile.open(QIODevice::WriteOnly) )
    {
        QDataStream out( &aFile );
        m_aLock.lock/*ForRead*/();
        out << m_iMaxItems;
        out << m_iMaxWidth;
        out << m_iMaxHeight;
        out << m_aMap;
        m_aLock.unlock();
        ret = true;
        m_bModified = false;
    }
//    qDebug() << "WriteCache() time=" << aTimer.elapsed() << endl;   // BadLiebenzell: ca. 1-2 sec lesen udn 14.5 sec schreiben !
    return ret;
}

bool QImageCache::ReadCache( const QString & sCacheFileName )
{
    QElapsedTimer aTimer;
    aTimer.start();

    bool ret = false;
    QFile aFile( sCacheFileName );
    if( aFile.open(QIODevice::ReadOnly) )
    {
        QDataStream in( &aFile );
        m_aLock.lock/*ForWrite*/();
        in >> m_iMaxItems;
        in >> m_iMaxWidth;
        in >> m_iMaxHeight;
        in >> m_aMap;
        ClearAccessCounters();          // ignore access counters after read !
        m_aLock.unlock();
        ret = true;
        m_bModified = false;
    }
//    qDebug() << "ReadCache() time=" << aTimer.elapsed() << endl;
    return ret;
}

void QImageCache::CheckCacheSpace()
{
    QString sFoundKey;
    int ulCount = -1;
    m_aLock.lock/*ForRead*/();
    if( (m_iMaxItems>=0) && (m_aMap.size()>=m_iMaxItems) )
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
//            m_aLock.lock/*ForWrite*/();
            m_aMap.remove(sFoundKey);
//            m_aLock.unlock();
        }
    }
    m_aLock.unlock();
}

void QImageCache::ClearAccessCounters()
{
    m_aLock.lock/*ForRead*/();
    foreach (const QString & sKey, m_aMap.keys())
    {
        m_aMap[sKey].second = 0;
    }
    m_aLock.unlock();
}

void QImageCache::PostMessage( const QString & sMessage ) const
{
    MyCustomEvent<QString> * pEvent = new MyCustomEvent<QString>( c_iCustomEvent_ShowStatus );
    pEvent->setData( sMessage );
    QApplication::postEvent( m_pTargetForMessages, pEvent );
}

static QImage ReadImageDetectingOrientation( const QString & sImageFileName )
{
    QImageReader aImgReader(sImageFileName);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))
    aImgReader.setAutoTransform( true );
#endif
    return aImgReader.read();
}

bool QImageCache::Get( const QString & sImageFileName, int maxWidth, int maxHeight, QImage & aImage )
{
    m_aLock.lock/*ForRead*/();
    // exists an entry for the requested image ?
    if( !m_aMap.contains(sImageFileName) )
    {
//        qDebug() << "Image Cache --> not found ! " << sImageFileName << endl;
        // NO
        CheckCacheSpace();
        // add a new image to the cache

        aImage = ReadImageDetectingOrientation(sImageFileName);

        // scale to maximum image size (if needed)
// TODO: disable cache to improve quality ! --> necessary for zoom into images !
        // limit size of image in cach --> otherwise 20 x 12MPixel images == 1GByte !
        //qDebug() << " size: " << aImage.width() << " " << aImage.height() << " max=" << m_iMaxWidth << " " << m_iMaxHeight << endl;
// TODO --> qt does not support rotating of images (reading exif informations !) --> https://bugreports.qt.io/browse/QTBUG-37946
        if( aImage.width()>m_iMaxWidth || aImage.height()>m_iMaxHeight )
        {
            aImage = aImage.scaled(m_iMaxWidth,m_iMaxHeight,Qt::KeepAspectRatio);
        }
        //if( !aImage.isNull() )
        {

//            m_aLock.lock/*ForWrite*/();
            m_aMap[sImageFileName] = QPair<QImage,unsigned long>(aImage,1);
            m_bModified = true;
//            m_aLock.unlock();
        }
        const QImage & ret = m_aMap[sImageFileName].first;
//        m_aLock.unlock();
        aImage = ret;
        m_aLock.unlock();
        return false;
    }
    else
    {
        //qDebug() << "Image Cache --> Found !!! " << sImageFileName << " " << maxWidth << " " << maxHeight << endl;
        // YES
        // increment access counter
        m_aMap[sImageFileName].second += 1;
        const QImage & ret = m_aMap[sImageFileName].first;
  //      m_aLock.unlock();
        if( ret.width()<maxWidth || ret.height()<maxHeight || maxWidth<0 || maxHeight<0 )
        {
            aImage = ReadImageDetectingOrientation(sImageFileName);

            if( aImage.isNull() )
            {
                // copy thumbnail cache image if large image is not available !
                aImage = ret;
            }
            m_aLock.unlock();
            return true;
        }
        aImage = ret;
        m_aLock.unlock();
        return true;
    }
//    aImage = QImage();
//    return false;
}

// *******************************************************************

void InitCacheInBackground( const QStringList & lstImageFileNames, QObject * pTargetForMessages )
{
    g_aImageCache.InitCacheInBackground( lstImageFileNames, pTargetForMessages );
}

void UpdateCache( const QStringList & lstImageFileNames, QObject * pTargetForMessages )
{
    g_aImageCache.RemoveUnusedItems( lstImageFileNames, pTargetForMessages );
}

void StopBackgroundCache()
{
    g_aImageCache.DoStop();
}

bool LoadGlobalImageCache( const QString & sCacheFileName )
{
    if( QFile::exists( sCacheFileName ) )
    {
        return g_aImageCache.ReadCache( sCacheFileName );
    }
    return false;
}

bool SaveGlobalImageCacheAs( const QString & sCacheFileName )
{
    if( g_aImageCache.IsModified() )
    {
        return g_aImageCache.WriteCache( sCacheFileName );
    }
    return false;
}

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

QImage * CreateImageArea( const QString & sImageFileName, double relX, double relY, double relDX, double relDY )
{
    QImage aImage = ReadQImageOrEmpty( sImageFileName );
    return new QImage( aImage.copy( GetArea( aImage.size(), relX, relY, relDX, relDY, GetCurrentImageRatio() ) ) );
}

void CopyImageAreaAsyncAndPostResult( QObject * pTarget, bool bIsPlaying, int iDissolveTimeInMS, const QString & sImageFileName, double relX, double relY, double relDX, double relDY )
{
    if( g_pAsyncImageReaderThread==0 )
    {
        g_pAsyncImageReaderThread = new QAsyncImageReaderThread();
        g_pAsyncImageReaderThread->start();
    }

    g_pAsyncImageReaderThread->push( pTarget, bIsPlaying, iDissolveTimeInMS, sImageFileName, relX, relY, relDX, relDY );
}

QImage ReadQImageOrEmpty( const QString & sFileName, int maxWidth, int maxHeight )
{
    QImage aImageOut;
    if( QColor::isValidColor(sFileName) )
    {
        aImageOut = CreateColorImage( QColor(sFileName) );
    }
    else
    {
        g_aImageCache.Get(sFileName,maxWidth,maxHeight,aImageOut);
    }
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

// *******************************************************************
/*
QAsyncImageReader::QAsyncImageReader(QObject * pTarget, bool bIsPlaying, int iDissolveTimeInMS, const QString & sImageFileName , double relX, double relY, double relDX, double relDY)
    : m_pTarget( pTarget ),
      m_sImageFileName( sImageFileName ),
      m_relX( relX ),
      m_relY( relY ),
      m_relDX( relDX ),
      m_relDY( relDY ),
      m_bIsPlaying( bIsPlaying ),
      m_iDissolveTimeInMS( iDissolveTimeInMS ),
      m_bCancel( false )
{
}

QAsyncImageReader::~QAsyncImageReader()
{
}

void QAsyncImageReader::cancel()
{
    m_bCancel = true;
}

void QAsyncImageReader::run()
{
    m_bCancel = false;

    QImage * pImage = CreateImageArea( m_sImageFileName, m_relX, m_relY, m_relDX, m_relDY );

    MyCustomEvent<ImageReaderData> * pEvent = new MyCustomEvent<ImageReaderData>( c_iCustomEvent_PostImage );
    ImageReaderData aData;
    if( m_bCancel )
    {
        delete pImage;
        aData.m_pImage = 0;
    }
    else
    {
        aData.m_pImage = pImage;
    }
    aData.m_pAsyncImageReader = this;
    aData.m_bIsPlaying = m_bIsPlaying;
    aData.m_iDissolveTimeInMS = m_iDissolveTimeInMS;
    pEvent->setData( aData );
    // even post message if image reading was canceled, because otherwise we have a memory leak for QAsyncImageReader objects !
    QApplication::postEvent( m_pTarget, pEvent );
}
*/
// *******************************************************************

QAsyncImageReaderThread::QAsyncImageReaderThread()
    : m_bCancel( false )
{
}

QAsyncImageReaderThread::~QAsyncImageReaderThread()
{
}

void QAsyncImageReaderThread::push(QObject * pTarget, bool bIsPlaying, int iDissolveTimeInMS, const QString & sImageFileName , double relX, double relY, double relDX, double relDY)
{
    ImageReaderStruct aItem;

    aItem.m_pTarget = pTarget;
    aItem.m_sImageFileName = sImageFileName;
    aItem.m_relX = relX;
    aItem.m_relY = relY;
    aItem.m_relDX = relDX;
    aItem.m_relDY = relDY;
    aItem.m_bIsPlaying = bIsPlaying;
    aItem.m_iDissolveTimeInMS = iDissolveTimeInMS;
//    aItem.m_bCancel = false;

    m_aLock.lock/*ForWrite*/();
    m_aQueue.enqueue( aItem );
    m_aLock.unlock();
}

void QAsyncImageReaderThread::cancel()
{
    m_bCancel = true;
}

void QAsyncImageReaderThread::run()
{
    while( !m_bCancel )
    {
        m_aLock.lock();
        if( !m_aQueue.isEmpty() )
        {
            // remove all but the last element
            while( m_aQueue.size()>1 )
            {
                m_aQueue.dequeue();
            }
            ImageReaderStruct aItem = m_aQueue.dequeue();

            QImage * pImage = CreateImageArea( aItem.m_sImageFileName, aItem.m_relX, aItem.m_relY, aItem.m_relDX, aItem.m_relDY );

            MyCustomEvent<ImageReaderData> * pEvent = new MyCustomEvent<ImageReaderData>( c_iCustomEvent_PostImage );
            ImageReaderData aData;
            aData.m_pImage = pImage;
            //aData.m_pAsyncImageReader = this;
            aData.m_bIsPlaying = aItem.m_bIsPlaying;
            aData.m_iDissolveTimeInMS = aItem.m_iDissolveTimeInMS;
            pEvent->setData( aData );
            // even post message if image reading was canceled, because otherwise we have a memory leak for QAsyncImageReader objects !
            QApplication::postEvent( aItem.m_pTarget, pEvent );
        }
        m_aLock.unlock();

        QThread::msleep(40);
    }
}

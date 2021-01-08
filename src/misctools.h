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
#include <QStringList>
#include <QSize>
#include <QMap>
#include <QPair>
#include <QQueue>
#include <QThread>
#include <QEvent>
#include <QMutex>

#include <string>

#include "minhandle.h"
#include "diainfo.h"

using namespace std;

class QDropEvent;
class QObject;

class MinDiaWindow;

// *******************************************************************
enum ImageRatio {
    RATIO_UNDEFINED, /* = 0 */
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
    SIZEX_1920 = 1920,
    SIZEX_3840 = 3840
};
/*
16:9            3:2             4:3                 ID  Enum
------------------------------------------------------------------
4096x2160
3840x2160                                          11   SIZEX_3840
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

void msSleepMindia(unsigned long sleepInMs);

QImage CreateWhiteImage( int iWidth = 1024, int iHeight = 768 );

QImage CreateColorImage( const QColor & aColor, int iWidth = 1024, int iHeight = 768 );

QImage CopyImageArea( const QImage & aImage, double relX = 0.0, double relY = 0.0, double relDX = 1.0, double relDY = 1.0 );

void CopyImageAreaAsyncAndPostResult( QObject * pTarget, bool bIsPlaying, int iDissolveTimeInMS, const QString & sImageFileName, minHandle<DiaInfo> hDia );

QImage ReadQImageOrEmpty( const QString & sFileName, int maxWidth = -1, int maxHeight = -1, bool bIsThumbnail = false );
const QImage & GetQImageOrEmptyReference( const QString & sFileName, QImage & aTempImage, int maxWidth = -1, int maxHeight = -1, bool bIsThumbnail = false );

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

MinDiaWindow * GetMainWindow();

void InitCacheInBackground( const QStringList & lstImageFileNames, QObject * pTargetForMessages );
void UpdateCache( const QStringList & lstImageFileNames, QObject * pTargetForMessages );
void StopBackgroundCache();
bool LoadGlobalImageCache( const QString & sCacheFileName );
bool SaveGlobalImageCacheAs( const QString & sCacheFileName );
void SetMessageSenderForCache( QObject * pTargetForMessages );

// *************************************************************************************************

const QEvent::Type c_iCustomEvent_Logging = (QEvent::Type)(QEvent::User+123);
const QEvent::Type c_iCustomEvent_ShowStatus = (QEvent::Type)(QEvent::User+124);
const QEvent::Type c_iCustomEvent_PostImage = (QEvent::Type)(QEvent::User+125);

class QAsyncImageReader;

// *************************************************************************************************
struct ImageReaderData
{
    ImageReaderData()
        : m_pAsyncImageReader( 0 ),
          m_pImage( 0 ),
          m_bIsPlaying( false ),
          m_iDissolveTimeInMS( 0 )
    {
    }
    ~ImageReaderData()
    {
    }

    QAsyncImageReader * m_pAsyncImageReader;
    QImage *            m_pImage;
    minHandle<DiaInfo>  m_hDia;
    bool                m_bIsPlaying;
    int                 m_iDissolveTimeInMS;
};

// *************************************************************************************************
template <class T>
class MyCustomEvent : public QEvent
{
public:
    MyCustomEvent( QEvent::Type aType )
        : QEvent(aType)
    {
    }
    void setData( const T & aValue )
    {
        m_aData = aValue;
    }
    T data() const
    {
        return m_aData;
    }

private:
    T m_aData;
};

// *************************************************************************************************
class QImageCache : public QThread
{
public:
    QImageCache(int iMaxWidth = 1920, int iMaxHeight = 1080, int iMaxItems = -1/*means unlimited*/, QImageCache * pResolveFullImages = 0);
    virtual ~QImageCache();

    bool IsModified() const;

    void Clean();
    void DoStop();

    void SetMessageSender( QObject * pTargetForMessages );
    void InitCacheInBackground( const QStringList & lstImageFileNames, QObject * pTargetForMessages );
    void RemoveUnusedItems( const QStringList & lstImageFileNames, QObject * pTargetForMessages );

    // returns true if image was already in cache
    //bool Get( const QString & sImageFileName, int maxWidth, int maxHeight, QImage & aImageOut );
    const QImage & GetRef( const QString & sImageFileName, QImage & aTempImage, int maxWidth, int maxHeight, bool bIsThumbnail );

    bool Add( const QString & sImageFileName );

    unsigned long GetCacheSizeInBytes();

    bool WriteCache( const QString & sCacheFileName );
    bool ReadCache( const QString & sCacheFileName );

    virtual void run();

private:
    const QImage & GetRef( const QString & sImageFileName, QImage & aTempImage, bool & bWasAdded, int maxWidth, int maxHeight, bool bIsThumbnail );

    bool IsMaxSizeCache() const;
    void CheckCacheSpace();
    void ClearAccessCounters();
    void PostMessage( const QString & sMessage ) const;
    void PushToLastImageFileNames( const QString & sImageFileName );
    bool IsInLastImageFileNames( const QString & sImageFileName );

    int                                         m_iMaxItems;
    int                                         m_iMaxWidth, m_iMaxHeight;
    QMap<QString,QPair<QImage,int> >            m_aMap;     // string --> (QImage, ImageAccessCounter)
    QStringList                                 m_lstImageFileNames;
    QStringList                                 m_lstLastPushedImageFileNames;
    QObject *                                   m_pTargetForMessages;       // not an owner !
    bool                                        m_bStopThread;
    bool                                        m_bModified;
    QMutex                                      m_aLock;
    QImageCache *                               m_pResolveFullImages;       // not an owner !
};

#define MAX_SIZE_OF_LAST_IMAGE_FILE_QUEUE   2

// *************************************************************************************************
struct ImageReaderStruct
{
    QObject *           m_pTarget;
    QString             m_sImageFileName;
    minHandle<DiaInfo>  m_hDia;
    bool                m_bIsPlaying;
    int                 m_iDissolveTimeInMS;
};

// *************************************************************************************************
class QAsyncImageReaderThread : public QThread
{
public:
    QAsyncImageReaderThread();
    virtual ~QAsyncImageReaderThread();

    void push( QObject * pTarget, bool bIsPlaying, int iDissolveTimeInMS, const QString & sImageFileName, minHandle<DiaInfo> hDia );

    void cancel();

    virtual void run();

private:
    QQueue<ImageReaderStruct>   m_aQueue;
    QMutex                      m_aLock;
    bool                        m_bCancel;
};

#endif

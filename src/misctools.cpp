
#include "misctools.h"

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

// function necassary because of linux gcc3.3
char mytolower( char ch )
{
	return tolower( ch );
}

bool ReadQImage( const char * sFileName, QImage & aImageOut )
{
    aImageOut = g_aImageCache.Get(QString(sFileName));
    // aImageOut = QImage( sFileName );
	return !aImageOut.isNull();
}

bool IsDiaDataFile( const char * sFileName )
{
	//ifstream aFile( sFileName, ios::in );

	//ctype<string::value_type> aConverter;

	// check if the given file name has .dia as extension
	if( sFileName!=0 && strlen( sFileName )>0 )
	{
		string sTemp( sFileName );
		
		//aConverter.tolower( sTemp.begin(), sTemp.end() );
		transform( sTemp.begin(), sTemp.end(), sTemp.begin(), mytolower );

		if( (sTemp.find(".dia")!=string::npos) )
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

            if( IsDiaDataFile( (const char *)sTest.toAscii() ) )
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
    
            if( QImage((const char *)sTest.toAscii()).isNull() )
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

static bool IsWAVorMP3( const char * sFileName )
{
	//ctype<string::value_type> aConverter;

	// check if the given file name has .jpg or .jpeg as extension
	if( sFileName!=0 && strlen( sFileName )>0 )
	{
		string sTemp( sFileName );
		
		//aConverter.tolower( sTemp.begin(), sTemp.end() );
		transform( sTemp.begin(), sTemp.end(), sTemp.begin(), mytolower );

		if( (sTemp.find(".wav")!=string::npos) || (sTemp.find(".mp3")!=string::npos) )
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
    
            if( !IsWAVorMP3( (const char *)sTest.toAscii() ) )
			{
				return false;
			}
		}
        
		return true;
	}

	return false;
}


#include "misctools.h"

#include <qdragobject.h>
#include <qurl.h>

#include <stdio.h>
#include <ctype.h>

#include <string>
//#include <fstream>
#include <locale.h>
#include <algorithm>

using namespace std;

// *******************************************************************

// function necassary because of linux gcc3.3
char mytolower( char ch )
{
	return tolower( ch );
}

static bool IsJPEG( const char * sFileName )
{
	//ctype<string::value_type> aConverter;

	// check if the given file name has .jpg or .jpeg as extension
	if( sFileName!=0 && strlen( sFileName )>0 )
	{
		string sTemp( sFileName );
		
		//aConverter.tolower( sTemp.begin(), sTemp.end() );
		transform( sTemp.begin(), sTemp.end(), sTemp.begin(), mytolower );

		if( (sTemp.find(".jpg")!=string::npos) || (sTemp.find(".jpeg")!=string::npos) )
		{
			return true;
		}
	}
	return false;
}

#if defined( _MSC_VER ) && defined( _WITH_EXT_JPG_LIB )

#include "jpg/inc/jpegdecoder.h"

// this function is the modified main()-function from jpg2tga.cpp
static bool ReadJPEG( const char * sFileName, QImage & aImageOut )
{
	const char *Psrc_filename = sFileName;

	Pjpeg_decoder_file_stream Pinput_stream = new jpeg_decoder_file_stream();

	if (Pinput_stream->open(Psrc_filename))
	{
		delete Pinput_stream;
		//printf("Error: Unable to open file \"%s\" for reading!\n", Psrc_filename);
		return false;
	}

	Pjpeg_decoder Pd = new jpeg_decoder( Pinput_stream, /*use_mmx*/true );

	if (Pd->get_error_code() != 0)
	{
		//printf("Error: Decoder failed! Error status: %i\n", Pd->get_error_code());

		// Always be sure to delete the input stream object _after_
		// the decoder is deleted. Reason: the decoder object calls the input
		// stream's detach() method.
		delete Pd;
		delete Pinput_stream;

		return false;
	}

	//printf("Width: %i\n", Pd->get_width());
	//printf("Height: %i\n", Pd->get_height());
	//printf("Components: %i\n", Pd->get_num_components());

	int iBytes = 1;
	if( Pd->get_num_components()==3 )
	{
		// we need 4 bytes for scaling the image !
		iBytes = 4;
	}

	if (Pd->begin())
	{
		//printf("Error: Decoder failed! Error status: %i\n", Pd->get_error_code());

		delete Pd;
		delete Pinput_stream;

		return false;
	}

	uchar *Pbuf = NULL;
	if (Pd->get_num_components() == 3)
	{
		Pbuf = (uchar *)malloc(Pd->get_width() * 3);
		if (!Pbuf)
		{
			//printf("Error: Out of memory!\n");

			delete Pd;
			delete Pinput_stream;

			return false;
		}
	}

	QImage aImage( Pd->get_width(), Pd->get_height(), 8*iBytes );

	int lines_decoded = 0;

	for ( ; ; )
	{
		void *Pscan_line_ofs;
		uint scan_line_len;

		if (Pd->decode(&Pscan_line_ofs, &scan_line_len))
		  break;

		uchar * pBuffer = aImage.scanLine( lines_decoded );

		lines_decoded++;

		if (Pd->get_num_components() == 3)
		{
			uchar *Psb = (uchar *)Pscan_line_ofs;
			int src_bpp = Pd->get_bytes_per_pixel();

			int xx = 0;
			int x = 0;
			for( x = Pd->get_width(); x > 0; x--, Psb += src_bpp )
			{
				pBuffer[xx]		= Psb[2];
				pBuffer[xx + 1] = Psb[1];
				pBuffer[xx + 2] = Psb[0];
				pBuffer[xx + 3] = 0;
				xx += 4;
			}
		}
		else
		{
			memcpy( pBuffer, Pscan_line_ofs, scan_line_len );
		}
	}

	free(Pbuf);

	if (Pd->get_error_code())
	{
		//printf("Error: Decoder failed! Error status: %i\n", Pd->get_error_code());

		delete Pd;
		delete Pinput_stream;

		return false;
	}

	//printf("Lines decoded: %i\n", lines_decoded);
	//printf("Input file size:  %i\n", Pinput_stream->get_size());
	//printf("Input bytes actually read: %i\n", Pd->get_total_bytes_read());

	delete Pd;
	delete Pinput_stream;

	aImageOut = aImage;

	return true;
}

bool ReadQImage( const char * sFileName, QImage & aImageOut )
{
	if( IsJPEG( sFileName ) )
	{
		return ReadJPEG( sFileName, aImageOut );
	}
	else
	{
		aImageOut = QImage( sFileName );
		return !aImageOut.isNull();
	}
}

#else

bool ReadQImage( const char * sFileName, QImage & aImageOut )
{
	aImageOut = QImage( sFileName );
	return !aImageOut.isNull();
}

#endif

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

bool IsDiaDataFileDrag( const QMimeSource * pEvent, QString & sFileNameOut )
{
	QStrList aStrList;

	if( QUriDrag::decode( pEvent, aStrList ) )
	{
		if( aStrList.count() == 1 )
		{
			const QString sTest = aStrList.at( 0 );
			const char * s = (const char *)sTest;
			sFileNameOut = QUriDrag::uriToLocalFile( s );
			s = (const char *)sFileNameOut;

			if( IsDiaDataFile( s ) )
			{
				return true;
			}
		}
	}

	return false;
}

bool IsImageFileDrag( const QMimeSource * pEvent )
{
	QStrList aStrList;

	if( QUriDrag::decode( pEvent, aStrList ) )
	{
		// all droped file-names have to be images with known format !!!
		for( int i=0; i<(int)aStrList.count(); i++ )
		{
			const QString sTest = aStrList.at( i );
			const char * s = (const char *)sTest;
			QString sFileName = QUriDrag::uriToLocalFile( s );
			s = (const char *)sFileName;

			if( (QImage::imageFormat( sFileName )==0) && !IsJPEG( s ) )
			{
				return false;
			}
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

bool IsSoundFileDrag( const QMimeSource * pEvent )
{
	QStrList aStrList;

	if( QUriDrag::decode( pEvent, aStrList ) )
	{
		// all droped file-names have to be images with known format !!!
		for( int i=0; i<(int)aStrList.count(); i++ )
		{
			const QString sTest = aStrList.at( i );
			const char * s = (const char *)sTest;
			QString sFileName = QUriDrag::uriToLocalFile( s );
			s = (const char *)sFileName;

			if( !IsWAVorMP3( s ) )
			{
				return false;
			}
		}
		return true;
	}

	return false;
}

/*
	The code in this file is taken from the kwav-project
	from Guenter Schwann (vogel@ibk.ml.org; www.ibk.ml.org/home/vogel),
	which is released under the GPL ver. 2.
	The code is modified by Michael Neuroth.
*/

#include "wavfile.h"

Wave addW;

#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#endif

// *******************************************************************

#include <QThread>

#include "minhandle.h"
#include "osdep2.h"

#include <queue>

using namespace std;

// *******************************************************************
template <class Type>
class ReadBuffer
{
	ReadBuffer( const ReadBuffer & );
	ReadBuffer & operator=( const ReadBuffer & );

public:
	ReadBuffer( int iMaxSize )
	: m_iMaxSize( iMaxSize )
	{
cout << "ReadBuffer this=" << (void *)this << " iMax=" << m_iMaxSize << endl;
		m_pBuffer = new Type[ m_iMaxSize ];
	}
	~ReadBuffer()
	 {
cout << "~ReadBuffer this=" << (void *)this << " iMax=" << m_iMaxSize << endl;
	 	delete [] m_pBuffer;
	 }

	int GetMaxSize() const
	{
		return m_iMaxSize;
	}

	int GetSize() const
	{
		return m_iSize;
	}

	void SetSize( int iSize )
	{
		m_iSize = iSize;
	}
/*
	Type GetData( int iIndex )
	{
		return m_pBuffer[ iIndex ];
	}
*/
	Type * GetDataPtr( int iIndex )
	{
// min todo gulp --> bug ?
//		if( (iIndex>=0) && (iIndex<m_iMaxSize) )
//		{
			return &(m_pBuffer[ iIndex ]);
//		}
//		else
//		{
//			cerr << "Error in WAF-File ReadBuffer iIndex=" << iIndex << endl;
//			return 0;
//		}
	}

private:
	int		m_iMaxSize;
	int		m_iSize;
	Type *	m_pBuffer;
};

// *******************************************************************

// ** simple helper class to handle a fadein/fadeout effect for a wav-file
class FadeWaveFilter
{
public:
	enum { FADE_IN, FADE_OUT, SET_SIGNAL };

	FadeWaveFilter( int iFadeMode, int iFadeStartPos, int iFadeStopPos, double dSignalFactor = 0 )
	: m_iFadeMode( iFadeMode ),
	  m_iFadeStartPos( iFadeStartPos ),
	  m_iFadeStopPos( iFadeStopPos ),
	  m_dSignalFactor( dSignalFactor ),
	  m_iUserData( 0 )
	{
	}

	bool IsPositionInFilter( int iSamplePos ) const
	{
		//cout << "start=" << m_iFadeStartPos << " stopt=" << m_iFadeStopPos << endl;
		return ( iSamplePos >= m_iFadeStartPos ) && ( iSamplePos <= m_iFadeStopPos );
	}
	bool operator<( const FadeWaveFilter & aOther ) const
	{
		return m_iFadeStartPos < aOther.m_iFadeStartPos;
	}
	int Length() const
	{
		return m_iFadeStopPos - m_iFadeStartPos;
	}
	bool IsFadeIn() const
	{
		return m_iFadeMode == FADE_IN;
	}
	bool IsFadeOut() const
	{
		return m_iFadeMode == FADE_OUT;
	}
	int & GetUserDataRef()
	{
		return m_iUserData;
	}

//private:
	int		m_iFadeMode;
	int		m_iFadeStartPos;
	int		m_iFadeStopPos;
	double	m_dSignalFactor;	// 0..1

	// ** temp data **
	int		m_iUserData;
};

class FadeContainer : public vector<FadeWaveFilter>
{
	void Sort()
	{
		sort( begin(), end() );
	}
};

// *******************************************************************

typedef ReadBuffer<short>	ShortReadBuffer;

// *******************************************************************
class ReadBufferQueue : public QThread
{
	typedef queue< minHandle< ShortReadBuffer > >		BufferContainer;

public:
	ReadBufferQueue( const char * sFileName, int iReadPos, int iChannels, int iBits, int iBufferSize, int iBufferCount );
	~ReadBufferQueue();

	void StartThread();

	bool IsReadDone() const;

	void SetReadPos( int iNewPos );
	int  GetReadPos() const;

	void ClearAllFadeInfos();
	void AddFadeIn( int iStartPos, int iStopPos );
	void AddFadeOut( int iStartPos, int iStopPos );

	minHandle<ShortReadBuffer> GetNextBuffer();

	void FillQueueInThread();

    void run();

private:
	// ** help methods **
	bool NeedNextBuffer();
	bool ReadNextBuffer();
	void CheckBuffer( minHandle<ShortReadBuffer> hBuffer );

	// ** data **
	ifstream 			m_aStream;
	int					m_iReadPos;
	BufferContainer		m_aQueue;
	minSyncObject 		m_aQueueSync;
	bool				m_bReadDone;
	int					m_iBufferSize;		// BufferSize in items, not in bytes !
											// BufferSize in bytes == BufferSize * sizeof(items)
	int					m_iMaxBufferCount;	// number of buffers in read queue
	int 				m_iChannels;
	int					m_iBits;
	int					m_iBufferPos;		// number of buffers allready read
	FadeContainer		m_aFadeContainer;
	unsigned long		m_ulThreadID;
	bool				m_bThreadStoped;
};

// *******************************************************************

#ifdef _with_min_threads
extern "C" void _CALLING_CONV _bufferReaderThreadStarter( void * pData )
{
#ifndef ZAURUS
	try {
#endif
		ReadBufferQueue * pQueue = (ReadBufferQueue *)pData;

		if( pQueue )
		{
			pQueue->run();
		}
#ifndef ZAURUS
	}
	catch( ... )
	{
		cerr << "Exception in bufferReaderThread" << endl;
	}
#endif
	//cout << "READ THREAD DONE." << endl;
}
#endif

// *******************************************************************
/*
inline int convertSecondsInSamples( double d )
{
	return (int)(d*22000.0)*2;
}
*/

ReadBufferQueue::ReadBufferQueue( const char * sFileName, int iReadPos, int iChannels, int iBits, int iBufferSize, int iBufferCount )
: m_aStream( sFileName, ios::in | ios::binary ),
  m_iReadPos( iReadPos ),
  m_bReadDone( false ),
  m_iBufferSize( iBufferSize ),
  m_iMaxBufferCount( iBufferCount ),
  m_iChannels( iChannels ),
  m_iBits( iBits ),
  m_iBufferPos( 0 ),
  m_ulThreadID( 0 ),
  m_bThreadStoped( false )
{
cout << "ReadBufferQueue this=" << (void *)this << endl;
}

ReadBufferQueue::~ReadBufferQueue()
{
cout << "~ReadBufferQueue this=" << (void *)this << endl;
	if( !IsReadDone() )
	{
		m_bReadDone = true;
		minSleep( 100 );
	}
	// wait until the reader thread stoped
	while( !m_bThreadStoped )
	{
		minSleep( 10 );
	}
}

void ReadBufferQueue::run()
{
    try {
        FillQueueInThread();
    }
	catch( ... )
	{
		cerr << "Exception in bufferReaderThread" << endl;
	}
}

void ReadBufferQueue::StartThread()
{
#ifdef _with_min_threads
	m_ulThreadID = minBeginThread( _bufferReaderThreadStarter, _DEFAULT_STACK_SIZE, this );
#else
    start();
    m_ulThreadID = 1;   // dummy id
#endif    
}

bool ReadBufferQueue::IsReadDone() const
{
	return m_bReadDone;
}

void ReadBufferQueue::SetReadPos( int iNewPos )
{
	minLock aLock( m_aQueueSync );

	m_iBufferPos = iNewPos;

	//cout << "************** SET READ pos=" << iNewPos << endl;

	// ** IMPORTANT: purge all the allready read buffers !!!
	while( !m_aQueue.empty() )
	{
		m_aQueue.pop();
	}
}

int  ReadBufferQueue::GetReadPos() const
{
	return m_iBufferPos;
}

void ReadBufferQueue::ClearAllFadeInfos()
{
	m_aFadeContainer.erase( m_aFadeContainer.begin(), m_aFadeContainer.end() );
}

void ReadBufferQueue::AddFadeIn( int iStartPos, int iStopPos )
{
	FadeWaveFilter aItem( FadeWaveFilter::FADE_IN, iStartPos, iStopPos );
	m_aFadeContainer.push_back( aItem );
}

void ReadBufferQueue::AddFadeOut( int iStartPos, int iStopPos )
{
	FadeWaveFilter aItem( FadeWaveFilter::FADE_OUT, iStartPos, iStopPos );
	m_aFadeContainer.push_back( aItem );
}

// will be called only from the reader thread
void ReadBufferQueue::FillQueueInThread()
{
	m_bThreadStoped = false;
	while( !IsReadDone() )
	{
		if( NeedNextBuffer() )
		{
			ReadNextBuffer();
		}
		else
		{
			minSleep( 10 );
		}
	}
	m_bThreadStoped = true;
}

minHandle<ShortReadBuffer> ReadBufferQueue::GetNextBuffer()
{
	minHandle<ShortReadBuffer> hBuffer;

	{
		minLock aLock( m_aQueueSync );

		if( m_aQueue.size() > 0 )
		{
			hBuffer = m_aQueue.front();
			m_aQueue.pop();
		}
	}

	return hBuffer;
}

bool ReadBufferQueue::NeedNextBuffer()
{
	minLock aLock( m_aQueueSync );

	//cout << "NeedBuffer: " << m_aQueue.size() << endl;

	return ( (int)m_aQueue.size() < m_iMaxBufferCount );
}

inline int chtoint( unsigned char ch )
{
	return ((ch-128)*256);
}

int ReadBufferItem( istream & ifl, int iReadPos, short * pDataTarget, int iBufferSize, int iChannels, int iBits )
{
	unsigned char st[10];

	// ** jump to read position ...
	ifl.seekg( iReadPos );
	//cout << "READ pos=" << iReadPos << endl;

	int iInc = 2;
	if( iBits==8 )
	{
		iInc = 1;
	}

	int i = 0;

	while( (i<iBufferSize) && !ifl.eof() && ifl.good()  )
	{
		if( iChannels==1 )
		{
			if( iBits==8 )
			{
				ifl.read( (char *)&st, 1 );
				pDataTarget[i] = chtoint( st[0] );
			}
			else
			{
				ifl.read( (char *)(pDataTarget+i), 2 );
			}
		}
		else
		{
			if( iBits==8 )
			{
				ifl.read( (char *)st, 2 );
				pDataTarget[i] = chtoint( st[0] );
				pDataTarget[i+1] = chtoint( st[1] );
			}
			else
			{
				ifl.read( (char *)(pDataTarget+i), 2 );
				ifl.read( (char *)(pDataTarget+i+1), 2 );
			}
		}

		i=i+iInc;
	}

	return i;
}

bool ReadBufferQueue::ReadNextBuffer()
{
	minHandle<ShortReadBuffer> hBuffer( new ShortReadBuffer( m_iBufferSize ) );

	int iCount = ReadBufferItem( m_aStream,
								m_iReadPos+m_iBufferPos*m_iBufferSize*sizeof(short),
								hBuffer->GetDataPtr( 0 ),
								m_iBufferSize,
								m_iChannels,
								m_iBits );

	hBuffer->SetSize( iCount );

	CheckBuffer( hBuffer );

	{
		minLock aLock( m_aQueueSync );
/*
		// ** geschuetztes read, damit purge in SetReadPos() funktioniert !
		int iCount = ReadBufferItem( m_aStream,
									m_iReadPos+m_iBufferPos*m_iBufferSize*sizeof(short),
									hBuffer->GetDataPtr( 0 ),
									m_iBufferSize,
									m_iChannels,
									m_iBits );

		hBuffer->SetSize( iCount );
*/
		m_aQueue.push( hBuffer );

		m_iBufferPos++;

		//cout << "read buffer " << m_iBufferPos << endl;
	}

	bool bOk = !m_aStream.eof() && m_aStream.good();

	// ** signal the end of the reading
	if( !bOk )
	{
		m_bReadDone = true;
	}

	return bOk;
}

void ReadBufferQueue::CheckBuffer( minHandle<ShortReadBuffer> hBuffer )
{
	//int iBytesPerSample = 2;
	int iChannelCount = m_iChannels;
	int iSampleCountInBuffer = m_iBufferSize;
	// currPlayPos in Bytes --> convert in samples
	// ein Eintrag im Buffer ist immer ein short, d.h. 2 Bytes !
	int currPlayPos = m_iBufferPos*m_iBufferSize /**sizeof(short) / iBytesPerSample*/ / iChannelCount;
	//cout << "check " << currPlayPos << endl;

	FadeContainer::iterator aIter = m_aFadeContainer.begin();
	while( aIter != m_aFadeContainer.end() )
	{
		FadeWaveFilter & aFilter = *aIter;

		if( aFilter.IsPositionInFilter( currPlayPos ) )
		{
			int & iOffset = aFilter.GetUserDataRef();
			double dRampFactor = 100.0/((double)aFilter.Length());

			double dScaleFactor = aFilter.m_dSignalFactor;
			for( int i=0; i<iSampleCountInBuffer; i+=iChannelCount )
			{
				if( aFilter.IsFadeOut() )
				{
					dScaleFactor = ( 100.0 - ((double)(iOffset+i/iChannelCount))*dRampFactor )*0.01;
				}
				if( aFilter.IsFadeIn() )
				{
					dScaleFactor = 1.0 - ( 100.0 - ((double)(iOffset+i/iChannelCount))*dRampFactor )*0.01;
				}
				hBuffer->GetDataPtr(0)[i] = (int)( ((double)hBuffer->GetDataPtr(0)[i])*dScaleFactor );
				if( iChannelCount>1 )
				{
					hBuffer->GetDataPtr(0)[i+1] = (int)( ((double)hBuffer->GetDataPtr(0)[i+1])*dScaleFactor );
				}
			}

			iOffset += iSampleCountInBuffer/iChannelCount;
		}

		++aIter;
	}
}

// *******************************************************************
// *******************************************************************
// *******************************************************************

#ifndef _with_main

#ifdef _with_min_threads
extern "C" void _CALLING_CONV _wavSoundThreadStarter( void * pData )
{
#ifndef ZAURUS
	try {
#endif
		Wave * pWave = (Wave *)pData;

		if( pWave )
		{
            pWav->run();
		}
#ifndef ZAURUS
	}
	catch( ... )
	{
		cerr << "Exception in wavSoundThread" << endl;
	}
#endif
	//cout << "**PLAY** THREAD DONE." << endl;
}
#endif //_with_min_threads

#endif

// *******************************************************************

Wave::Wave()
{
	m_ulThreadID = (unsigned long)-1;
	m_iMaxPos = -1;
	m_iFileReadPos = -1;
	m_pReadQueue = 0;
	m_dFadeInStartTimeInSeconds = -1;
	m_dFadeInLengthInSeconds = 0;
	m_dFadeOutStartTimeInSeconds = -1;
	m_dFadeOutLengthInSeconds = 0;

	loaded=0;
	Size=0;
	bit=8;
	Sampling=11025;
	NumSamples=1000;
	Channels=1;
	currPlayPos=0;
	playActive=0;
	buffer_size=256;

	waveData1 = 0;
	waveData2 = 0;

	buffer = 0;
}

Wave::~Wave()
{
	freemem();

	if( m_pReadQueue )
	{
		delete m_pReadQueue;
	}
}

void Wave::run()
{
	try {
        while( getActivePlay() )
        {
            if( isAsync() )
            {
                playAsync();
            }
            else
            {
                play();
            }
        }
	}
	catch( ... )
	{
		cerr << "Exception in wavSoundThread" << endl;
	}
}

int Wave::getSize()
{
	return Size;
}

int Wave::getSampling()
{
	return Sampling;
}

int Wave::getBit()
{
	return bit;
}

int Wave::getChannels()
{
	return Channels;
}

int Wave::getNumSamples()
{
	return NumSamples;
}

int Wave::chtoint( unsigned char ch )
{
	return ((ch-128)*256);
}

unsigned char Wave::inttoch( int i )
{
	return ((i/256)+128);
}

void Wave::setFileName( const char *filen )
{
	strcpy(FileName,filen);
}

char *Wave::getFileName()
{
	return FileName;
}

int Wave::isAsync() const
{
	return m_iFileReadPos != -1;
}

int Wave::openWaveAsync( const char * filename )
{
 int ok;
 int i, avBpS;
 short sh, bpS;
 //void *ch, *ch1;
 char str[40];
 //unsigned char st[10];

 ifstream ifl( filename, ios::in|ios::binary);
 if (!ifl) ok=1;
 else
 {
  ok=0;
  ifl.read( str, 4 );
  str[4]='\0';
  if ( strcmp(str,"RIFF")!=0 ) ok=2;
  else
  {
   ifl.read( (char *)&i, 4 );	//istream
   ifl.read( str, 4 );
   str[4]='\0';
   if ( strcmp(str,"WAVE")!=0 ) ok=2;
   else
   {
	setFileName( filename );
	free( waveData1 );
	if (getChannels()==2) free( waveData2 );
	i=0;
	while (i==0)
	{
	 ifl.read( str, 1 );
	 if (str[0]=='f')
	 {
	  ifl.read( str, 1 );
	  if (str[0]=='m')
	  {
	   ifl.read( str, 1 );
	   if (str[0]=='t')
	   {
	    ifl.read( str, 1 );
	    if (str[0]==' ') i=1;
	   }
	  }
	 }
	}

	ifl.read( (char *)&i, 4 );
	ifl.read( (char *)&sh, 2 );
	ifl.read( (char *)&sh, 2 );
	i=sh;
        setChannels( i );
        ifl.read( (char *)&i, 4 );
	setSampling( i );
        ifl.read( (char *)&i, 4 );
	avBpS=i;
        ifl.read( (char *)&sh, 2 );
	bpS=sh;
        ifl.read( (char *)&sh, 2 );
	i=sh;
        setBit( i );
	ifl.read( str, 4 );
	ifl.read( (char *)&i, 4 );
	setSize( i );
	setNumSamples( getSize() / bpS );

	m_iFileReadPos = ifl.tellg();

	// ** reading of the data will be done later (with ReadBufferQueu) ...

	loaded=1;
   }
  }
 }
 if (ok!=0)
 {
  switch ( ok )
  {
   case 1: strcpy(str, "Can't open file!" ); break;
   case 2: strcpy(str, "file is no WAV-file" ); break;
  }
//  m->warn( str );
   cerr << str << endl;
 }
 return ok;
}

int Wave::openWave( char *filename )
{
 int ok;
 int i, avBpS;
 short sh, bpS;
 void *ch, *ch1;
 char str[40];
 unsigned char st[10];

 ifstream ifl( filename, ios::in|ios::binary);
 if (!ifl) ok=1;
 else
 {
  ok=0;
  ifl.read( str, 4 );
  str[4]='\0';
  if ( strcmp(str,"RIFF")!=0 ) ok=2;
  else
  {
   ifl.read( (char *)&i, 4 );	//istream
   ifl.read( str, 4 );
   str[4]='\0';
   if ( strcmp(str,"WAVE")!=0 ) ok=2;
   else
   {
	setFileName( filename );
	free( waveData1 );
	if (getChannels()==2) free( waveData2 );
	i=0;
	while (i==0)
	{
	 ifl.read( str, 1 );
	 if (str[0]=='f')
	 {
	  ifl.read( str, 1 );
	  if (str[0]=='m')
	  {
	   ifl.read( str, 1 );
	   if (str[0]=='t')
	   {
	    ifl.read( str, 1 );
	    if (str[0]==' ') i=1;
	   }
	  }
	 }
	}

	ifl.read( (char *)&i, 4 );
	ifl.read( (char *)&sh, 2 );
	ifl.read( (char *)&sh, 2 );
	i=sh;
        setChannels( i );
        ifl.read( (char *)&i, 4 );
	setSampling( i );
        ifl.read( (char *)&i, 4 );
	avBpS=i;
        ifl.read( (char *)&sh, 2 );
	bpS=sh;
        ifl.read( (char *)&sh, 2 );
	i=sh;
        setBit( i );
	ifl.read( str, 4 );
	ifl.read( (char *)&i, 4 );
	setSize( i );
	setNumSamples( getSize() / bpS );

	// ** read data **
	if (getChannels()==1)
	{
	 ch=waveData1;
	 waveData1 = (short*) calloc( getNumSamples(), 2 );
	 if (getBit()==8)
	 {
	  for (i=0;i<getNumSamples();i++)
	  {
	   ifl.read( (char *)st, 1 );
	   waveData1[i]=chtoint( st[0] );
	  }
	 }
	 else
	 {
	  for (i=0;i<getNumSamples();i++)
	  {
	   ch=&waveData1[i];
	   ifl.read( (char *)ch, 2 );
	  }
	 }
	}
	else
	{
	 if (getBit()==8)
	 {
	  waveData1 = (short*) calloc( getNumSamples(), 2 );
	  waveData2 = (short*) calloc( getNumSamples(), 2 );
	  for (i=0;i<getNumSamples();i++)
	  {
           ifl.read( (char *)st, 2 );
	   waveData1[i]=chtoint( st[0] );
	   waveData2[i]=chtoint( st[1] );
	  }
	 }
	 else
	 {
	  waveData1 = (short*) calloc( getNumSamples(), 2 );
	  waveData2 = (short*) calloc( getNumSamples(), 2 );
	  for (i=0;i<getNumSamples();i++)
	  {
	   ch=&waveData1[i];
	   ch1=&waveData2[i];
           ifl.read( (char *)ch, 2 );
           ifl.read( (char *)ch1, 2 );
	  }
	 }
	}
	loaded=1;
   }
  }
 }
 if (ok!=0)
 {
  switch ( ok )
  {
   case 1: strcpy(str, "Can't open file!" ); break;
   case 2: strcpy(str, "file is no WAV-file" ); break;
  }
//  m->warn( str );
   cerr << str << endl;
 }
 return ok;
}

void Wave::saveWave( char *filename, int startpos, int endpos )
{
 int i, length;
 char str[40];
 short sh;
 unsigned char ch;
 void *vo;

 ofstream ofl( filename, ios::out|ios::binary);
 if (!ofl)
 {
  strcpy( str, "Can't open file!" );
  //m->warn( str );
   cerr << str << endl;
 }
 else
 {
  length=endpos-startpos;
  strcpy( str, "RIFF" );
  ofl.write( str, 4 );
  i=length*(getBit()/8)*getChannels()+36;
  ofl.write( (char *)&i, 4 );
  strcpy( str, "WAVE" );
  ofl.write( str, 4 );
  strcpy( str, "fmt " );
  ofl.write( str, 4 );
  i=16;
  ofl.write( (char *)&i, 4 );
  sh=1;
  ofl.write( (char *)&sh, 2);
  sh=getChannels();
  ofl.write( (char *)&sh, 2);
  i=getSampling();
  ofl.write( (char *)&i, 4 );
  i=getSampling()*getChannels()*(getBit()/8);
  ofl.write( (char *)&i, 4 );
  sh=getChannels()*(getBit()/8);
  ofl.write( (char *)&sh, 2 );
  sh=getBit();
  ofl.write( (char *)&sh, 2 );
  strcpy( str, "data");
  ofl.write( str, 4 );
  i=length*getBit()/8*getChannels();
  ofl.write( (char *)&i, 4 );
  if (getChannels()==1)
  {
   if (getBit()==8)
   {
    for(i=startpos;i<endpos;i++)
    {
     ch=inttoch( waveData1[i] );
     vo=&ch;
     ofl.write( (char *)vo, 1 );
    }
   }
   else
   {
    for(i=startpos;i<endpos;i++)
    {
     vo=&waveData1[i];
     ofl.write( (char *)vo, 2 );
    }
   }
  }
  else
  {
   if (getBit()==8)
   {
    for(i=startpos;i<endpos;i++)
    {
     ch=inttoch( waveData1[i] );
     vo=&ch;
     ofl.write( (char *)vo, 1 );
     ch=inttoch( waveData2[i] );
     vo=&ch;
     ofl.write( (char *)vo, 1 );
    }
   }
   else
   {
    for(i=startpos;i<endpos;i++)
    {
     vo=&waveData1[i];
     ofl.write( (char *)vo, 2 );
     vo=&waveData2[i];
     ofl.write( (char *)vo, 2 );
    }
   }
  }
 }
}

void Wave::freemem()
{
	free( waveData1 );
	if (getChannels()==2) free( waveData2 );
}

int Wave::getWloadStat()
{
	return loaded;
}

void Wave::setSize( int i )
{
	Size=i;
}

void Wave::setSampling( int i )
{
	Sampling=i;
}

void Wave::setBit( int i )
{
	bit=i;
}

void Wave::setChannels(int i )
{
	Channels=i;
}

void Wave::setNumSamples(int i )
{
	NumSamples=i;
}

void Wave::setWloadStat( int l )
{
	loaded=l;
}

void Wave::toStereo()
{
 int i;

 if ( getChannels()==1)
 {
  waveData2 = (short*) calloc( getNumSamples(), 2 );
  for (i=0;i<getNumSamples();i++) waveData2[i]=waveData1[i];
  setChannels( 2 );
  setSize( getSize()*2 );
 }
}

void Wave::toMono()
{
 if ( getChannels()==2 )
 {
  free( waveData2 );
  setChannels( 1 );
  setSize( getSize()/2 );
 }
}

void Wave::Bitsto8()
{
 if ( getBit()!=8 )
 {
  setBit( 8 );
  setSize( getSize()/2 );
 }
}

void Wave::Bitsto16()
{
 if ( getBit()!=16 )
 {
  setBit( 16 );
  setSize( getSize()*2 );
 }
}

void Wave::downmix1( int newfreq )
{
 float f;
 int i, s;
 int newNumSamples;
 short *newW;

 f=getSampling()/newfreq;
 newNumSamples=int(getNumSamples()/f);
 newW = (short*) calloc( newNumSamples, 2 );
 for (i=0;i<newNumSamples;i++)
 {
  s=int(i*f);
  newW[i]=waveData1[s];
 }
 waveData1 = (short*) realloc ( waveData1, newNumSamples*2 );
 for (i=0;i<newNumSamples;i++) waveData1[i]=newW[i];
 free ( newW );
 setNumSamples( newNumSamples );
 setSampling( newfreq );
 setSize( getNumSamples()*(getBit()/8)*getChannels() );
}

void Wave::downmix2( int newfreq )
{
 float f;
 int i, s;
 int newNumSamples;
 short *newW;

 f=getSampling()/newfreq;
 newNumSamples=int(getNumSamples()/f);
 newW = (short*) calloc( newNumSamples, 2 );
 for (i=0;i<newNumSamples;i++)
 {
  s=int(i*f);
  newW[i]=waveData2[s];
 }
 waveData2 = (short*) realloc ( waveData2, newNumSamples*2 );
 for (i=0;i<newNumSamples;i++) waveData2[i]=newW[i];
 free ( newW );
}

void Wave::upmix1( int newfreq )
{
 float f;
 int i, s;
 int newNumSamples;
 short *newW;

 f=newfreq/getSampling();
 newNumSamples=int(getNumSamples()*f);
 newW = (short*) calloc( newNumSamples, 2 );
 for (i=0;i<newNumSamples;i++)
 {
  s=int(i/f);
  newW[i]=waveData1[s];
 }
 waveData1 = (short*) realloc ( waveData1, newNumSamples*2 );
 for (i=0;i<newNumSamples;i++) waveData1[i]=newW[i];
 free ( newW );
 setNumSamples( newNumSamples );
 setSampling( newfreq );
 setSize( getNumSamples()*(getBit()/8)*getChannels() );
}

void Wave::upmix2( int newfreq )
{
 float f;
 int i, s;
 int newNumSamples;
 short *newW;

 f=newfreq/getSampling();
 newNumSamples=int(getNumSamples()*f);
 newW = (short*) calloc( newNumSamples, 2 );
 for (i=0;i<newNumSamples;i++)
 {
  s=int(i/f);
  newW[i]=waveData2[s];
 }
 waveData2 = (short*) realloc ( waveData2, newNumSamples*2 );
 for (i=0;i<newNumSamples;i++) waveData2[i]=newW[i];
 free ( newW );
}

void Wave::Hzto( int freq )
{
 if ( getSampling()>freq)
 {
  if (getChannels()==2) downmix2( freq );
  downmix1( freq );
 }
 if ( getSampling()<freq)
 {
  if (getChannels()==2) upmix2( freq );
  upmix1( freq );
 }
}

void Wave::ChangeFrq( int Freq )
{
	setSampling( Freq );
}

short Wave::leftWave(int i)
{
	return waveData1[i];
}

short Wave::rightWave(int i)
{
	return waveData2[i];
}

void Wave::cut(int start, int end)
{
 int i, length, newSamples;

 if (start>end)
 {
  i=end;
  end=start;
  start=i;
 }
 saveWave( "/tmp/kwav-clipboard.wav", start, end );
 length=end-start;
 newSamples=getNumSamples()-length;
 for (i=start;i<newSamples;i++)
 {
  waveData1[i]=waveData1[i+length];
  if (getChannels()==2) waveData2[i]=waveData2[i+length];
 }
 waveData1 = (short*) realloc( waveData1, newSamples*2 );
 if (getChannels()==2)
  waveData2 = (short*) realloc( waveData2, newSamples*2 );
 setNumSamples( newSamples );
 setSize( getNumSamples()*getBit()/8*getChannels() );
}

void Wave::copy( int start, int end)
{
	saveWave( "/tmp/kwav-clipboard.wav", start, end );
}

void Wave::insertWavemix( char *file, int pos )
{
 int i, length;

 addW.openWave( file );
 if (addW.getWloadStat()!=0)
 {
  if ( Channels==1 ) addW.toMono();
  else addW.toStereo();
  if ( bit==8 ) addW.Bitsto8();
  else addW.Bitsto16();
  if ( Sampling==11025 ) addW.Hzto(11025);
  if ( Sampling==22050 ) addW.Hzto(22050);
  if ( Sampling==44100 ) addW.Hzto(44100);

  length=addW.getNumSamples();
  if ( addW.getNumSamples()>NumSamples-pos) length=NumSamples-pos;

  for (i=0;i<length;i++)
  {
   waveData1[i+pos]+=addW.waveData1[i];
   if (Channels==2)
    waveData2[i+pos]+=addW.waveData2[i];
  }
 }
// addW.freemem();
// addW.setWloadStat( 0 );
}

void Wave::insertWave( char *file, int pos )
{
 int i;

 addW.openWave( file );
 if (addW.getWloadStat()!=0)
 {
  if ( Channels==1 ) addW.toMono();
   else addW.toStereo();
  if ( bit==8 ) addW.Bitsto8();
   else addW.Bitsto16();
  if ( Sampling==11025 ) addW.Hzto( 11025 );
  if ( Sampling==22050 ) addW.Hzto( 22050 );
  if ( Sampling==44100 ) addW.Hzto( 44100 );

  waveData1 = ( short * ) realloc( waveData1, (NumSamples+addW.getNumSamples())*2 );
  if (Channels==2)
   waveData2 = ( short * ) realloc( waveData2, (NumSamples+addW.getNumSamples())*2 );
  NumSamples+=addW.getNumSamples();
  for (i=NumSamples-1;i>pos+addW.getNumSamples();i--)
  {
   waveData1[i]=waveData1[i-addW.getNumSamples()];
   if (Channels==2) waveData2[i]=waveData2[i-addW.getNumSamples()];
  }
  for (i=0;i<addW.getNumSamples();i++)
  {
   waveData1[i+pos]=addW.waveData1[i];
   if (Channels==2)
    waveData2[i+pos]=addW.waveData2[i];
  }
  Size=NumSamples*bit/8*Channels;
 }
// addW.freemem();
// addW.setWloadStat( 0 );
}

void Wave::fadeIn( int startp, int endp, int perc, int chann )
{
 int i;

 for (i=startp;i<endp;i++)
 {
  if ( chann==1 )
  waveData1[i]=int(waveData1[i]*((100-perc*(1-float(i-startp)/(endp-startp)))/100));
  else
  waveData2[i]=int(waveData2[i]*((100-perc*(1-float(i-startp)/(endp-startp)))/100));
 }
}

void Wave::fadeOut( int startp, int endp, int perc, int chann )
{
 int i;

 for (i=startp;i<=endp;i++)
 {
  if ( chann==1 )
   waveData1[i]=int(waveData1[i]*((100-perc*float(i-startp)/(endp-startp))/100));
  else
   waveData2[i]=int(waveData2[i]*((100-perc*float(i-startp)/(endp-startp))/100));
 }
}

int Wave::initPlay()
{
#ifdef __linux__
 dsp_samplesize = 16;
 dsp_speed = Sampling;
 dsp_stereo = Channels-1;
 buffer_size = 65535;
 strcpy( audio_dev, "/dev/dsp" );

 audio = open (audio_dev, O_WRONLY, 0);
 if(audio == -1)
 {
  return -1;
 }
 if(ioctl(audio,SNDCTL_DSP_SPEED,&dsp_speed) == -1)
 {
  close(audio);
  audio = -1;
  return -2;
 }
 if(ioctl(audio,SNDCTL_DSP_SAMPLESIZE,&dsp_samplesize) == -1)
 {
  close(audio);
  audio = -1;
  return -3;
 }
 if(ioctl(audio,SNDCTL_DSP_STEREO,&dsp_stereo) == -1)
 {
  close(audio);
  audio = -1;
  return -4;
 }
// if (ioctl(audio,SNDCTL_DSP_SETFRAGMENT,16)!=-1)
// {
  ioctl(audio,SNDCTL_DSP_GETBLKSIZE,&buffer_size);
  if((buffer = (short*) malloc(buffer_size)) == NULL)
  {
   close(audio);
   audio = -1;
   return -4;
  }
// }
// else
// {
//  close(audio);
//  audio = -1;
//  return -4;
// }
 currPlayPos=0;
 playActive=1;

#endif
 return 0;
}

void Wave::playAsync()
{
#ifdef __linux__
 if( currPlayPos <= getActMaxPlayPos() )		// bugfix 26.12.2001: <=
 {
  minHandle<ShortReadBuffer> hBuffer;

  while( !hBuffer.IsOk() )
  {
	if( m_pReadQueue )
	{
	  	hBuffer = m_pReadQueue->GetNextBuffer();
	}
	/*
	if( !hBuffer.IsOk() )
	{
		minSleep( 100 );
	}
	*/
  }

  currPlayPos++;
  // ** der buffer wird nur zur haelfte mit daten gefuellt, siehe play() Methode !
  if( write( audio, hBuffer->GetDataPtr(0), buffer_size ) != buffer_size )
  {
   stopPlay();
  }
 }
 else
 {
  stopPlay();
 }
#endif
}

void Wave::play()
{
#ifdef __linux__
 int i;

 if (currPlayPos <= getActMaxPlayPos()/*Channels*NumSamples/(buffer_size/2)*/ )		// bugfix 26.12.2001: <=
 {
  if(dsp_stereo==1)
  {
	for(i = 0;i < buffer_size/4;i++)
	{
		buffer[i * 2] = waveData1[currPlayPos*buffer_size/4+i];
		buffer[i * 2 + 1] = waveData2[currPlayPos*buffer_size/4+i];
	}
  }
  else
  {
	for(i = 0;i < buffer_size/2;i++)
	{
		buffer[i] = waveData1[currPlayPos*buffer_size/2+i];
	}
  }

  currPlayPos++;
  if(write(audio,buffer,buffer_size) != buffer_size)
  {
   stopPlay();
  }
 }
 else
 {
  stopPlay();
 }
#endif
}

void Wave::stopPlay()
{
#ifdef __linux__
 //currPlayPos=0;

 close(audio);
 audio = -1;
 free(buffer);
 buffer=0;
 playActive=0;		// stops the play-thread

 if( isAsync() )
 {
    // ** stops the read-thread
 	delete m_pReadQueue;
	m_pReadQueue = 0;
 	m_iFileReadPos = -1;
 }

#endif
}

void Wave::pausePlay()
{
	playActive = 0;
}

void Wave::resumePlay()
{
	playActive = 1;
}

int Wave::getActivePlay()
{
	return playActive;
}

int Wave::getCurrPlayPos()
{
	return currPlayPos;
}

int Wave::setCurrPlayPos( int iPos )
{
	if( (iPos>=0) && (iPos<getMaxPlayPos()) )
	{
		currPlayPos = iPos;

		if( m_pReadQueue )
		{
			m_pReadQueue->SetReadPos( currPlayPos );
		}

		return 1;
	}

	return 0;	// not ok !
}

int Wave::getCurrPlaySamplePos()
{
	return (currPlayPos*(buffer_size/2))/Channels;	// rounding error, bugfix (8.11.2002): currPlayPos/Channels*(buffer_size/2);
}

double Wave::getCurrPlayPosInSeconds()
{
	if( getSampling() == 0 )
		return 0;

	return (double)(getCurrPlaySamplePos())/(double)(getSampling());
}

int Wave::convertSecondsInSamples( double dTimeInSeconds )
{
	if( (dTimeInSeconds>=0) && (dTimeInSeconds<=getTotalPlayTimeInSeconds()) && (getSampling()!=0) )
	{
		return (int)(dTimeInSeconds*(double)getSampling()); //*Channels/(buffer_size/2);
	}
	return -1;	// not ok !
}

int Wave::setCurrPlayPosInSeconds( double dPos )
{
	if( (dPos>=0) && (dPos<=getTotalPlayTimeInSeconds()) && (getSampling()!=0) )
	{
		int iPos = (int)(dPos*(double)getSampling())*Channels/(buffer_size/2);
		setCurrPlayPos( iPos );
		return 1;
	}
	return 0;  // not ok !
}

int Wave::setMaxPlayPosInSeconds( double dPos )
{
	if( (dPos>=0) && (dPos<=getTotalPlayTimeInSeconds()) && (getSampling()!=0) )
	{
		// ** + 1, because of the rounding error (double --> int)
		m_iMaxPos = (int)(dPos*(double)getSampling())*Channels/(buffer_size/2) + 1;
		return 1;
	}
	m_iMaxPos = -1;
	return 0;  // not ok !
}

double Wave::getTotalPlayTimeInSeconds()
{
	if( getSampling() == 0 )
		return 0;

	return (double)getNumSamples()/(double)getSampling();
}

int Wave::getDSPBuffer()
{
	return buffer_size;
}

int Wave::initRec()
{
#ifdef __linux__

 dsp_samplesize = 16;
 dsp_speed = Sampling;
 dsp_stereo = Channels-1;
 buffer_size = 256;
 strcpy( audio_dev, "/dev/dsp" );

 audio = open (audio_dev, O_RDONLY, 0);
 if(audio == -1)
 {
  return -1;
 }
 if(ioctl(audio,SNDCTL_DSP_SPEED,&dsp_speed) == -1)
 {
  close(audio);
  audio = -1;
  return -2;
 }
 if(ioctl(audio,SNDCTL_DSP_SAMPLESIZE,&dsp_samplesize) == -1)
 {
  close(audio);
  audio = -1;
  return -3;
 }
 if(ioctl(audio,SNDCTL_DSP_STEREO,&dsp_stereo) == -1)
 {
  close(audio);
  audio = -1;
  return -4;
 }
 ioctl(audio,SNDCTL_DSP_GETBLKSIZE,&buffer_size);
 if((buffer = (short*) malloc(buffer_size)) == NULL)
 {
  close(audio);
  audio = -1;
  return -4;
 }
 orec.open("/tmp/kwav-rec.dat", ios::out|ios::binary );
 currPlayPos=0;
 playActive=1;
#endif
 return 0;
}

void Wave::Rec()
{
#ifdef __linux__
 if(read(audio,(char *)buffer,buffer_size) != buffer_size)
 {
  stopPlay();
 }
 orec.write( (char *)buffer, buffer_size );
#endif
}

void Wave::stopRec()
{
#ifdef __linux__
 int r_size, i;

 orec.close();
 currPlayPos=0;
 close(audio);
 audio = -1;
 free(buffer);
 buffer=0;
 playActive=0;

 ifstream irec( "/tmp/kwav-rec.dat", ios::in|ios::binary );
 irec.seekg(0, ios::end);
 r_size=irec.tellg();
 cout << r_size;
 irec.seekg(0, ios::beg);
 if ( Channels==1 )
 {
  NumSamples=r_size/2;
  waveData1 = (short*) calloc( getNumSamples(), 2 );
  for (i=1; i<NumSamples; i++) irec.read( (char *)&waveData1[i], 2 );
 }
 else
 {
  NumSamples=r_size/4;
  waveData1 = (short*) calloc( getNumSamples(), 2 );
  waveData2 = (short*) calloc( getNumSamples(), 2 );
  for (i=1; i<NumSamples; i++)
  {
   irec.read( (char *)&waveData1[i], 2 );
   irec.read( (char *)&waveData2[i], 2 );
  }
 }
 orec.close();

 system( "rm /tmp/kwav-rec.dat" );
//min todo m->setStatSize( r_size );
//min todo m->setStatTime();
 loaded=1;
//min todo m->initPaintWave();
//min todo m->paintWave();
 if (Channels==2)
 {
//min todo  m->effects->setItemEnabled( ID_EFFECTS_TURNLR, TRUE );
//min todo  m->effects->setItemEnabled( ID_EFFECTS_TURNRL, TRUE );
 }
 else
 {
//min todo  m->effects->setItemEnabled( ID_EFFECTS_TURNLR, FALSE );
//min todo  m->effects->setItemEnabled( ID_EFFECTS_TURNRL, FALSE );
 }
//min todo m->setCaption("Kwav");
#endif
}

int Wave::getActiveRec()
{
	return playActive;
}

int Wave::getMaxPlayPos()
{
	return Channels*NumSamples/(buffer_size/2);
}

int Wave::getActMaxPlayPos()
{
	if( (m_iMaxPos<0) || (m_iMaxPos>getMaxPlayPos()) )
	{
		return getMaxPlayPos();
	}
	return m_iMaxPos;
}

void Wave::startAsyncReadBuffer()
{
	m_pReadQueue = new ReadBufferQueue( FileName,
										m_iFileReadPos,
										getChannels(),
										getBit(),
										buffer_size/2,
										64 );
	if( HasFadeInInfo() )
	{
		m_pReadQueue->AddFadeIn( convertSecondsInSamples( m_dFadeInStartTimeInSeconds ),
								 convertSecondsInSamples( m_dFadeInStartTimeInSeconds + m_dFadeInLengthInSeconds ) );
	}
	if( HasFadeOutInfo() )
	{
		m_pReadQueue->AddFadeOut( convertSecondsInSamples( m_dFadeOutStartTimeInSeconds ),
								  convertSecondsInSamples( m_dFadeOutStartTimeInSeconds + m_dFadeOutLengthInSeconds ) );
	}
	m_pReadQueue->SetReadPos( currPlayPos );

	m_pReadQueue->StartThread();

	// ** give some time to read the first few buffers...
	minSleep( 100 );
}

bool Wave::HasFadeInInfo() const
{
	return ( m_dFadeInStartTimeInSeconds >= 0 );
}

bool Wave::HasFadeOutInfo() const
{
	return ( m_dFadeOutStartTimeInSeconds >= 0 );
}

void Wave::setFadeInInfos( double dStartTimeInSeconds, double dLengthInSeconds )
{
	m_dFadeInStartTimeInSeconds = dStartTimeInSeconds;
	m_dFadeInLengthInSeconds = dLengthInSeconds;
}

void Wave::setFadeOutInfos( double dStartTimeInSeconds, double dLengthInSeconds )
{
	m_dFadeOutStartTimeInSeconds = dStartTimeInSeconds;
	m_dFadeOutLengthInSeconds = dLengthInSeconds;
}

void Wave::playInThread()
{
#ifndef _with_main
#ifdef _with_min_threads
	m_ulThreadID = minBeginThread( _wavSoundThreadStarter, _DEFAULT_STACK_SIZE, this );
#else
    start();
    m_ulThreadID = 1;   // dummy id
#endif //_with_min_threads    
#endif
}


#ifdef _with_main

int main( int argc, char * argv[] )
{
	Wave aWave;

	if( argc>1 )
	{
		cout << "read wav-file: " << argv[1] << endl;
		aWave.openWave( argv[1] );
/*
		cout << "no of samples: " << aWave.getNumSamples() << endl;
		cout << "no of channels: " << aWave.getChannels() << endl;
		cout << "no of bits: " << aWave.getBit() << endl;
		cout << "size: " << aWave.getSize() << endl;
*/
/*
		for( int i=0; i<100000; i++ )
		{
			cout << i << " " << aWave.leftWave( i ) << " " << aWave.rightWave( i ) << endl;
		}
*/
		cout << "init playing..." << endl;
		int iRet = aWave.initPlay();
		cout << "playing... ret=" << iRet << endl;
		while( aWave.getActivePlay() )
		{
			cout << "pos=" << aWave.getCurrPlayPos()
//				<< " dsp=" << aWave.getDSPBuffer()
//				<< " act=" << aWave.getActivePlay()
				<< " size=" << aWave.getSize()
				<< " sampl=" << aWave.getNumSamples()
				<< " #samples=" << aWave.getSampling()
				<< " pos=" << aWave.getCurrPlaySamplePos()
				<< " time=" << aWave.getCurrPlayPosInSeconds() << endl;

			aWave.play();
		}
	}
	cout << "done." << endl;

	return 0;
}

#endif

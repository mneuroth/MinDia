/*
	The code in this file is taken from the kwav-project 
	from Guenter Schwann (vogel@ibk.ml.org; www.ibk.ml.org/home/vogel),
	which is released under the GPL ver. 2.
	The code is modified by Michael Neuroth.
*/

#ifndef _WAVFILE_H
#define _WAVFILE_H

#include <iostream>
#include <fstream>

using namespace std;

class ReadBufferQueue;

//class for wave-file
class Wave
{
 public:
  Wave();
  ~Wave();
  int getSize();
  int getSampling();
  int getBit();
  int getChannels();
  int getNumSamples();			// get size
  int openWave( char * );
  int openWaveAsync( const char * );
  int isAsync() const;
  void saveWave( char *, int, int );
  void freemem();
  int getWloadStat();
  void setWloadStat( int );
  void setFileName( const char *);
  char *getFileName();
  void toStereo();
  void toMono();
  void Bitsto8();
  void Bitsto16();
  void Hzto( int );
  void ChangeFrq( int );
  void hall(int, int);
  void reverse(short *);
  void loudness( int );
  void invert();
  short leftWave(int);			// access to data
  short rightWave(int);			// access to data
  void cut(int, int);
  void copy(int, int);
  void insertWavemix(char *,int);
  void insertWave(char *,int);
  int initPlay();
  void play();
  void playAsync();
  void stopPlay();
  void pausePlay();
  void resumePlay();
  void fadeIn( int, int, int, int );
  void fadeOut( int, int, int, int );
  int getActivePlay();
  int getCurrPlayPos();
  int setCurrPlayPos( int iPos );
  int getCurrPlaySamplePos();
  double getCurrPlayPosInSeconds();
  int setCurrPlayPosInSeconds( double dPos );
  int setMaxPlayPosInSeconds( double dPos );
  double getTotalPlayTimeInSeconds();
  int getDSPBuffer();
  int initRec();
  void Rec();
  void stopRec();
  void pauseRec();
  int getActiveRec();

  void setSampling( int );
  void setBit( int );
  void setChannels( int );

  void playInThread();
  void startAsyncReadBuffer();

  // ** fade in and fade out for async play
  void setFadeInInfos( double dStartTimeInSeconds, double dLengthInSeconds );
  void setFadeOutInfos( double dStartTimeInSeconds, double dLengthInSeconds );

 private:
  bool HasFadeInInfo() const;
  bool HasFadeOutInfo() const;

  int convertSecondsInSamples( double dTimeInSeconds );

  int getMaxPlayPos();
  int getActMaxPlayPos();

  unsigned long 		m_ulThreadID;
  int 					m_iMaxPos;
  int 					m_iFileReadPos;
  ReadBufferQueue *		m_pReadQueue;
  double 				m_dFadeInStartTimeInSeconds;
  double 				m_dFadeInLengthInSeconds;
  double 				m_dFadeOutStartTimeInSeconds;
  double 				m_dFadeOutLengthInSeconds;

  int buffer_size;

  short *waveData1;
  short *waveData2;

  char FileName[80];
  int Size;
  int Sampling;
  int bit;
  int Channels;
  int loaded;
  int NumSamples;
  int currPlayPos;
  int playActive;

  int dsp_speed;
  int dsp_samplesize;
  int dsp_stereo;
  char audio_dev[80];
  int audio;
  short *buffer;
  ofstream orec;

  void setSize( int );
  void setNumSamples( int );
  int chtoint( unsigned char );
  unsigned char inttoch( int );
  void downmix1( int ); //INTERPOLATION EINBAUEN!!!
  void downmix2( int ); //INTERPOLATION EINBAUEN!!!
  void upmix1( int ); //INTERPOLATION EINBAUEN!!!
  void upmix2( int ); //INTERPOLATION EINBAUEN!!!
};

#endif

/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/rolleicom.cpp,v $
 *
 *  $Revision: 1.1.1.1 $
 *
 *	$Log: not supported by cvs2svn $
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2002 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "rolleicom.h"

#include "osdep2.h"

#include "mincmdproc.h"
#include "minlog.h"
#include "miniini.h"

#include <stdio.h>

//#include <iostream>
//using namespace std;


// *******************************************************************
// *******************************************************************
// *******************************************************************

#ifdef _MSC_VER

#include <windows.h>

//#define _use_wfc_serial

#ifdef _use_wfc_serial

#include <winnt.h>
#include <winbase.h>

//BOOL PASCAL wfc_close_handle( HANDLE handle );
//void UNICODE_to_ASCII( LPCWSTR unicode_string, LPSTR ansi_string, long number_of_unicode_characters_to_convert = (-1) );

#undef _INC_CRTDBG

#define WFC_STL
#define WFC_NO_DUMPING
#define WFC_NO_SERIALIZATION
#define WFCLTRACEINIT( x )
#define WFC_COVERAGE( x )
#define WFCTRACEERROR( x ) 
#define WFCTRACE( x )
#define WFCTRACEVAL( x, y )
//#define WFC_DOESNT_THROW_EXCEPTIONS

#if ! defined( DIMENSION_OF )
#define DIMENSION_OF( argument ) ( sizeof( argument ) / sizeof( *( argument ) ) )
#endif

#if ! defined( CFILE_HFILE )
#define CFILE_HFILE HANDLE
#endif

#if ! defined( CFILE_LENGTH )
#define CFILE_LENGTH ULONGLONG
#endif

#if ! defined( CFILE_POSITION )
#define CFILE_POSITION ULONGLONG
#endif

#if ! defined( CFILE_SEEK_OFFSET )
#define CFILE_SEEK_OFFSET LONGLONG
#endif

#if ! defined( CFILE_HFILE )
#define CFILE_HFILE HANDLE
#endif

#if ! defined( WFC_DONT_USE_NAMESPACES )
#define USING_WFC_NAMESPACE using namespace Win32FoundationClasses;
#else // WFC_DONT_USE_NAMESPACES
#define USING_WFC_NAMESPACE
#endif // WFC_DONT_USE_NAMESPACES


#include "wfc/wfc_exceptions.h"
namespace Win32FoundationClasses {
BOOL PASCAL wfc_close_handle( HANDLE handle ) WFC_DOESNT_THROW_EXCEPTIONS;
}
#include "wfc/Mfc2stl.h"
using namespace Win32FoundationClasses;
#include "wfc/DCB.HPP"
#include "wfc/Structs.hpp"
#include "wfc/CFile.hpp"
#include "wfc/DumyFile.hpp"
#include "wfc/Serial.hpp"
#include "wfc/DCB.CPP"
#include "wfc/Structs.cpp"
#include "wfc/CFile.cpp"
#include "wfc/DumyFile.cpp"
#include "wfc/Serial.cpp"
#include "wfc/STL_CString.cpp"
//#include "wfc/STL_CByteArray.cpp"
#include "wfc/wfc_close_handle.cpp"
#include "wfc/U2A_A2U.cpp"


/** OS depending helper data */
struct RolleiComHelperData
{
	RolleiComHelperData( int iComPortNo )
	{
		m_bOk = FALSE;
		m_iLastError = 0;
		Open( iComPortNo );
	}
	~RolleiComHelperData()
	{
		Close();
	}

	bool Open( int iComPortNo )
	{
		char sBuffer[255];
		sprintf( sBuffer, "COM%d", iComPortNo );

		m_bOk = m_aCom.Open( "COM1:1200,e,7,2");

		return IsOk();
	}
	bool Close()
	{
		if( IsOk() )
		{
			m_aCom.Close();
		}
		return true;
	}

	bool SetComData( bool bIgnoreComSettings, int iBaudrate, int iParityMode, const string & sStopBits, int iDataBits, int iFlowMode )
	{		
		if( bIgnoreComSettings )
		{
			//BOOL bDCBOk = GetCommState( m_hFile, &aDCB );
			// ggf. Ausgabe der Settings ...

			return true;
		}
		else
		{
			m_aCom.SetBaudRate( iBaudrate );
			m_aCom.SetFlowControl( /*iFlowMode*/ CSerialFile::flowHardware );

			COMMTIMEOUTS aTimeouts;
			m_aCom.GetTimeouts( aTimeouts );
			aTimeouts.ReadIntervalTimeout = 100;
			aTimeouts.ReadTotalTimeoutMultiplier = 100;		// timeout per character
			aTimeouts.ReadTotalTimeoutConstant = 20000;		// total timeout for read (20s) !
			aTimeouts.WriteTotalTimeoutMultiplier = 100;		// timeout per character
			aTimeouts.WriteTotalTimeoutConstant = 2000; 
			m_aCom.SetTimeouts( &aTimeouts );

			CCommunicationsConfiguration aComConf;
			m_aCom.GetConfiguration( aComConf );
			m_aCom.ConfigurationDialog( aComConf );
			//m_aCom.SetConfiguration( aComConf );

			return true;
		}

		return false;
	}

	bool IsOk() const
	{
		return m_bOk;
	}

	bool Write( const string & sMsg )
	{
		if( IsOk() )
		{
			CString sTemp = sMsg.c_str();
			m_aCom.Write( sTemp );
			return true;
		}
		return false;
	}

	bool Read( string & sMsgOut, int iCount )
	{
		if( IsOk() )
		{
			char sBuffer[255];
			strcpy( sBuffer, "" );
			m_aCom.Read( sBuffer, iCount );
			sMsgOut = sBuffer;
			return true;
		}
		return false;
	}

	void Delay( int iTimeInMS )
	{
		::Sleep( (DWORD)iTimeInMS );
	}

	DWORD GetLastErrorCode() const
	{
		return m_iLastError;
	}

	CSerialFile		m_aCom;
	BOOL			m_bOk;
	DWORD			m_iLastError;
};


#else

/** OS depending helper data */
struct RolleiComHelperData
{
	RolleiComHelperData( int iComPortNo )
	{
		m_iLastError = 0;
		m_hFile = INVALID_HANDLE_VALUE;
		Open( iComPortNo );
	}
	~RolleiComHelperData()
	{
		Close();
		m_hFile = INVALID_HANDLE_VALUE;
	}

	bool Open( int iComPortNo )
	{
		char sBuffer[255];
		sprintf( sBuffer, "COM%d", iComPortNo );
		m_hFile = CreateFile( (LPCTSTR)sBuffer,
							  (GENERIC_READ | GENERIC_WRITE),
							  0,
							  NULL,
							  OPEN_EXISTING,
// min todo gulp
// org:							  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH /* | FILE_FLAG_NO_BUFFERING | FILE_FLAG_RANDOM_ACCESS*/,
// tty.c:						  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
// GRIT:						  FILE_ATTRIBUTE_NORMAL,
// other:					      0,
							  0,
							  NULL );
		if( !m_hFile )
		{
			m_iLastError = ::GetLastError();
		}
		return IsOk();
	}
	bool Close()
	{
		if( m_hFile != INVALID_HANDLE_VALUE )
		{
			return CloseHandle( m_hFile ) != 0;
		}
		return true;
	}

	bool SetComData( bool bIgnoreComSettings, int iBaudrate, int iParityMode, const string & sStopBits, int iDataBits, int iFlowMode )
	{		
		char sBuffer[255];
		char chParity = 'n';
		int iParity = 0;
		if( iParityMode == RolleiCom::NONE )
		{
			chParity = 'n';
		}
		else if( iParityMode == RolleiCom::ODD )
		{
			chParity = 'o';
			iParity = 1;
		}
		else if( iParityMode == RolleiCom::EVEN )
		{
			chParity = 'e';
			iParity = 1;
		}
		/* funktioniert leider nicht !
		char sFlow[255];
		strcpy( sFlow, "" );
		if( iFlowMode == RolleiCom::XON_XOFF_FLOW )
		{
			sprintf( sFlow, " xon=on" );
		}
		if( iFlowMode == RolleiCom::HARDWARE_FLOW )
		{
			sprintf( sFlow, " odsr=on octs=on dtr=hs rts=hs" );
		}
		*/
		sprintf( sBuffer, "BAUD=%d PARITY=%c DATA=%d STOP=%s", iBaudrate, chParity, iDataBits, sStopBits.c_str() );
//		sprintf( sBuffer, "12,n,7,2,p" );
		//sprintf( sBuffer, "baud=%d parity=%c data=%d stop=%s%s", iBaudrate, chParity, iDataBits, sStopBits.c_str(), sFlow );
		// old version of modem-string: mode com1 12,e,7,2,p

		// ** VERIY IMPORTANT: ** 
		// ** the time for the execution of the commands can be very long !
		// ** the success is only indicated, if the slide is visible (dissolve-time).
		COMMTIMEOUTS aTimeout;
// min todo --> Werte in config-datei
		GetCommTimeouts( m_hFile, &aTimeout );
		aTimeout.ReadIntervalTimeout = 100;
		aTimeout.ReadTotalTimeoutMultiplier = 100;		// timeout per character
		aTimeout.ReadTotalTimeoutConstant = 10000;		// total timeout for read (20s) !
//		aTimeout.ReadTotalTimeoutConstant = 20000;		// total timeout for read (20s) !
		aTimeout.WriteTotalTimeoutMultiplier = 100;		// timeout per character
		aTimeout.WriteTotalTimeoutConstant = 1000; 
//		aTimeout.WriteTotalTimeoutConstant = 2000; 
// min todo gulp --> tests mit verschiedenen timeouts, insbes. fuer senden --> RS232 Problem ?
		SetCommTimeouts( m_hFile, &aTimeout ); 

		DCB aDCB;
		// ** bugfix 28.12.2001: initialize the structure before using it !
		memset( &aDCB, 0, sizeof( DCB ) );
		aDCB.DCBlength = sizeof( aDCB );
		//debug: GetCommState( m_hFile, &aDCB );
		if( bIgnoreComSettings )
		{
			//BOOL bDCBOk = GetCommState( m_hFile, &aDCB );
			// ggf. Ausgabe der Settings ...

			return true;
		}
		else
		{
			if( BuildCommDCB( (LPCTSTR)sBuffer, &aDCB ) && IsOk() )
			{
				// search for DCB in MSDEVLIB
				// see: MSDEVLIB: Serial Communications in Win32
				//aDCB.fParity = iParity;
				aDCB.fBinary = TRUE;			// new since 29.3.2002	
				
				// min todo gulp --> bessere Fehlerbehandlung fuer RS232 unter Windows 
				//   --> ClearCommError EscapeCommFunction 

				// ** VERIY IMPORTANT: **
				// ** hardware-handshake needed !
				if( iFlowMode == RolleiCom::HARDWARE_FLOW )
				{
					aDCB.fInX         = FALSE;
					aDCB.fOutX        = FALSE;
					aDCB.fOutxCtsFlow = TRUE;					// 1
					aDCB.fOutxDsrFlow = TRUE;					// 0
					aDCB.fDtrControl  = DTR_CONTROL_HANDSHAKE;	// 0
					aDCB.fRtsControl  = RTS_CONTROL_HANDSHAKE;	// 2
				}
				if( iFlowMode == RolleiCom::XON_XOFF_FLOW )
				{
					aDCB.fInX         = TRUE;
					aDCB.fOutX        = TRUE;
					aDCB.fOutxCtsFlow = FALSE;
					aDCB.fOutxDsrFlow = FALSE;
					aDCB.fDtrControl  = DTR_CONTROL_ENABLE;
					aDCB.fRtsControl  = RTS_CONTROL_ENABLE;
				}

				BOOL bRet = SetCommState( m_hFile, &aDCB );
				/* in case of an error... */
				if( !bRet )
				{
					m_iLastError = ::GetLastError();
				}

				return bRet != 0;
			}
		}

		return false;
	}

	bool IsOk() const
	{
		return (m_hFile != INVALID_HANDLE_VALUE);
	}

	bool Write( const string & sMsg )
	{
		if( IsOk() )
		{
			DWORD dwCount = 0;
			BOOL bOk = WriteFile( m_hFile, sMsg.c_str(), sMsg.length(), &dwCount, 0 );
			/* in case of an error... */
			if( !bOk )
			{
				m_iLastError = ::GetLastError();
				cerr<< "Error: COM write errno=" << m_iLastError << endl;
			}
			return bOk != 0;
		}
		return false;
	}

	bool Read( string & sMsgOut, int iCount )
	{
		if( IsOk() )
		{
			char sBuffer[255];
			DWORD dwCount = 0;
			DWORD dwSize = iCount;		// ** expect: * or ! and CR

			BOOL bRet = ReadFile( m_hFile, sBuffer, dwSize, &dwCount, 0 );
			if( bRet )
			{
				sBuffer[ dwCount ] = 0;
				sMsgOut = sBuffer;
			}
			else
			{
				m_iLastError = ::GetLastError();
				cerr<< "Error: COM read errno=" << m_iLastError << endl;
			}
			return bRet != 0;
		}
		return false;
	}

	void Delay( int iTimeInMS )
	{
		::Sleep( (DWORD)iTimeInMS );
	}

	DWORD GetLastErrorCode() const
	{
		return m_iLastError;
	}

	HANDLE	m_hFile;
	DWORD	m_iLastError;
};

#endif //_use_wfc_serial

#endif

// *******************************************************************
// *******************************************************************
// *******************************************************************

#ifdef __linux__

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
//#include <sys/termios.h>
//#include <term.h>
#include <sys/time.h>
#include <errno.h>

// ** in sysdep1.c implemented (from the program minicom) **
extern "C" void m_setparms( int fd, char * baudr, char * par, char * bits, char * stopb, int hwf, int swf);

/** OS depending helper data */
struct RolleiComHelperData
{
	RolleiComHelperData( int iComPortNo )
	{
		m_iLastError = 0;
		m_hFile = 0;
		Open( iComPortNo );
	}
	~RolleiComHelperData()
	{
		Close();
		m_hFile = 0;
	}

	bool Open( int iComPortNo )
	{
        char sDevice[255];
        sprintf( sDevice, "/dev/ttyS%d", iComPortNo-1 );
        m_hFile = open( sDevice, O_RDWR | O_NOCTTY );
        //dbg: cout << "open " << m_hFile << endl;
		if( !m_hFile )
		{
			m_iLastError = errno;
		}
		return IsOk();
	}
	bool Close()
	{
		if( m_hFile != 0 )
		{
	        int iRet = close( m_hFile );
		    //dbg: cout << "close " << m_hFile << endl;
			return (iRet == 0);
		}
		return true;
	}

	bool SetComData( bool bIgnoreComSettings, int iBaudrate, int iParityMode, const string & sStopBitsStrg, int iDataBits, int iFlowMode )
	{
        char sBaudRate[255];
        char sParity[255];
        char sStopBits[255];
        char sDataBits[255];

        sprintf( sBaudRate, "%d", iBaudrate );
        sprintf( sStopBits, "%s", sStopBitsStrg.c_str() );
        sprintf( sDataBits, "%d", iDataBits );
		if( iParityMode == RolleiCom::NONE )
		{
			strcpy( sParity, "N" );
		}
		else if( iParityMode == RolleiCom::ODD )
		{
			strcpy( sParity, "O" );
		}
		else if( iParityMode == RolleiCom::EVEN )
		{
			strcpy( sParity, "E" );
		}

		if( !bIgnoreComSettings )
		{
			int iHwFlow = 0;
			int iSwFlow = 0;
			if( iFlowMode == RolleiCom::HARDWARE_FLOW )
			{
				iHwFlow = 1;
			}
			if( iFlowMode == RolleiCom::XON_XOFF_FLOW )
			{
				iSwFlow = 1;
			}
			// ** use code from minicom-program **
			m_setparms( m_hFile, sBaudRate, sParity, sDataBits, sStopBits, iHwFlow, iSwFlow );
			//org: m_setparms( m_hFile, "1200", "E", "7", "2", 1, 0 );
		}

        return true;
	}

	bool IsOk() const
	{
		return (m_hFile != 0);
	}

	bool Write( const string & sMsg )
	{
		if( IsOk() )
		{
            int iRet = write( m_hFile, sMsg.c_str(), sMsg.length() );
			/* in case of an error... */
			if( iRet == 0 )
			{
				m_iLastError = errno;
			}
            //dbg: cout << "write: " << sMsg.c_str() << " wcount=" << iRet << endl;
            return (iRet != 0);
		}
		return false;
	}

	bool Read( string & sMsgOut, int iCount )
	{
		if( IsOk() )
		{
            char sBuffer[255];

            // ** check if there is any response on the com-port --> timeout !
            fd_set aReadfs;    /* file descriptor set */
            FD_SET( m_hFile, &aReadfs );  /* set testing for source 1 */

// min todo --> hier ggf. optimierter timeout, z.b. beim Testen kleiner als waehrend der Praesentation !
            struct timeval aTimeout;
            aTimeout.tv_usec = 0;  /* milliseconds */
            aTimeout.tv_sec  = 20;  /* seconds */

            int iRes = select( m_hFile+1, &aReadfs, NULL, NULL, &aTimeout );
            if( iRes==0 )
            {
                /* number of file descriptors with input = 0, timeout occurred. */
                return false;
            }

            // ** if we are here, there is something to read !
            int iRet = read( m_hFile, sBuffer, iCount );

            if( iRet>0 )
			{
				sBuffer[ iRet ] = 0;
                //dbg: cout << "read: " << (int)sBuffer[0] << " == " << sBuffer[0] << " rcount=" << iRet << " size=" << iCount << endl;
				sMsgOut += sBuffer;

                // ** read() has no timeout...
                // ** read until the cmd-feedback is complete (CR/LF was send)
                char ch = sMsgOut[ sMsgOut.length()-1 ];
                if( (iCount>iRet) && !((ch=='\r') || (ch=='\n')) )
                {
                    Delay( 10 );
                    Read( sMsgOut, iCount-iRet );
                }
			}
			else
			{
				m_iLastError = errno;
			}

            return (iRet != 0);
		}
		return false;
	}

	void Delay( int iTimeInMS )
	{
        usleep( iTimeInMS );
	}

	int GetLastErrorCode() const
	{
		return m_iLastError;
	}

    int		m_hFile;
	int		m_iLastError;
};

#endif

// *******************************************************************
// *******************************************************************
// *******************************************************************

const int RolleiCom::NONE					= 0;
const int RolleiCom::ODD					= 1;
const int RolleiCom::EVEN					= 2;

const int RolleiCom::ONE					= 0;
const int RolleiCom::ONE_POINT_FIVE			= 1;
const int RolleiCom::TWO					= 2;

const int RolleiCom::NO_FLOW				= 0;
const int RolleiCom::XON_XOFF_FLOW			= 1;
const int RolleiCom::HARDWARE_FLOW			= 2;

const int RolleiCom::LIGHT_OFF				= 0;
const int RolleiCom::LIGHT_80				= 1;
const int RolleiCom::LIGHT_90				= 2;
const int RolleiCom::LIGHT_100				= 3;
const int RolleiCom::LIGHT_DISPLAY_LO		= 4;
const int RolleiCom::LIGHT_DISPLAY_HIGH		= 5;

const int RolleiCom::TWIN_DIGITAL_P			= 1;
const int RolleiCom::MSC_300_P				= 2;

const char * _PORT_NO		= "com.port_no";
const char * _BAUDRATE		= "com.baudrate";
const char * _PARITIY_MODE	= "com.parity_mode";
const char * _STOPBITS		= "com.stopbits";
const char * _DATABITS		= "com.databits";
const char * _FLOW_CONTROL	= "com.flow";
const char * _COM_LOGGING	= "com.logging";

#ifdef _WIN32
const int c_iDelay = 15;
#else
const int c_iDelay = 10;
#endif

/* example for ini file settings (NO WHITESPACES !)
com.port_no=2
com.baudrate=9600
com.parity_mode=odd
com.stopbits=1.5
com.databits=6
com.logging=0
*/

RolleiCom::RolleiCom( bool bIgnoreComSettings, bool bSimulation, int iProjectorType, minLoggingInterface * pLoggingChannel, MiniIniDB * pIniDB, int iComPortNo /*, int iBaudrate, int iParityMode, int iStopBits, int iDataBits*/ )
: m_pIniDB( pIniDB ),
  m_pLoggingChannel( pLoggingChannel ),
  m_pData( 0 ),
  m_pCmdProcessor( 0 ),
  m_bIsSimulation( bSimulation ),
  m_bDoLogging( true ),  
  m_bStopGoFlag( false ),
  m_bIgnoreComSettings( bIgnoreComSettings ),
  m_iComPortNo( iComPortNo ),
  m_iProjectorType( iProjectorType ),
  m_pComPortSync( 0 )
{
	m_pComPortSync = new minSyncObject();

	// ** default values for projector type may be overwritten with ini-file values
	SetDefaultValues( m_iProjectorType );
				
	// ** if there is a ini-db available, try to find values in it...
	if( m_pIniDB )
	{
		if( m_pIniDB->HasKey( _PORT_NO ) )
		{
			m_iComPortNo = m_pIniDB->GetValueAsInt( _PORT_NO );
		}
		if( m_pIniDB->HasKey( _BAUDRATE ) )
		{
			m_iBaudrate = m_pIniDB->GetValueAsInt( _BAUDRATE );
		}
		if( m_pIniDB->HasKey( _PARITIY_MODE ) )
		{
			string sValue = m_pIniDB->GetValue( _PARITIY_MODE );

			int iTempValue;
			if( GetParityModeFromStrg( sValue, iTempValue ) )
			{
				m_iParityMode = iTempValue;
			}
		}
		if( m_pIniDB->HasKey( _STOPBITS ) )
		{
			string sValue = m_pIniDB->GetValue( _STOPBITS );

			int iTempValue;
			if( GetStopBitsFromStrg( sValue, iTempValue ) )
			{
				m_iStopBits = iTempValue;
			}
		}
		if( m_pIniDB->HasKey( _DATABITS ) )
		{
			m_iDataBits = m_pIniDB->GetValueAsInt( _DATABITS );
		}
		if( m_pIniDB->HasKey( _FLOW_CONTROL ) )
		{
			m_iFlowMode = m_pIniDB->GetValueAsInt( _FLOW_CONTROL );
		}
		if( m_pIniDB->HasKey( _COM_LOGGING ) )
		{
			m_bDoLogging = (bool)m_pIniDB->GetValueAsInt( _COM_LOGGING );
		}
	}

	Start( m_iComPortNo, m_iBaudrate, m_iParityMode, m_iStopBits, m_iDataBits, m_iFlowMode );
}

RolleiCom::~RolleiCom()
{
	Stop();

	delete m_pComPortSync;
}

void RolleiCom::Start( int iComPortNo, int iBaudrate, int iParityMode, int iStopBits, int iDataBits, int iFlowMode )
{
	// ** save the current settings for access methods
	m_iComPortNo = iComPortNo;
	m_iBaudrate = iBaudrate;
	m_iParityMode = iParityMode;
	m_iStopBits = iStopBits;
	m_iDataBits = iDataBits;
	m_iFlowMode = iFlowMode;

	if( !IsSimulation() )
	{
		m_pData = new RolleiComHelperData( iComPortNo );
		bool bOk = m_pData->SetComData( m_bIgnoreComSettings, iBaudrate, iParityMode, GetStopBitsStrg( iStopBits ), iDataBits, iFlowMode );
		// in case of an error...
		if( !bOk && m_pLoggingChannel && m_bDoLogging )
		{
			m_pLoggingChannel->LogMsg( "Error opening COM-Port !" );
		}
	}
	else
	{
		m_pData = 0;
	}

	// ** start handler for asynchonious communication to projector
	m_pCmdProcessor = new minCmdProcessor( this );
	m_pCmdProcessor->Start();
}

void RolleiCom::Stop()
{
	delete m_pCmdProcessor;
	m_pCmdProcessor = 0;
	if( m_pData )
	{
		delete m_pData;
		m_pData = 0;
	}
}

bool RolleiCom::IsOk() const
{
	return !m_pData || m_pData->IsOk();
}

bool RolleiCom::IsTwinDigitalP() const
{
	return m_iProjectorType == TWIN_DIGITAL_P;
}

bool RolleiCom::IsMSC300P() const
{
	return m_iProjectorType == MSC_300_P;
}

void RolleiCom::SetProjectorType( int iType )
{
	m_iProjectorType = iType;
}

bool RolleiCom::IsSimulation() const
{
	return m_bIsSimulation;
}

void RolleiCom::SetSimulation( bool bSimulation )
{
	m_bIsSimulation = bSimulation;
}

bool RolleiCom::IsLogging() const
{
	return m_bDoLogging;
}

void RolleiCom::SetLogging( bool bLogging )
{
	m_bDoLogging = bLogging;
}

int RolleiCom::GetComPortNo() const
{
	return m_iComPortNo;
}

int RolleiCom::GetBaudrate() const
{
	return m_iBaudrate;
}

int RolleiCom::GetParityMode() const
{
	return m_iParityMode;
}

int RolleiCom::GetStopBits() const
{
	return m_iStopBits;
}

int RolleiCom::GetDataBits() const
{
	return m_iDataBits;
}

int RolleiCom::GetFlowModus() const
{
	return m_iFlowMode;
}

bool RolleiCom::SendCmd( const string & sMsg, bool bExpectReturnValue )
{
	if( IsTwinDigitalP() )
	{
		return SendCmdTwin( sMsg, bExpectReturnValue );
	}
	else if( IsMSC300P() )
	{
		return SendCmdMSC( sMsg, bExpectReturnValue );
	}
	return false;
}

bool RolleiCom::SendCmdTwin( const string & sMsg, bool bExpectReturnValue )
{
	bool bOk = true;

	{
		// Bugfix: 15.2.2003 synchronize access to Com-Port
		// now the synchronious stop-cmd can not interfere with cmd-thread command handling
		minLock aLock( *m_pComPortSync );
			
		// ** send every character of the cmd as a single character
		for( int i=0; i<(int)sMsg.length(); i++ )
		{
			string s;
			s += sMsg[i];
			if( !IsSimulation() )
			{
				bOk = m_pData->Write( s );
				// ** manual says: delay of 10ms after every character
				m_pData->Delay( c_iDelay );
			}
		}
		// ** every command ends with an CR (0x13)
		if( !IsSimulation() )
		{
			string s;
			s += 13;
			bOk = m_pData->Write( s );
			m_pData->Delay( c_iDelay );
		}

		if( m_pLoggingChannel && m_bDoLogging )
		{
			string s = "--> "+sMsg;
			m_pLoggingChannel->LogMsg( s.c_str() );
		}

		// ** check only for return value, if no special return value is expected !
		if( bOk && !bExpectReturnValue )
		{
			string sRet = GetMsg();

			if( (sRet.length()>0) && (sRet[0] == '*') )
			{
				return true;
			}
			else if( (sRet.length()>0) && (sRet[0] == '!') )
			{
				return false;
			}
			else
			{
				//ASSERT
				return false;
			}
		}
	}

	return bOk;
}

bool RolleiCom::SendCmdMSC( const string & sMsg, bool bExpectReturnValue )
{
	bool bOk = true;

	string sEcho;
	string sEchoTemp;

	{
		// Bugfix: 15.2.2003 synchronize access to Com-Port
		// now the synchronious stop-cmd can not interfere with cmd-thread command handling
		minLock aLock( *m_pComPortSync );

		// ** maybe needed: check the projector if he is ready to accept commands
		//bool bReady = CheckReady();

		// ** send every character of the cmd as a single character
		// ** for every character an echo is expected
		for( int i=0; i<(int)sMsg.length(); i++ )
		{
			string s;
			s += sMsg[i];
			if( !IsSimulation() )
			{
				bOk = m_pData->Write( s );

				// ** now wait for the echo from the projector
				/*bOk =*/ ReceiveEcho( s, sEchoTemp );
				sEcho += sEchoTemp;
			}
			else
			{
				sEcho += s;
			}
		}
		// ** every command ends with an CR (0x13)
		if( !IsSimulation() )
		{
			string s;
			s += 13;
			bOk = m_pData->Write( s );

			// ** now wait for the echo from the projector
			/*bOk =*/ ReceiveEcho( s, sEchoTemp );
			sEcho += sEchoTemp;
		}

		if( m_pLoggingChannel && m_bDoLogging )
		{
			string s = "--> "+sMsg;
			m_pLoggingChannel->LogMsg( s.c_str() );
			s = "<-- "+sEcho;
			m_pLoggingChannel->LogMsg( s.c_str() );
		}

		// ** check only for return value, if no special return value is expected !
		if( bOk && !bExpectReturnValue )
		{
			// ** read the final CR and LF from the projector
			string sRet = GetMsg( 2 );

			// ** the msc projector sends no special ok character
			// ** use the echo string to verify the success.
			if( sEcho.substr( 0, sMsg.length() ) == sMsg )
			{
				// ** if echo was received successfully:
				// ** wait synchroniously for the executing command
				return CheckReady();
			}
			else
			{
				return false;
			}
		}
	}

	return bOk;
}

bool RolleiCom::ReceiveEcho( const string & sMsgCharacter, string & sEcho )
{
	bool bOk = true;

	// ** now wait for the echo from the projector
	sEcho = "";
	if( !IsSimulation() )
	{
		// Bugfix: 15.2.2003 synchronize access to Com-Port
		// now the synchronious stop-cmd can not interfere with cmd-thread command handling
		minLock aLock( *m_pComPortSync );

		if( m_pData->Read( sEcho, 1 ) )
		{
			if( !((sEcho.length()>0) && (sEcho[0]==sMsgCharacter[0])) )
			{
				// ** ONLY FOR DEBUGGING... **
				cerr << "WARNING: no or invalid echo for character=" << sMsgCharacter.c_str() << " received !" << endl;

				bOk = false;
			}
		}
		else
		{
			// ** ONLY FOR DEBUGGING... **
			cerr << "WARNING: no error for character=" << sMsgCharacter.c_str() << " received !" << endl;

			bOk = false;
		}

		//m_pData->Delay( c_iDelay );
	}

	return bOk;
}

bool RolleiCom::CheckReady()
{
	char ch = GetStatus();
	
	while( ch=='v' || ch=='B' )
	{
		if( !IsSimulation() )
		{
			m_pData->Delay( 100 );
		}

		// ** ONLY FOR DEBUGGING... **
		cerr << "WARNING: CheckReady() status=\"" << ch << "\"" << endl;

		ch = GetStatus();
	}

	// ** ONLY FOR DEBUGGING... **
	cerr << "WARNING: Finally CheckReady() status=\"" << ch << "\"" << endl;

	return ch=='R';
}

bool RolleiCom::SendMsg( const string & sMsg, bool bExpectReturnValue, bool bSync )
{
	if( bExpectReturnValue || bSync )
	{
		return SendCmd( sMsg, bExpectReturnValue );
	}
	else
	{
		if( true )
		{
			return m_pCmdProcessor->AppendBatchCmd( sMsg );
		}
		else
		{
			return m_pCmdProcessor->ExecuteRealtimeCmd( sMsg );
		}
	}
}

string RolleiCom::GetMsg( int iCount )
{
	string sRet;

	if( !IsSimulation() )
	{
		if( IsTwinDigitalP() )
		{
			m_pData->Delay( c_iDelay*iCount );
		}
		else if( IsMSC300P() )
		{
			// ggf. Status lesen ?
		}
		{
			// Bugfix: 15.2.2003 synchronize access to Com-Port
			// now the synchronious stop-cmd can not interfere with cmd-thread command handling
			minLock aLock( *m_pComPortSync );

			if( m_pData->Read( sRet, iCount ) )
			{
				if( m_pLoggingChannel && m_bDoLogging )
				{
					string s = "<-- "+sRet;
					m_pLoggingChannel->LogMsg( s.c_str() );
				}

				return sRet;
			}
		}
	}
	else
	{
		if( IsTwinDigitalP() )
		{
			sRet = "*\r";

			if( m_pLoggingChannel && m_bDoLogging )
			{
				string s = "<-- "+sRet;
				m_pLoggingChannel->LogMsg( s.c_str() );
			}
		}
		else if( IsMSC300P() )
		{
			sRet = "\r\n";	// ggf. "\r\n" --> CR LF

			if( m_pLoggingChannel && m_bDoLogging )
			{
				string s = "<-- "+sRet;
				m_pLoggingChannel->LogMsg( s.c_str() );
			}
		}
		else
		{
			sRet = "<?>";	// unknown projector type !
		}
		return sRet;
	}

	return "";
}

void RolleiCom::SetLoggingChannel( minLoggingInterface * pLogChannel )
{
	m_pLoggingChannel = pLogChannel;
}


bool RolleiCom::DirectMode( bool bSync )
{
	if( IsTwinDigitalP() )
	{
		return SendMsg( "DM", /*bExpectReturnValue*/false, bSync );
	}
	else if( IsMSC300P() )
	{
		// ** this is the pc modus
		return SendMsg( "PE", /*bExpectReturnValue*/false, bSync );
	}
	return false;
}

bool RolleiCom::MemoryMode( bool bSync )
{
	if( IsTwinDigitalP() )
	{
		return SendMsg( "MM", /*bExpectReturnValue*/false, bSync );
	}
	else if( IsMSC300P() )
	{
		// ** this modus does not exist for msc projectors !?
		// ** functionality is supported in the pc modus !
		return false;
	}
	return false;
}

bool RolleiCom::Reset( bool bSync )
{
	return SendMsg( "RS", /*bExpectReturnValue*/false, bSync );
}

bool RolleiCom::SlideBack( bool bSync )
{
	return SendMsg( "BR", /*bExpectReturnValue*/false, bSync );
}

bool RolleiCom::SlideForward( bool bSync )
{
	return SendMsg( "BV", /*bExpectReturnValue*/false, bSync );
}

bool RolleiCom::StopGo( bool bSync )
{
	m_bStopGoFlag = !m_bStopGoFlag;

	if( IsTwinDigitalP() )
	{
		return SendMsg( "ST", /*bExpectReturnValue*/false, bSync );
	}
	else if( IsMSC300P() )
	{
		// ** the msc projectors have a different stop/go handling 
		if( m_bStopGoFlag )
		{
			return SendMsg( "ST", /*bExpectReturnValue*/false, bSync );
		}
		else
		{
			return SendMsg( "WE", /*bExpectReturnValue*/false, bSync );
		}
	}
	return false;
}

string RolleiCom::GetActSlideNoStrg( bool bSync )
{
	bool bOk = SendMsg( "AB", true, bSync );
	if( bOk )
	{
		string s = GetMsg( 8 );		// ** expect 000 and CR
		//long lNo = atol( s.c_str() );
		//return (int)lNo;
		return s.substr( 0, 3 );
	}
	return "-1";
}

bool RolleiCom::SetAutofocus( bool bValue, bool bSync )
{
	if( bValue )
	{
		return SendMsg( "AE", /*bExpectReturnValue*/false, bSync );
	}
	else
	{
		return SendMsg( "AA", /*bExpectReturnValue*/false, bSync );
	}
}

bool RolleiCom::FocusStepForward( bool bSync )
{
	return SendMsg( "FV", /*bExpectReturnValue*/false, bSync );
}

bool RolleiCom::FocusStepBackward( bool bSync )
{
	return SendMsg( "FR", /*bExpectReturnValue*/false, bSync );
}

string RolleiCom::GetActLineNoStrg( bool bSync )
{
	bool bOk = SendMsg( "AZ", true, bSync );
	if( bOk )
	{
		string s = GetMsg( 8 );		// ** expect 000 and CR
		return s.substr( 0, 3 );
	}
	return "-1";
}

bool RolleiCom::SetDissolveTime( int iTimeInTenthOfASecond, bool bSync )
{
	if( (iTimeInTenthOfASecond>=0) && (iTimeInTenthOfASecond<=999) )
	{
		/*
		if( iTimeInTenthOfASecond<5 )
		{
			iTimeInTenthOfASecond = 5;		// ** minimum dissolve time **
		}
		*/
		char sBuffer[255];
		sprintf( sBuffer, "SD:%03d", iTimeInTenthOfASecond );
		string sCmd( sBuffer );
		return SendMsg( sCmd, /*bExpectReturnValue*/false, bSync );
	}
	return false;
}

bool RolleiCom::SetLight( int iLightValue, bool bSync  )
{
	if( (iLightValue>=LIGHT_OFF) && (iLightValue<=LIGHT_DISPLAY_HIGH) )
	{
		char sBuffer[255];

		if( IsTwinDigitalP() )
		{
			sprintf( sBuffer, "SL:%03d", iLightValue );
			string sCmd( sBuffer );
			return SendMsg( sCmd, /*bExpectReturnValue*/false, bSync );
		}
		else if( IsMSC300P() )
		{
			// ** calculate the needed value (0..255)
			if( iLightValue == LIGHT_OFF )
			{
				iLightValue = 0;
			}
			else if( iLightValue == LIGHT_80 )
			{
				iLightValue = 204;	// 0.8 * 255
			}
			else if( iLightValue == LIGHT_90 )
			{
				iLightValue = 229;	// 0.9 * 255
			}
			else if( iLightValue == LIGHT_100 )
			{
				iLightValue = 255;
			}
			else if( iLightValue == LIGHT_DISPLAY_LO )
			{
				iLightValue = 128;
			}
			else if( iLightValue == LIGHT_DISPLAY_HIGH )
			{
				iLightValue = 255;
			}
			sprintf( sBuffer, "SL:%03d", iLightValue );
			string sCmd( sBuffer );
			return SendMsg( sCmd, /*bExpectReturnValue*/false, bSync );
		}		
	}
	return false;
}

bool RolleiCom::GotoPosition( int iPosition )
{
	if( (iPosition>0) && (iPosition<=50) )
	{
		// ** reset projector and go to direct mode
		Reset();
		DirectMode();

		// ** goto the first slide (001) and wait until it is reached !
		// ** projector is slow but fast with the feedback !
		FocusStepForward();
// min todo gulp: hier ggf. robustere Behandlung, z.B. auf < ueberpruefen...
		while( (GetActSlideNoStrg() != "001") && !IsSimulation() )
		{
			m_pData->Delay( 200 );
		}

		// ** now to to the selected position
		for( int i=1; i<=iPosition; i++ )
		{
			FocusStepForward();
		}

		return true;
	}
	return false;
}

char RolleiCom::GetStatus( bool /*bSync*/ )
{
	// ** only the msc projectors support status informations !
	if( IsMSC300P() )
	{
		if( !IsSimulation())
		{
			if( m_pLoggingChannel && m_bDoLogging )
			{
				string s = "--> �";
				m_pLoggingChannel->LogMsg( s.c_str() );
			}

			{
				// Bugfix: 15.2.2003 synchronize access to Com-Port
				// now the synchronious stop-cmd can not interfere with cmd-thread command handling
				minLock aLock( *m_pComPortSync );

				bool bOk = m_pData->Write( "�" );
				if( bOk )
				{
					string sRet;

					bOk = m_pData->Read( sRet, 1 );

					if( m_pLoggingChannel && m_bDoLogging )
					{
						string s = "<-- "+sRet;
						m_pLoggingChannel->LogMsg( s.c_str() );
					}

					if( sRet.length()>0 )
					{
						return sRet[0];
					}
				}
			}
		}
		else
		{
			if( m_pLoggingChannel && m_bDoLogging )
			{
				string s = "--> �";
				m_pLoggingChannel->LogMsg( s.c_str() );
				s = "<-- R";
				m_pLoggingChannel->LogMsg( s.c_str() );
			}

			return 'R';
		}
	}
	return '?';		// no status avaliable !
}

string RolleiCom::GetStatusStrg( char ch )
{
	string sRet;

	switch( ch )
	{
		case 'v':
			sRet = "cmd accepted";
			break;
		case 'B':
			sRet = "busy";
			break;
		case 'R':
			sRet = "ready";
			break;
		case 'g':
			sRet = "not in pc mode";
			break;
		case 'i':
			sRet = "unknown cmd";
			break;
		case 'e':
			sRet = "error";
			break;
		case 'p':
			sRet = "bad parameter";
			break;
		case 'j':
			sRet = "cmd processing";
			break;
		case '?':		// internal signal: not implemented
			sRet = "<not implemented>";
			break;
		default:
			sRet = "<UNKOWN>";
	}

	return sRet;
}

bool RolleiCom::IsProjectorConnected()
{
	// ** try to send a simple command to the projector...
	return SetAutofocus( true );
}

bool RolleiCom::IsKeyboardMode()
{
	return !IsDirectMode();
}

bool RolleiCom::IsDirectMode()
{
	// ** is only allowed in Direct -Mode
	return SetDissolveTime( 25 );
}

string RolleiCom::GetParityModeStrg( int iParityMode ) const
{
	switch( iParityMode )
	{
		case NONE:
			return "none";
		case EVEN:
			return "even";
		case ODD:
			return "odd";

		default:
			return "";
	}
}

string RolleiCom::GetStopBitsStrg( int iStopBits ) const
{
	switch( iStopBits )
	{
		case ONE:
			return "1";
		case TWO:
			return "2";
		case ONE_POINT_FIVE:
			return "1.5";

		default:
			return "";
	}
}

bool RolleiCom::GetParityModeFromStrg( const string & s, int & iParityModeOut ) const
{
	if( s == GetParityModeStrg( NONE ) )
	{
		iParityModeOut = NONE;
	}
	else if( s == GetParityModeStrg( EVEN ) )
	{
		iParityModeOut = EVEN;
	}
	else if( s == GetParityModeStrg( ODD ) )
	{
		iParityModeOut = ODD;
	}
	else
	{
		return false;
	}

	return true;
}

bool RolleiCom::GetStopBitsFromStrg( const string & s, int & iStopBitsOut ) const
{
	if( s == GetStopBitsStrg( ONE ) )
	{
		iStopBitsOut = ONE;
	}
	else if( s == GetStopBitsStrg( TWO ) )
	{
		iStopBitsOut = TWO;
	}
	else if( s == GetStopBitsStrg( ONE_POINT_FIVE ) )
	{
		iStopBitsOut = ONE_POINT_FIVE;
	}
	else
	{
		return false;
	}

	return true;
}

void RolleiCom::SetDefaultValues( int iProjectorType )
{
	if( iProjectorType == TWIN_DIGITAL_P )
	{
		m_iBaudrate = 1200;
		m_iParityMode = EVEN;
		m_iStopBits = TWO;
		m_iDataBits = 7;
		m_iFlowMode = HARDWARE_FLOW;
	}
	else if( iProjectorType == MSC_300_P )
	{
		m_iBaudrate = 9600;
		m_iParityMode = NONE;
		m_iStopBits = ONE;
		m_iDataBits = 8;
		m_iFlowMode = NO_FLOW;
	}
}

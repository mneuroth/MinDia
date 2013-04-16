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
 * Copyright (C) 2012 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "rolleicom.h"

#include "mincmdproc.h"
#include "minlog.h"
#include "misctools.h"

#include <stdio.h>
#include <string.h>

#include <iostream>
using namespace std;

#include <QSettings>
#include <QThread>

class MySleep : public QThread
{
public:
    static void msleep(int ms)
    {
        QThread::msleep(ms);
    }
};

#define BUFFER_MAX 255

// *******************************************************************
// *******************************************************************
// *******************************************************************

#if !defined(Q_OS_WIN32)
#define _with_qextserial
#endif

#ifdef _with_qextserial

#include "../qextserialport/src/qextserialport.h"

#define MAX_READ_TIMEOUT    50  /*ms*/

// command should end with CR LF ! --> anything with ending CR LF is a command
static bool IsCommandReceived(const string & sMsgOut)
{
    if( sMsgOut.length()>1 )
    {
        char ch1 = sMsgOut[ sMsgOut.length()-1 ];
        char ch2 = sMsgOut[ sMsgOut.length()-2 ];
        return ((ch2=='\n') && (ch1=='\r'));     // slash n == 10; slash r == 13
    }
    return false;
}

/** OS depending helper data */
struct RolleiComHelperData
{
    RolleiComHelperData( const string & sComPort )
    {
        Open( sComPort );
    }
    ~RolleiComHelperData()
    {
        Close();
    }

    bool Open( const string & sComPort )
    {
        m_pPort = new QextSerialPort(sComPort.c_str());
        m_pPort->open(QIODevice::ReadWrite);
        return IsOk();
    }
    bool Close()
    {
        m_pPort->close();
        delete m_pPort;
        m_pPort = 0;
        return true;
    }

    bool SetComData( bool bIgnoreComSettings, int iBaudrate, int iParityMode, const string & sStopBitsStrg, int iDataBits, int iFlowMode )
    {
        if( !bIgnoreComSettings )
        {
            BaudRateType aBaudRate;

            switch( iBaudrate )
            {
                case 110:
                    aBaudRate = BAUD110;
                    break;
                case 300:
                    aBaudRate = BAUD300;
                    break;
                case 1200:
                    aBaudRate = BAUD1200;
                    break;
                case 2400:
                    aBaudRate = BAUD2400;
                    break;
                case 4800:
                    aBaudRate = BAUD4800;
                    break;
                case 9600:
                    aBaudRate = BAUD9600;
                    break;
                case 19200:
                    aBaudRate = BAUD19200;
                    break;
                case 38400:
                    aBaudRate = BAUD38400;
                    break;
                case 57600:
                    aBaudRate = BAUD57600;
                    break;
                case 115200:
                    aBaudRate = BAUD115200;
                    break;
                default:
                    aBaudRate = BAUD1200;
            }

            DataBitsType aDataBits;

            switch( iDataBits )
            {
                case 6:
                    aDataBits = DATA_6;
                    break;
                case 7:
                    aDataBits = DATA_7;
                    break;
                case 8:
                    aDataBits = DATA_8;
                    break;
                default:
                    aDataBits = DATA_7;
            }

            ParityType aParity = PAR_EVEN;

            if( iParityMode == RolleiCom::NONE )
            {
                aParity = PAR_NONE;
            }
            else if( iParityMode == RolleiCom::ODD )
            {
                aParity = PAR_ODD;
            }
            else if( iParityMode == RolleiCom::EVEN )
            {
                aParity = PAR_EVEN;
            }

            FlowType aFlowType = FLOW_OFF;

            if( iFlowMode == RolleiCom::NO_FLOW )
            {
                aFlowType = FLOW_OFF;
            }
            if( iFlowMode == RolleiCom::HARDWARE_FLOW )
            {
                aFlowType = FLOW_HARDWARE;
            }
            if( iFlowMode == RolleiCom::XON_XOFF_FLOW )
            {
                aFlowType = FLOW_XONXOFF;
            }

            StopBitsType aStopBits = STOP_2;

            if( sStopBitsStrg=="1" )
            {
                aStopBits = STOP_1;
            }
            if( sStopBitsStrg=="1.5" )
            {
#if defined(Q_OS_WIN) || defined(qdoc)
                aStopBits = STOP_1_5;
#endif
            }
            if( sStopBitsStrg=="2" )
            {
                aStopBits = STOP_2;
            }

            m_pPort->setBaudRate(aBaudRate);
            m_pPort->setDataBits(aDataBits);
            m_pPort->setParity(aParity);
            m_pPort->setFlowControl(aFlowType);
            m_pPort->setStopBits(aStopBits);
            m_pPort->setTimeout(1000);
        }
        return true;
    }

    bool IsOk() const
    {
        return ( m_pPort && m_pPort->isOpen() );
    }

    bool Write( const string & sMsg )
    {
        if( IsOk() )
        {
            int iRet = m_pPort->write(sMsg.c_str());
            return iRet!=0;
//            int iRet = write( m_hFile, sMsg.c_str(), sMsg.length() );
//            /* in case of an error... */
//            if( iRet == 0 )
//            {
//                m_iLastError = errno;
//            }
//            //dbg: cout << "write: " << sMsg.c_str() << " wcount=" << iRet << endl;
//            return (iRet != 0);
        }
        return false;
    }

    bool Read( string & sMsgOut/*, int iCount*/ )
    {
        if( IsOk() )
        {
            char sBuffer[BUFFER_MAX];

//            // ** check if there is any response on the com-port --> timeout !
//            fd_set aReadfs;    /* file descriptor set */
//            FD_ZERO(&aReadfs);
//            FD_SET( m_hFile, &aReadfs );  /* set testing for source 1 */

//            // hier ggf. optimierter timeout, z.b. beim Testen kleiner als waehrend der Praesentation !?
//            struct timeval aTimeout;
//            aTimeout.tv_usec = 0;  /* milliseconds */
//            aTimeout.tv_sec  = 20;  /* seconds */

//            // wait for first character which could be read...
//            int iRes = select( m_hFile+1, &aReadfs, NULL, NULL, &aTimeout );
//            if( iRes==0 )
//            {
//                /* number of file descriptors with input = 0, timeout occurred. */
//                return false;
//            }

//            // a maximum nuber of 000<CR><LF> with delay of 10ms
//            //Delay(50);      // give a little bit time to read all characters...
//            // this is not needed since new handling of command reading below

            // new since 17.1.2011: try to read one command (ending with CR LF) or timeout when trying to read more characters
            int iReadTimeout = 0;
            while( !IsCommandReceived(sMsgOut) && /*NotTimeoutSinceLastSuccessfullRead==*/iReadTimeout<MAX_READ_TIMEOUT )
            {
                // ** if we are here, there is something to read !
//                int iRet = read( m_hFile, sBuffer, BUFFER_MAX );
                int iRet = m_pPort->read(sBuffer,BUFFER_MAX);
//                int iRet = m_pPort->readLine(sBuffer,BUFFER_MAX);
                if( iRet>0 )
                {
                    sBuffer[ iRet ] = 0;
                    //dbg: cout << "read: " << (int)sBuffer[0] << " == " << sBuffer << " rcount=" << iRet << endl;
                    sMsgOut += sBuffer;
                    // reset timeout, because we read something !
                    iReadTimeout = 0;


                }
                else
                {
                    iReadTimeout += 10;
                    Delay(10);
                    //old: m_iLastError = errno;
                }
            }
            return IsCommandReceived(sMsgOut);
        }
        return false;
    }

    void Delay( int iTimeInMS )
    {
        MySleep::msleep( iTimeInMS );
    }

    int GetLastErrorCode() const
    {
        return -1;
    }

    QextSerialPort * m_pPort;
};

#endif //_with_qextserial

// *******************************************************************
// *******************************************************************
// *******************************************************************

// temp. constants because of Borland C++ 5.5 
const int _NONE = 0;
const int _ODD = 1;
const int _EVEN = 2;
const int _ONE = 0;
const int _ONE_POINT_FIVE = 1;
const int _TWO = 2;

const int RolleiCom::NONE					= _NONE;
const int RolleiCom::ODD					= _ODD;
const int RolleiCom::EVEN					= _EVEN;

const int RolleiCom::ONE					= _ONE;
const int RolleiCom::ONE_POINT_FIVE			= _ONE_POINT_FIVE;
const int RolleiCom::TWO					= _TWO;

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

#ifdef _WIN32
const int c_iDelay = 15;        /* TODO check for Windows !!! */
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

RolleiCom::RolleiCom( bool bIgnoreComSettings, bool bSimulation, int iProjectorType, minLoggingInterface * pLoggingChannel )
: m_pLoggingChannel( pLoggingChannel ),
  m_pData( 0 ),
  m_pCmdProcessor( 0 ),
  m_bIsSimulation( bSimulation ),
  m_bDoLogging( true ),
  m_bStopGoFlag( false ),
  m_bIgnoreComSettings( bIgnoreComSettings ),
  m_sComPort(""),
  m_iProjectorType( iProjectorType ),
  m_bIsPcMode(false)
{
	// ** default values for projector type may be overwritten with ini-file values
	SetDefaultValues( m_iProjectorType );

    RestoreSettings();
    
    Start( m_sComPort, m_iBaudrate, m_iParityMode, m_iStopBits, m_iDataBits, m_iFlowMode );
}

RolleiCom::~RolleiCom()
{
	Stop();

    SaveSettings();
}

void RolleiCom::SaveSettings()
{
    QSettings aSettings;
    
    aSettings.setValue("RolleiCom/ComPort",m_sComPort.c_str());
    aSettings.setValue("RolleiCom/BaudRate",m_iBaudrate);
    aSettings.setValue("RolleiCom/ParityMode",GetParityModeStrg( m_iParityMode ).c_str());
    aSettings.setValue("RolleiCom/StopBits",GetStopBitsStrg( m_iStopBits ).c_str());
    aSettings.setValue("RolleiCom/DataBits",m_iDataBits);
    aSettings.setValue("RolleiCom/FlowControl",m_iFlowMode);
    aSettings.setValue("RolleiCom/Logging",(int)m_bDoLogging);
}

void RolleiCom::RestoreSettings()
{
    QSettings aSettings;
    QString sTemp;
    int iTempValue;
   
    m_sComPort = ToStdString(aSettings.value("RolleiCom/ComPort",QString()).toString());
    m_iBaudrate = aSettings.value("RolleiCom/BaudRate",9600).toInt();
    sTemp = aSettings.value("RolleiCom/ParityMode",QString()).toString();
    if( GetParityModeFromStrg( (const char*)sTemp.toAscii(), iTempValue ) )
	{
		m_iParityMode = iTempValue;
	}
    sTemp = aSettings.value("RolleiCom/StopBits",QString()).toString();
    if( GetStopBitsFromStrg( (const char*)sTemp.toAscii(), iTempValue ) )
	{
		m_iStopBits = iTempValue;
	}
    m_iDataBits = aSettings.value("RolleiCom/DataBits",8).toInt();
    m_iFlowMode = aSettings.value("RolleiCom/FlowControl",0).toInt();
    m_bDoLogging = (bool)aSettings.value("RolleiCom/Logging",0).toInt();
}

void RolleiCom::Start( const string & sComPort, int iBaudrate, int iParityMode, int iStopBits, int iDataBits, int iFlowMode )
{
	// ** save the current settings for access methods
    m_sComPort = sComPort;
	m_iBaudrate = iBaudrate;
	m_iParityMode = iParityMode;
	m_iStopBits = iStopBits;
	m_iDataBits = iDataBits;
	m_iFlowMode = iFlowMode;

	UpdateComPort();

	// ** start handler for asynchonious communication to projector
	m_pCmdProcessor = new minCmdProcessor( this );
	m_pCmdProcessor->Start();
}

void RolleiCom::UpdateComPort()
{
	if( m_pData )
	{
		delete m_pData;
		m_pData = 0;
	}

	if( !IsSimulation() )
	{
        m_pData = new RolleiComHelperData( m_sComPort );
        bool bOk = m_pData->SetComData(m_bIgnoreComSettings, m_iBaudrate, m_iParityMode, GetStopBitsStrg( m_iStopBits ), m_iDataBits, m_iFlowMode );
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

int RolleiCom::GetProjectorType() const
{
	return m_iProjectorType;
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

	UpdateComPort();
}

bool RolleiCom::IsLogging() const
{
	return m_bDoLogging;
}

void RolleiCom::SetLogging( bool bLogging )
{
	m_bDoLogging = bLogging;
}

string RolleiCom::GetComPort() const
{
    return m_sComPort;
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
        QMutexLocker aLock( &m_aComPortSync );

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
        QMutexLocker aLock( &m_aComPortSync );

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
			// osc 2003-08-29 begin: echo is already there
			//string sRet = GetMsg( 2 );
			// osc 2003-08-29 end

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
        QMutexLocker aLock( &m_aComPortSync );

        if( m_pData->Read( sEcho ) )
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
            QMutexLocker aLock( &m_aComPortSync );

            if( m_pData->Read( sRet ) )
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
		m_bIsPcMode=true;
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
	bool bStatus = SendMsg( "RS", /*bExpectReturnValue*/false, bSync );
	m_bIsPcMode=false;
	return bStatus;
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
			// osc 2003-08-29 begin
			//sprintf( sBuffer, "SL:%03d", iLightValue );
			//string sCmd( sBuffer );
			//return SendMsg( sCmd, /*bExpectReturnValue*/false, bSync );

			sprintf( sBuffer, "LD1:%03d", iLightValue );
			string sCmd( sBuffer );
			bool bStatus = SendMsg( sCmd, /*bExpectReturnValue*/false, bSync );
			if (bStatus)
			{
				sprintf( sBuffer, "LD2:%03d", iLightValue );
				string sCmd( sBuffer );
				bStatus = SendMsg( sCmd, /*bExpectReturnValue*/false, bSync );
			}
			return bStatus;
			// osc 2003-08-29 end
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
		// min todo: hier ggf. robustere Behandlung, z.B. auf < ueberpruefen...
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
// TODO encoding ?
				string s = "--> �";
				m_pLoggingChannel->LogMsg( s.c_str() );
			}

			{
				// Bugfix: 15.2.2003 synchronize access to Com-Port
				// now the synchronious stop-cmd can not interfere with cmd-thread command handling
                QMutexLocker aLock( &m_aComPortSync );

				// osc 2003-08-29 begin: fix character set problems
				//bool bOk = m_pData->Write( "�" );
				string sStatusCmd ;
				sStatusCmd += 225;
				bool bOk = m_pData->Write( sStatusCmd );
				// osc 2003-08-29 end
				if( bOk )
				{
					string sRet;

                    bOk = m_pData->Read( sRet );

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
	if( IsMSC300P() )
	{
		return CheckReady();
	}
	else
	{
		return SetAutofocus( true );
	}
}

bool RolleiCom::IsKeyboardMode()
{
	return !IsDirectMode();
}

bool RolleiCom::IsDirectMode()
{
	if( IsMSC300P() )
	{
		return m_bIsPcMode;
	}
	else
	{
		// ** is only allowed in Direct -Mode
		return SetDissolveTime( 25 );
	}
}

string RolleiCom::GetParityModeStrg( int iParityMode ) const
{
	switch( iParityMode )
	{
		case _NONE:
			return "none";
		case _EVEN:
			return "even";
		case _ODD:
			return "odd";

		default:
			return "";
	}
}

string RolleiCom::GetStopBitsStrg( int iStopBits ) const
{
	switch( iStopBits )
	{
		case _ONE:
			return "1";
		case _TWO:
			return "2";
		case _ONE_POINT_FIVE:
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

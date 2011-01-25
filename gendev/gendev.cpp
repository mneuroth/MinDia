/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002-2003 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/gendev/gendev.cpp,v $
 *
 *  $Revision: 1.1 $
 *
 *	$Log: not supported by cvs2svn $
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2002-2003 by Michael Neuroth.							   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#define _with_service

#include "dllbase.h"
#include "minbase.h"
#include "minhandle.h"
//#include "minutils.h"

#include "igendev.h"

// *************************************************************************

IGeneralDevice::IGeneralDevice( const string & sModuleName )
: minService( g_IGeneralDeviceID, sModuleName )
{}

IGeneralDevice::~IGeneralDevice()
{
}

// *************************************************************************

class IGeneralDeviceImpl : public IGeneralDevice
{
public:
	IGeneralDeviceImpl( const string & sModuleName, minDll * pDll );
	virtual ~IGeneralDeviceImpl();

	virtual bool IsOk() const;

	// ** get the number of devices to handle
	virtual int GetDeviceCount();

	// ** this method should implement a configuration dialog for a single device
	//virtual bool GetConfigureData( int iDeviceNo );

	// ** get the status of the device
	virtual int  GetDeviceStatus( int iDeviceNo );
	// ** set the configuration data for device iDeviceNo
	//virtual bool SetDeviceConfigureData( int iDeviceNo );

	// ** opens the device
	virtual bool OpenDevice( int iDeviceNo );
	// ** closes the device
	virtual bool CloseDevice( int iDeviceNo );

	// ** move to the slide iSlideNo on the device iDeviceNo
	virtual bool SetNextSlide( int iDeviceNo, int iSlideNo );

	// ** set the dissolve time (in ms) for the next slide change for device iDeviceNo
	virtual bool SetDissolveTime( int iDeviceNo, double dDissolveTime );
	// ** change the slide for the device iDeviceNo now !
	virtual bool SlideForward( int iDeviceNo );

	// ** the slide show starts now (or again after a pause)
	virtual bool Start();
	// ** the slide show is suspended
	virtual bool Pause();
	// ** the slide show is stoped
	virtual bool Stop();

private:
	bool Init();
	bool Exit();
};

// *************************************************************************

IGeneralDeviceImpl::IGeneralDeviceImpl( const string & sModuleName, minDll * pDll )
: IGeneralDevice( sModuleName )
{
	Init();
}

IGeneralDeviceImpl::~IGeneralDeviceImpl()
{
	Exit();
}

bool IGeneralDeviceImpl::Init()
{
	bool bOk = true;

	for( int i=0; i<GetDeviceCount(); i++ )
	{
		bOk = bOk && OpenDevice( i );
	}
	
	return bOk;
}

bool IGeneralDeviceImpl::Exit()
{
	bool bOk = true;

	for( int i=GetDeviceCount()-1; i>=0; i-- )
	{
		bOk = bOk && CloseDevice( i );
	}
	
	return bOk;
}


bool IGeneralDeviceImpl::IsOk() const
{
	return true;
}

int IGeneralDeviceImpl::GetDeviceCount()
{
	return 2;
}

//bool IGeneralDeviceImpl::GetConfigureData( int iDeviceNo )
//{
//	aConfigureDataInOut.Clear();
//
//	return false;
//}

bool IGeneralDeviceImpl::OpenDevice( int iDeviceNo )
{
	cout << "OpenDevice() i=" << iDeviceNo << endl;
	
	return true;
}

bool IGeneralDeviceImpl::CloseDevice( int iDeviceNo )
{
	cout << "CloseDevice() i=" << iDeviceNo << endl;

	return true;
}

int IGeneralDeviceImpl::GetDeviceStatus( int iDeviceNo )
{
	cout << "GetDeviceStatus() i=" << iDeviceNo << endl;

	return 0;
}

//bool IGeneralDeviceImpl::SetDeviceConfigureData( int iDeviceNo )
//{
//	return false;
//}

bool IGeneralDeviceImpl::SetNextSlide( int iDeviceNo, int iSlideNo )
{
	cout << "SeNextSlide() i=" << iDeviceNo << " slide-no=" << iSlideNo << endl;

	return true;
}

bool IGeneralDeviceImpl::SetDissolveTime( int iDeviceNo, double dDissolveTime )
{
	cout << "SetDissolveTime() i=" << iDeviceNo << " dissolve=" << dDissolveTime << endl;

	return true;
}

bool IGeneralDeviceImpl::SlideForward( int iDeviceNo )
{
	cout << "SlideForward() i=" << iDeviceNo << endl;

	return true;
}

bool IGeneralDeviceImpl::Start()
{
	cout << "Start()" << endl;

	return true;
}

bool IGeneralDeviceImpl::Pause()
{
	cout << "Pause()" << endl;

	return true;
}

bool IGeneralDeviceImpl::Stop()
{
	cout << "Stop()" << endl;

	return true;
}

// *************************************************************************
// *************************************************************************
// *************************************************************************

#define	_DLL_NAME				"gendev"

static int						g_nModulInitCounter	= 0;

#ifdef _with_service
static IGeneralDeviceImpl *		g_pGenDeviceModule = 0;
#endif

extern "C" int MINDLLEXPORT minModuleInit( void * pData, void * pDllData )
{
	//cout << "Init DLL called ! data=" << pData << endl;

	// Modul soll NICHT mehrfach geladen werden !!!
	if( g_nModulInitCounter>0 )
	{
		return -1;
	}

	g_nModulInitCounter++;

	// Daten in entsprechende Klassen casten
	minDll * pDll = (minDll *)pDllData;
	minServiceManager * pSrvManager = (minServiceManager *)pData;

#ifdef _with_service
	g_pGenDeviceModule = new IGeneralDeviceImpl( _DLL_NAME, pDll );

	// Service-Objekte beim Service-Manager registrieren
	if( pSrvManager )
	{
		pSrvManager->RegisterService( g_pGenDeviceModule, /*bOwnerIn*/false );
	}
#endif

	return 0;
}

extern "C" int MINDLLEXPORT minModuleExit( void * pData )
{
	//cout << "Exit DLL called ! data=" << pData << endl;

	// Service-Objekte beim Service-Manager abmelden (optional,
	// da beim Loeschen des Services automatisch abgemeldet wird)

//	return 1;	// Funktionen am Interpreter registriert, d.h. darf nicht freigegeben werden !

#ifdef _with_service
	minServiceManager * pSrvManager = (minServiceManager *)pData;
	if( pSrvManager )
	{
		pSrvManager->UnRegisterService( g_pGenDeviceModule );
	}

	// Service-Objekte wieder loeschen, dabei wird der Service automatisch
	// beim Service-Manager abgemeldet ! (umgekehrte Reihenfolge)
	delete g_pGenDeviceModule;
	g_pGenDeviceModule = 0;

#endif

	return 0;
}



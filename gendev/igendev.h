/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002-2003 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/gendev/igendev.h,v $
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

#ifndef _IGENDEV_H
#define _IGENDEV_H

class MiniIniDB;

const long g_IGeneralDeviceID			= 700;

// *************************************************************************
// general device interface, this has to be implemented by
// real projectors to support the MinDia program.
class IGeneralDevice : public minService
{
public:
	IGeneralDevice( const string & sModuleName );
    virtual ~IGeneralDevice();

	virtual bool IsOk() const = 0;

	// ** get the number of devices to handle
	virtual int GetDeviceCount() = 0;

	// ** this method should implement a configuration dialog for a single device
	virtual bool GetConfigureData( int iDeviceNo, MiniIniDB & aConfigureDataInOut ) = 0;

	// ** get the status of the device
	virtual int  GetDeviceStatus( int iDeviceNo ) = 0;
	// ** set the configuration data for device iDeviceNo
	virtual bool SetDeviceConfigureData( int iDeviceNo, const MiniIniDB & aConfigureData ) = 0;

	// ** opens the device
	virtual bool OpenDevice( int iDeviceNo ) = 0;
	// ** closes the device
	virtual bool CloseDevice( int iDeviceNo ) = 0;

	// ** move to the slide iSlideNo on the device iDeviceNo (for: start show from position)
	virtual bool SetNextSlide( int iDeviceNo, int iSlideNo ) = 0;

	// ** set the dissolve time (in seconds) for the next slide change for device iDeviceNo
	virtual bool SetDissolveTime( int iDeviceNo, double dDissolveTime ) = 0;
	// ** change the slide for the device iDeviceNo now !
	virtual bool SlideForward( int iDeviceNo ) = 0;

	// ** the slide show starts now (or again after a pause)
	virtual bool Start() = 0;
	// ** the slide show is suspended
	virtual bool Pause() = 0;
	// ** the slide show is stoped
	virtual bool Stop() = 0;
};

#endif

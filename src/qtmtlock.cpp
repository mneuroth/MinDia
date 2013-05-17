/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2013 by Michael Neuroth
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

#include "qtmtlock.h"

#include <qapplication.h>

// *******************************************************************

// ** this function is defined in main.cpp */
QApplication * GetApplication();

// *******************************************************************
// *******************************************************************
// *******************************************************************

QtMTLock::QtMTLock()
{
	QApplication * pAppl = GetApplication();

	if( pAppl )
	{
// TODO porting		pAppl->lock();
	}
}

QtMTLock::~QtMTLock()
{
	QApplication * pAppl = GetApplication();

	if( pAppl )
	{
// TODO porting		pAppl->unlock();
	}
}

void QtMTLock::Set()
{
}

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

#ifndef _QTMTLOCK_H
#define _QTMTLOCK_H

class QtMTLock
{
public:
	QtMTLock();
	~QtMTLock();

	void Set();
};

#endif
	

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
 * Copyright (C) 2013 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#ifndef _MINEXCEPION_H
#define _MINEXCEPION_H

#include <string>

using namespace std;

/** class to represent a exception
  */
class MinException
{
public:
	MinException( const string sMsg )
	{
		m_sMsg = sMsg;
	}

	string GetMsg() const	{ return m_sMsg; }

private:
	string	m_sMsg;
};

#endif

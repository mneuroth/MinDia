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

#ifndef _DIAPROJECTOR_H
#define _DIAPROJECTOR_H

#include "diainfo.h"

#include "minhandle.h"		// for the smart-ptr
#include "writexml.h"

#include <string>
#include <vector>

using namespace std;

// *******************************************************************
/** Represents one dia-projector.
  */
class DiaProjector
{
public:
	DiaProjector( const string & sName = "Projector" );

	XmlTree	GetXMLTree() const;

	bool	Read( istream & aStream );
	bool	Write( ostream & aStream ) const;

private:
	// *** data ***
	string				m_sName;
};

#endif

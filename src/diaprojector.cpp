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

#include "diaprojector.h"

#include "minutils.h"

// *******************************************************************
// *******************************************************************
// *******************************************************************

DiaProjector::DiaProjector( const string & sName )
{
	m_sName = sName;
}

XmlTree	DiaProjector::GetXMLTree() const
{
	XmlTree aTree( "diaprojector" );

	aTree.PushTag( "name", m_sName );

	return aTree;
}

bool DiaProjector::Read( istream & aStream )
{
	FileUtilityObj aFU;

	aFU.ReadStructBegin( aStream );
	ReadString( aStream, m_sName );
	aFU.ReadStructEnd( aStream );

	return aStream.good();
}

bool DiaProjector::Write( ostream & aStream ) const
{
	FileUtilityObj aFU;

	aFU.WriteStructBegin( aStream );
	WriteString( aStream, m_sName );
	aFU.WriteStructEnd( aStream );

	return aStream.good();
}


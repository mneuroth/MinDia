/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/diacallback.h,v $
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

#ifndef _DIACALLBACK_H
#define _DIACALLBACK_H

#include <string>

using namespace std;

// *******************************************************************
/** Represents a callback class, so that the
    class DiaPresentation does not need to be
	a QObject.
  */
class DiaCallback
{
public:
    virtual ~DiaCallback() {}

	// ** Methods used in playing modus **
    virtual void TriggerDissolveActDiaNo( int iNo, const string & sScript, const string & sFileName, double dDissolveTime ) = 0;
    virtual void TriggerShowActDiaNo( int iNo, const string & sScript, const string & sFileName, double dShowTime ) = 0;
    virtual void TriggerSetNextDiaNo( int iNextNo, const string & sNextFileName ) = 0;
	// ** other methods **
	virtual void PresentationModusChanged() = 0;
    virtual void ShowError( const string & sMsg ) = 0;
};

#endif

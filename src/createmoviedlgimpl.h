/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/createmoviedlgimpl.h,v $
 *
 *  $Revision: 1.1 $
 *
 *	$Log: not supported by cvs2svn $
 *	
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

#ifndef _CREATEMOVIEDLGIMPL_H
#define _CREATEMOVIEDLGIMPL_H

#include "CreateMovieDlg.h"

// *******************************************************************
/** Implementation of the Create-Movie-Dialog.
  */
class CreateMovieDlgImpl : public CreateMovieDlg
{
	Q_OBJECT

public:
    CreateMovieDlgImpl( double dTotalTimeMS, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	virtual ~CreateMovieDlgImpl();

public slots:
    virtual void sltSelectOutputDirectory();
    virtual void sltImageRatioSelected( const QString &);

private:
	double	m_dTotalTimeMS;
};

#endif

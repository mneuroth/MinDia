/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/dyntextdlgimpl.h,v $
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
 * Copyright (C) 2002-2003 by Michael Neuroth.							   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#ifndef _DYNTEXTDLGIMPL_H
#define _DYNTEXTDLGIMPL_H

#include "DynTextDlg.h"

#include <qfont.h>

class DynamicTextDlgImpl : public DynamicTextDlg
{
	Q_OBJECT

public:
    DynamicTextDlgImpl( const QFont & aInitFont, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );

	QFont GetFont() const;

public slots:
	virtual void sltDeleteText();
	virtual void sltSelectFont();
    virtual void sltSelectFontcolor();

private:
	QFont	m_aInitFont;
};

#endif

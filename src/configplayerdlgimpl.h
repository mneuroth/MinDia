/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/configplayerdlgimpl.h,v $
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

#ifndef _CONFIGPLAYERDLGIMPL_H
#define _CONFIGPLAYERDLGIMPL_H

#ifdef ZAURUS
#include "ZConfigPlayerDlg.h"
#else
#include "ConfigPlayerDlg.h"
#endif

#include "miniini.h"

// *******************************************************************
/** Implementation of the (global) mp3 player Configuration-Dialog
  */
class ConfigPlayerDlgImpl : public ConfigPlayerDlg
{
	Q_OBJECT

public:
    ConfigPlayerDlgImpl( MiniIniDB & aIniDB, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	virtual ~ConfigPlayerDlgImpl();

public slots:
    virtual void sltCancel();
    virtual void sltOk();
    virtual void sltPlayerChanged(int);

private:
	void WriteComboBoxes();
	void ReadComboBoxes();

	MiniIniDB &		m_aIniDB;
};

#endif

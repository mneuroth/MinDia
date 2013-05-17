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

#include "ui_ConfigPlayerDlg4.h"

#include <QDialog>

// *******************************************************************
/** Implementation of the (global) mp3 player Configuration-Dialog
  */
class ConfigPlayerDlgImpl : public QDialog, Ui_ConfigPlayerDlg
{
	Q_OBJECT

public:
    ConfigPlayerDlgImpl( QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~ConfigPlayerDlgImpl();

public slots:
    virtual void sltCancel();
    virtual void sltOk();
    virtual void sltPlayerChanged(int);

private:
	void SaveSettings();
    void RestoreSettings();
};

#endif

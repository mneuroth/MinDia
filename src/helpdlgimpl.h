/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2004 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/helpdlgimpl.h,v $
 *
 *  $Revision: 1.2 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.1  2004/02/21 14:59:02  min
 *	Help dialog improved
 *	
 *	
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2004 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#ifndef _HELPDLGIMPL_H
#define _HELPDLGIMPL_H

#include "ui_HelpDlg4.h"

#include <QDialog>

// *******************************************************************
/** Implementation of the Help-Dialog
  */
class HelpDlgImpl : public QDialog, public Ui_HelpDialog
{
	Q_OBJECT

public:
	HelpDlgImpl( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
	virtual ~HelpDlgImpl();

public slots:
    virtual void sltFindClicked();
    virtual void sltSearchTextChanged(const QString &);

private:
	int m_iParagraph;
	int m_iIndex;
};

#endif

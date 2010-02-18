/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/comlogimpl.h,v $
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

#ifndef _COMLOGIMPL_H
#define _COMLOGIMPL_H

#include "ComLoggingDlg.h"

#include "minlog.h"
//Added by qt3to4:
#include <QCustomEvent>
#include <QKeyEvent>
#include <QCloseEvent>


// *******************************************************************
/** Implementation of the Logging-Dialog
  */
class ComLoggingDialogImpl : public ComLoggingDialog, public minLoggingInterface
{
	Q_OBJECT

public:
    ComLoggingDialogImpl( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
	virtual ~ComLoggingDialogImpl();

	// *** implements the minLoggingInterface ***
	virtual void LogMsg( const char * sMsg );

public slots:
	virtual void sltCloseDialog();

signals:
	void sigDialogClosed();
	void sigDialogHelp( const QString & sHelpTag);

protected:
	virtual void closeEvent( QCloseEvent * pCloseEvent );
	virtual void done( int iRet );
	virtual void keyPressEvent( QKeyEvent * pEvent ); 
	virtual void customEvent( QCustomEvent * pEvent );
};

#endif

/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/eventmapdlgimpl.h,v $
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

#ifndef _EVENTMAPDLGIMPL_H
#define _EVENTMAPDLGIMPL_H

#include "EventMapDlg.h"

class ApplScriptEnvironment;

// *******************************************************************
/** Implementation of the EventMapDialog.
  */
class EventMapDlgImpl : public EventMapDlg
{
	Q_OBJECT

public:
    EventMapDlgImpl( ApplScriptEnvironment * pScriptEnv, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	virtual ~EventMapDlgImpl();

public slots:
    virtual void sltModifyScript();
    virtual void sltEventSelected( int iIndex );

signals:
	void sigDialogHelp( QWidget * pParent, const QString & sHelpTag );

protected:
	virtual void keyPressEvent( QKeyEvent * pEvent ); 

private:
	// ** helper methods **
	void FillComboBox();

	// ** data **
	ApplScriptEnvironment *		m_pScriptEnv;		// no owner !
};

#endif

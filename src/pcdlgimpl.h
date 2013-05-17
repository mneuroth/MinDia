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

#ifndef _PCDLGIMPL_H
#define _PCDLGIMPL_H

#include "ui_ProjectorControlDlg4.h"

#include <QKeyEvent>
#include <QCloseEvent>

#include <string>

using namespace std;

class RolleiCom;
class DiaPresentation;

// *******************************************************************
/** Implementation of the ProjectorControl-Dialog
  */
class ProjectorControlDlgImpl  : public QDialog, Ui_ProjectorControlDialog   // public ProjectorControlDialog
{
	Q_OBJECT

public:
    ProjectorControlDlgImpl( RolleiCom * pProjectorCom, DiaPresentation * pPresentation, QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~ProjectorControlDlgImpl();

public slots:
    virtual void sltDirectMode();
    virtual void sltGetSlideNo();
    virtual void sltResetProjector();
    virtual void sltSlideBack();
    virtual void sltSlideForward();
    virtual void sltStopGo();
    virtual void sltAutofocusOffToggled( bool );
    virtual void sltAFBackward();
    virtual void sltAFForward();
    virtual void sltAutofocusOnToggled( bool );
    virtual void sltDissolve1Toggled( bool );
    virtual void sltDissolve2Toggled( bool );
    virtual void sltDissolve3Toggled( bool );
    virtual void sltDissolve4Toggled( bool );
    virtual void sltDissolve5Toggled( bool );
    virtual void sltLight1Toggled( bool );
    virtual void sltLight2Toggled( bool );
    virtual void sltLight3Toggled( bool );
    virtual void sltLight4Toggled( bool );
	virtual void sltCloseDialog();
    virtual void sltCheckMode();
    virtual void sltTestProjector();
    virtual void sltGetStatus();

signals:
	void sigDialogClosed();
	void sigDialogHelp( const QString & sHelpTag );

protected:
	virtual void closeEvent( QCloseEvent * pCloseEvent );
	virtual void done( int iRet );
	virtual void keyPressEvent( QKeyEvent * pEvent ); 

private:
	// ** help methods **
	void CheckMode();
	bool CheckReturn( bool bValue );
	string CheckReturn( const string & sValue );
	void DoDialogEnable( bool bEnable );
	void switchToNotConnectedMode();
	void switchToDirectMode();
	void switchToKeyboardMode();
	void setModeString( const QString & sStrg );
	QString AddToModeString() const;
	bool DoSynchronCmd() const;

	// ** data **
	RolleiCom *			m_pProjectorCom;		// no owner !
	DiaPresentation *	m_pPresentation;		// no owner !
};

#endif

/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/mindiapyc/scriptdlgimpl.h,v $
 *
 *  $Revision: 1.2 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.1.1.1  2003/08/15 16:38:22  min
 *	Initial checkin of MinDia Ver. 0.97.1
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

#ifndef _SCRIPTDLGIMPL_H
#define _SCRIPTDLGIMPL_H

#include "ScriptDlg.h"

#include "minbase.h"
#include "iscript.h"

// *******************************************************************
/** Implementation of the Script-Dialog
  */
class ScriptDlgImpl : public ScriptDlg
{
    Q_OBJECT

public:
    ScriptDlgImpl( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0, bool bCheckForSave = true );
    ~ScriptDlgImpl();

public slots:
    virtual void sltHelp();
    virtual void sltCloseDialog();
	virtual void sltCancelDialog();
    virtual void sltExecuteScript();
    virtual void sltSaveScript();
    virtual void sltLoadScript();
    virtual void sltClearOutput();
    virtual void sltClearScript();
	virtual void sltScriptChanged();
    virtual void sltPrintToOutput( const QString & sText );
	virtual void sltSetScript( const QString & sScript );

	QString GetScript() const;

signals:
	void sigDialogClosed();
	void sigDocumentUpdate();
	void sigDialogHelp( QWidget * pParent, const QString & sHelpTag );

protected:
	virtual void closeEvent( QCloseEvent * pCloseEvent );
	virtual void keyPressEvent( QKeyEvent * pEvent ); 

private:
	// ** help methods **
	void SetFileName( const char * sName );
	void SetModified( bool bIsModified );

	// ** data **
	minClientHandle<IGeneralScriptVM> 	m_hScriptVM;
	minClientHandle<IGeneralScriptFcn>	m_hScriptFcn;

	bool	m_bModified;
	bool	m_bCheckForSave;
};

#endif

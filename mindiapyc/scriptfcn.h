/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/mindiapyc/scriptfcn.h,v $
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

#ifndef _SCRIPTFCN_H
#define _SCRIPTFCN_H

#include "iscript.h"

class DynamicSubMenuContainer;

class ScriptDlgImpl;

class QWidget;
class QDialog;
class QAction;
class QPopupMenu;

#include <qobject.h>
#include <qstring.h>

// *************************************************************************
/** Implements the general script environment for applications.
  */
class ScriptMenuHandler : public QObject
{
	Q_OBJECT

public:
	ScriptMenuHandler( IGeneralScriptEnvironment * pScriptEnv );
	~ScriptMenuHandler();

	bool IsOk() const;

	void Init();

	bool DoModifyScript( const QString & sUserText, QString & sScript );

public slots:
	void sltDoModifyScript();
	void sltUpdateScriptsMenu();
	void sltHideScriptsMenu();
	void sltScriptsMenuActivated( int iIndex );
	void sltUpdatePluginsMenu();
	void sltPrintToScriptOutput( const QString & sText ); 

signals:
	void sigShowStatusMessage( const QString & sMsg ); 

private:
	// ** data **
	ScriptDlgImpl *				m_pScriptModifyDlg;
	QAction *					m_pModifyScriptAction;
	QPopupMenu *				m_pScriptMenu;

	DynamicSubMenuContainer *	m_pMenuItemContainer;

	QPopupMenu *				m_pPluginMenu;			// ** no owner !
	QWidget *					m_pMainWindow;			// ** no owner !

	IGeneralScriptEnvironment *	m_pScriptEnv;			// ** no owner !
};

// *************************************************************************
/** Implements the general script environment for applications.
  */
class IGeneralScriptEnvironmentImpl : public IGeneralScriptEnvironment
{
public:
	IGeneralScriptEnvironmentImpl( const string & sModuleName, minDll * pDll );
	virtual ~IGeneralScriptEnvironmentImpl();

	virtual bool	IsOk() const;

	virtual bool			HasLanguage( ScriptLanguage aLanguage ) const;
	virtual string			GetLanguageStrg( ScriptLanguage aLanguage ) const;
	virtual ScriptLanguage	GetLanguageFromStrg( const string & sLanguage ) const;

	virtual bool			ModifyScript( const string & sUserComment, string & sScriptInOut, ScriptLanguage aLanguage );
	virtual int				ExecuteScript( const string & sScriptIn, ScriptLanguage aLanguage );

private:
	// ** helper methods **
	bool IsOkImpl() const;
	bool IsInitialized() const;
	void InitEnvironment();

	// ** data **
	minClientHandle<IGeneralScriptVM>	m_hScriptVM;

	ScriptMenuHandler *					m_pScriptMenuHandler;
};

#endif

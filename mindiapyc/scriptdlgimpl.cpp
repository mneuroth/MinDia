/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/mindiapyc/scriptdlgimpl.cpp,v $
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

#include "scriptdlgimpl.h"

#include "minutils.h"	// ReadTextFile(), WriteTextFile()

#include <qmultilineedit.h>
#include <qlayout.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qlabel.h>

// *******************************************************************
// *******************************************************************
// *******************************************************************

#define _PYTHON_EXT		"*.py"


ScriptDlgImpl::ScriptDlgImpl( QWidget* parent, const char* name, bool modal, WFlags fl, bool bCheckForSave )
: ScriptDlg( parent, name, modal, fl ),
  m_hScriptVM( g_IGeneralScriptVMID ),
  m_hScriptFcn( g_IGeneralScriptFcnID ),
  m_bModified( false ),
  m_bCheckForSave( bCheckForSave )
{
	// ** script edit field should be larger than the output edit field
	ScriptDlgLayout->setStretchFactor( m_pScript, 3 );

	m_pScript->setFocus();

    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltPresentationDataDialogClosed() ) );
    connect( this, SIGNAL( sigDocumentUpdate() ), parent, SLOT( sltDoDocumentStateUpdate() ) );
    connect( this, SIGNAL( sigDialogHelp(QWidget *, const QString &) ), parent, SLOT( sltShowModalHelp(QWidget *, const QString &) ) );

	SetModified( m_bModified );
}

ScriptDlgImpl::~ScriptDlgImpl()
{
}

void ScriptDlgImpl::sltHelp()
{
	emit sigDialogHelp( this, "ScriptModifyDialog" );
}

void ScriptDlgImpl::sltCloseDialog()
{
	if( m_bModified && m_bCheckForSave )
	{
		int iRet = QMessageBox::warning( this, tr( "Warning" ), 
								tr( "Script was changed, save file now ?" ), 
								tr( "Yes" ), tr( "No" ), tr( "Cancel" ), 0, 2 );
		if( iRet == 0 )
		{
			sltSaveScript();
		}
		else if( iRet == 2 )	// cancel --> do not close the dialog !
		{
			return;
		}
	}

	emit accept();

	emit sigDialogClosed();
}

void ScriptDlgImpl::sltCancelDialog()
{
	emit reject();

	emit sigDialogClosed();
}

void ScriptDlgImpl::sltExecuteScript()
{
	QString sScript = m_pScript->text();

	if( m_hScriptVM.IsValid() )
	{
		const char * s = (const char *)sScript;

		if( s != 0 )
		{
			int iRet = m_hScriptVM->ExecuteScript( s );

			if( iRet == 0 )
			{
				sltPrintToOutput( tr( "script done." ) );
			}
			else
			{
				sltPrintToOutput( tr( "script error !" ) );
			}

			emit sigDocumentUpdate();
		}
	}
}

void ScriptDlgImpl::sltPrintToOutput( const QString & sText )
{
	if( m_pOutput )
	{
		QString s = m_pOutput->text();

		s = s + sText + "\n";

		m_pOutput->setText( s );

		// ** update the visible area, so that the last output is visible
		m_pOutput->setCursorPosition( m_pOutput->numLines(), 1 );
	}
}

void ScriptDlgImpl::sltSetScript( const QString & sScript )
{
	if( m_pScript )
	{
		m_pScript->setText( sScript );
	}
}

void ScriptDlgImpl::sltSaveScript()
{
	if( m_pScript )
	{
	    QString sFileName = QFileDialog::getSaveFileName( m_hScriptFcn->GetScriptDirecotry(), _PYTHON_EXT, this );

	    if( !sFileName.isEmpty() )
		{
			QString s = m_pScript->text();

			const char * sStr = (const char *)s;
			string sTemp = (sStr ? sStr : "");

			bool bOk = WriteTextFile( (const char *)sFileName, sTemp );

			if( !bOk )
			{
				int iRet = QMessageBox::warning( this, tr( "Warning" ), 
										tr( "Can not write script file " )+sFileName, 1, 0 );
			}
			else
			{
				SetModified( false );
				SetFileName( sFileName );
			}
		}
	}
}

void ScriptDlgImpl::sltLoadScript()
{
	if( m_pScript )
	{
		QString sFileName = QFileDialog::getOpenFileName( m_hScriptFcn->GetScriptDirecotry(), _PYTHON_EXT, this );

	    if( !sFileName.isEmpty() )
		{
			string sTemp;

			bool bOk = ReadTextFile( (const char *)sFileName, sTemp );

			if( !bOk )
			{
				int iRet = QMessageBox::warning( this, tr( "Warning" ), 
										tr( "Can not read script file " )+sFileName, 1, 0 );
			}
			else
			{
				m_pScript->setText( QString( sTemp.c_str() ) );

				SetModified( false );
				SetFileName( sFileName );
			}
		}
	}
}

void ScriptDlgImpl::sltClearOutput()
{
	if( m_pOutput )
	{
		m_pOutput->setText( "" );
	}
}

void ScriptDlgImpl::sltClearScript()
{
	if( m_pScript )
	{
		m_pScript->setText( "" );
	}
}

void ScriptDlgImpl::sltScriptChanged()
{
	SetModified( true );
}

QString ScriptDlgImpl::GetScript() const
{
	if( m_pScript )
	{
		return m_pScript->text();
	}
	return "";
}

void ScriptDlgImpl::closeEvent( QCloseEvent * pCloseEvent )
{
	sltCloseDialog();
}

void ScriptDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Key_F1) )
	{
		emit sigDialogHelp( this, "ScriptModifyDialog" );
	}
	else
	{
		ScriptDlg::keyPressEvent( pEvent );
	}
}

void ScriptDlgImpl::SetFileName( const char * sName )
{
	QString s;
	
	s += "[";
	s += sName;
	s += "]";

	m_pScriptFileName->setText( s );
}

void ScriptDlgImpl::SetModified( bool bIsModified )
{
	if( bIsModified )
	{
		setCaption( tr( "Script *" ) );
	}
	else
	{
		setCaption( tr( "Script" ) );
	}

	m_bModified = bIsModified;
}


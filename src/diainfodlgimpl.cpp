/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/diainfodlgimpl.cpp,v $
 *
 *  $Revision: 1.2 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.1.1.1  2003/08/15 16:38:21  min
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

#include "appconfig.h"
#include "diainfodlgimpl.h"
#include "hitem.h"
#include "diainfo.h"
#ifndef ZAURUS
#include "applscriptenv.h"
#endif

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qmultilineedit.h>
#include <qvalidator.h>
#include <qmessagebox.h>
#include <qfiledialog.h>

DiaInfoDlgImpl::DiaInfoDlgImpl( QWidget* pEventConsumer, QWidget* parent, const char* name, bool modal, WFlags fl )
: DiaInfoDlg( parent, name, modal, fl )
{
	m_pItem = 0;
	m_bDataChanged = false;

	m_pDissolveValidator = new QDoubleValidator( 0, 10.0, 1, m_pDissolveEdit );
	m_pTimerValidator = new QDoubleValidator( 0, 100.0, 1, m_pTimerEdit );
	m_pEffectCountValidator = new QIntValidator( 0, 4, m_pEffectCount );

	m_pDissolveEdit->setValidator( m_pDissolveValidator );
	m_pTimerEdit->setValidator( m_pTimerValidator );
	m_pEffectCount->setValidator( m_pEffectCountValidator );

	// ** set new accelerators for fast movement
    m_pPrevious->setAccel( CTRL+Key_Left );
    m_pNext->setAccel( CTRL+Key_Right );

	// min todo --> Effekte werden noch nicht unterstuezt
	m_pEffectButtonGroup->setEnabled( false );

	m_pScript->setEnabled( false );

    connect( this, SIGNAL( sigUpdateViews() ), parent, SLOT( sltDoUpdateAllViews() ) );
    connect( this, SIGNAL( sigDataChanged() ), parent, SLOT( sltDoDataChanged() ) );
    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltModifyItemDialogClosed() ) );
	connect( this, SIGNAL( sigPrevItem() ), pEventConsumer, SLOT( sltSelectPrev() ) );
	connect( this, SIGNAL( sigNextItem() ), pEventConsumer, SLOT( sltSelectNext() ) );
	connect( this, SIGNAL( sigNewItem() ), pEventConsumer, SLOT( sltNewItemAfterSelected() ) );
    connect( this, SIGNAL( sigDialogHelp(const QString &) ), parent, SLOT( sltShowHelp(const QString &) ) );
}

DiaInfoDlgImpl::~DiaInfoDlgImpl()
{
	delete m_pDissolveValidator;
	delete m_pTimerValidator;
	delete m_pEffectCountValidator;
}

void DiaInfoDlgImpl::sltDisableDialog( bool bCheckData )
{
	if( bCheckData )
	{
		CheckIfDataChanged();
	}

	m_pApply->setEnabled( false );
	m_pApplyAndNext->setEnabled( false );
	m_pApplyAndPrev->setEnabled( false );

	bool bValue = false;
	m_pIDEdit->setEnabled( bValue );
	m_pIDEdit->setText( "" );
	m_pFileNameEdit->setEnabled( bValue );
	m_pFileNameEdit->setText( "" );
	m_pCommentEdit->setEnabled( bValue );
	m_pCommentEdit->setText( "" );
	m_pDissolveEdit->setEnabled( bValue );
	m_pDissolveEdit->setText( "" );
	m_pTimerEdit->setEnabled( bValue );
	m_pTimerEdit->setText( "" );
	m_pFileNameButton->setEnabled( bValue );
	m_pScript->setEnabled( bValue );
	m_pScript->setText( "" );
	m_pModifyScript->setEnabled( bValue );
	m_pPrevious->setEnabled( bValue );
	m_pNext->setEnabled( bValue );
	m_pSlideFormat->setEnabled( bValue );
	m_pNewItem->setEnabled( bValue );

	SetDataChanged( false );
}

void DiaInfoDlgImpl::sltUpdateData( HItem * pFirstSelectedItem, bool bEnable )
{
	CheckIfDataChanged();

	bool bValue = bEnable;
	m_pIDEdit->setEnabled( bValue );
	m_pFileNameEdit->setEnabled( bValue );
	m_pCommentEdit->setEnabled( bValue );
	m_pDissolveEdit->setEnabled( bValue );
	m_pTimerEdit->setEnabled( bValue );
	m_pFileNameButton->setEnabled( bValue );
	m_pScript->setEnabled( bValue );
	m_pModifyScript->setEnabled( bValue );
	m_pPrevious->setEnabled( bValue );
	m_pNext->setEnabled( bValue );
	m_pSlideFormat->setEnabled( bValue );
	m_pNewItem->setEnabled( bValue );

#ifndef ZAURUS
	if( pFirstSelectedItem )
	{
		minHandle<DiaInfo> hData = pFirstSelectedItem->GetInfoData();

		if( hData.IsOk() )
		{
			m_pIDEdit->setText( hData->GetId() );
			m_pFileNameEdit->setText( hData->GetImageFile() );
			m_pScript->setText( hData->GetScript() );
			m_pCommentEdit->setText( hData->GetComment() );

			if( hData->IsHorizontalFormat() )
			{
				m_pHorizontalFormat->setChecked( true );
			}
			else
			{
				m_pVerticalFormat->setChecked( true );
			}

			for( int i=0; i<hData->GetOperationCount(); i++ )
			{
				TimeOperation aOp = hData->GetOperation( i );

				if( aOp.GetOperationType() == TimeOperation::DISSOLVE_IN )
				{
					QString s;
					s.setNum( aOp.GetOperationTime() );

					m_pDissolveEdit->setText( s );
				}
				if( aOp.GetOperationType() == TimeOperation::SHOW )
				{
					QString s;
					s.setNum( aOp.GetOperationTime() );

					m_pTimerEdit->setText( s );
				}
			}

			SetDataChanged( false );
		}
	}
#endif

	m_pItem = pFirstSelectedItem;
}

void DiaInfoDlgImpl::sltApplyData()
{
	if( m_pItem )
	{
#ifndef ZAURUS
		minHandle<DiaInfo> hData = m_pItem->GetInfoData();

		if( hData.IsOk() )
		{
			QString s1 = m_pIDEdit->text();
			QString s2 = m_pFileNameEdit->text();
			QString s3 = m_pCommentEdit->text();
			QString s4 = m_pScript->text();

			hData->SetData( (const char *)s1, (const char *)s2, (const char *)s3, (const char *)s4 );

			hData->SetHorizontalFormat( m_pHorizontalFormat->isChecked() );

			for( int i=0; i<hData->GetOperationCount(); i++ )
			{
				TimeOperation aOp = hData->GetOperation( i );

				if( aOp.GetOperationType() == TimeOperation::DISSOLVE_IN )
				{
					bool bOk	= false;
					QString s	= m_pDissolveEdit->text();
					double d	= s.toDouble( &bOk  );

					if( bOk )
					{
						hData->ModifyOperation( i, TimeOperation( TimeOperation::DISSOLVE_IN, d ) );
					}
				}
				if( aOp.GetOperationType() == TimeOperation::SHOW )
				{
					bool bOk	= false;
					QString s	= m_pTimerEdit->text();
					double d	= s.toDouble( &bOk  );

					if( bOk )
					{
						hData->ModifyOperation( i, TimeOperation( TimeOperation::SHOW, d ) );
					}
				}
			}

			// ** data was writen, so (new) data is now unchanged !
			SetDataChanged( false );

			emit sigDataChanged();
			emit sigUpdateViews();
		}
#endif
	}
}

void DiaInfoDlgImpl::sltApplyAndNext()
{
	sltApplyData();
	sltNextItem();
}

void DiaInfoDlgImpl::sltApplyAndPrev()
{
	sltApplyData();
	sltPrevItem();
}

void DiaInfoDlgImpl::sltTextChanged( const QString & /*sText*/ )
{
	SetDataChanged( true );
}

void DiaInfoDlgImpl::sltTextChanged()
{
	SetDataChanged( true );
}

void DiaInfoDlgImpl::sltDataChanged()
{
	SetDataChanged( true );
}

void DiaInfoDlgImpl::sltCloseDialog()
{
	sltApplyData();

	emit sigDialogClosed();

	emit accept();
}

void DiaInfoDlgImpl::sltDialogCanceled()
{
	// clear changed data in controls
	sltDisableDialog( /*bCheckData*/false );

	emit sigDialogClosed();

	emit reject();
}

void DiaInfoDlgImpl::sltCreateNewItem()
{
	emit sigNewItem();
}

void DiaInfoDlgImpl::sltPrevItem()
{
	emit sigPrevItem();
}

void DiaInfoDlgImpl::sltNextItem()
{
	emit sigNextItem();
}

void DiaInfoDlgImpl::sltModifyScript()
{
#ifndef ZAURUS
	minClientHandle<IGeneralScriptEnvironment> hScriptEnv( g_IGeneralScriptEnvironmentID );

	IGeneralScriptEnvironment::ScriptLanguage aLanguage = IGeneralScriptEnvironment::PYTHON;

	QString sQt = m_pScript->text();
	const char * s = (const char *)sQt;
	string sScript = (s ? s : "");

	bool bOk = hScriptEnv->ModifyScript( "event: ", sScript, aLanguage );

	if( bOk )
	{
		m_pScript->setText( sScript.c_str() );
	}
#endif
}

void DiaInfoDlgImpl::sltSelectFileName()
{
#ifndef ZAURUS
	QString sFileName = QFileDialog::getOpenFileName( /*QString::null*/GetImagePath().c_str(), /*QString::null*/"*.bmp", this );

	if( !sFileName.isEmpty() )
	{
		m_pFileNameEdit->setText( sFileName );
	}
#endif
}

void DiaInfoDlgImpl::SetDataChanged( bool bState )
{
	m_bDataChanged = bState;

	m_pApply->setEnabled( m_bDataChanged );
	m_pApplyAndNext->setEnabled( m_bDataChanged );
	m_pApplyAndPrev->setEnabled( m_bDataChanged );

	//emit sigDataChanged();
	//emit sigUpdateViews();
}

bool DiaInfoDlgImpl::GetDataChanged() const
{
	return m_bDataChanged;
}

void DiaInfoDlgImpl::CheckIfDataChanged()
{
	if( GetDataChanged() )
	{
		if( m_pItem )
		{
			int iRet = QMessageBox::warning( this, tr( "MinDia - Warning" ), tr( "Write changed data ?" ), tr( "Yes" ), tr( "No" ) );

			if( iRet == 0 )
			{
				sltApplyData();
			}
		}
	}
}

void DiaInfoDlgImpl::closeEvent( QCloseEvent * pCloseEvent )
{
	emit sigDialogClosed();

	pCloseEvent->accept();
}

void DiaInfoDlgImpl::done( int iRet )
{
	emit sigDialogClosed();
	
	DiaInfoDlg::done( iRet );
}

void DiaInfoDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Key_F1) )
	{
		emit sigDialogHelp( "DiaInfoDialog" );
	}
	else
	{
		DiaInfoDlg::keyPressEvent( pEvent );
	}
}

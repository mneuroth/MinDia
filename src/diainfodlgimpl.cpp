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

#include "appconfig.h"
#include "diainfodlgimpl.h"
#include "hitem.h"
#include "diainfo.h"
#include "applscriptenv.h"
#include "misctools.h"

#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QValidator>
#include <QMessageBox>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QFileDialog>

DiaInfoDlgImpl::DiaInfoDlgImpl( QWidget* pEventConsumer, QWidget* parent, Qt::WFlags fl )
: QDialog( parent/*, name, modal*/, fl )
{
    setupUi(this);

    m_pItem = 0;
	m_bDataChanged = false;

	m_pDissolveValidator = new QDoubleValidator( 0, 10.0, 1, m_pDissolveEdit );
	m_pTimerValidator = new QDoubleValidator( 0, 100.0, 1, m_pTimerEdit );
	m_pEffectCountValidator = new QIntValidator( 0, 4, m_pEffectCount );

	m_pDissolveEdit->setValidator( m_pDissolveValidator );
	m_pTimerEdit->setValidator( m_pTimerValidator );
	m_pEffectCount->setValidator( m_pEffectCountValidator );

	// ** set new accelerators for fast movement
    m_pPrevious->setShortcut( Qt::CTRL+Qt::Key_Left );
    m_pNext->setShortcut( Qt::CTRL+Qt::Key_Right );

    // FUTURE --> Effekte werden noch nicht unterstuezt
	m_pEffectButtonGroup->setEnabled( false );

    m_pBoxScripts->setVisible( false );
	m_pScript->setEnabled( false );

    m_pScreen = new PlayScreenEditing();
    m_pClipping->setScene( m_pScreen );

    connect( this, SIGNAL( sigUpdateViews() ), parent, SLOT( sltDoUpdateAllViews() ) );
    connect( this, SIGNAL( sigDataChanged() ), parent, SLOT( sltDoDataChanged() ) );
    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltModifyItemDialogClosed() ) );
	connect( this, SIGNAL( sigPrevItem() ), pEventConsumer, SLOT( sltSelectPrev() ) );
	connect( this, SIGNAL( sigNextItem() ), pEventConsumer, SLOT( sltSelectNext() ) );
	connect( this, SIGNAL( sigNewItem() ), pEventConsumer, SLOT( sltNewItemAfterSelected() ) );
	connect( this, SIGNAL( sigDeleteItem() ), pEventConsumer, SLOT( sltDeleteSelectedItem() ) );
    connect( this, SIGNAL( sigDialogHelp(const QString &) ), parent, SLOT( sltShowHelp(const QString &) ) );
    connect( m_pScreen, SIGNAL(sigDataChanged()), this, SLOT(sltDataChanged()) );

    UpdateScreenEditData();
}

DiaInfoDlgImpl::~DiaInfoDlgImpl()
{
	delete m_pDissolveValidator;
	delete m_pTimerValidator;
	delete m_pEffectCountValidator;
}

void DiaInfoDlgImpl::UpdateScreenEditData()
{
    m_pScreen->setSceneRect( 0, 0, m_pClipping->width()-4, m_pClipping->height()-4 );

    m_pScreen->SetBackgroundImage( m_pFileNameEdit->text() );

    if( m_hItem.IsOk() )
    {
        double relX = m_hItem->GetRelX();
        double relY = m_hItem->GetRelY();
        double relDX = m_hItem->GetRelDX();
        double relDY = m_hItem->GetRelDY();

        m_pScreen->SetClippingData( relX, relY, relDX, relDY );
    }
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
	m_pScript->setEnabled( bValue );
    m_pScript->setPlainText( "" );
	m_pFileNameButton->setEnabled( bValue );
	m_pModifyScript->setEnabled( bValue );
	m_pSlideFormat->setEnabled( bValue );
	m_pPrevious->setEnabled( bValue );
	m_pNext->setEnabled( bValue );
	m_pNewItem->setEnabled( bValue );
	m_pDeleteItem->setEnabled( bValue );

    UpdateScreenEditData();

	SetDataChanged( false );
}

void DiaInfoDlgImpl::sltUpdateData( HItem * pFirstSelectedItem, bool bEnable )
{
	if( pFirstSelectedItem )
	{
		minHandle<DiaInfo> hData = pFirstSelectedItem->GetInfoData();

		sltUpdateData( hData, bEnable );
	}

	m_pItem = pFirstSelectedItem;
}

void DiaInfoDlgImpl::sltUpdateData( minHandle<DiaInfo> hData, bool bEnable )
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
	m_pSlideFormat->setEnabled( bValue );
	m_pPrevious->setEnabled( bValue );
	m_pNext->setEnabled( bValue );
	m_pNewItem->setEnabled( bValue );
	m_pDeleteItem->setEnabled( bValue );

	m_hItem = hData;

	if( hData.IsOk() )
	{
        QImage aImage;
        ReadQImageOrEmpty( ToQString( hData->GetImageFile() ), aImage );
        m_pImageSize->setText( QString("%1 x %2").arg(aImage.width()).arg(aImage.height()) );

        m_pIDEdit->setText( ToQString(hData->GetId()) );
        m_pFileNameEdit->setText( ToQString(hData->GetImageFile()) );
        m_pCommentEdit->setText( ToQString(hData->GetComment()) );
        m_pScript->setPlainText( ToQString(hData->GetScript()) );

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

    UpdateScreenEditData();
}

void DiaInfoDlgImpl::sltApplyData()
{
	minHandle<DiaInfo> hData;

	if( m_pItem )
	{
		hData = m_pItem->GetInfoData();
	}
	else
	{
		hData = m_hItem;
	}

	if( hData.IsOk() )
	{
		QString s1 = m_pIDEdit->text();
		QString s2 = m_pFileNameEdit->text();
		QString s3 = m_pCommentEdit->text();
		QString s4;

        s4 = m_pScript->toPlainText();
        hData->SetData( ToStdString(s1), ToStdString(s2), ToStdString(s3), ToStdString(s4) );

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

        double relX, relY, relDX, relDY;
        m_pScreen->GetClippingData( relX, relY, relDX, relDY );
        hData->SetRelX( relX );
        hData->SetRelY( relY );
        hData->SetRelDX( relDX );
        hData->SetRelDY( relDY );

		// ** data was writen, so (new) data is now unchanged !
		SetDataChanged( false );

		emit sigDataChanged();
		emit sigUpdateViews();
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

    accept();
}

void DiaInfoDlgImpl::sltDialogCanceled()
{
	// clear changed data in controls
	sltDisableDialog( /*bCheckData*/false );

	emit sigDialogClosed();

    reject();
}

void DiaInfoDlgImpl::sltCreateNewItem()
{
	emit sigNewItem();
}

void DiaInfoDlgImpl::sltDeleteItem()
{
	emit sigDeleteItem();
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
	minClientHandle<IGeneralScriptEnvironment> hScriptEnv( g_IGeneralScriptEnvironmentID );

	IGeneralScriptEnvironment::ScriptLanguage aLanguage = IGeneralScriptEnvironment::PYTHON;

    QString sQt = m_pScript->toPlainText();
    string sScript = ToStdString(sQt);

    if( hScriptEnv.IsValid() )
    {
        bool bOk = hScriptEnv->ModifyScript( "event: ", sScript, aLanguage );

        if( bOk )
        {
            m_pScript->setPlainText( ToQString(sScript) );
        }
    }
}

void DiaInfoDlgImpl::sltSelectFileName()
{
    QString sFileName = QFileDialog::getOpenFileName( this, tr("Select file"), ToQString(GetImagePath()), tr("Images (*.jpg *.png *.bmp)"));

	if( !sFileName.isEmpty() )
	{
		m_pFileNameEdit->setText( sFileName );
	}
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
	
    QDialog::done( iRet );
}

void DiaInfoDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Qt::Key_F1) )
	{
		emit sigDialogHelp( "DiaInfoDialog" );
	}
	else
	{
        QDialog::keyPressEvent( pEvent );
	}
}

void DiaInfoDlgImpl::resizeEvent( QResizeEvent * pEvent )
{
    QDialog::resizeEvent( pEvent );
    UpdateScreenEditData();
}

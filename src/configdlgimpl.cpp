/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/configdlgimpl.cpp,v $
 *
 *  $Revision: 1.3 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.2  2004/02/16 19:50:26  min
 *	Fixes for Borland C++
 *	
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

#include "configdlgimpl.h"

#include "doccontroler.h"

#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qradiobutton.h>


ConfigurationDlgImpl::ConfigurationDlgImpl( DocumentAndControler * pControler, QWidget* parent, const char* name, bool modal, WFlags fl )
: ConfigurationDlg( parent, name, modal, fl ),
  m_pControler( pControler )
{
    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltConfigurationDialogClosed() ) );
    connect( this, SIGNAL( sigDocumentUpdate() ), parent, SLOT( sltDoDocumentStateUpdate() ) );
    connect( this, SIGNAL( sigDialogHelp(QWidget *, const QString &) ), parent, SLOT( sltShowModalHelp(QWidget *, const QString &) ) );

	// ** timeouts are not supported yet
#ifndef ZAURUS
	m_pReadTimeout->setEnabled( false );
	m_pWriteTimeout->setEnabled( false );
#endif
}

ConfigurationDlgImpl::~ConfigurationDlgImpl()
{
}

void ConfigurationDlgImpl::sltCloseDialog()
{
	TransferDataFromControl();

	emit accept();

	emit sigDialogClosed();
}

void ConfigurationDlgImpl::sltCancelDialog()
{
	emit reject();

	emit sigDialogClosed();
}

void ConfigurationDlgImpl::sltMSC300PToggled( bool bValue )
{
	if( bValue )
	{
		// ** set communication settings for MSC 3x0 P projector

		QString s;

		s.setNum( 9600 );
		SetCurrentItem( m_pBaudRateList, s );

		s.setNum( 8 );
		SetCurrentItem( m_pDataBitsList, s );

		// ** important: Combobox and constants in class must be the same !!!
		m_pStopBitsList->setCurrentItem( RolleiCom::ONE );

		// ** important: Combobox and constants in class must be the same !!!
		m_pParityList->setCurrentItem( RolleiCom::NONE );

		// ** important: Combobox and constants in class must be the same !!!
		m_pFlowControlList->setCurrentItem( RolleiCom::NO_FLOW );
	}
}

void ConfigurationDlgImpl::sltTwinDigitalPToggled( bool bValue )
{
	if( bValue )
	{
		// ** set communication settings for Twin Digital P projector

		QString s;

		s.setNum( 1200 );
		SetCurrentItem( m_pBaudRateList, s );

		s.setNum( 7 );
		SetCurrentItem( m_pDataBitsList, s );

		// ** important: Combobox and constants in class must be the same !!!
		m_pStopBitsList->setCurrentItem( RolleiCom::TWO );

		// ** important: Combobox and constants in class must be the same !!!
		m_pParityList->setCurrentItem( RolleiCom::EVEN );

		// ** important: Combobox and constants in class must be the same !!!
		m_pFlowControlList->setCurrentItem( RolleiCom::HARDWARE_FLOW );
	}
}

void ConfigurationDlgImpl::closeEvent( QCloseEvent * pCloseEvent )
{
	pCloseEvent->accept();

	emit sigDialogClosed();
}

void ConfigurationDlgImpl::showEvent( QShowEvent * /*pShowEvent*/ )
{
	TransferDataToControl();
}

void ConfigurationDlgImpl::TransferDataToControl()
{
	// ** fill the combo-boxes with all possible values **

#if defined( _MSC_VER ) || defined( __BORLANDC__ )
	m_pComPortList->insertItem( "COM1" );
	m_pComPortList->insertItem( "COM2" );
	m_pComPortList->insertItem( "COM3" );
	m_pComPortList->insertItem( "COM4" );
#endif
#ifdef __linux__
	m_pComPortList->insertItem( "/dev/ttyS0" );
	m_pComPortList->insertItem( "/dev/ttyS1" );
	m_pComPortList->insertItem( "/dev/ttyS2" );
	m_pComPortList->insertItem( "/dev/ttyS3" );
#endif
	
	m_pBaudRateList->insertItem( "110" );
	m_pBaudRateList->insertItem( "300" );
	m_pBaudRateList->insertItem( "1200" );
	m_pBaudRateList->insertItem( "2400" );
	m_pBaudRateList->insertItem( "4800" );
	m_pBaudRateList->insertItem( "9600" );
	m_pBaudRateList->insertItem( "19200" );
	m_pBaudRateList->insertItem( "38400" );
	m_pBaudRateList->insertItem( "57600" );
	m_pBaudRateList->insertItem( "115200" );

	m_pDataBitsList->insertItem( "6" );
	m_pDataBitsList->insertItem( "7" );
	m_pDataBitsList->insertItem( "8" );

	m_pStopBitsList->insertItem( "1" );
	m_pStopBitsList->insertItem( "1.5" );
	m_pStopBitsList->insertItem( "2" );

	m_pParityList->insertItem( tr( "none" ) );
	m_pParityList->insertItem( tr( "odd" ) );
	m_pParityList->insertItem( tr( "even" ) );

	m_pFlowControlList->insertItem( tr( "none" ) );
	m_pFlowControlList->insertItem( tr( "XON/XOFF" ) );
	m_pFlowControlList->insertItem( tr( "hardware" ) );

#ifndef ZAURUS
	m_pReadTimeout->setText( "20000" );
	m_pWriteTimeout->setText( "20000" );
#endif

	// ** set the document data **

	QString s;
	RolleiCom & aCom = m_pControler->GetDiaCom();

	m_pDigitalTwinP->setChecked( aCom.IsTwinDigitalP() );
	m_pMSC300P->setChecked( aCom.IsMSC300P() );

	m_pComPortList->setCurrentItem( aCom.GetComPortNo()-1 );

	s.setNum( aCom.GetBaudrate() );
	SetCurrentItem( m_pBaudRateList, s );

	s.setNum( aCom.GetDataBits() );
	SetCurrentItem( m_pDataBitsList, s );

	// ** important: Combobox and constants in class must be the same !!!
	m_pStopBitsList->setCurrentItem( aCom.GetStopBits() );

	// ** important: Combobox and constants in class must be the same !!!
	m_pParityList->setCurrentItem( aCom.GetParityMode() );

	// ** important: Combobox and constants in class must be the same !!!
	m_pFlowControlList->setCurrentItem( aCom.GetFlowModus() );

	m_pSimulation->setChecked( m_pControler->IsSimulation() );

	m_pDbgLogging->setChecked( aCom.IsLogging() );

#ifndef ZAURUS
	m_pExecuteEvents->setChecked( m_pControler->GetPresentation().GetScriptEnvironment().IsEnabled() );
#endif
}

void ConfigurationDlgImpl::TransferDataFromControl()
{
	// ** warning: the value in the ini-file will NOT be updated !!!
	m_pControler->SetSimulation( m_pSimulation->isChecked() );

#ifndef ZAURUS
	m_pControler->GetPresentation().GetScriptEnvironment().SetEnabled( m_pExecuteEvents->isChecked() );
#endif

	RolleiCom & aCom = m_pControler->GetDiaCom();

	int iType = RolleiCom::TWIN_DIGITAL_P;
	if( m_pDigitalTwinP->isChecked() )
	{
		iType = RolleiCom::TWIN_DIGITAL_P;
	}
	if( m_pMSC300P->isChecked() )
	{
		iType = RolleiCom::MSC_300_P;
	}
	aCom.SetProjectorType( iType );

	aCom.SetLogging( m_pDbgLogging->isChecked() );

	aCom.Stop();

	QString s;
	bool bOk;
	int iComPortNo = aCom.GetComPortNo();
	int	iBaudrate = aCom.GetBaudrate();
	int	iParityMode = aCom.GetParityMode();
	int	iStopBits = aCom.GetStopBits();
	int	iDataBits = aCom.GetDataBits();
	int iFlowMode = aCom.GetFlowModus();

	iComPortNo = m_pComPortList->currentItem()+1;

	s = m_pBaudRateList->currentText();
	iBaudrate = s.toInt( &bOk );

	s = m_pDataBitsList->currentText();
	iDataBits = s.toInt( &bOk );
	
	// ** important: Combobox and constants in class must be the same !!!
	iStopBits = m_pStopBitsList->currentItem();

	// ** important: Combobox and constants in class must be the same !!!
	iParityMode = m_pParityList->currentItem();

	// ** important: Combobox and constants in class must be the same !!!
	iFlowMode = m_pFlowControlList->currentItem();

	aCom.Start( iComPortNo, iBaudrate, iParityMode, iStopBits, iDataBits, iFlowMode );
}

void ConfigurationDlgImpl::SetCurrentItem( QComboBox * pList, const QString & s )
{
	for( int i=0; i<pList->count(); i++ )
	{
		if( pList->text( i ) == s )
		{
			pList->setCurrentItem( i );
			break;
		}
	}
}

void ConfigurationDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Key_F1) )
	{
		emit sigDialogHelp( this, "ConfigurationDialog" );
	}
	else
	{
		ConfigurationDlg::keyPressEvent( pEvent );
	}
}

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
#include <QShowEvent>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QDir>

ConfigurationDlgImpl::ConfigurationDlgImpl( DocumentAndControler * pControler, QWidget* parent, Qt::WFlags fl )
: QDialog( parent, fl ),
  m_pControler( pControler )
{
    setupUi(this);
    
    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltConfigurationDialogClosed() ) );
    connect( this, SIGNAL( sigDocumentUpdate() ), parent, SLOT( sltDoDocumentStateUpdate() ) );
    connect( this, SIGNAL( sigDialogHelp(QWidget *, const QString &) ), parent, SLOT( sltShowModalHelp(QWidget *, const QString &) ) );

	// ** timeouts are not supported yet
	m_pReadTimeout->setEnabled( false );
	m_pWriteTimeout->setEnabled( false );
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
        m_pStopBitsList->setCurrentIndex( RolleiCom::ONE );

		// ** important: Combobox and constants in class must be the same !!!
        m_pParityList->setCurrentIndex( RolleiCom::NONE );

		// ** important: Combobox and constants in class must be the same !!!
        m_pFlowControlList->setCurrentIndex( RolleiCom::NO_FLOW );
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
        m_pStopBitsList->setCurrentIndex( RolleiCom::TWO );

		// ** important: Combobox and constants in class must be the same !!!
        m_pParityList->setCurrentIndex( RolleiCom::EVEN );

		// ** important: Combobox and constants in class must be the same !!!
        m_pFlowControlList->setCurrentIndex( RolleiCom::HARDWARE_FLOW );
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

#if defined( _MSC_VER ) || defined( __BORLANDC__ ) || defined(__MINGW32__)
    QStringList aDeviceLst;
    // "\\\\.\\COM%d"
    aDeviceLst.append(QString("COM1"));
    aDeviceLst.append(QString("COM2"));
    aDeviceLst.append(QString("COM3"));
    aDeviceLst.append(QString("COM4"));
    aDeviceLst.append(QString("COM5"));
    aDeviceLst.append(QString("COM6"));
    aDeviceLst.append(QString("COM7"));
    aDeviceLst.append(QString("COM8"));
    aDeviceLst.append(QString("COM9"));
#endif
#ifdef __linux__
    // scan /dev directory for RS232 devices for unix plattforms
    QDir aDevDir("/dev","ttyS*;ttyUSB*",QDir::Name|QDir::IgnoreCase,QDir::AllEntries|QDir::System);
    QStringList aDeviceLst = aDevDir.entryList();
    for( int i=0; i<aDeviceLst.size(); i++ )
    {
        aDeviceLst[i] = "/dev/"+aDeviceLst[i];
    }
#endif
#ifdef __APPLE__
    QDir aDevDir("/dev","ttys*;tty.usb*",QDir::Name|QDir::IgnoreCase,QDir::AllEntries|QDir::System);
    QStringList aDeviceLst = aDevDir.entryList();
    for( int i=0; i<aDeviceLst.size(); i++ )
    {
        aDeviceLst[i] = "/dev/"+aDeviceLst[i];
    }
#endif
    // update Combobox with available RS232 devices
    for (int j=0; j<aDeviceLst.size(); j++)
    {
        m_pComPortList->insertItem( j, aDeviceLst.at(j) );
    }

    m_pBaudRateList->insertItem( 0, "110" );
    m_pBaudRateList->insertItem( 1, "300" );
    m_pBaudRateList->insertItem( 2, "1200" );
    m_pBaudRateList->insertItem( 3, "2400" );
    m_pBaudRateList->insertItem( 4, "4800" );
    m_pBaudRateList->insertItem( 5, "9600" );
    m_pBaudRateList->insertItem( 6, "19200" );
    m_pBaudRateList->insertItem( 7, "38400" );
    m_pBaudRateList->insertItem( 8, "57600" );
    m_pBaudRateList->insertItem( 9, "115200" );

    m_pDataBitsList->insertItem( 0, "6" );
    m_pDataBitsList->insertItem( 1, "7" );
    m_pDataBitsList->insertItem( 2, "8" );

    m_pStopBitsList->insertItem( 0, "1" );
    m_pStopBitsList->insertItem( 1, "1.5" );
    m_pStopBitsList->insertItem( 2, "2" );

    m_pParityList->insertItem( 0, tr( "none" ) );
    m_pParityList->insertItem( 1, tr( "odd" ) );
    m_pParityList->insertItem( 2, tr( "even" ) );

    m_pFlowControlList->insertItem( 0, tr( "none" ) );
    m_pFlowControlList->insertItem( 1, tr( "XON/XOFF" ) );
    m_pFlowControlList->insertItem( 2, tr( "hardware" ) );

	m_pReadTimeout->setText( "20000" );
	m_pWriteTimeout->setText( "20000" );

	// ** set the document data **

	QString s;
	RolleiCom & aCom = m_pControler->GetDiaCom();

	m_pDigitalTwinP->setChecked( aCom.IsTwinDigitalP() );
	m_pMSC300P->setChecked( aCom.IsMSC300P() );

    m_pComPortList->setCurrentIndex( m_pComPortList->findText(QString(aCom.GetComPort().c_str())) );

	s.setNum( aCom.GetBaudrate() );
	SetCurrentItem( m_pBaudRateList, s );

	s.setNum( aCom.GetDataBits() );
	SetCurrentItem( m_pDataBitsList, s );

	// ** important: Combobox and constants in class must be the same !!!
    m_pStopBitsList->setCurrentIndex( aCom.GetStopBits() );

	// ** important: Combobox and constants in class must be the same !!!
    m_pParityList->setCurrentIndex( aCom.GetParityMode() );

	// ** important: Combobox and constants in class must be the same !!!
    m_pFlowControlList->setCurrentIndex( aCom.GetFlowModus() );

	m_pSimulation->setChecked( m_pControler->IsSimulation() );

	m_pDbgLogging->setChecked( aCom.IsLogging() );

	m_pExecuteEvents->setChecked( m_pControler->GetPresentation().GetScriptEnvironment().IsEnabled() );
}

void ConfigurationDlgImpl::TransferDataFromControl()
{
	// ** warning: the value in the ini-file will NOT be updated !!!
	m_pControler->SetSimulation( m_pSimulation->isChecked() );

	m_pControler->GetPresentation().GetScriptEnvironment().SetEnabled( m_pExecuteEvents->isChecked() );

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
    string sComPort = aCom.GetComPort();
	int	iBaudrate = aCom.GetBaudrate();
	int	iParityMode = aCom.GetParityMode();
	int	iStopBits = aCom.GetStopBits();
	int	iDataBits = aCom.GetDataBits();
	int iFlowMode = aCom.GetFlowModus();

    sComPort = string(m_pComPortList->currentText().toAscii());

	s = m_pBaudRateList->currentText();
	iBaudrate = s.toInt( &bOk );

	s = m_pDataBitsList->currentText();
	iDataBits = s.toInt( &bOk );
	
	// ** important: Combobox and constants in class must be the same !!!
    iStopBits = m_pStopBitsList->currentIndex();

	// ** important: Combobox and constants in class must be the same !!!
    iParityMode = m_pParityList->currentIndex();

	// ** important: Combobox and constants in class must be the same !!!
    iFlowMode = m_pFlowControlList->currentIndex();

    aCom.Start( sComPort, iBaudrate, iParityMode, iStopBits, iDataBits, iFlowMode );
}

void ConfigurationDlgImpl::SetCurrentItem( QComboBox * pList, const QString & s )
{
	for( int i=0; i<pList->count(); i++ )
	{
        if( pList->itemText( i ) == s )
		{
            pList->setCurrentIndex( i );
			break;
		}
	}
}

void ConfigurationDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Qt::Key_F1) )
	{
		emit sigDialogHelp( this, "ConfigurationDialog" );
	}
	else
	{
		QDialog::keyPressEvent( pEvent );
	}
}

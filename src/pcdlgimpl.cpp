/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/pcdlgimpl.cpp,v $
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

#include "pcdlgimpl.h"

#include "rolleicom.h"
#include "diapresentation.h"

#include <qlabel.h>
#include <qobject.h>
#include <qpushbutton.h> 
#include <qmessagebox.h>
#include <QCloseEvent>
#include <QKeyEvent>


ProjectorControlDlgImpl::ProjectorControlDlgImpl( RolleiCom * pProjectorCom, DiaPresentation * pPresentation,  QWidget* parent, Qt::WFlags fl )
: QDialog(parent, fl) //ProjectorControlDialog( parent, name, modal, fl )
{
    setupUi(this);

	m_pProjectorCom = pProjectorCom;
	m_pPresentation = pPresentation;

    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltProjectorControlDialogClosed() ) );
    connect( this, SIGNAL( sigDialogHelp(const QString &) ), parent, SLOT( sltShowHelp(const QString &) ) );

	CheckMode();
}

ProjectorControlDlgImpl::~ProjectorControlDlgImpl()
{
}

bool ProjectorControlDlgImpl::CheckReturn( bool bValue )
{
	if( bValue )
	{
		m_pLastCmd->setText( tr( "ok" ) );
	}
	else
	{
		m_pLastCmd->setText( tr( "error" ) );
	}
	return bValue;
}

string ProjectorControlDlgImpl::CheckReturn( const string & sValue )
{
	if( sValue != "-1" )
	{
		m_pLastCmd->setText( tr( "ok" ) );
	}
	else
	{
		m_pLastCmd->setText( tr( "error" ) );
	}
	return sValue;
}

void ProjectorControlDlgImpl::DoDialogEnable( bool bEnable )
{
	/*QObjectList * */QList<QObject *> aList = /*(QObjectList *)*/children();

    for( int i=0; i<aList.count(); i++ )
	{
		QObject * pItem = aList.at( i );
        
		QWidget * pWidget = (QWidget *)pItem;

//TODO:        cout << "enable: " << (const char *)pWidget->objectName().constData() << endl;

        if( pWidget->isWidgetType() /*&& !pWidget->inherits("QGroupBox")*/)
        {
            pWidget->setEnabled( bEnable );
        }
	}

    m_pBack->setEnabled( bEnable );
    m_pForward->setEnabled( bEnable );
    m_pDirectMode->setEnabled( bEnable );

	// ** close-, status- and test-button is always active
	m_pCloseButton->setEnabled( true );
	m_pTestButton->setEnabled( true );
	m_pStatusButton->setEnabled( true );
}


void ProjectorControlDlgImpl::sltDirectMode()
{
// min todo
//	doEnable( false );

	if( m_pProjectorCom )
	{
		CheckReturn( m_pProjectorCom->DirectMode( DoSynchronCmd() ) );
		// ** do a mode-check after mode-switch 
		CheckMode();
	}

//	doEnable( true );
}

void ProjectorControlDlgImpl::sltGetSlideNo()
{
	if( m_pProjectorCom && DoSynchronCmd() )
	{
		string s = CheckReturn( m_pProjectorCom->GetActSlideNoStrg() );

		m_pSlideNoLabel->setText( s.c_str() );
	}
}

void ProjectorControlDlgImpl::sltResetProjector()
{
	if( m_pProjectorCom )
	{
		CheckReturn( m_pProjectorCom->Reset( DoSynchronCmd() ) );
		// ** do a mode-check after mode-switch 
		CheckMode();
	}
}

void ProjectorControlDlgImpl::sltSlideBack()
{
	if( m_pProjectorCom )
	{
		CheckReturn( m_pProjectorCom->SlideBack( DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltSlideForward()
{
	if( m_pProjectorCom )
	{
		CheckReturn( m_pProjectorCom->SlideForward( DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltStopGo()
{
	if( m_pProjectorCom )
	{
		CheckReturn( m_pProjectorCom->StopGo( DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltAutofocusOffToggled( bool bIsOn )
{
	if( m_pProjectorCom && bIsOn )
	{
		CheckReturn( m_pProjectorCom->SetAutofocus( false, DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltAFBackward()
{
	if( m_pProjectorCom )
	{
		CheckReturn( m_pProjectorCom->FocusStepBackward( DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltAFForward()
{
	if( m_pProjectorCom )
	{
		CheckReturn( m_pProjectorCom->FocusStepForward( DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltAutofocusOnToggled( bool bIsOn )
{
	if( m_pProjectorCom && bIsOn )
	{
		CheckReturn( m_pProjectorCom->SetAutofocus( true, DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltDissolve1Toggled( bool bIsOn )
{
	if( m_pProjectorCom && bIsOn )
	{
		CheckReturn( m_pProjectorCom->SetDissolveTime( 5, DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltDissolve2Toggled( bool bIsOn )
{
	if( m_pProjectorCom && bIsOn )
	{
		CheckReturn( m_pProjectorCom->SetDissolveTime( 15, DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltDissolve3Toggled( bool bIsOn )
{
	if( m_pProjectorCom && bIsOn )
	{
		CheckReturn( m_pProjectorCom->SetDissolveTime( 30, DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltDissolve4Toggled( bool bIsOn )
{
	if( m_pProjectorCom && bIsOn )
	{
		CheckReturn( m_pProjectorCom->SetDissolveTime( 60, DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltDissolve5Toggled( bool bIsOn )
{
	if( m_pProjectorCom && bIsOn )
	{
		CheckReturn( m_pProjectorCom->SetDissolveTime( 120, DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltLight1Toggled( bool bIsOn )
{
	if( m_pProjectorCom && bIsOn )
	{
		CheckReturn( m_pProjectorCom->SetLight( RolleiCom::LIGHT_OFF, DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltLight2Toggled( bool bIsOn )
{
	if( m_pProjectorCom && bIsOn )
	{
		CheckReturn( m_pProjectorCom->SetLight( RolleiCom::LIGHT_80, DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltLight3Toggled( bool bIsOn )
{
	if( m_pProjectorCom && bIsOn )
	{
		CheckReturn( m_pProjectorCom->SetLight( RolleiCom::LIGHT_90, DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltLight4Toggled( bool bIsOn )
{
	if( m_pProjectorCom && bIsOn )
	{
		CheckReturn( m_pProjectorCom->SetLight( RolleiCom::LIGHT_100, DoSynchronCmd() ) );
	}
}

void ProjectorControlDlgImpl::sltCloseDialog()
{
	emit sigDialogClosed();

	emit accept();
}

void ProjectorControlDlgImpl::sltCheckMode()
{
	CheckMode();
}

void ProjectorControlDlgImpl::sltTestProjector()
{
	CheckMode();
}

void ProjectorControlDlgImpl::sltGetStatus()
{
	char chStatus = m_pProjectorCom->GetStatus();

	string sStatus = m_pProjectorCom->GetStatusStrg( chStatus );

	m_pStatusLabel->setText( sStatus.c_str() );
}

void ProjectorControlDlgImpl::switchToNotConnectedMode()
{
	setModeString( tr( "not connected" ) );

	DoDialogEnable( false );
}

void ProjectorControlDlgImpl::switchToDirectMode()
{
	setModeString( tr( "direct mode" )+AddToModeString() );

	// ** enable all controls
	DoDialogEnable( true );

	// ** and than selectivly disable controls
	bool bValue = false;
	m_pDirectMode->setEnabled( bValue );
// todo --> hier ggf. forward und backward button enablen !!! --> neue Widget Hirachie --> aderes verhalten von enable/disable !!!
    m_pTestButton->setEnabled( true );
}

void ProjectorControlDlgImpl::switchToKeyboardMode()
{
	setModeString( tr( "keyboard mode" ) );

	// ** enable all controls
	DoDialogEnable( true );

	// ** and than selectivly disable controls
	bool bValue = false;
	m_pBack->setEnabled( bValue );
	m_pForward->setEnabled( bValue );
	m_pDissolve->setEnabled( bValue );

    m_pTestButton->setEnabled( true );
}

void ProjectorControlDlgImpl::setModeString( const QString & sStrg )
{
	if( m_pMode )
	{
		m_pMode->setText( sStrg );
	}
}

QString ProjectorControlDlgImpl::AddToModeString() const
{
	if( m_pProjectorCom )
	{
		return m_pProjectorCom->IsSimulation() ? " (sim)" : "";
	}
	return "";
}

void ProjectorControlDlgImpl::CheckMode()
{
	if( m_pProjectorCom && m_pPresentation && m_pPresentation->IsEdit() )
	{
		if( !m_pProjectorCom->IsProjectorConnected() )
		{
			QMessageBox::warning( 0, tr( "MinDia - Warning" ), tr( "Projector is not connected !" ) );

			switchToNotConnectedMode();
		}
		else
		{
			if( m_pProjectorCom->IsKeyboardMode() )
			{
				switchToKeyboardMode();
			}
			else
			{
				switchToDirectMode();
			}
		}
	}
	else
	{
		DoDialogEnable( false );
	}
}

bool ProjectorControlDlgImpl::DoSynchronCmd() const
{
	if( m_pPresentation )
	{
		return m_pPresentation->IsEdit();
	}
	return false;
}

void ProjectorControlDlgImpl::closeEvent( QCloseEvent * pCloseEvent )
{
	emit sigDialogClosed();

	pCloseEvent->accept();
}

void ProjectorControlDlgImpl::done( int iRet )
{
	emit sigDialogClosed();
	
	/*ProjectorControlDialog::*/QDialog::done( iRet );
}

void ProjectorControlDlgImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Qt::Key_F1) )
	{
		emit sigDialogHelp( "ProjectorControlDialog" );
	}
	else
	{
		/*ProjectorControlDialog::*/QDialog::keyPressEvent( pEvent );
	}
}

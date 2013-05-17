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

#include "comlogimpl.h"
#include "qtmtlock.h"
#include "misctools.h"

#include <QEvent>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QApplication>

const QEvent::Type c_iCustomEvent_Logging = (QEvent::Type)(QEvent::User+123);

// *******************************************************************
// *******************************************************************
// *******************************************************************

class MyCustomEvent : public QEvent
{
public:
    MyCustomEvent( QEvent::Type aType );
    void setData( const QString & sValue );
    QString data() const;

private:
    QString m_sData;

};

MyCustomEvent::MyCustomEvent( QEvent::Type aType )
    : QEvent(aType)
{
}

void MyCustomEvent::setData( const QString & sValue )
{
    m_sData = sValue;
}

QString MyCustomEvent::data() const
{
    return m_sData;
}

// *******************************************************************

ComLoggingDialogImpl::ComLoggingDialogImpl( QWidget* parent, Qt::WFlags fl )
: QDialog( parent, fl )
{
    setupUi(this);

    connect( this, SIGNAL( sigDialogClosed() ), parent, SLOT( sltLoggingDialogClosed() ) );
    connect( this, SIGNAL( sigDialogHelp(const QString &) ), parent, SLOT( sltShowHelp(const QString &) ) );
}

ComLoggingDialogImpl::~ComLoggingDialogImpl()
{
}

void ComLoggingDialogImpl::LogMsg( const string & sMsg )
{
    MyCustomEvent * pEvent = new MyCustomEvent( c_iCustomEvent_Logging );
    pEvent->setData( ToQString(sMsg) );
    QApplication::postEvent( this, pEvent );
/*
	if( m_pOutput )
	{
		QtMTLock aMTLock;

		m_pOutput->insertLine( QString( sMsg ) );
		// ** update the visible area, so that the last output is visible
		m_pOutput->setCursorPosition( m_pOutput->numLines(), 1 );
	}
*/
}

void ComLoggingDialogImpl::sltCloseDialog()
{
	emit sigDialogClosed();

    accept();
}

void ComLoggingDialogImpl::closeEvent( QCloseEvent * pCloseEvent )
{
	emit sigDialogClosed();

	pCloseEvent->accept();
}

void ComLoggingDialogImpl::done( int iRet )
{
	emit sigDialogClosed();
	
	QDialog::done( iRet );
}

void ComLoggingDialogImpl::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Qt::Key_F1) )
	{
		emit sigDialogHelp( "LoggingDialog" );
	}
	else
	{
		QDialog::keyPressEvent( pEvent );
	}
}

void ComLoggingDialogImpl::customEvent( QEvent * pEventIn )
{
    switch( pEventIn->type() )
	{
		case c_iCustomEvent_Logging:
		{
            MyCustomEvent * pEvent = (MyCustomEvent *)pEventIn;

            if( m_pOutput )
			{
				QtMTLock aMTLock;

                QString sMsg = pEvent->data();

                m_pOutput->setPlainText( m_pOutput->toPlainText()+sMsg+"\n");
				// ** update the visible area, so that the last output is visible
                m_pOutput->moveCursor(QTextCursor::End);
                m_pOutput->ensureCursorVisible();                
			}
		} break;
		
		default:
		{
		}
	}
}


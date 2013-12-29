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

#ifndef _CONFIGDLGIMPL_H
#define _CONFIGDLGIMPL_H

#include <QShowEvent>
#include <QKeyEvent>
#include <QCloseEvent>

#include "ui_ConfigurationDlg4.h"

class DocumentAndControler;

// *******************************************************************
/** Implementation of the (global) Configuration-Dialog
  */
class ConfigurationDlgImpl : public QDialog, Ui_ConfigurationDlg
{
	Q_OBJECT

public:
    ConfigurationDlgImpl( DocumentAndControler * pControler, QWidget* parent = 0, Qt::WindowFlags fl = 0 );
	virtual ~ConfigurationDlgImpl();

public slots:
	virtual void sltCloseDialog();
	virtual void sltCancelDialog();
    virtual void sltMSC300PToggled(bool);
    virtual void sltTwinDigitalPToggled(bool);
    //virtual void sltDebugLoggingToggled(bool);
    //virtual void sltSimulationToggled(bool);

signals:
	void sigDialogClosed();
	void sigDocumentUpdate();
	void sigDialogHelp( QWidget * pParent, const QString & sHelpTag);

protected:
	virtual void closeEvent( QCloseEvent * pCloseEvent );
	virtual void showEvent( QShowEvent * pShowEvent ); 
	virtual void keyPressEvent( QKeyEvent * pEvent ); 

private:
	// ** help methods **
	void TransferDataToControl();
	void TransferDataFromControl();
	void SetCurrentItem( QComboBox * pList, const QString & s );

	// ** data **
	DocumentAndControler *	m_pControler;		// no owner !
};

#endif

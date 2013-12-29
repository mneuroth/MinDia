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

#ifndef _PDDLGIMPL_H
#define _PDDLGIMPL_H

#include "ui_PresentationDataDlg4.h"

#include <QShowEvent>
#include <QKeyEvent>
#include <QCloseEvent>

#include "misctools.h"

class DiaPresentation;

// *******************************************************************
/** Implementation of the Presentation-Data-Dialog
  */
class PresentationDataDlgImpl : public QDialog, public Ui_PresentationDataDlg
{
	Q_OBJECT

public:
    PresentationDataDlgImpl( DiaPresentation * pData, QWidget* parent = 0, Qt::WindowFlags fl = 0 );
	virtual ~PresentationDataDlgImpl();

public slots:
	virtual void sltCloseDialog();
	virtual void sltCancelDialog();

    void sltCurrentOutputSizeChanged( const QString & sValue );
    void sltUserWidthChanged( const QString & sValue );
    void sltImageRatioChanged();

signals:
	void sigDialogClosed();
	void sigDocumentUpdate();
	void sigDialogHelp( QWidget * pParent, const QString & sHelpTag );

protected:
	virtual void closeEvent( QCloseEvent * pCloseEvent );
	virtual void showEvent( QShowEvent * pShowEvent ); 
	virtual void keyPressEvent( QKeyEvent * pEvent ); 

private:
    ImageRatio GetCurrentImageRatio() const;

	// ** help methods **
	void TransferDataToControl();
	void TransferDataFromControl();
		
	// ** data **
	DiaPresentation *		m_pData;		// no owner !
};	

#endif


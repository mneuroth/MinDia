//Added by qt3to4:
#include <QKeyEvent>
#include <QCloseEvent>
/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/sndinfodlgimpl.h,v $
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

#ifndef _SNDINFODLGIMPL_H
#define _SNDINFODLGIMPL_H

#include "ui_SoundInfoDlg4.h"

#include <QDialog>

class SoundInfoContainer;

// *******************************************************************
/** Implementation of the SoundComment-Dialog
  */
class SoundInfoDlgImpl :  public QDialog, public Ui_SoundInfoDlg
{
	Q_OBJECT

public:
    SoundInfoDlgImpl( SoundInfoContainer * pSoundData, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
	virtual ~SoundInfoDlgImpl();

public slots:
	virtual void sltCloseDialog();
	virtual void sltDialogCanceled();
    virtual void sltNewRow();
    virtual void sltDeleteRow();
	virtual void sltRowUp();
	virtual void sltRowDown();
    virtual void sltTableSelectionChanged();
    virtual void sltValueChanged( int, int );

signals:
	void sigDialogClosed();
	void sigDocumentUpdate();
	void sigUpdateViews();
	void sigDataChanged();
	void sigDialogHelp( QWidget * pParent, const QString & sHelpTag );

protected:
	virtual void closeEvent( QCloseEvent * pCloseEvent );
	virtual void keyPressEvent( QKeyEvent * pEvent ); 

private:
	// ** helper methods **
	void TransferData( bool bToTable );
	void UpdateCalculatedData();
	int  GetSelectedRow() const;
	void RepaintRow( int iRow );

	// ** data **
	SoundInfoContainer *	m_pSoundData;	// no owner !
};

#endif

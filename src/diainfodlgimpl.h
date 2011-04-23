/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/diainfodlgimpl.h,v $
 *
 *  $Revision: 1.3 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.2  2004/02/26 22:19:24  min
 *	Fixes to compile MinDia for the Zaurus.
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

#ifndef _DIAINFODLGIMPL_H
#define _DIAINFODLGIMPL_H

//#ifndef ZAURUS
//#include "DiaInfoDlg.h"
//#else
//#include "ZDiaInfoDlg.h"
//#endif

#include "ui_DiaInfoDlg4.h"


#include "minhandle.h"
#include "diainfo.h"
#include "hitem.h"
#include <QDialog>
#include <QKeyEvent>
#include <QCloseEvent>

class QDoubleValidator;
class QIntValidator;

/** Implementation of the Dia-Info Dialog
  */
class DiaInfoDlgImpl : public QDialog, public Ui_DiaInfoDlg
{
	Q_OBJECT

public:
    DiaInfoDlgImpl( QWidget* pEventConsumer, QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~DiaInfoDlgImpl();

	void SetDataChanged( bool bState );
	bool GetDataChanged() const;

	void CheckIfDataChanged();

public slots:
    virtual void sltDisableDialog( bool bCheckData );
    virtual void sltUpdateData( HItem * pFirstSelectedItem, bool bEnable );
    virtual void sltUpdateData( minHandle<DiaInfo> hFirstSelectedItem, bool bEnable );
    virtual void sltApplyData();
    virtual void sltApplyAndNext();
    virtual void sltApplyAndPrev();
	virtual void sltCloseDialog();
	virtual void sltDialogCanceled();
    virtual void sltPrevItem();
    virtual void sltNextItem();
    virtual void sltSelectFileName();
	virtual void sltModifyScript();
    virtual void sltDataChanged();
    virtual void sltCreateNewItem();
	virtual void sltDeleteItem();

	virtual void sltTextChanged( const QString & sText );
	virtual void sltTextChanged();

signals:
	void sigUpdateViews();
	void sigDataChanged();
	void sigDialogClosed();
	void sigPrevItem();
	void sigNextItem();
	void sigNewItem();
	void sigDeleteItem();
	void sigDialogHelp( const QString & sHelpTag);

protected:
	virtual void closeEvent( QCloseEvent * pCloseEvent );
	virtual void done( int iRet );
	virtual void keyPressEvent( QKeyEvent * pEvent ); 

private:
	bool				m_bDataChanged;
	HItem *				m_pItem;			// NO OWNER !!!
	minHandle<DiaInfo>	m_hItem;

	QDoubleValidator *	m_pDissolveValidator;
	QDoubleValidator *	m_pTimerValidator;
	QIntValidator *		m_pEffectCountValidator;
};

#endif


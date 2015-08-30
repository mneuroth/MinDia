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

#ifndef _DIAINFODLGIMPL_H
#define _DIAINFODLGIMPL_H

#include "ui_DiaInfoDlg4.h"

#include "minhandle.h"
#include "diainfo.h"
#include "hitem.h"
#include "playscreenediting.h"

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
    DiaInfoDlgImpl( QWidget* pEventConsumer, QWidget* parent = 0, Qt::WindowFlags fl = 0 );
	virtual ~DiaInfoDlgImpl();

	void SetDataChanged( bool bState );
	bool GetDataChanged() const;

	void CheckIfDataChanged();

    void UpdateScreenEditData();

public slots:
    void sltDisableDialog( bool bCheckData );
    void sltSetDarkRectangle( bool bValue );
    void sltSetKenBurnsEffect( bool bValue );
    void sltUpdateData( HItem * pFirstSelectedItem, bool bEnable );
    void sltUpdateData( minHandle<DiaInfo> hFirstSelectedItem, bool bEnable );
    void sltApplyData();
    void sltApplyAndNext();
    void sltApplyAndPrev();
    void sltCloseDialog();
    void sltDialogCanceled();
    void sltPrevItem();
    void sltNextItem();
    void sltSelectFileName();
    void sltModifyScript();
    void sltDataChanged();
    void sltCreateNewItem();
    void sltDeleteItem();
    void sltModifyColor();

    void sltTextChanged( const QString & sText );
    void sltTextChanged();

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
    virtual void resizeEvent( QResizeEvent * pEvent );

private:
	bool				m_bDataChanged;
	HItem *				m_pItem;			// NO OWNER !!!
	minHandle<DiaInfo>	m_hItem;
    PlayScreenEditing * m_pScreen;
    QColor              m_aCurrentColor;

	QDoubleValidator *	m_pDissolveValidator;
	QDoubleValidator *	m_pTimerValidator;
	QIntValidator *		m_pEffectCountValidator;
};

#endif



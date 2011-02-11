/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/commentdlgimpl.h,v $
 *
 *  $Revision: 1.1.1.1 $
 *
 *	$Log: not supported by cvs2svn $
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

#ifndef _COMMENTDLGIMPL_H
#define _COMMENTDLGIMPL_H

#include "ui_CommentDlg4.h"

#include <QKeyEvent>
#include <QCloseEvent>
#include <QDialog>

class GenericCommentContainer;

// *******************************************************************
/** Implementation of the SoundInfo-Dialog
  */
class CommentDlgImpl : public QDialog, Ui_CommentDlg
{
	Q_OBJECT

public:
    CommentDlgImpl( GenericCommentContainer * pComments, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
	virtual ~CommentDlgImpl();

public slots:
	virtual void sltCloseDialog();
	virtual void sltDialogCanceled();
    virtual void sltNewRow();
    virtual void sltDeleteRow();
    virtual void sltTableSelectionChanged();
    virtual void sltValueChanged( int, int );
    virtual void sltSortTable();

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

	// ** data **
	GenericCommentContainer *		m_pComments;		// no owner !
};

// *******************************************************************
// *******************************************************************
// *******************************************************************

typedef CommentDlgImpl SoundCommentDlgImpl;

#endif

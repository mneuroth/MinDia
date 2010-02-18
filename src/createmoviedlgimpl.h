/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/createmoviedlgimpl.h,v $
 *
 *  $Revision: 1.2 $
 *
 *	$Log: not supported by cvs2svn $
 *	Revision 1.1  2004/04/09 19:27:27  min
 *	Dialog for movie support
 *	
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

#ifndef _CREATEMOVIEDLGIMPL_H
#define _CREATEMOVIEDLGIMPL_H

#include "CreateMovieDlg.h"

class Q3Process;
class DocumentAndControler;

// *******************************************************************
/** Implementation of the Create-Movie-Dialog.
  */
class CreateMovieDlgImpl : public CreateMovieDlg
{
	Q_OBJECT

public:
    CreateMovieDlgImpl( DocumentAndControler * pDocControler, double dTotalTimeMS, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
	virtual ~CreateMovieDlgImpl();

public slots:
    virtual void sltSelectOutputDirectory();
    virtual void sltImageRatioSelected( const QString &);
    virtual void sltCreateImages();
    virtual void sltCreateAVI();
    virtual void sltAddSound();
    virtual void sltCreateVCD();
    virtual void sltMakeShow();
    virtual void sltDeleteTempFiles();
    virtual void sltUpdateCmds();
    virtual void sltReadFromStdout();
    virtual void sltReadFromStderr();

private:
	void UpdateCmds();

	double					m_dTotalTimeMS;
	DocumentAndControler *	m_pDocControler;	// not an owner !
	Q3Process *				m_pProcess;
};

#endif

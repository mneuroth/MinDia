/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2004 by Michael Neuroth
 *
 * ------------------------------------------------------------------------
 *
 *  $Source: /Users/min/Documents/home/cvsroot/mindia/src/helpdlgimpl.cpp,v $
 *
 *  $Revision: 1.1 $
 *
 *	$Log: not supported by cvs2svn $
 *	
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2004 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

#include "helpdlgimpl.h"

#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtextbrowser.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qmessagebox.h>

// *******************************************************************

/* XPM */
/* Drawn  by Mark Donohoe for the K Desktop Environment */
/* See http://www.kde.org */
static char*back[]={
"16 16 5 1",
"# c #000000",
"a c #ffffff",
"c c #808080",
"b c #c0c0c0",
". c None",
"................",
".......#........",
"......##........",
".....#a#........",
"....#aa########.",
"...#aabaaaaaaa#.",
"..#aabbbbbbbbb#.",
"...#abbbbbbbbb#.",
"...c#ab########.",
"....c#a#ccccccc.",
".....c##c.......",
"......c#c.......",
".......cc.......",
"........c.......",
"................",
"......................"};

/* XPM */
/* Drawn  by Mark Donohoe for the K Desktop Environment */
/* See http://www.kde.org */
static char*forward[]={
"16 16 5 1",
"# c #000000",
"a c #ffffff",
"c c #808080",
"b c #c0c0c0",
". c None",
"................",
"................",
".........#......",
".........##.....",
".........#a#....",
"..########aa#...",
"..#aaaaaaabaa#..",
"..#bbbbbbbbbaa#.",
"..#bbbbbbbbba#..",
"..########ba#c..",
"..ccccccc#a#c...",
"........c##c....",
"........c#c.....",
"........cc......",
"........c.......",
"................",
"................"};

/* XPM */
/* Drawn  by Mark Donohoe for the K Desktop Environment */
/* See http://www.kde.org */
static char*home[]={
"16 16 4 1",
"# c #000000",
"a c #ffffff",
"b c #c0c0c0",
". c None",
"........... ....",
"   ....##.......",
"..#...####......",
"..#..#aabb#.....",
"..#.#aaaabb#....",
"..##aaaaaabb#...",
"..#aaaaaaaabb#..",
".#aaaaaaaaabbb#.",
"###aaaaaaaabb###",
"..#aaaaaaaabb#..",
"..#aaa###aabb#..",
"..#aaa#.#aabb#..",
"..#aaa#.#aabb#..",
"..#aaa#.#aabb#..",
"..#aaa#.#aabb#..",
"..#####.######..",
"................"};

static char*findsearch[]={
"16 16 4 1",
"# c #000000",
"a c #ffffff",
"b c #c0c0c0",
". c None",
"................",
"................",
".....######.....",
"....##bbbb##....",
"...##bbbbbb##...",
"...##bbbbbb##...",
"...##bbbbbb##...",
"....##bbbb##....",
".....######.....",
"........###.....",
".........###....",
"..........###...",
"..........###...",
"...........###..",
"...........###..",
"................"};

static char*closeicon[]={
"16 16 4 1",
"# c #FF0000",
"a c #ffffff",
"b c #c0c0c0",
". c None",
"................",
"................",
"..##........##..",
"...##......##...",
"....##....##....",
".....##..##.....",
"......####......",
".......##.......",
".......##.......",
"......####......",
".....##..##.....",
"....##....##....",
"...##......##...",
"..##........##..",
"................",
"................"};

// *******************************************************************
// *******************************************************************
// *******************************************************************

HelpDlgImpl::HelpDlgImpl( QWidget* parent, const char* name, bool modal, WFlags fl )
: HelpDialog( parent, name, modal, fl )
{
	QPixmap aBackIcon( back );
	QPixmap aForwardIcon( forward );
	QPixmap aHomeIcon( home );
	QPixmap aFindIcon( findsearch );
	QPixmap aCloseIcon( closeicon );
	m_pBack->setIconSet( aBackIcon );
	m_pForward->setIconSet( aForwardIcon );
	m_pHome->setIconSet( aHomeIcon );
	m_pSearch->setIconSet( aFindIcon );
	m_pClose->setIconSet( aCloseIcon );

	m_iParagraph = 0;
	m_iIndex = 0;
}

HelpDlgImpl::~HelpDlgImpl()
{
}

void HelpDlgImpl::sltFindClicked()
{
	m_pTextBrowser->setCursorPosition( m_iParagraph, m_iIndex );

	if( m_pSearchText->text().length()>0 )
	{
		if( m_pTextBrowser->find( m_pSearchText->text(), FALSE, FALSE, TRUE, &m_iParagraph, &m_iIndex ) )
		{
			m_iIndex += m_pSearchText->text().length();
			m_pTextBrowser->setCursorPosition( m_iParagraph, m_iIndex );
		}
		else
		{
			QMessageBox::warning( this, tr( "MinDia - Warning" ), tr( "Text not found !" ) );
		}
	}
}

void HelpDlgImpl::sltSearchTextChanged(const QString & sText)
{
	// reset the position for search
	m_iParagraph = 0;
	m_iIndex = 0;
}



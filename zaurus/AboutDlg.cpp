/****************************************************************************
** Form implementation generated from reading ui file 'AboutDlg.ui'
**
** Created: Sat Jan 4 15:40:34 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "AboutDlg.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a AboutDlg which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AboutDlg::AboutDlg( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "AboutDlg" );
    resize( 200, 147 ); 
    setCaption( tr( "About" ) );
    AboutDlgLayout = new QVBoxLayout( this ); 
    AboutDlgLayout->setSpacing( 6 );
    AboutDlgLayout->setMargin( 11 );

    m_pNameLabel = new QLabel( this, "m_pNameLabel" );
    QFont m_pNameLabel_font(  m_pNameLabel->font() );
    m_pNameLabel_font.setPointSize( 16 );
    m_pNameLabel_font.setBold( TRUE );
    m_pNameLabel->setFont( m_pNameLabel_font ); 
    m_pNameLabel->setFrameShape( QLabel::MShape );
    m_pNameLabel->setFrameShadow( QLabel::MShadow );
    m_pNameLabel->setText( tr( "ZMinDia\n"
"Player for Zaurus" ) );
    m_pNameLabel->setAlignment( int( QLabel::AlignCenter ) );
    AboutDlgLayout->addWidget( m_pNameLabel );

    m_pVersionLabel = new QLabel( this, "m_pVersionLabel" );
    m_pVersionLabel->setText( tr( "Version 0.97.1 (3.1.2003)" ) );
    m_pVersionLabel->setAlignment( int( QLabel::AlignCenter ) );
    AboutDlgLayout->addWidget( m_pVersionLabel );

    m_pCopyrightLabel = new QLabel( this, "m_pCopyrightLabel" );
    QFont m_pCopyrightLabel_font(  m_pCopyrightLabel->font() );
    m_pCopyrightLabel->setFont( m_pCopyrightLabel_font ); 
    m_pCopyrightLabel->setFrameShape( QLabel::MShape );
    m_pCopyrightLabel->setText( tr( "(c) 2003 by Michael Neuroth" ) );
    m_pCopyrightLabel->setAlignment( int( QLabel::AlignCenter ) );
    AboutDlgLayout->addWidget( m_pCopyrightLabel );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
AboutDlg::~AboutDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*  
 *  Main event handler. Reimplemented to handle application
 *  font changes
 */
bool AboutDlg::event( QEvent* ev )
{
    bool ret = QDialog::event( ev ); 
    if ( ev->type() == QEvent::ApplicationFontChange ) {
	QFont m_pNameLabel_font(  m_pNameLabel->font() );
	m_pNameLabel_font.setPointSize( 16 );
	m_pNameLabel_font.setBold( TRUE );
	m_pNameLabel->setFont( m_pNameLabel_font ); 
	QFont m_pCopyrightLabel_font(  m_pCopyrightLabel->font() );
	m_pCopyrightLabel->setFont( m_pCopyrightLabel_font ); 
    }
    return ret;
}


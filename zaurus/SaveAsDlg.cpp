/****************************************************************************
** Form implementation generated from reading ui file 'SaveAsDlg.ui'
**
** Created: Sat Jan 4 15:40:26 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "SaveAsDlg.h"

#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a SaveAsDlg which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
SaveAsDlg::SaveAsDlg( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "SaveAsDlg" );
    resize( 222, 142 ); 
    setCaption( tr( "Save as" ) );
    SaveAsDlgLayout = new QGridLayout( this ); 
    SaveAsDlgLayout->setSpacing( 6 );
    SaveAsDlgLayout->setMargin( 11 );

    m_pFileNameLabel = new QLabel( this, "m_pFileNameLabel" );
    m_pFileNameLabel->setText( tr( "File name:" ) );

    SaveAsDlgLayout->addWidget( m_pFileNameLabel, 0, 0 );

    m_pFileName = new QLineEdit( this, "m_pFileName" );

    SaveAsDlgLayout->addWidget( m_pFileName, 0, 1 );

    m_pFolderLabel = new QLabel( this, "m_pFolderLabel" );
    m_pFolderLabel->setText( tr( "Folder:" ) );

    SaveAsDlgLayout->addWidget( m_pFolderLabel, 1, 0 );

    m_pTypeLabel = new QLabel( this, "m_pTypeLabel" );
    m_pTypeLabel->setText( tr( "Type:" ) );

    SaveAsDlgLayout->addWidget( m_pTypeLabel, 2, 0 );

    m_pFolder = new QComboBox( FALSE, this, "m_pFolder" );
    m_pFolder->insertItem( tr( "Internal Storage" ) );
    m_pFolder->insertItem( tr( "CF Card" ) );
    m_pFolder->insertItem( tr( "SD Card" ) );

    SaveAsDlgLayout->addWidget( m_pFolder, 1, 1 );

    m_pType = new QComboBox( FALSE, this, "m_pType" );
    m_pType->insertItem( tr( "text/python" ) );
    m_pType->insertItem( tr( "text/perl" ) );
    m_pType->insertItem( tr( "text/minscript" ) );
    m_pType->insertItem( tr( "text/plain" ) );

    SaveAsDlgLayout->addWidget( m_pType, 2, 1 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
SaveAsDlg::~SaveAsDlg()
{
    // no need to delete child widgets, Qt does it all for us
}


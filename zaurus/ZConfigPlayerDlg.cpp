/****************************************************************************
** Form implementation generated from reading ui file 'ZConfigPlayerDlg.ui'
**
** Created: Sat Jan 4 15:53:00 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "ZConfigPlayerDlg.h"

#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ConfigPlayerDlg which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ConfigPlayerDlg::ConfigPlayerDlg( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ConfigPlayerDlg" );
    resize( 220, 81 ); 
    setCaption( tr( "Player Configuration" ) );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout4" );
    privateLayoutWidget->setGeometry( QRect( 7, 80, 206, 30 ) ); 
    Layout4 = new QHBoxLayout( privateLayoutWidget ); 
    Layout4->setSpacing( 6 );
    Layout4->setMargin( 0 );

    m_pOk = new QPushButton( privateLayoutWidget, "m_pOk" );
    m_pOk->setText( tr( "&Ok" ) );
    Layout4->addWidget( m_pOk );

    m_pCancel = new QPushButton( privateLayoutWidget, "m_pCancel" );
    m_pCancel->setText( tr( "&Cancel" ) );
    Layout4->addWidget( m_pCancel );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout4->addItem( spacer );

    GroupBox4 = new QGroupBox( this, "GroupBox4" );
    GroupBox4->setGeometry( QRect( 4, 0, 212, 74 ) ); 
    GroupBox4->setTitle( tr( "mp3 player settings" ) );

    m_pPlayerLabel = new QLabel( GroupBox4, "m_pPlayerLabel" );
    m_pPlayerLabel->setGeometry( QRect( 8, 16, 55, 22 ) ); 
    m_pPlayerLabel->setText( tr( "player:" ) );

    m_pPlayeSettingsLabel = new QLabel( GroupBox4, "m_pPlayeSettingsLabel" );
    m_pPlayeSettingsLabel->setGeometry( QRect( 8, 43, 55, 22 ) ); 
    m_pPlayeSettingsLabel->setText( tr( "settings:" ) );

    m_pPlayerSettings = new QComboBox( FALSE, GroupBox4, "m_pPlayerSettings" );
    m_pPlayerSettings->setGeometry( QRect( 71, 43, 135, 22 ) ); 
    m_pPlayerSettings->setEditable( TRUE );

    m_pPlayer = new QComboBox( FALSE, GroupBox4, "m_pPlayer" );
    m_pPlayer->setGeometry( QRect( 71, 16, 135, 22 ) ); 
    m_pPlayer->setEditable( TRUE );

    // signals and slots connections
    connect( m_pPlayer, SIGNAL( activated(int) ), this, SLOT( sltPlayerChanged(int) ) );
    connect( m_pOk, SIGNAL( clicked() ), this, SLOT( sltOk() ) );
    connect( m_pCancel, SIGNAL( clicked() ), this, SLOT( sltCancel() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ConfigPlayerDlg::~ConfigPlayerDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

void ConfigPlayerDlg::sltCancel()
{
    qWarning( "ConfigPlayerDlg::sltCancel(): Not implemented yet!" );
}

void ConfigPlayerDlg::sltOk()
{
    qWarning( "ConfigPlayerDlg::sltOk(): Not implemented yet!" );
}

void ConfigPlayerDlg::sltPlayerChanged(int)
{
    qWarning( "ConfigPlayerDlg::sltPlayerChanged(int): Not implemented yet!" );
}


/****************************************************************************
** Form implementation generated from reading ui file 'ZProjectorControlDlg.ui'
**
** Created: Sat Jan 4 15:58:10 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "ZProjectorControlDlg.h"

#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ProjectorControlDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ProjectorControlDialog::ProjectorControlDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ProjectorControlDialog" );
    resize( 210, 274 ); 
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 32767, 32767 ) );
    setCaption( tr( "Projector Control" ) );

    m_pReset = new QPushButton( this, "m_pReset" );
    m_pReset->setGeometry( QRect( 156, 35, 48, 25 ) ); 
    m_pReset->setText( tr( "&Reset" ) );

    m_pInfoGroup = new QGroupBox( this, "m_pInfoGroup" );
    m_pInfoGroup->setGeometry( QRect( 2, 23, 151, 43 ) ); 
    m_pInfoGroup->setTitle( tr( "Info" ) );

    m_pSlideNoLabel = new QLabel( m_pInfoGroup, "m_pSlideNoLabel" );
    m_pSlideNoLabel->setGeometry( QRect( 80, 14, 65, 22 ) ); 
    m_pSlideNoLabel->setText( tr( "0" ) );
    m_pSlideNoLabel->setAlignment( int( QLabel::AlignCenter ) );

    m_pGetDiaNo = new QPushButton( m_pInfoGroup, "m_pGetDiaNo" );
    m_pGetDiaNo->setGeometry( QRect( 4, 15, 75, 24 ) ); 
    m_pGetDiaNo->setText( tr( "Get s&lide no" ) );

    m_pCloseButton = new QPushButton( this, "m_pCloseButton" );
    m_pCloseButton->setGeometry( QRect( 225, 200, 80, 26 ) ); 
    m_pCloseButton->setText( tr( "&Close" ) );

    m_pBack = new QPushButton( this, "m_pBack" );
    m_pBack->setGeometry( QRect( 3, 0, 60, 24 ) ); 
    m_pBack->setText( tr( "&Back" ) );

    m_pForward = new QPushButton( this, "m_pForward" );
    m_pForward->setGeometry( QRect( 74, 0, 60, 24 ) ); 
    m_pForward->setText( tr( "&Forward" ) );
    m_pForward->setDefault( TRUE );

    m_pDirectMode = new QPushButton( this, "m_pDirectMode" );
    m_pDirectMode->setGeometry( QRect( 144, 0, 60, 24 ) ); 
    m_pDirectMode->setText( tr( "&Dir. Mode" ) );

    m_pDissolve = new QButtonGroup( this, "m_pDissolve" );
    m_pDissolve->setGeometry( QRect( 2, 66, 122, 115 ) ); 
    m_pDissolve->setTitle( tr( "Dissolve" ) );

    m_pDissolve1 = new QRadioButton( m_pDissolve, "m_pDissolve1" );
    m_pDissolve1->setGeometry( QRect( 7, 17, 56, 17 ) ); 
    m_pDissolve1->setText( tr( "&0.5 s" ) );

    m_pStopGo = new QPushButton( m_pDissolve, "m_pStopGo" );
    m_pStopGo->setGeometry( QRect( 57, 15, 60, 24 ) ); 
    m_pStopGo->setText( tr( "&Stop/Go" ) );

    m_pDissolve2 = new QRadioButton( m_pDissolve, "m_pDissolve2" );
    m_pDissolve2->setGeometry( QRect( 7, 36, 53, 17 ) ); 
    m_pDissolve2->setText( tr( "&1.5 s" ) );

    m_pDissolve3 = new QRadioButton( m_pDissolve, "m_pDissolve3" );
    m_pDissolve3->setGeometry( QRect( 7, 55, 53, 17 ) ); 
    m_pDissolve3->setText( tr( "&3.0 s" ) );
    m_pDissolve3->setChecked( TRUE );

    m_pDissolve4 = new QRadioButton( m_pDissolve, "m_pDissolve4" );
    m_pDissolve4->setGeometry( QRect( 7, 74, 51, 17 ) ); 
    m_pDissolve4->setText( tr( "&6.0 s" ) );

    m_pDissolve5 = new QRadioButton( m_pDissolve, "m_pDissolve5" );
    m_pDissolve5->setGeometry( QRect( 7, 93, 61, 17 ) ); 
    m_pDissolve5->setText( tr( "1&2.0 s" ) );

    m_pLight = new QButtonGroup( this, "m_pLight" );
    m_pLight->setGeometry( QRect( 127, 66, 77, 115 ) ); 
    m_pLight->setTitle( tr( "Light" ) );

    m_pLightInt0 = new QRadioButton( m_pLight, "m_pLightInt0" );
    m_pLightInt0->setGeometry( QRect( 5, 20, 69, 17 ) ); 
    m_pLightInt0->setText( tr( "0 % (off)" ) );

    m_pLightInt1 = new QRadioButton( m_pLight, "m_pLightInt1" );
    m_pLightInt1->setGeometry( QRect( 5, 43, 69, 17 ) ); 
    m_pLightInt1->setText( tr( "80 %" ) );

    m_pLightInt2 = new QRadioButton( m_pLight, "m_pLightInt2" );
    m_pLightInt2->setGeometry( QRect( 5, 65, 69, 17 ) ); 
    m_pLightInt2->setText( tr( "90 %" ) );

    m_pLightInt3 = new QRadioButton( m_pLight, "m_pLightInt3" );
    m_pLightInt3->setGeometry( QRect( 5, 88, 65, 17 ) ); 
    m_pLightInt3->setText( tr( "100 %" ) );
    m_pLightInt3->setChecked( TRUE );

    m_pAutoFocus = new QButtonGroup( this, "m_pAutoFocus" );
    m_pAutoFocus->setGeometry( QRect( 2, 180, 122, 54 ) ); 
    m_pAutoFocus->setTitle( tr( "Autofocus" ) );

    m_pStatusLabel = new QLabel( m_pAutoFocus, "m_pStatusLabel" );
    m_pStatusLabel->setGeometry( QRect( 90, 59, 95, 20 ) ); 
    m_pStatusLabel->setText( tr( "unknown" ) );
    m_pStatusLabel->setAlignment( int( QLabel::AlignCenter ) );

    m_pAF_On = new QRadioButton( m_pAutoFocus, "m_pAF_On" );
    m_pAF_On->setGeometry( QRect( 7, 15, 42, 17 ) ); 
    m_pAF_On->setText( tr( "O&n" ) );
    m_pAF_On->setChecked( TRUE );

    m_pAF_Off = new QRadioButton( m_pAutoFocus, "m_pAF_Off" );
    m_pAF_Off->setGeometry( QRect( 7, 33, 42, 17 ) ); 
    m_pAF_Off->setText( tr( "&Off" ) );

    m_pAF_minus = new QPushButton( m_pAutoFocus, "m_pAF_minus" );
    m_pAF_minus->setGeometry( QRect( 53, 26, 30, 24 ) ); 
    m_pAF_minus->setText( tr( "&-" ) );

    m_pAF_plus = new QPushButton( m_pAutoFocus, "m_pAF_plus" );
    m_pAF_plus->setGeometry( QRect( 84, 26, 30, 24 ) ); 
    m_pAF_plus->setText( tr( "&+" ) );

    m_pStatusButton = new QPushButton( this, "m_pStatusButton" );
    m_pStatusButton->setGeometry( QRect( 135, 210, 60, 24 ) ); 
    m_pStatusButton->setText( tr( "St&atus" ) );

    m_pTestButton = new QPushButton( this, "m_pTestButton" );
    m_pTestButton->setGeometry( QRect( 135, 185, 60, 24 ) ); 
    m_pTestButton->setText( tr( "&Test" ) );

    m_pLastCmdLabel = new QLabel( this, "m_pLastCmdLabel" );
    m_pLastCmdLabel->setGeometry( QRect( 4, 252, 70, 20 ) ); 
    m_pLastCmdLabel->setText( tr( "Last cmd:" ) );
    m_pLastCmdLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    m_pModeLabel = new QLabel( this, "m_pModeLabel" );
    m_pModeLabel->setGeometry( QRect( 7, 235, 53, 20 ) ); 
    m_pModeLabel->setText( tr( "Mode:" ) );
    m_pModeLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    m_pMode = new QLabel( this, "m_pMode" );
    m_pMode->setGeometry( QRect( 65, 235, 125, 19 ) ); 
    m_pMode->setText( tr( "not connected" ) );
    m_pMode->setAlignment( int( QLabel::AlignCenter ) );

    m_pLastCmd = new QLabel( this, "m_pLastCmd" );
    m_pLastCmd->setGeometry( QRect( 74, 250, 130, 20 ) ); 
    m_pLastCmd->setText( tr( "ok" ) );
    m_pLastCmd->setAlignment( int( QLabel::AlignCenter ) );

    // signals and slots connections
    connect( m_pBack, SIGNAL( clicked() ), this, SLOT( sltSlideBack() ) );
    connect( m_pForward, SIGNAL( clicked() ), this, SLOT( sltSlideForward() ) );
    connect( m_pStopGo, SIGNAL( clicked() ), this, SLOT( sltStopGo() ) );
    connect( m_pReset, SIGNAL( clicked() ), this, SLOT( sltResetProjector() ) );
    connect( m_pDirectMode, SIGNAL( clicked() ), this, SLOT( sltDirectMode() ) );
    connect( m_pGetDiaNo, SIGNAL( clicked() ), this, SLOT( sltGetSlideNo() ) );
    connect( m_pAF_plus, SIGNAL( clicked() ), this, SLOT( sltAFForward() ) );
    connect( m_pAF_minus, SIGNAL( clicked() ), this, SLOT( sltAFBackward() ) );
    connect( m_pAF_On, SIGNAL( toggled(bool) ), this, SLOT( sltAutofocusOnToggled( bool ) ) );
    connect( m_pAF_Off, SIGNAL( toggled(bool) ), this, SLOT( sltAutofocusOffToggled( bool ) ) );
    connect( m_pDissolve1, SIGNAL( toggled(bool) ), this, SLOT( sltDissolve1Toggled( bool ) ) );
    connect( m_pDissolve2, SIGNAL( toggled(bool) ), this, SLOT( sltDissolve2Toggled( bool ) ) );
    connect( m_pDissolve3, SIGNAL( toggled(bool) ), this, SLOT( sltDissolve3Toggled( bool ) ) );
    connect( m_pDissolve4, SIGNAL( toggled(bool) ), this, SLOT( sltDissolve4Toggled( bool ) ) );
    connect( m_pDissolve5, SIGNAL( toggled(bool) ), this, SLOT( sltDissolve5Toggled( bool ) ) );
    connect( m_pLightInt0, SIGNAL( toggled(bool) ), this, SLOT( sltLight1Toggled( bool ) ) );
    connect( m_pLightInt1, SIGNAL( toggled(bool) ), this, SLOT( sltLight2Toggled( bool ) ) );
    connect( m_pLightInt2, SIGNAL( toggled(bool) ), this, SLOT( sltLight3Toggled( bool ) ) );
    connect( m_pLightInt3, SIGNAL( toggled(bool) ), this, SLOT( sltLight4Toggled( bool ) ) );
    connect( m_pCloseButton, SIGNAL( clicked() ), this, SLOT( sltCloseDialog() ) );
    connect( m_pTestButton, SIGNAL( clicked() ), this, SLOT( sltTestProjector() ) );
    connect( m_pTestButton, SIGNAL( clicked() ), this, SLOT( sltTestProjector() ) );
    connect( m_pStatusButton, SIGNAL( clicked() ), this, SLOT( sltGetStatus() ) );

    // tab order
    setTabOrder( m_pForward, m_pBack );
    setTabOrder( m_pBack, m_pDirectMode );
    setTabOrder( m_pDirectMode, m_pReset );
    setTabOrder( m_pReset, m_pStopGo );
    setTabOrder( m_pStopGo, m_pDissolve3 );
    setTabOrder( m_pDissolve3, m_pLightInt3 );
    setTabOrder( m_pLightInt3, m_pAF_On );
    setTabOrder( m_pAF_On, m_pAF_plus );
    setTabOrder( m_pAF_plus, m_pAF_minus );
    setTabOrder( m_pAF_minus, m_pCloseButton );
    setTabOrder( m_pCloseButton, m_pGetDiaNo );
    setTabOrder( m_pGetDiaNo, m_pLightInt2 );
    setTabOrder( m_pLightInt2, m_pLightInt0 );
    setTabOrder( m_pLightInt0, m_pLightInt1 );
    setTabOrder( m_pLightInt1, m_pDissolve4 );
    setTabOrder( m_pDissolve4, m_pDissolve5 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ProjectorControlDialog::~ProjectorControlDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

void ProjectorControlDialog::sltGetStatus()
{
    qWarning( "ProjectorControlDialog::sltGetStatus(): Not implemented yet!" );
}

void ProjectorControlDialog::sltAFBackward()
{
    qWarning( "ProjectorControlDialog::sltAFBackward(): Not implemented yet!" );
}

void ProjectorControlDialog::sltAFForward()
{
    qWarning( "ProjectorControlDialog::sltAFForward(): Not implemented yet!" );
}

void ProjectorControlDialog::sltAutofocusOffToggled( bool )
{
    qWarning( "ProjectorControlDialog::sltAutofocusOffToggled( bool ): Not implemented yet!" );
}

void ProjectorControlDialog::sltAutofocusOnToggled( bool )
{
    qWarning( "ProjectorControlDialog::sltAutofocusOnToggled( bool ): Not implemented yet!" );
}

void ProjectorControlDialog::sltCheckMode()
{
    qWarning( "ProjectorControlDialog::sltCheckMode(): Not implemented yet!" );
}

void ProjectorControlDialog::sltCloseDialog()
{
    qWarning( "ProjectorControlDialog::sltCloseDialog(): Not implemented yet!" );
}

void ProjectorControlDialog::sltDirectMode()
{
    qWarning( "ProjectorControlDialog::sltDirectMode(): Not implemented yet!" );
}

void ProjectorControlDialog::sltDissolve1Toggled( bool )
{
    qWarning( "ProjectorControlDialog::sltDissolve1Toggled( bool ): Not implemented yet!" );
}

void ProjectorControlDialog::sltDissolve2Toggled( bool )
{
    qWarning( "ProjectorControlDialog::sltDissolve2Toggled( bool ): Not implemented yet!" );
}

void ProjectorControlDialog::sltDissolve3Toggled( bool )
{
    qWarning( "ProjectorControlDialog::sltDissolve3Toggled( bool ): Not implemented yet!" );
}

void ProjectorControlDialog::sltDissolve4Toggled( bool )
{
    qWarning( "ProjectorControlDialog::sltDissolve4Toggled( bool ): Not implemented yet!" );
}

void ProjectorControlDialog::sltDissolve5Toggled( bool )
{
    qWarning( "ProjectorControlDialog::sltDissolve5Toggled( bool ): Not implemented yet!" );
}

void ProjectorControlDialog::sltGetSlideNo()
{
    qWarning( "ProjectorControlDialog::sltGetSlideNo(): Not implemented yet!" );
}

void ProjectorControlDialog::sltLight1Toggled( bool )
{
    qWarning( "ProjectorControlDialog::sltLight1Toggled( bool ): Not implemented yet!" );
}

void ProjectorControlDialog::sltLight2Toggled( bool )
{
    qWarning( "ProjectorControlDialog::sltLight2Toggled( bool ): Not implemented yet!" );
}

void ProjectorControlDialog::sltLight3Toggled( bool )
{
    qWarning( "ProjectorControlDialog::sltLight3Toggled( bool ): Not implemented yet!" );
}

void ProjectorControlDialog::sltLight4Toggled( bool )
{
    qWarning( "ProjectorControlDialog::sltLight4Toggled( bool ): Not implemented yet!" );
}

void ProjectorControlDialog::sltResetProjector()
{
    qWarning( "ProjectorControlDialog::sltResetProjector(): Not implemented yet!" );
}

void ProjectorControlDialog::sltSlideBack()
{
    qWarning( "ProjectorControlDialog::sltSlideBack(): Not implemented yet!" );
}

void ProjectorControlDialog::sltSlideForward()
{
    qWarning( "ProjectorControlDialog::sltSlideForward(): Not implemented yet!" );
}

void ProjectorControlDialog::sltStopGo()
{
    qWarning( "ProjectorControlDialog::sltStopGo(): Not implemented yet!" );
}

void ProjectorControlDialog::sltTestProjector()
{
    qWarning( "ProjectorControlDialog::sltTestProjector(): Not implemented yet!" );
}


/****************************************************************************
** Form implementation generated from reading ui file 'ZDiaInfoDlg.ui'
**
** Created: Sat Jan 4 15:40:57 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "ZDiaInfoDlg.h"

#include <qbuttongroup.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmultilineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a DiaInfoDlg which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
DiaInfoDlg::DiaInfoDlg( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "DiaInfoDlg" );
    resize( 231, 193 ); 
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 32767, 32767 ) );
    setBaseSize( QSize( 0, 0 ) );
    setCaption( tr( "Dia Info" ) );
    setSizeGripEnabled( TRUE );

    m_pFileNameButton = new QPushButton( this, "m_pFileNameButton" );
    m_pFileNameButton->setGeometry( QRect( 333, 74, 80, 28 ) ); 
    m_pFileNameButton->setText( tr( "&..." ) );

    m_pSlideFormat = new QButtonGroup( this, "m_pSlideFormat" );
    m_pSlideFormat->setGeometry( QRect( 290, 108, 123, 76 ) ); 
    m_pSlideFormat->setTitle( tr( "Format" ) );
    m_pSlideFormat->setColumnLayout(0, Qt::Vertical );
    m_pSlideFormat->layout()->setSpacing( 0 );
    m_pSlideFormat->layout()->setMargin( 0 );
    m_pSlideFormatLayout = new QGridLayout( m_pSlideFormat->layout() );
    m_pSlideFormatLayout->setAlignment( Qt::AlignTop );
    m_pSlideFormatLayout->setSpacing( 6 );
    m_pSlideFormatLayout->setMargin( 11 );

    Layout25 = new QVBoxLayout; 
    Layout25->setSpacing( 6 );
    Layout25->setMargin( 0 );

    m_pHorizontalFormat = new QRadioButton( m_pSlideFormat, "m_pHorizontalFormat" );
    m_pHorizontalFormat->setText( tr( "hori&zontal" ) );
    m_pHorizontalFormat->setChecked( TRUE );
    Layout25->addWidget( m_pHorizontalFormat );

    m_pVerticalFormat = new QRadioButton( m_pSlideFormat, "m_pVerticalFormat" );
    m_pVerticalFormat->setText( tr( "&vertical" ) );
    Layout25->addWidget( m_pVerticalFormat );

    m_pSlideFormatLayout->addLayout( Layout25, 0, 0 );

    m_pScript = new QMultiLineEdit( this, "m_pScript" );
    m_pScript->setGeometry( QRect( 100, 411, 315, 51 ) ); 
    m_pScript->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, m_pScript->sizePolicy().hasHeightForWidth() ) );
    m_pScript->setReadOnly( TRUE );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout10" );
    privateLayoutWidget->setGeometry( QRect( 10, 409, 82, 51 ) ); 
    Layout10 = new QVBoxLayout( privateLayoutWidget ); 
    Layout10->setSpacing( 6 );
    Layout10->setMargin( 0 );

    m_pScriptLabel = new QLabel( privateLayoutWidget, "m_pScriptLabel" );
    m_pScriptLabel->setText( tr( "Script:" ) );
    m_pScriptLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    Layout10->addWidget( m_pScriptLabel );

    m_pModifyScript = new QPushButton( privateLayoutWidget, "m_pModifyScript" );
    m_pModifyScript->setText( tr( "&Modify..." ) );
    Layout10->addWidget( m_pModifyScript );

    m_pEffectButtonGroup = new QButtonGroup( this, "m_pEffectButtonGroup" );
    m_pEffectButtonGroup->setGeometry( QRect( 8, 280, 403, 78 ) ); 
    m_pEffectButtonGroup->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, m_pEffectButtonGroup->sizePolicy().hasHeightForWidth() ) );
    m_pEffectButtonGroup->setTitle( tr( "Effects" ) );
    m_pEffectButtonGroup->setColumnLayout(0, Qt::Vertical );
    m_pEffectButtonGroup->layout()->setSpacing( 0 );
    m_pEffectButtonGroup->layout()->setMargin( 0 );
    m_pEffectButtonGroupLayout = new QHBoxLayout( m_pEffectButtonGroup->layout() );
    m_pEffectButtonGroupLayout->setAlignment( Qt::AlignTop );
    m_pEffectButtonGroupLayout->setSpacing( 6 );
    m_pEffectButtonGroupLayout->setMargin( 11 );

    Layout21 = new QHBoxLayout; 
    Layout21->setSpacing( 6 );
    Layout21->setMargin( 0 );

    Layout19 = new QGridLayout; 
    Layout19->setSpacing( 6 );
    Layout19->setMargin( 0 );

    m_pFreeze = new QRadioButton( m_pEffectButtonGroup, "m_pFreeze" );
    m_pFreeze->setText( tr( "f&reeze" ) );
    m_pEffectButtonGroup->insert( m_pFreeze, -1 );

    Layout19->addWidget( m_pFreeze, 0, 2 );

    m_pDarkPhase = new QRadioButton( m_pEffectButtonGroup, "m_pDarkPhase" );
    m_pDarkPhase->setText( tr( "&dark phase" ) );
    m_pEffectButtonGroup->insert( m_pDarkPhase, -1 );

    Layout19->addWidget( m_pDarkPhase, 1, 0 );

    m_pFlash = new QRadioButton( m_pEffectButtonGroup, "m_pFlash" );
    m_pFlash->setText( tr( "&flash" ) );
    m_pEffectButtonGroup->insert( m_pFlash, -1 );

    Layout19->addWidget( m_pFlash, 0, 1 );

    m_pFadeIn = new QRadioButton( m_pEffectButtonGroup, "m_pFadeIn" );
    m_pFadeIn->setText( tr( "fade &in" ) );
    m_pEffectButtonGroup->insert( m_pFadeIn, -1 );

    Layout19->addWidget( m_pFadeIn, 1, 2 );

    m_pNoEffect = new QRadioButton( m_pEffectButtonGroup, "m_pNoEffect" );
    m_pNoEffect->setText( tr( "&no effect" ) );
    m_pNoEffect->setAccel( 276824142 );
    m_pNoEffect->setChecked( TRUE );

    Layout19->addWidget( m_pNoEffect, 0, 0 );

    m_pSwing = new QRadioButton( m_pEffectButtonGroup, "m_pSwing" );
    m_pSwing->setText( tr( "&swing" ) );
    m_pEffectButtonGroup->insert( m_pSwing, -1 );

    Layout19->addWidget( m_pSwing, 1, 1 );
    Layout21->addLayout( Layout19 );

    Layout20 = new QGridLayout; 
    Layout20->setSpacing( 6 );
    Layout20->setMargin( 0 );

    m_pEffectCount = new QLineEdit( m_pEffectButtonGroup, "m_pEffectCount" );

    Layout20->addWidget( m_pEffectCount, 0, 1 );

    m_pEffectCountLabel = new QLabel( m_pEffectButtonGroup, "m_pEffectCountLabel" );
    m_pEffectCountLabel->setText( tr( "count:" ) );
    m_pEffectCountLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    Layout20->addWidget( m_pEffectCountLabel, 0, 0 );
    Layout21->addLayout( Layout20 );
    m_pEffectButtonGroupLayout->addLayout( Layout21 );

    CommenLabel = new QLabel( this, "CommenLabel" );
    CommenLabel->setGeometry( QRect( 4, 29, 61, 16 ) ); 
    CommenLabel->setText( tr( "Comment:" ) );
    CommenLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    FileNameLabel = new QLabel( this, "FileNameLabel" );
    FileNameLabel->setGeometry( QRect( 9, 49, 55, 19 ) ); 
    FileNameLabel->setText( tr( "Filename:" ) );
    FileNameLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    IDLabel = new QLabel( this, "IDLabel" );
    IDLabel->setGeometry( QRect( 9, 7, 56, 19 ) ); 
    IDLabel->setText( tr( "Id:" ) );
    IDLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    TimerLabel = new QLabel( this, "TimerLabel" );
    TimerLabel->setGeometry( QRect( 7, 89, 60, 19 ) ); 
    TimerLabel->setText( tr( "Timer [s]:" ) );
    TimerLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    DissolveLabel = new QLabel( this, "DissolveLabel" );
    DissolveLabel->setGeometry( QRect( -2, 69, 70, 19 ) ); 
    DissolveLabel->setText( tr( "Dissolve [s]:" ) );
    DissolveLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    m_pPrevious = new QPushButton( this, "m_pPrevious" );
    m_pPrevious->setGeometry( QRect( 149, 3, 35, 24 ) ); 
    m_pPrevious->setText( tr( "&<--" ) );
    m_pPrevious->setAccel( 4198418 );

    m_pNext = new QPushButton( this, "m_pNext" );
    m_pNext->setGeometry( QRect( 188, 3, 35, 24 ) ); 
    m_pNext->setText( tr( "--&>" ) );
    m_pNext->setAccel( 4198420 );

    m_pCommentEdit = new QLineEdit( this, "m_pCommentEdit" );
    m_pCommentEdit->setGeometry( QRect( 67, 28, 157, 22 ) ); 

    m_pIDEdit = new QLineEdit( this, "m_pIDEdit" );
    m_pIDEdit->setGeometry( QRect( 67, 4, 79, 22 ) ); 

    m_pFileNameEdit = new QLineEdit( this, "m_pFileNameEdit" );
    m_pFileNameEdit->setGeometry( QRect( 67, 53, 157, 22 ) ); 

    m_pDissolveEdit = new QLineEdit( this, "m_pDissolveEdit" );
    m_pDissolveEdit->setGeometry( QRect( 67, 77, 158, 22 ) ); 

    m_pTimerEdit = new QLineEdit( this, "m_pTimerEdit" );
    m_pTimerEdit->setGeometry( QRect( 66, 100, 159, 22 ) ); 

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setGeometry( QRect( 244, 220, 80, 28 ) ); 
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( FALSE );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setGeometry( QRect( 330, 220, 80, 28 ) ); 
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setAutoDefault( TRUE );

    m_pApply = new QPushButton( this, "m_pApply" );
    m_pApply->setGeometry( QRect( 7, 128, 60, 25 ) ); 
    m_pApply->setText( tr( "&Apply" ) );
    m_pApply->setAutoDefault( TRUE );
    m_pApply->setDefault( TRUE );

    m_pApplyAndPrev = new QPushButton( this, "m_pApplyAndPrev" );
    m_pApplyAndPrev->setGeometry( QRect( 72, 128, 100, 25 ) ); 
    m_pApplyAndPrev->setText( tr( "Apply and pre&v." ) );
    m_pApplyAndPrev->setAutoDefault( TRUE );
    m_pApplyAndPrev->setDefault( TRUE );

    m_pApplyAndNext = new QPushButton( this, "m_pApplyAndNext" );
    m_pApplyAndNext->setGeometry( QRect( 71, 158, 100, 25 ) ); 
    m_pApplyAndNext->setText( tr( "Apply and ne&xt" ) );
    m_pApplyAndNext->setAutoDefault( TRUE );
    m_pApplyAndNext->setDefault( TRUE );

    m_pNewItem = new QPushButton( this, "m_pNewItem" );
    m_pNewItem->setGeometry( QRect( 7, 157, 60, 25 ) ); 
    m_pNewItem->setText( tr( "&New dia" ) );
    m_pNewItem->setAutoDefault( TRUE );

    // signals and slots connections
    connect( m_pApply, SIGNAL( clicked() ), this, SLOT( sltApplyData() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( sltCloseDialog() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( sltDialogCanceled() ) );
    connect( m_pIDEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sltTextChanged( const QString &) ) );
    connect( m_pFileNameEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sltTextChanged( const QString &) ) );
    connect( m_pCommentEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sltTextChanged( const QString &) ) );
    connect( m_pDissolveEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sltTextChanged( const QString &) ) );
    connect( m_pTimerEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( sltTextChanged( const QString &) ) );
    connect( m_pFileNameButton, SIGNAL( clicked() ), this, SLOT( sltSelectFileName() ) );
    connect( m_pPrevious, SIGNAL( clicked() ), this, SLOT( sltPrevItem() ) );
    connect( m_pNext, SIGNAL( clicked() ), this, SLOT( sltNextItem() ) );
    connect( m_pHorizontalFormat, SIGNAL( stateChanged(int) ), this, SLOT( sltDataChanged() ) );
    connect( m_pVerticalFormat, SIGNAL( stateChanged(int) ), this, SLOT( sltDataChanged() ) );
    connect( m_pNoEffect, SIGNAL( stateChanged(int) ), this, SLOT( sltDataChanged() ) );
    connect( m_pDarkPhase, SIGNAL( stateChanged(int) ), this, SLOT( sltDataChanged() ) );
    connect( m_pFlash, SIGNAL( stateChanged(int) ), this, SLOT( sltDataChanged() ) );
    connect( m_pSwing, SIGNAL( stateChanged(int) ), this, SLOT( sltDataChanged() ) );
    connect( m_pFreeze, SIGNAL( stateChanged(int) ), this, SLOT( sltDataChanged() ) );
    connect( m_pFadeIn, SIGNAL( stateChanged(int) ), this, SLOT( sltDataChanged() ) );
    connect( m_pEffectCount, SIGNAL( textChanged(const QString&) ), this, SLOT( sltTextChanged( const QString &) ) );
    connect( m_pNewItem, SIGNAL( clicked() ), this, SLOT( sltCreateNewItem() ) );
    connect( m_pApplyAndNext, SIGNAL( clicked() ), this, SLOT( sltApplyAndNext() ) );
    connect( m_pApplyAndPrev, SIGNAL( clicked() ), this, SLOT( sltApplyAndPrev() ) );
    connect( m_pModifyScript, SIGNAL( clicked() ), this, SLOT( sltModifyScript() ) );
    connect( m_pScript, SIGNAL( textChanged() ), this, SLOT( sltTextChanged() ) );

    // tab order
    setTabOrder( m_pNoEffect, m_pIDEdit );
    setTabOrder( m_pIDEdit, m_pPrevious );
    setTabOrder( m_pPrevious, m_pNext );
    setTabOrder( m_pNext, m_pFileNameEdit );
    setTabOrder( m_pFileNameEdit, m_pFileNameButton );
    setTabOrder( m_pFileNameButton, m_pCommentEdit );
    setTabOrder( m_pCommentEdit, m_pDissolveEdit );
    setTabOrder( m_pDissolveEdit, m_pTimerEdit );
    setTabOrder( m_pTimerEdit, m_pEffectCount );
    setTabOrder( m_pEffectCount, m_pNewItem );
    setTabOrder( m_pNewItem, m_pApply );
    setTabOrder( m_pApply, buttonOk );
    setTabOrder( buttonOk, buttonCancel );
    setTabOrder( buttonCancel, m_pDarkPhase );
    setTabOrder( m_pDarkPhase, m_pFlash );
    setTabOrder( m_pFlash, m_pSwing );
    setTabOrder( m_pSwing, m_pFreeze );
    setTabOrder( m_pFreeze, m_pFadeIn );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
DiaInfoDlg::~DiaInfoDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

void DiaInfoDlg::sltTextChanged()
{
    qWarning( "DiaInfoDlg::sltTextChanged(): Not implemented yet!" );
}

void DiaInfoDlg::sltApplyAndNext()
{
    qWarning( "DiaInfoDlg::sltApplyAndNext(): Not implemented yet!" );
}

void DiaInfoDlg::sltApplyAndPrev()
{
    qWarning( "DiaInfoDlg::sltApplyAndPrev(): Not implemented yet!" );
}

void DiaInfoDlg::sltApplyData()
{
    qWarning( "DiaInfoDlg::sltApplyData(): Not implemented yet!" );
}

void DiaInfoDlg::sltCloseDialog()
{
    qWarning( "DiaInfoDlg::sltCloseDialog(): Not implemented yet!" );
}

void DiaInfoDlg::sltCreateNewItem()
{
    qWarning( "DiaInfoDlg::sltCreateNewItem(): Not implemented yet!" );
}

void DiaInfoDlg::sltDataChanged()
{
    qWarning( "DiaInfoDlg::sltDataChanged(): Not implemented yet!" );
}

void DiaInfoDlg::sltDialogCanceled()
{
    qWarning( "DiaInfoDlg::sltDialogCanceled(): Not implemented yet!" );
}

void DiaInfoDlg::sltDisableDialog()
{
    qWarning( "DiaInfoDlg::sltDisableDialog(): Not implemented yet!" );
}

void DiaInfoDlg::sltModifyScript()
{
    qWarning( "DiaInfoDlg::sltModifyScript(): Not implemented yet!" );
}

void DiaInfoDlg::sltNextItem()
{
    qWarning( "DiaInfoDlg::sltNextItem(): Not implemented yet!" );
}

void DiaInfoDlg::sltPrevItem()
{
    qWarning( "DiaInfoDlg::sltPrevItem(): Not implemented yet!" );
}

void DiaInfoDlg::sltScriptFlagToggled(bool)
{
    qWarning( "DiaInfoDlg::sltScriptFlagToggled(bool): Not implemented yet!" );
}

void DiaInfoDlg::sltSelectFileName()
{
    qWarning( "DiaInfoDlg::sltSelectFileName(): Not implemented yet!" );
}

void DiaInfoDlg::sltTextChanged( const QString &)
{
    qWarning( "DiaInfoDlg::sltTextChanged( const QString &): Not implemented yet!" );
}

void DiaInfoDlg::sltUpdateData(HItem * pFirstSelectedItem, bool bEnable )
{
    qWarning( "DiaInfoDlg::sltUpdateData(HItem * pFirstSelectedItem, bool bEnable ): Not implemented yet!" );
}


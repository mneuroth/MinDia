/****************************************************************************
** Form implementation generated from reading ui file 'ZConfigurationDlg.ui'
**
** Created: Sat Jan 4 15:40:40 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "ZConfigurationDlg.h"

#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ConfigurationDlg which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ConfigurationDlg::ConfigurationDlg( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ConfigurationDlg" );
    resize( 215, 244 ); 
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 0, 0 ) );
    setMaximumSize( QSize( 32767, 32767 ) );
    setCaption( tr( "Configuration" ) );

    m_pProjectorType = new QButtonGroup( this, "m_pProjectorType" );
    m_pProjectorType->setGeometry( QRect( 1, -1, 211, 39 ) ); 
    m_pProjectorType->setTitle( tr( "Projector type" ) );

    m_pDigitalTwinP = new QRadioButton( m_pProjectorType, "m_pDigitalTwinP" );
    m_pDigitalTwinP->setGeometry( QRect( 6, 16, 100, 17 ) ); 
    m_pDigitalTwinP->setText( tr( "&Twin digital P" ) );

    m_pMSC300P = new QRadioButton( m_pProjectorType, "m_pMSC300P" );
    m_pMSC300P->setGeometry( QRect( 110, 16, 92, 17 ) ); 
    m_pMSC300P->setText( tr( "&MSC 3x0 P" ) );

    m_pComPortGroup = new QGroupBox( this, "m_pComPortGroup" );
    m_pComPortGroup->setGeometry( QRect( 1, 37, 210, 163 ) ); 
    m_pComPortGroup->setTitle( tr( "Com port settings" ) );

    m_pComPortLabel = new QLabel( m_pComPortGroup, "m_pComPortLabel" );
    m_pComPortLabel->setGeometry( QRect( 12, 16, 95, 20 ) ); 
    m_pComPortLabel->setText( tr( "Port:" ) );
    m_pComPortLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    m_pBaudRateLabel = new QLabel( m_pComPortGroup, "m_pBaudRateLabel" );
    m_pBaudRateLabel->setGeometry( QRect( 12, 40, 95, 20 ) ); 
    m_pBaudRateLabel->setText( tr( "Baudrate:" ) );
    m_pBaudRateLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    m_pDataBitsLabel = new QLabel( m_pComPortGroup, "m_pDataBitsLabel" );
    m_pDataBitsLabel->setGeometry( QRect( 12, 64, 95, 20 ) ); 
    m_pDataBitsLabel->setText( tr( "Databits:" ) );
    m_pDataBitsLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    m_pStopBitsLabel = new QLabel( m_pComPortGroup, "m_pStopBitsLabel" );
    m_pStopBitsLabel->setGeometry( QRect( 12, 88, 95, 20 ) ); 
    m_pStopBitsLabel->setText( tr( "Stopbits:" ) );
    m_pStopBitsLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    m_pParityLabel = new QLabel( m_pComPortGroup, "m_pParityLabel" );
    m_pParityLabel->setGeometry( QRect( 12, 112, 95, 20 ) ); 
    m_pParityLabel->setText( tr( "Parity:" ) );
    m_pParityLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    m_pFlowControlLabel = new QLabel( m_pComPortGroup, "m_pFlowControlLabel" );
    m_pFlowControlLabel->setGeometry( QRect( 12, 136, 95, 20 ) ); 
    m_pFlowControlLabel->setText( tr( "Flowcontrol:" ) );
    m_pFlowControlLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    m_pWriteTimeoutLabel = new QLabel( m_pComPortGroup, "m_pWriteTimeoutLabel" );
    m_pWriteTimeoutLabel->setGeometry( QRect( 3, 184, 104, 20 ) ); 
    m_pWriteTimeoutLabel->setText( tr( "Write-timeout [ms]:" ) );
    m_pWriteTimeoutLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    m_pComPortList = new QComboBox( FALSE, m_pComPortGroup, "m_pComPortList" );
    m_pComPortList->setGeometry( QRect( 113, 15, 91, 22 ) ); 

    m_pWriteTimeout = new QLineEdit( m_pComPortGroup, "m_pWriteTimeout" );
    m_pWriteTimeout->setGeometry( QRect( 113, 183, 79, 22 ) ); 

    m_pDataBitsList = new QComboBox( FALSE, m_pComPortGroup, "m_pDataBitsList" );
    m_pDataBitsList->setGeometry( QRect( 113, 63, 81, 22 ) ); 

    m_pStopBitsList = new QComboBox( FALSE, m_pComPortGroup, "m_pStopBitsList" );
    m_pStopBitsList->setGeometry( QRect( 113, 87, 81, 22 ) ); 

    m_pParityList = new QComboBox( FALSE, m_pComPortGroup, "m_pParityList" );
    m_pParityList->setGeometry( QRect( 113, 111, 81, 22 ) ); 

    m_pFlowControlList = new QComboBox( FALSE, m_pComPortGroup, "m_pFlowControlList" );
    m_pFlowControlList->setGeometry( QRect( 113, 135, 81, 22 ) ); 

    m_pBaudRateList = new QComboBox( FALSE, m_pComPortGroup, "m_pBaudRateList" );
    m_pBaudRateList->setGeometry( QRect( 113, 39, 81, 22 ) ); 

    m_pReadTimeout = new QLineEdit( m_pComPortGroup, "m_pReadTimeout" );
    m_pReadTimeout->setGeometry( QRect( 113, 163, 79, 22 ) ); 

    m_pReadTimeoutLabel = new QLabel( m_pComPortGroup, "m_pReadTimeoutLabel" );
    m_pReadTimeoutLabel->setGeometry( QRect( 2, 163, 105, 20 ) ); 
    m_pReadTimeoutLabel->setText( tr( "Read-timeout [ms]:" ) );
    m_pReadTimeoutLabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    m_pModusGroup = new QGroupBox( this, "m_pModusGroup" );
    m_pModusGroup->setGeometry( QRect( 2, 199, 104, 39 ) ); 
    m_pModusGroup->setTitle( tr( "Modus" ) );

    m_pExecuteEvents = new QCheckBox( m_pModusGroup, "m_pExecuteEvents" );
    m_pExecuteEvents->setGeometry( QRect( 8, 41, 89, 17 ) ); 
    m_pExecuteEvents->setText( tr( "&Execute sript events" ) );

    m_pSimulation = new QCheckBox( m_pModusGroup, "m_pSimulation" );
    m_pSimulation->setGeometry( QRect( 8, 15, 88, 17 ) ); 
    m_pSimulation->setText( tr( "&Simulation" ) );

    m_pDebugGroup = new QGroupBox( this, "m_pDebugGroup" );
    m_pDebugGroup->setGeometry( QRect( 112, 199, 99, 39 ) ); 
    m_pDebugGroup->setTitle( tr( "Debug" ) );

    m_pDbgLogging = new QCheckBox( m_pDebugGroup, "m_pDbgLogging" );
    m_pDbgLogging->setGeometry( QRect( 12, 15, 77, 17 ) ); 
    m_pDbgLogging->setText( tr( "&Logging" ) );

    // signals and slots connections
    connect( m_pSimulation, SIGNAL( toggled(bool) ), this, SLOT( sltSimulationToggled(bool) ) );
    connect( m_pDbgLogging, SIGNAL( toggled(bool) ), this, SLOT( sltDebugLoggingToggled(bool) ) );
    connect( m_pDigitalTwinP, SIGNAL( toggled(bool) ), this, SLOT( sltTwinDigitalPToggled(bool) ) );
    connect( m_pMSC300P, SIGNAL( toggled(bool) ), this, SLOT( sltMSC300PToggled(bool) ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ConfigurationDlg::~ConfigurationDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

void ConfigurationDlg::sltMSC300PToggled(bool)
{
    qWarning( "ConfigurationDlg::sltMSC300PToggled(bool): Not implemented yet!" );
}

void ConfigurationDlg::sltCloseDialog()
{
    qWarning( "ConfigurationDlg::sltCloseDialog(): Not implemented yet!" );
}

void ConfigurationDlg::sltDebugLoggingToggled(bool)
{
    qWarning( "ConfigurationDlg::sltDebugLoggingToggled(bool): Not implemented yet!" );
}

void ConfigurationDlg::sltSimulationToggled(bool)
{
    qWarning( "ConfigurationDlg::sltSimulationToggled(bool): Not implemented yet!" );
}

void ConfigurationDlg::sltTwinDigitalPToggled(bool)
{
    qWarning( "ConfigurationDlg::sltTwinDigitalPToggled(bool): Not implemented yet!" );
}


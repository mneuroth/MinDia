/****************************************************************************
** Form interface generated from reading ui file 'ZConfigurationDlg.ui'
**
** Created: Sat Jan 4 15:37:17 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef CONFIGURATIONDLG_H
#define CONFIGURATIONDLG_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QRadioButton;

class ConfigurationDlg : public QDialog
{ 
    Q_OBJECT

public:
    ConfigurationDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ConfigurationDlg();

    QButtonGroup* m_pProjectorType;
    QRadioButton* m_pDigitalTwinP;
    QRadioButton* m_pMSC300P;
    QGroupBox* m_pComPortGroup;
    QLabel* m_pComPortLabel;
    QLabel* m_pBaudRateLabel;
    QLabel* m_pDataBitsLabel;
    QLabel* m_pStopBitsLabel;
    QLabel* m_pParityLabel;
    QLabel* m_pFlowControlLabel;
    QLabel* m_pWriteTimeoutLabel;
    QComboBox* m_pComPortList;
    QLineEdit* m_pWriteTimeout;
    QComboBox* m_pDataBitsList;
    QComboBox* m_pStopBitsList;
    QComboBox* m_pParityList;
    QComboBox* m_pFlowControlList;
    QComboBox* m_pBaudRateList;
    QLineEdit* m_pReadTimeout;
    QLabel* m_pReadTimeoutLabel;
    QGroupBox* m_pModusGroup;
    QCheckBox* m_pExecuteEvents;
    QCheckBox* m_pSimulation;
    QGroupBox* m_pDebugGroup;
    QCheckBox* m_pDbgLogging;

public slots:
    virtual void sltMSC300PToggled(bool);
    virtual void sltCloseDialog();
    virtual void sltDebugLoggingToggled(bool);
    virtual void sltSimulationToggled(bool);
    virtual void sltTwinDigitalPToggled(bool);

};

#endif // CONFIGURATIONDLG_H

/****************************************************************************
** Form interface generated from reading ui file 'ZConfigPlayerDlg.ui'
**
** Created: Sat Jan 4 15:52:19 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef CONFIGPLAYERDLG_H
#define CONFIGPLAYERDLG_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QGroupBox;
class QLabel;
class QPushButton;

class ConfigPlayerDlg : public QDialog
{ 
    Q_OBJECT

public:
    ConfigPlayerDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ConfigPlayerDlg();

    QPushButton* m_pOk;
    QPushButton* m_pCancel;
    QGroupBox* GroupBox4;
    QLabel* m_pPlayerLabel;
    QLabel* m_pPlayeSettingsLabel;
    QComboBox* m_pPlayerSettings;
    QComboBox* m_pPlayer;

public slots:
    virtual void sltCancel();
    virtual void sltOk();
    virtual void sltPlayerChanged(int);

protected:
    QHBoxLayout* Layout4;
};

#endif // CONFIGPLAYERDLG_H

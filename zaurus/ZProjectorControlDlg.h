/****************************************************************************
** Form interface generated from reading ui file 'ZProjectorControlDlg.ui'
**
** Created: Sat Jan 4 15:57:40 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef PROJECTORCONTROLDIALOG_H
#define PROJECTORCONTROLDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QGroupBox;
class QLabel;
class QPushButton;
class QRadioButton;

class ProjectorControlDialog : public QDialog
{ 
    Q_OBJECT

public:
    ProjectorControlDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ProjectorControlDialog();

    QPushButton* m_pReset;
    QGroupBox* m_pInfoGroup;
    QLabel* m_pSlideNoLabel;
    QPushButton* m_pGetDiaNo;
    QPushButton* m_pCloseButton;
    QPushButton* m_pBack;
    QPushButton* m_pForward;
    QPushButton* m_pDirectMode;
    QButtonGroup* m_pDissolve;
    QRadioButton* m_pDissolve1;
    QPushButton* m_pStopGo;
    QRadioButton* m_pDissolve2;
    QRadioButton* m_pDissolve3;
    QRadioButton* m_pDissolve4;
    QRadioButton* m_pDissolve5;
    QButtonGroup* m_pLight;
    QRadioButton* m_pLightInt0;
    QRadioButton* m_pLightInt1;
    QRadioButton* m_pLightInt2;
    QRadioButton* m_pLightInt3;
    QButtonGroup* m_pAutoFocus;
    QLabel* m_pStatusLabel;
    QRadioButton* m_pAF_On;
    QRadioButton* m_pAF_Off;
    QPushButton* m_pAF_minus;
    QPushButton* m_pAF_plus;
    QPushButton* m_pStatusButton;
    QPushButton* m_pTestButton;
    QLabel* m_pLastCmdLabel;
    QLabel* m_pModeLabel;
    QLabel* m_pMode;
    QLabel* m_pLastCmd;

public slots:
    virtual void sltGetStatus();
    virtual void sltAFBackward();
    virtual void sltAFForward();
    virtual void sltAutofocusOffToggled( bool );
    virtual void sltAutofocusOnToggled( bool );
    virtual void sltCheckMode();
    virtual void sltCloseDialog();
    virtual void sltDirectMode();
    virtual void sltDissolve1Toggled( bool );
    virtual void sltDissolve2Toggled( bool );
    virtual void sltDissolve3Toggled( bool );
    virtual void sltDissolve4Toggled( bool );
    virtual void sltDissolve5Toggled( bool );
    virtual void sltGetSlideNo();
    virtual void sltLight1Toggled( bool );
    virtual void sltLight2Toggled( bool );
    virtual void sltLight3Toggled( bool );
    virtual void sltLight4Toggled( bool );
    virtual void sltResetProjector();
    virtual void sltSlideBack();
    virtual void sltSlideForward();
    virtual void sltStopGo();
    virtual void sltTestProjector();

};

#endif // PROJECTORCONTROLDIALOG_H

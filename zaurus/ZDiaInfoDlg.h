/****************************************************************************
** Form interface generated from reading ui file 'zaurus/ZDiaInfoDlg.ui'
**
** Created: Fri Mar 12 23:42:11 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef DIAINFODLG_H
#define DIAINFODLG_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QLabel;
class QLineEdit;
class QMultiLineEdit;
class QPushButton;
class QRadioButton;
class HItem;

class DiaInfoDlg : public QDialog
{ 
    Q_OBJECT

public:
    DiaInfoDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~DiaInfoDlg();

    QButtonGroup* m_pEffectButtonGroup;
    QRadioButton* m_pFreeze;
    QRadioButton* m_pDarkPhase;
    QRadioButton* m_pFlash;
    QRadioButton* m_pFadeIn;
    QRadioButton* m_pNoEffect;
    QRadioButton* m_pSwing;
    QLineEdit* m_pEffectCount;
    QLabel* m_pEffectCountLabel;
    QPushButton* buttonOk;
    QPushButton* m_pFileNameButton;
    QButtonGroup* m_pSlideFormat;
    QRadioButton* m_pHorizontalFormat;
    QRadioButton* m_pVerticalFormat;
    QLabel* m_pScriptLabel;
    QPushButton* m_pModifyScript;
    QMultiLineEdit* m_pScript;
    QPushButton* buttonCancel;
    QLabel* IDLabel;
    QPushButton* m_pNext;
    QPushButton* m_pPrevious;
    QLineEdit* m_pIDEdit;
    QPushButton* m_pApplyAndNext;
    QPushButton* m_pApplyAndPrev;
    QPushButton* m_pApply;
    QPushButton* m_pNewItem;
    QPushButton* m_pDeleteItem;
    QLabel* CommenLabel;
    QLabel* TimerLabel;
    QLabel* DissolveLabel;
    QLabel* FileNameLabel;
    QLineEdit* m_pCommentEdit;
    QLineEdit* m_pFileNameEdit;
    QLineEdit* m_pDissolveEdit;
    QLineEdit* m_pTimerEdit;

public slots:
    virtual void sltDeleteItem();
    virtual void sltApplyAndNext();
    virtual void sltApplyAndPrev();
    virtual void sltApplyData();
    virtual void sltCloseDialog();
    virtual void sltCreateNewItem();
    virtual void sltDataChanged();
    virtual void sltDialogCanceled();
    virtual void sltDisableDialog();
    virtual void sltModifyScript();
    virtual void sltNextItem();
    virtual void sltPrevItem();
    virtual void sltScriptFlagToggled(bool);
    virtual void sltSelectFileName();
    virtual void sltTextChanged( const QString &);
    virtual void sltTextChanged();
    virtual void sltUpdateData(HItem * pFirstSelectedItem, bool bEnable );

protected:
    QHBoxLayout* m_pEffectButtonGroupLayout;
    QHBoxLayout* Layout21;
    QGridLayout* Layout19;
    QGridLayout* Layout20;
    QGridLayout* m_pSlideFormatLayout;
    QVBoxLayout* Layout25;
    QVBoxLayout* Layout10;
};

#endif // DIAINFODLG_H

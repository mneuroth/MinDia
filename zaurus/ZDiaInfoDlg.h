/****************************************************************************
** Form interface generated from reading ui file 'ZDiaInfoDlg.ui'
**
** Created: Sat Jan 4 15:37:17 2003
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

    QPushButton* m_pFileNameButton;
    QButtonGroup* m_pSlideFormat;
    QRadioButton* m_pHorizontalFormat;
    QRadioButton* m_pVerticalFormat;
    QMultiLineEdit* m_pScript;
    QLabel* m_pScriptLabel;
    QPushButton* m_pModifyScript;
    QButtonGroup* m_pEffectButtonGroup;
    QRadioButton* m_pFreeze;
    QRadioButton* m_pDarkPhase;
    QRadioButton* m_pFlash;
    QRadioButton* m_pFadeIn;
    QRadioButton* m_pNoEffect;
    QRadioButton* m_pSwing;
    QLineEdit* m_pEffectCount;
    QLabel* m_pEffectCountLabel;
    QLabel* CommenLabel;
    QLabel* FileNameLabel;
    QLabel* IDLabel;
    QLabel* TimerLabel;
    QLabel* DissolveLabel;
    QPushButton* m_pPrevious;
    QPushButton* m_pNext;
    QLineEdit* m_pCommentEdit;
    QLineEdit* m_pIDEdit;
    QLineEdit* m_pFileNameEdit;
    QLineEdit* m_pDissolveEdit;
    QLineEdit* m_pTimerEdit;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QPushButton* m_pApply;
    QPushButton* m_pApplyAndPrev;
    QPushButton* m_pApplyAndNext;
    QPushButton* m_pNewItem;

public slots:
    virtual void sltTextChanged();
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
    virtual void sltUpdateData(HItem * pFirstSelectedItem, bool bEnable );

protected:
    QGridLayout* m_pSlideFormatLayout;
    QVBoxLayout* Layout25;
    QVBoxLayout* Layout10;
    QHBoxLayout* m_pEffectButtonGroupLayout;
    QHBoxLayout* Layout21;
    QGridLayout* Layout19;
    QGridLayout* Layout20;
};

#endif // DIAINFODLG_H

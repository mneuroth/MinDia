/****************************************************************************
** Form interface generated from reading ui file 'SaveAsDlg.ui'
**
** Created: Sat Jan 4 15:37:17 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef SAVEASDLG_H
#define SAVEASDLG_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QLabel;
class QLineEdit;

class SaveAsDlg : public QDialog
{ 
    Q_OBJECT

public:
    SaveAsDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~SaveAsDlg();

    QLabel* m_pFileNameLabel;
    QLineEdit* m_pFileName;
    QLabel* m_pFolderLabel;
    QLabel* m_pTypeLabel;
    QComboBox* m_pFolder;
    QComboBox* m_pType;

protected:
    QGridLayout* SaveAsDlgLayout;
};

#endif // SAVEASDLG_H

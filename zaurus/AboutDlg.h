/****************************************************************************
** Form interface generated from reading ui file 'zaurus/AboutDlg.ui'
**
** Created: Fri Mar 12 23:42:10 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;

class AboutDlg : public QDialog
{ 
    Q_OBJECT

public:
    AboutDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AboutDlg();

    QLabel* m_pNameLabel;
    QLabel* m_pVersionLabel;
    QLabel* m_pCopyrightLabel;

protected:
    QVBoxLayout* AboutDlgLayout;
    bool event( QEvent* );
};

#endif // ABOUTDLG_H

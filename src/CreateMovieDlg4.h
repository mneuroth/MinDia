
#ifndef _CREATEMOVIEDLG_H
#define _CREATEMOVIEDLG_H

#include "ui_CreateMovieDlg4.h"

#include <QDialog>

class DocumentAndControler;
class QProcess;

class CreateMovieDlg4 : public QDialog
{
	Q_OBJECT

public:
    CreateMovieDlg4(DocumentAndControler * pDocControler = 0, double dTotalTimeMS = 0.0, QWidget *parent = 0);
    virtual ~CreateMovieDlg4();

public slots:
    void sltImageRatioSelected( const QString &);
    void sltSelectOutputDirectory();
    void sltCreateImages();
    void sltCreateAVI();

private:
    void UpdateCmds();

    Ui::CreateMovieDlg4 ui;

    double					m_dTotalTimeMS;
    DocumentAndControler *	m_pDocControler;	// not an owner !
    QProcess *              m_pProcess;
};

#endif //_CREATEMOVIEDLG_H

#ifndef PLAYINFOGRAPHICSVIEW_H
#define PLAYINFOGRAPHICSVIEW_H

#include <QWidget>
#include <QMenu>
#include <QGraphicsView>

class PlayInfoDlgImpl;

// *******************************************************************
/** The context menu for the canvas view
  */
class PlayInfoContextMenu : public QMenu
{
    Q_OBJECT

public:
    PlayInfoContextMenu( QWidget * parent, PlayInfoDlgImpl * pMyDialog );
    ~PlayInfoContextMenu();

public slots:
    void sltStartStopFade();
    void sltToggleFullScreen();
    void sltClose();

    void sltImageFormatTriggered( QAction * pAction );
    void sltShowMenu();

private:
    PlayInfoDlgImpl *	m_pMyDialog;
    QMenu *		        m_pImageFormats;
    QAction *           m_pActionFullScreen;
};


// *******************************************************************
class PlayInfoGraphicsView : public QGraphicsView
{
public:
    PlayInfoGraphicsView(QWidget *parent);

    virtual void contextMenuEvent(QContextMenuEvent *event);

    void SetDlg(PlayInfoDlgImpl * pDlg);

private:
    PlayInfoDlgImpl *   m_pDlg;
};

#endif // PLAYINFOGRAPHICSVIEW_H

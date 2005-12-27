/**
  * This file is taken from ZFax 0.9.6 written by Silvio Iaccarino
  * http://www.iaccarino.de/silvio/zfax.htm
  *
  */

#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <qdialog.h>
#include <qfile.h>
#include <qdir.h>
#include <qstringlist.h>
#include <qlabel.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qpopupmenu.h>


class QTimer;
class QListView;
class QListViewItem;
class QComboBox;
class QToolButton;
class QLineEdit;
class QIconView;
class QIconViewItem;
class QVBoxLayout;
class QCloseEvent;

class fileBrowser : public QDialog
{
  Q_OBJECT

public:
  fileBrowser( QWidget* parent,bool LoadFile,const QString filter=0, const QString iPath=0, const QString iFile=0);
  ~fileBrowser();

  QString selectedFileName, filterStr;
  QDir currentDir;
  static QString standardPictureFilter;

public slots:
  void refresh();
  void goUp();
  void addBookmark();
  void onChangeView();

protected:
  QPixmap fileImg;
  QPixmap dirImg;
  QPixmap picImg;
  QPixmap docImg;
  QIconSet dirIcon;
  QIconSet listIconSet;
  QIconSet iconIconSet;
  QToolButton *typeFilterBtn;
  QToolButton *hiddenFilterBtn;
  QToolButton *upBtn;
  QToolButton *bookmarkBtn;
  QToolButton *changeViewBtn;
  QListView* ListView;
  QIconView* IconView;
  QVBoxLayout* vlayout;
  QComboBox *dirLabel;
  QLineEdit *fileEdit;
  QPopupMenu bookmarkMenu;
  QStringList bookmarks;
  bool listViewMode;
  QTimer *timer;
private slots:
  void DoubleClicked(QListViewItem *);
  void DoubleClicked(QIconViewItem *);
  void Clicked(QListViewItem *);
  void Clicked(QIconViewItem *);
  void changeDir(int);
  void selectBookmark(int ItemID);
  void OnOk();

protected slots:
  void adjustHeader();
  void accept();

protected:
  void OnOk(bool bAccept);
  //virtual void closeEvent(QCloseEvent* e);
  virtual void resizeEvent(QResizeEvent* e);
  void updateBookmarkMenu();
  void DoubleClicked(QString item);
  void Clicked(QString item);
  void editBookmarks();
};

#endif // FILEBROWSER_H

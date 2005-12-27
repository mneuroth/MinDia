/**
  * This file is taken from ZFax 0.9.6 written by Silvio Iaccarino
  * http://www.iaccarino.de/silvio/zfax.htm
  *
  */

#include <stdlib.h>
#include <qlistview.h>
#include <qiconview.h>
#include <qheader.h>
#include <qcombobox.h>
#include <qfile.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <qtoolbutton.h>
#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif
#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qstring.h>
#include "filebrowser.h"
//#include "icons.h"
#ifdef QWS
	#include <qpe/config.h>
#else
    #include <qsettings.h>
#endif

#include "icons.hhp"

static const QByteArray& loadIcon( const char* name )
{
	return qembed_findData(name);
}

QString fileBrowser::standardPictureFilter("*.h;*.cpp;*.py;*.pl;*.rb;*.php;*.java;*.tcl");

fileBrowser::fileBrowser( QWidget* parent,bool LoadFile, const QString filter, const QString iPath,const QString iFile )
    : QDialog( parent, "fileBrowser", true, 0)
    , fileImg(loadIcon("empty.png"))
    , dirImg(loadIcon("folder.png"))
    , picImg(loadIcon("image.png"))
    , docImg(loadIcon("doc.png"))
	, dirIcon(dirImg)
	, listIconSet(QPixmap(loadIcon("view_detailed.png")))
	, iconIconSet(QPixmap(loadIcon("view_multicolumn.png")))
    , typeFilterBtn(NULL)
    , fileEdit(NULL)
    , listViewMode(true)
    , timer(NULL)
{
  if (LoadFile)
	  setCaption(tr( "Open file" ) );
  else
	setCaption(tr( "Save file" ) );

  filterStr=filter;

  dirLabel = new QComboBox(false,this, "DirLabel");

  ListView = new QListView( this, "ListView" );
  ListView->addColumn( tr( "Name" ) );
  ListView->setSorting( 2, FALSE);
  ListView->addColumn( tr( "Size" ) );
  ListView->setSelectionMode(QListView::Single);
  ListView->setAllColumnsShowFocus( TRUE );
  ListView->setColumnWidthMode(0,QListView::Manual);
  ListView->setColumnWidthMode(1,QListView::Maximum);
  ListView->setColumnAlignment(1,Qt::AlignRight);

  connect( ListView, SIGNAL(doubleClicked( QListViewItem*)), SLOT(DoubleClicked(QListViewItem *)) );
  connect( ListView, SIGNAL(clicked( QListViewItem*)), SLOT(Clicked(QListViewItem *)) );
  connect( ListView, SIGNAL(returnPressed( QListViewItem*)), SLOT(Clicked(QListViewItem *)) );
  connect( dirLabel, SIGNAL(activated(int)), SLOT(changeDir(int)));

  IconView = new QIconView(this,"IconView");
  IconView->setSelectionMode(QIconView::Single);
  IconView->setItemsMovable(false);
  IconView->setArrangement(QIconView::TopToBottom);
  IconView->setWordWrapIconText(true);
  IconView->hide();

  connect( IconView, SIGNAL(doubleClicked( QIconViewItem*)), SLOT(DoubleClicked(QIconViewItem *)) );
  connect( IconView, SIGNAL(clicked( QIconViewItem*)), SLOT(Clicked(QIconViewItem *)) );
  connect( IconView, SIGNAL(returnPressed( QIconViewItem*)), SLOT(Clicked(QIconViewItem *)) );

  vlayout = new QVBoxLayout(this);
  vlayout->addWidget(dirLabel);
  vlayout->addWidget(ListView);
  vlayout->addWidget(IconView);

  QHBoxLayout *hb;
  if (!LoadFile)
  {
  	vlayout->addSpacing(2);
  	hb = new QHBoxLayout(vlayout);
	  QLabel *label = new QLabel(this);
	  label->setText(tr("Name :"));
	  fileEdit = new QLineEdit(this);
	  fileEdit->setText(iFile);
	  selectedFileName = iFile;
	  hb->addSpacing(4);
	  hb->addWidget(label);
	  hb->addSpacing(4);
	  hb->addWidget(fileEdit);
	  hb->addSpacing(4);
	  connect( fileEdit, SIGNAL(returnPressed()), SLOT(OnOk()) );
  }
  vlayout->addSpacing(2);
  hb = new QHBoxLayout(vlayout);

  QToolButton *btn = new QToolButton(this);
  btn->setIconSet(QIconSet(QPixmap(loadIcon("reload.png"))));
  btn->setAutoRaise(true);
  connect(btn, SIGNAL(clicked()), this, SLOT(refresh()));
  hb->addWidget(btn);

  upBtn = new QToolButton(this);
  upBtn->setIconSet(QIconSet(QPixmap(loadIcon("up.png"))));
  upBtn->setAutoRaise(true);
  connect(upBtn, SIGNAL(clicked()), this, SLOT(goUp()));
  hb->addWidget(upBtn);

  hiddenFilterBtn = new QToolButton(this);
  hiddenFilterBtn->setIconSet(QIconSet(QPixmap(loadIcon("files.png"))));
  hiddenFilterBtn->setAutoRaise(true);
  hiddenFilterBtn->setToggleButton(true);
  connect(hiddenFilterBtn, SIGNAL(clicked()), this, SLOT(refresh()));
  hb->addWidget(hiddenFilterBtn);

  if (!filterStr.isEmpty())
  {
	  typeFilterBtn = new QToolButton(this);
	  typeFilterBtn->setIconSet(QIconSet(QPixmap(loadIcon("filter.png"))));
	  typeFilterBtn->setAutoRaise(true);
	  typeFilterBtn->setToggleButton(true);
	  typeFilterBtn->toggle();
	  connect(typeFilterBtn, SIGNAL(clicked()), this, SLOT(refresh()));
	  hb->addWidget(typeFilterBtn);
  }

  hb->addSpacing(10);
  btn = new QToolButton(this);
  btn->setIconSet(QIconSet(QPixmap(loadIcon("bookmark_add.png"))));
  btn->setAutoRaise(true);
  connect(btn, SIGNAL(clicked()), this, SLOT(addBookmark()));
  hb->addWidget(btn);

#ifdef QWS
	Config cfg("zmindia");
	cfg.setGroup("filemanager");
	bookmarks = (cfg.readListEntry("bookmarks",';'));
	listViewMode = cfg.readBoolEntry("listView",true);  
#else
	QSettings aCfg;

	aCfg.setPath("mneuroth.de","zmindia",QSettings::User);
	bookmarks = (aCfg.readListEntry("bookmarks",';'));
	listViewMode = aCfg.readBoolEntry("listView",true);  
#endif
    if (!listViewMode)
    {
		ListView->hide();
		IconView->show();
	}

  updateBookmarkMenu();

  bookmarkBtn = new QToolButton(this);
  bookmarkBtn->setIconSet(QIconSet(QPixmap(loadIcon("bookmark.png"))));
  bookmarkBtn->setAutoRaise(true);
  bookmarkBtn->setPopup(&bookmarkMenu);
  bookmarkBtn->setPopupDelay(0);
  hb->addWidget(bookmarkBtn);

  hb->addSpacing(10);

  changeViewBtn = new QToolButton(this);
  changeViewBtn->setIconSet(listViewMode ? iconIconSet : listIconSet);
  changeViewBtn->setAutoRaise(true);
  hb->addWidget(changeViewBtn);
  connect(changeViewBtn, SIGNAL(clicked()), this, SLOT(onChangeView()));

  hb->addStretch(10);
  btn = new QToolButton(this);
  btn->setIconSet(QIconSet(QPixmap(loadIcon("ok.png"))));
  btn->setAutoRaise(true);
  connect(btn, SIGNAL(clicked()), this, SLOT(OnOk()));
  hb->addWidget(btn);

  // ???
//  connect(this, SIGNAL(accept()), this, SLOT(OnOk()));

  if (QFileInfo(iPath).exists())
  {
    currentDir.setPath(iPath);
    chdir(iPath.latin1());
  }
  else
  {
    currentDir.setPath(QDir::currentDirPath());
    chdir(QDir::currentDirPath().latin1());
  }

  connect( &bookmarkMenu, SIGNAL(activated(int)), SLOT(selectBookmark(int)));

  timer = new QTimer(this);
  connect( timer, SIGNAL(timeout()), SLOT(adjustHeader()));

  refresh();
  if (fileEdit != NULL)
	  fileEdit->setFocus();
#ifdef QWS
	showMaximized();
#else
	resize( 240, 320 );
#endif
}

void fileBrowser::resizeEvent(QResizeEvent *)
{
  adjustHeader();
}

fileBrowser::~fileBrowser()
{
	delete timer;
}


void fileBrowser::goUp()
{
  currentDir.cdUp();
  chdir("..");
  refresh();
}

enum fileType
{
	ftUnknown,
	ftText,
	ftPic
};

struct fileTypeDecl
{
	char ext[8];
	fileType type;
};

#define TYPES 29
static fileTypeDecl fileTypes[TYPES] =
{
	{"001",ftPic},
	{"002",ftPic},
	{"003",ftPic},
	{"004",ftPic},
	{"asm",ftText},
	{"bmp",ftPic},
	{"c",ftText},
	{"cpp",ftText},
	{"cfg",ftText},
	{"conf",ftText},
	{"cpp",ftText},
	{"desktop",ftText},
	{"log",ftText},
	{"h",ftText},
	{"hhp",ftText},
	{"ini",ftText},
	{"java",ftText},
	{"jpeg",ftPic},
	{"jpg",ftPic},
	{"me",ftText},
	{"pl",ftText},
	{"png",ftPic},
	{"py",ftText},
	{"rb",ftText},
	{"sh",ftText},
	{"tif",ftPic},
	{"tiff",ftPic},
	{"txt",ftText},
	{"xbm",ftPic}
};

static int mystrcmp(const void *a,const void *b)
{
	return strcmp((const char *)a,(const char *)b);
}

void fileBrowser::refresh()
{
	if (listViewMode)
	{
		//ListView->setUpdatesEnabled(false);
		ListView->clear();
	}
	else
	{
		//IconView->setUpdatesEnabled(false);
		IconView->clear();
	}

  if (hiddenFilterBtn->state() == QButton::On)
    currentDir.setFilter( QDir::All | QDir::Hidden );
  else
    currentDir.setFilter( QDir::All );

  currentDir.setSorting(/* QDir::Size*/ /*| QDir::Reversed | */QDir::DirsFirst);
  currentDir.setMatchAllDirs(TRUE);

  if (typeFilterBtn!=NULL)
  {
	  if (typeFilterBtn->state() == QButton::On)
		  currentDir.setNameFilter(filterStr);
      else
		currentDir.setNameFilter("");
  }
  else
	  currentDir.setNameFilter(filterStr);

  bool isRootDir = currentDir.isRoot();//currentDir.canonicalPath() == "/";

  upBtn->setEnabled(!isRootDir);

  QString fileL, fileS;
  const QFileInfoList *list = currentDir.entryInfoList();
  QFileInfo *fi;

  QListViewItem *lv_item = NULL;
  QIconViewItem *iv_item = NULL;

  for (QFileInfoListIterator it(*list);(fi=it.current());++it)
  {
    if (fi->fileName() == ".")
      continue;

    if (isRootDir && fi->fileName() == "..")
      continue;

    fileL.sprintf( "%s",fi->fileName().data() );
    if( fi->isDir() )
    {
		if (listViewMode)
		{
			lv_item = new QListViewItem( ListView,fileL );
			lv_item->setPixmap(0,dirImg);
		}
		else
		{
			iv_item = new QIconViewItem( IconView,fileL );
			iv_item->setPixmap(dirImg);
		}
    }
    else
    {
		fileS.sprintf( "%.2f kb", float(float(fi->size())/1024.0) );
		if (listViewMode)
			lv_item = new QListViewItem( ListView,fileL,fileS );
		else
			iv_item = new QIconViewItem(IconView,fileL);

      fileTypeDecl *type = (fileTypeDecl *)bsearch(fi->extension(false).lower().latin1(),fileTypes,TYPES,sizeof(fileTypeDecl),mystrcmp);
      if (type)
      {
		  switch(type->type)
		  {
			  case ftText :
				  if (listViewMode)
					  lv_item->setPixmap(0,docImg);
				  else
					  iv_item->setPixmap(docImg);
				  break;
			  case ftPic :
				  if (listViewMode)
					  lv_item->setPixmap(0,picImg);
				  else
					  iv_item->setPixmap(picImg);
				  break;
		      default :
				  if (listViewMode)
					  lv_item->setPixmap(0,fileImg);
				  else
					  iv_item->setPixmap(fileImg);
		  }
	  }
	  else
	  {
		  if (listViewMode)
			  lv_item->setPixmap(0,fileImg);
		  else
			  iv_item->setPixmap(fileImg);
	  }
    }
  }

  // dir Combo refresh
  dirLabel->clear();
  QString s(currentDir.canonicalPath());
  int i;
  while (!s.isEmpty())
  {
    dirLabel->insertItem(dirImg,s,dirLabel->count());
    i = s.findRev('/');
    if (i<0)
      break;
    s.truncate(i);

  }
  int nItems = dirLabel->count();
  if (nItems<1 || dirLabel->text(nItems-1) != "/")
    dirLabel->insertItem(dirImg,"/",nItems);
  dirLabel->setCurrentItem(0);

  if (listViewMode)
  {
	  timer->start(20,true);
	  ListView->setFocus();
  }
  else
  {
	  IconView->setFocus();
  }


}

void fileBrowser::Clicked(QListViewItem *selectedItem)
{
  if (selectedItem == NULL)
    return;
  Clicked(selectedItem->text(0));
}

void fileBrowser::Clicked(QIconViewItem *selectedItem)
{
  if (selectedItem == NULL)
    return;
  Clicked(selectedItem->text());
}

void fileBrowser::Clicked(QString strItem)
{
  QFileInfo fi(strItem);
  while (fi.isSymLink())
    fi.setFile(fi.readLink());

  if (fi.isDir())
  {
    strItem=QDir::cleanDirPath(currentDir.canonicalPath()+"/"+strItem);
    if(QDir(strItem).exists())
    {
      currentDir.cd(strItem, TRUE);
      chdir(strItem.latin1());
      refresh();
    }
  }
  else if (fileEdit!=NULL)
  {
	  fileEdit->setText(strItem);
	  fileEdit->setFocus();
  }
}

void fileBrowser::DoubleClicked(QListViewItem * selectedItem)
{
	DoubleClicked(selectedItem->text(0));
}

void fileBrowser::DoubleClicked(QIconViewItem * selectedItem)
{
	DoubleClicked(selectedItem->text());
}

void fileBrowser::DoubleClicked(QString strItem)
{
  QFileInfo fi(strItem);
  while (fi.isSymLink())
    fi.setFile(fi.readLink());

  if (fi.isDir())
  {
    strItem=QDir::cleanDirPath(currentDir.canonicalPath()+"/"+strItem);
    if(QDir(strItem).exists())
    {
      currentDir.cd(strItem, TRUE);
      chdir(strItem.latin1());
      refresh();
    }
    return;
  }

  if (fileEdit!=NULL)
  {
	  fileEdit->setText(strItem);
  }
 OnOk();
}

void fileBrowser::changeDir(int ix)
{
  currentDir.cd(dirLabel->text(ix), TRUE);
  chdir(dirLabel->text(ix));
  refresh();
}

void fileBrowser::OnOk(bool bAccept)
{
  QString filename;
  if (fileEdit != NULL)
  {
	filename = fileEdit->text();

  }
  else
  {
	  if (listViewMode)
	  {
		  QListViewItem * selectedItem = ListView->selectedItem();
		  if (selectedItem == NULL)
			  return;
		filename = selectedItem->text(0);
	}
	else
	{
		  QIconViewItem * selectedItem = IconView->currentItem();
		  if (selectedItem == NULL)
			  return;
		filename = selectedItem->text();
	}
  }

  if (filename.isEmpty())
	  return;

  selectedFileName = currentDir.canonicalPath();
  selectedFileName += "/";
  selectedFileName += filename;

  if (QFileInfo(selectedFileName).isDir())
	  return;

  if( bAccept )
  {
	accept();
  }
}

void fileBrowser::OnOk()
{
	OnOk(true);
}

void fileBrowser::addBookmark()
{
	if (currentDir.isRoot())
		return;

	QString dir = currentDir.canonicalPath();
	QString s(getenv("HOME"));

	if ((dir == "/mnt/card") || (dir == "/mnt/cf") || (dir == s))
		return;
	s+="/Documents";
	if (dir == s)
		return;

	for (int i=0,m=bookmarks.count();i<m;i++)
	{
		if (bookmarks[i] == dir)
			return;
	}
	if (bookmarks.count() == 10)
	{
		bookmarks.remove(bookmarks.first());
	}
	bookmarks << dir;
	updateBookmarkMenu();
#ifdef QWS
	Config cfg("zmindia");
	cfg.setGroup("filemanager");
	cfg.writeEntry("bookmarks",bookmarks,';');
#else    
	QSettings aCfg;

	aCfg.setPath("mneuroth.de","zmindia",QSettings::User);
	aCfg.writeEntry("bookmarks",bookmarks,';');
#endif
}

void fileBrowser::updateBookmarkMenu()
{
  int MenuItems = 0;
	bookmarkMenu.clear();
	if (bookmarks.count() > 0)
	{
		for (int i=0,m=bookmarks.count();i<m;i++)
		{
			bookmarkMenu.insertItem(dirIcon,bookmarks[i],100+i,MenuItems++);
		}
	}

  if (MenuItems)
  	bookmarkMenu.insertSeparator(MenuItems++);

  bookmarkMenu.insertItem(QIconSet(QPixmap(loadIcon("gohome.png"))),tr("Home"),200,MenuItems++);
  bookmarkMenu.insertItem(QIconSet(QPixmap(loadIcon("company.png"))),tr("Documents"),201,MenuItems++);
  bookmarkMenu.insertItem(QIconSet(QPixmap(loadIcon("cf.png"))),tr("Compact Flash card"),202,MenuItems++);
  bookmarkMenu.insertItem(QIconSet(QPixmap(loadIcon("sd.png"))),tr("SD card"),203,MenuItems++);
  //bookmarkMenu.insertSeparator(MenuItems++);
  //bookmarkMenu.insertItem(QIconSet(QPixmap(loadIcon("configure.png"))),tr("verwalten..."),204,MenuItems++);
}

void fileBrowser::selectBookmark(int ItemID)
{
	QString newDir;
	switch (ItemID)
	{
		case 100 :
		case 101 :
		case 102 :
		case 103 :
		case 104 :
		case 105 :
		case 106 :
		case 107 :
		case 108 :
		case 109 :
			newDir = bookmarks[ItemID - 100];
			break;
		case 200 :
			newDir = getenv("HOME");
			break;
		case 201 :
			newDir = getenv("HOME");
			newDir += "/Documents";
			break;
		case 202 :
			newDir = "/mnt/cf";
			break;
		case 203 :
			newDir = "/mnt/card";
			break;
		case 204 :
			editBookmarks();
			return;
	}
	if (!newDir.isEmpty())
	{
		currentDir.cd(newDir, true);
		chdir(newDir);
		refresh();
	}
}

void fileBrowser::onChangeView()
{
	if (listViewMode)
	{
		ListView->hide();
		IconView->show();
		listViewMode = false;
		changeViewBtn->setIconSet(listIconSet);
		refresh();
	}
	else	
	{
		IconView->hide();
		ListView->show();
		listViewMode = true;
		changeViewBtn->setIconSet(iconIconSet);
		refresh();
	}
#ifdef QWS
	Config cfg("zmindia");
	cfg.setGroup("filemanager");
	cfg.writeEntry("listView",(int)listViewMode);
#else
	QSettings aCfg;

	aCfg.setPath("mneuroth.de","zmindia",QSettings::User);
	aCfg.writeEntry("listView",(int)listViewMode);
#endif
}

void fileBrowser::editBookmarks()
{
}

void fileBrowser::adjustHeader()
{
	ListView->setColumnWidthMode(1,QListView::Maximum);
	ListView->setColumnWidth(0,ListView->header()->width() - ListView->columnWidth(1));
	ListView->triggerUpdate();
}

void fileBrowser::accept()
{
	// handle the ok-button in the right top corner (only for the zaurus)
	OnOk(false);
	QDialog::accept();
}

/*
void fileBrowser::closeEvent(QCloseEvent* e)
{
	if( e->isAccepted() )
	{
		OnOk();
	}
	QDialog::closeEvent(e);
}
*/
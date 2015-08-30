/**************************************************************************
 *
 *	project				 : MinDia
 *
 *	copyright            : (C) 2002 by Michael Neuroth
 *
 ***************************************************************************/
/***************************************************************************
 *																		   *
 * This file is part of the MinDia package (program to make slide shows),  *
 *																		   *
 * Copyright (C) 2013 by Michael Neuroth.								   *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 ***************************************************************************/

/*
Bugs/TODOs:
//- Ueberblend-Zeiten bei Dia 1 veraendert DynText Position bei Dia 2 und 3 !!!??? Dissolve <--> Timer
((- ggf. Laenge des DynTexts in Timeline View anzeigen
((- ggf. DynText ebenfalls als CommentContainer behandeln
((- Aenderung der Show-Zeit fuer DynamicText ermoeglichen
//- Verschieben von Sound Dateien mit Sound Data Dialog (hoch, runter) funktioniert nicht ==> ggf. Dialog entfernen
//- ggf. Presentation Data Dialog anzeigen, wenn neue Praesenetation angelegt wird
((- Sprachresource *.qm korrekt laden
//- Frage: sind *.qm in .app noch notwendig obwohl *.qm auch als qt-resource enthalten ist ? --> nein
//- ist Open for Edit notwendig? --> nein, nur wenn scripte enabled sind
//- fuer was ist die Suche gut? --> text in daten des Dias (und des attached dyn text neu)
//- Loeschen --> Dia loeschen
//- Aendere Eintrag --> Dia Daten aendern
//- Musik Daten --> Musikdaten
((- Kommentare zur Musik --> Kommentare zu der Musik
//- Dyn. Graf. Operation --> Dyn. Text
//- About Dialog aktualisieren
//- ggf. Eingaben in Logging Dialog disablen
((- Behandlung DynText edit verbessern --> attach to dia
//- Disablen fuer Spalten im CommentDialog realisieren --> fuer attached texts
//- Live Play Mark Anzeige zeigt falsches Seitenverhaeltnis fuer Images an !
//- Abbrechen Button fuer CommentDialog realisieren
//- Uebersetung: Musik --> Ton
//- Hilfe-Label Anspringen funktioniert fuer Mac nicht ? --> http://www.qtcentre.org/threads/34669-QTextBrowser-go-to-an-anchor
//    --> F1 kommt nicht vom PlayInfo Dialog zur MainWindow --> wird sofort abgefischt vom MainWindow !!! --> andere Key Behandlungsreihenfolge
//- Icon fuer Windows Version korrekt setzen...
((- Verzeichnisse fuer Bilder, Sound und Daten korrigieren --> nicht auf installationsverzeichnis gehen sondern auf userdaten (win)
((- *.qm Dateien in src.tar.gz aufnehmen
((- Sprachresourcen aktualisierens
((- Quellcode aufrauemen
((- Text Anzeige mit Live Play Mark ist unterschiedlich zu Play: Position und Groesse falsch --> siehe unterschiedliche Verwendung von GetSlideForTime()
((   im Show-Play wird ein DynText ein und ausgeschaltet, vorher jedoch die Position gesetzt und diese nicht mehr an die Größe der Ausgabe angepasst !
((   dito mit Font-Größe
((   Skalierung Font siehe DynamicTextDlgImpl::SetTextFont( )
//- Ausschnitt Edit und Ausgabe (PlayInfo) unterschiedlich --> anscheinend ist Optimal initial nicht korrekt gesetzt --> nach manuellem umschalten ok
((- Positionierung text waehrend play ist nicht korrekt
//- Umschaltung im PlayInfoDialog notwendig bevor text Ausgabe und bild korrekt skaliert wird
//- Bug: play, anschliessend ist Text nicht mehr korrekt --> text zu klein --> dyngraphop.cpp ==> font original groesse cachen
//- Fontgroesse aendern funktioniert nicht !
//- Standard Standzeit eingebar machen
((- Bug: wahrscheinlich existiert Memory Leak --> Speicher geht bei Praesentation auf ueber 1GB ! --> ist original Image groesse 48MB pro Bild !
//- Bilder in Grundfarben auswaehlbar machen, z. B. schwarz, blau, etc.
//- Bug: standard dissolve and show time fuer neue dias anwenden
//- Bug: Praesentations Groesse aendern funktioniert nicht !

Aufgaben vor Release:
- Changes.txt fuer Version erstellen:
    port to Qt4, using phonon instead of external music player, using qserialport, removed script support (temporary)
    live play mark, vorschau in play info, suche auch in attached dyn text, attached dyn text
- knownbugs.txt anlegen --> Zellen-Update Problem unter Mac beschreiben
- icons.xpm Datei aus source.zip loeschen --> dafuer neue icon *.png aufnehmen
- ungenutzte SourceCode Dateien in old Verzeichnis verschieben
- RS232 Kommunikation verbessern -> Verbesserung des RS232 Dialogs zur direkten Ansteuerung --> nicht disablen nach erstem erfolglosen Versuch…
- encoding Text in rolleicom.cpp pruefen
- CreateMovieDlg und main SourceCode aufraeumen
- Binaere Packete erzeugen (Innosetup, macdeploy)
- Hilfe Dokument / Webseite fuer Bedienung erstellen
- Hilfe Dialog verbessern --> schoenere Icons verwenden, Signals/Slots verbinden und realisieren, z.B. Suche
- Uebertragung von CommentDialog in Daten funktioniert nicht korrekt ? Aenderung im Dialog werden nicht uebernommen --> fehlt update ?
    => anscheinend muss Feld verlassen werden bevor Aenderungen akzeptiert werden => Mac only ?
- ggf. Seitenverhaeltniss fuer Leinwand/PlayInfoDlg entfernen, verwende Seitenverhaeltniss von Praesentation
Bugs:
((- Bug: Fontsize und Fontcolor aendern funktioniert nicht richtig --> Groesse wird verkleinert wenn position geaendert wird
((- Bug: Fontsize/Art aendern ok, Position aendern ok --> alter Font ist wieder restauriert
//- Bug: left mouse click auf timeline view bewirkt jump zum anfang der scroll line... ==> m_iSelectedItemNo == 0 show selected Image
//- Bug: click auf Slide-Item in TimeLineView verschiebt scrollbar, falls scrollbar nicht ganz links ist !
//- Bug: verschieben von Dynamic Text funktioniert nicht korrekt wenn Scrollbar nicht ganz links ist
//- Bug: Selektieren Dia und Show/Dissolve aendern in TimeLineView funktioniert nicht wenn scrollbar nicht ganz links ist
//- Bug: wenn dyn Text selektiert und bewegt wird, sollte dazu passendes Dia selektiert werden
//- Bug: Dyn Texte koennen nicht gut von hinten nach vorne verschoben werden, wenn Scrollbar involviert ist
((- Bug: wenn horizontale Scrollbar nicht ganz rechts (oder links) und Dia Show Time veraendert wird, wird Zeit maximiert ohne sich zu bewegen
//- Bug: nicht immer das Dia zentrieren im Timeline window, falls nicht notwendig
//- Bug: Texte an Bildern bleiben nicht kleben wenn show-time weiter vorne vergroessert wird
- Bug: Performance schlecht, wenn in TimeLineView ein Item angeclickt wird !!! --> ggf. image cache optimieren !
- Bug: Performance Problem CPU Auslastung nach einmaligem Play show --> unnoetige? Aufrufe von _FadeImage()
- Bug: Fuer Play from Dia auch den Sound korrekt/passend abspielen (sound faengt immer von ganz vorne an)
- Bug: Ausgabegroesse der Praesentation wird nicht korrekt angezeigt nach Aenderung --> ggf. muss playinfo dialog geschlossen und neu geoeffnet werden
- Bug: Effekt (manchmal): Speichern unter merkt nicht korrekt den letzten dateipfad
- Optimize: Performance für Image Cache verbessern --> thumbnails in .dia Datei speichern und grosses Bild ggf. nachladen
Features:
//- Default-Groesse Font verbessern --> immer auf 72 pixel setzen
- Zentrieren der Cliping-Area implementieren --> -1 dazu verwenden ?
//- Togglen der Farbe beim Clipping-Rect realisieren
- zweites Clipping-Rect realisiern --> kenn burns --> DiaPresentation::GetSlideForTime()
- Geschwindigkeits-Optimierungen beim Film Erzeugen? Und Anzeige?
//- Anzeige des Dias/Images bei Positionierung von Schrift
((- Menupunkt / Skript um Praesentation an Sound anzupassen !
((- Menupunkt: Dia-Show an Musik anpassen
- Ken Burns Effekt realisieren
- Einblenden/Ausblenden der DynText Elemente realisieren
((- korrektes skalieren der Zeichensaetze bei Aenderung der Aufloesung durchfuehren --> geht anscheinend schon !
((- korrektes skalieren der Dia-Ausgabe --> 3:2 auf 16:9 fuer Movie !
- alle Fonts und Font-Groessen gleichzeitig aendern,
- default font und fontgroesse aenderbar machen
- Alle dissolve und show zeiten gleichzeitig anpassen/aendern
- Bilder cachen, ggf. Thumbnails in dia-Datei oder in globaler oder lokaler (pro dia-Datei) mindia-Image cache sqlite datenbank?
- ggf. Raum nach letztem Dia vorsehen, damit man an das Ende der Praesentation drag&droppen kann... --> ggf. leer bild ans Ende einfuegen
- Dia Dissolv und Show Time als CommentDlg aenderbar machen
- ggf. Unterstuetzung von gedrehten Images
- Synchronisiere Timeline mit HItem view besser implementieren

- ggf. test ffmpeg button in export dialog
- ggf. automatisch ffmpeg binary suchen (PATH suche)
- ggf. Geometrie-Zustand von anderen Dialogen ebenfalls sichern und restaurieren
- ggf. Plot Comments Dialog / Menue entfernen ==> ist nicht an Dias geheftet
- ggf. SoundInfo Markierungen an Sound-Dateien anheften
- ggf. beim Shift+Click auf leeres Dia den Datei-Auswahl Dialog oeffnen um Image anzugeben
- ggf. Show-Zeit fuer DynamicText via Maus-Move ermoeglichen
- ggf. ImagePath in settings speicherun und restaurieren --> appconfig.h.GetImagePath()

Mobile Version:
- moeglichst viele Dialog obsolet machen
- Icon/Menu Item fuer Add Sound und Add Dia
- Minus Icon auf Dia und Sound-Datei zum loeschen
- Pfeile Icon zum verschieben auf Dia und Sound-Datei zum loeschen
- Frage: wie kann man Ueberblend- und Standzeigen mit Touch modifizieren ? Funkioniert das schon?
- Frage: wie kann man DynTexte anlegen? Lange druecken?

Dialogs:                    Mobile                          Demo
- About                                                     modal
- PresentationData          Aufloesung, Seitenverhaeltnis	modal
- PlayInfo                  Anzeige
- DiaInfo                   Clipping Area
- DynText                   Position, Text
- CreateMovie		-       Movie Export                    modal (saved)
- Help              -
- CommentDialog		-                                       modal
- SoundInfo         -                                   	modal
- Logging           (-)     -
- Configuration (RS232)		-           					modal (saved)
- ProjektorControl          -

Hilfe:
- Praesentation anlegen: Ausgabegroesse und Format festlegen
- Dias hinzufuegen: Drag&Drop oder Icon/Menu
- Reihenfolge der Dias festlegen: Drag&Drop oder Pfeil-Icons auf Dia
- Standzeiten und Ueberblendezeiten festlegen: Mouse-Move oder Dialog
- Musik hinzufuegen: Drag&Drop oder Icon/Menu
- Reihenfolge der Musik festlegen: Drag&Drop oder Pfeil-Icons auf Sound
- DynText hinzufuegen: Menu-Shortcut waehrend Play oder Context-Menu
- SoundInfo hinzufuegen (zum Markieren von Sound-Stellen): Menu-Shortcut waehrend Play oder Dialog
- Vorfuehren (Play)
- Exportieren als Movie
- Speichern/Laden

ffmpeg fuer Android:
https://play.google.com/store/apps/details?id=com.netcompss.ffmpeg4android&hl=de
http://sourceforge.net/projects/ffmpeg4android/files/20130409/
http://sourceforge.net/projects/ffmpeg4android/
*/

#include "mindiawindow.h"

#include <QPixmap>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QTranslator>
#include <QString>
#include <QStringList>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QAction>
#include <QMessageBox>
#include <QStatusBar>
#include <QClipboard>
#include <QTimer>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QTextCodec>
#include <QRadioButton>
#include <QSettings>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QPrintDialog>

#include "iscript.h"
#include "qtmtlock.h"
#include "misctools.h"

#include "configdlgimpl.h"
#include "comlogimpl.h"
#include "pcdlgimpl.h"
#include "diainfodlgimpl.h"
#include "playinfodlgimpl.h"
#include "pddlgimpl.h"
#include "sndinfodlgimpl.h"
#include "commentdlgimpl.h"
#include "helpdlgimpl.h"
#include "CreateMovieDlg4.h"
#include "ui_AboutExtDlg4.h"

#include <stdlib.h>

/**
	@mainpage MinDia Documentation


	The program MinDia is usefull to edit and run a slide show.
	It supports also the possibility to control a slide show
	with the computer controlable projectors from Rollei:
	this are the Rolleivision 35 twin digital P dia projector
	and the Rollei twin MSC 3x0 P dia projector family.

	@author		Michael Neuroth
	@version	1.0
    @date		2001-2013

	./COPYING

*/

// *******************************************************************

const unsigned long c_ulStatusTimeout		= 2000;
const unsigned long c_ulStatusBarTimer		= 100;
const unsigned long c_ulAutoStartTimeout	= 100;

// ***********************************************************************

#define DIA_EXTENSION "*.dia"

// ** this function is defined in main.cpp */
QApplication * GetApplication();

static MinDiaWindow * g_pMainWindow = 0;

MinDiaWindow * GetMainWindow()
{
    return g_pMainWindow;
}

void GetDefaultTimes( double & dDissolveTime, double & dShowTime )
{
    dDissolveTime = GetMainWindow()->GetDefaultDissolveTime();
    dShowTime = GetMainWindow()->GetDefaultShowTime();
}

DiaPresentation * GetCurrentPresentation()
{
    return &(g_pMainWindow->GetDocument()->GetPresentation());
}

static bool g_bExecuteScript = false;

bool IsExecuteScriptAllowed()
{
    return g_bExecuteScript;
}

// ***********************************************************************

class AboutExtDlg : public QDialog, public Ui_AboutExtDlg
{
public:
    AboutExtDlg( QWidget* parent, Qt::WindowFlags fl=0 );
};

AboutExtDlg::AboutExtDlg( QWidget* parent, Qt::WindowFlags fl )
: QDialog(parent, fl)
{
    setupUi(this);
}

// ***********************************************************************

MinDiaWindow::MinDiaWindow( const QString & sLanguage, bool bIgnoreComSettings, bool bSimulation, int iProjectorType, QWidget* parent, Qt::WindowFlags f )
    : QMainWindow( parent, f ),
	  m_pLoggingDialog( 0 ),
	  m_pProjectorControlDialog( 0 ),
      //m_pPresentationEventsDialog( 0 ),
      m_pDiaInfoDialog( 0 ),
	  m_pPlayInfoDialog( 0 ),
	  m_pHelpDialog( 0 ),
	  m_pStatusUpdateTimer( 0 ),
	  m_pAutoStartTimer( 0 ),
	  m_iCount( 0 ),
	  m_pFirstSelectedItem( 0 ),
	  m_iFirstSelectedItemNo( -1 ),
	  m_sLanguage( sLanguage ),
	  m_dDissolveTime( 2.5 ),
      m_dShowTime( 5.0 ),
      m_bExpandImage( false ),
      m_bExitOnFinished( false ),
      m_iScreenX( 0 ),
      m_iScreenY( 0 ),
	  m_iPosX( 0 ),     // 10
	  m_iPosY( 0 )      // 350
{
    g_pMainWindow = this;

    setObjectName("MinDiaMainWindow");

    // ** prepare application for different languages...**
	m_pTranslator = new QTranslator( this );

    QCoreApplication::setOrganizationName("mneuroth.de");       // mindia
    QCoreApplication::setOrganizationDomain("mneuroth.de");     // mindia.sf.net
    QCoreApplication::setApplicationName("mindia");         

    setWindowTitle( tr( "MinDia" ) );

    // set application icon
    QPixmap aIcon(":/icons/mindia_new.png");
    setWindowIcon( aIcon );

	// *** create some needed sub-dialogs ***
	m_pLoggingDialog = new ComLoggingDialogImpl( this );
	//m_pControler->GetDiaCom().SetLoggingChannel( m_pLoggingDialog );

    m_pHelpDialog = new HelpDlgImpl( this );
	m_pHelpDialog->move( 470, 5 );

    m_pControler = new DocumentAndControler( /*bEnableScript*/true, bIgnoreComSettings, bSimulation, iProjectorType, this, this, m_pLoggingDialog );
    // bEnableScript will be changed in LoadSettings

    // set default value for last file name
    m_sLastFileName = ToQString( m_pControler->GetDocName() );

    CreateChildWidgets();
    CreateMenus();

    SetMessageSenderForCache( this );

    resize( 800, 600 );
	move( 25, 25 );
    LoadSettings();

    // create screen dialog at startup time to enable showing of current image after opening dialog
    CreatePlayInfoDlg();

    // autoload last file
    if( !m_sLastFileName.isEmpty() && QFile::exists(m_sLastFileName) )
    {
        sltLoadDoc( m_sLastFileName, /*bExecuteEvent*/IsExecuteScriptAllowed() );
    }

    sltModusIsSwitched();
    sltDoDocumentStateUpdate();
}

MinDiaWindow::~MinDiaWindow()
{
    SaveSettings();
    
    delete m_pPlayInfoDialog;

	delete m_pControler;

//	delete m_pDiaStateView;
//	delete m_pAudioPultView;
//	delete m_pDiaPultView;

	delete m_pTimeLineView;
	delete m_pSlideView;

//	delete m_pSourceBox;
    delete m_pTargetBox;

	delete m_pHBox;

	delete m_pStatusUpdateTimer;
	delete m_pAutoStartTimer;

	delete m_pTranslator;
}

DocumentAndControler * MinDiaWindow::GetDocument()
{
	return m_pControler;
}

QMenu * MinDiaWindow::GetPluginsMenuPtr()
{
	return m_pPlugins;
}

double MinDiaWindow::GetDefaultDissolveTime() const
{
    return m_dDissolveTime;
}

double MinDiaWindow::GetDefaultShowTime() const
{
    return m_dShowTime;
}

void MinDiaWindow::CreateMenus()
{
    // popuplate a menu with all actions

    QPixmap aOpenIcon( ":/icons/icons/openfile.png" );      // original in xmp file contained
    QPixmap aSaveIcon( ":/icons/icons/savefile.png" );
    QPixmap aRunIcon( ":/icons/icons/run.png" );
    QPixmap aPauseIcon( ":/icons/icons/pause.png" );
    QPixmap aStopIcon( ":/icons/icons/stop.png" );
    QPixmap aPlayInfoIcon(":/icons/icons/display.png");    // for icons see nuvola in Develop/libs/nuvola

    QToolBar * pTools = addToolBar("MinDia");
    pTools->setObjectName("MinDiaToolBar");
    pTools->setIconSize(QSize(14,14));

    // *** create toplevel menu items ***
    m_pFile		= new QMenu( tr( "&File" ), this );
    m_pEdit		= new QMenu( tr( "&Edit" ), this );
    m_pPlay		= new QMenu( tr( "&Play" ), this );
    m_pPresentation	= new QMenu( tr( "Pre&sentation" ), this );
    m_pProjector	= new QMenu( tr( "P&rojector" ), this );
    m_pPlugins	= 0; //new QMenu( tr( "Plu&gins" ), this );
    m_pHelp		= new QMenu( tr( "&Help" ), this );

    menuBar()->addMenu( m_pFile );
    menuBar()->addMenu( m_pEdit );
    menuBar()->addMenu( m_pPlay );
    menuBar()->addMenu( m_pPresentation );
    menuBar()->addMenu( m_pProjector );
    //menuBar()->addMenu( m_pPlugins );
    menuBar()->addMenu( m_pHelp );

	// *** submenu: File ***
    m_pFileNewAction = new QAction( tr( "&New" ), this );
    m_pFileNewAction->setStatusTip(tr( "Create a new presentation" ));
    m_pFileNewAction->setShortcut(Qt::CTRL+Qt::Key_N);
    connect( m_pFileNewAction, SIGNAL( triggered() ), this, SLOT( sltAskNewDoc() ) );
    m_pFileLoadAction = new QAction( aOpenIcon, tr( "&Open..." ), this );
    m_pFileLoadAction->setStatusTip(tr( "Open an existing presentation" ));
    m_pFileLoadAction->setShortcut(Qt::CTRL+Qt::Key_O);
    connect( m_pFileLoadAction, SIGNAL( triggered() ), this, SLOT( sltAskLoadDoc() ) );
    pTools->addAction(m_pFileLoadAction);
    // open for edit is only needed if script events are enabled (script not supported yet)
    m_pFileLoadForEditAction = 0;
    //m_pFileLoadForEditAction = new QAction( tr( "Open for e&dit..." ), this );
    //m_pFileLoadForEditAction->setStatusTip(tr( "Open an existing file for edit (script-events are not executed)" ));
    //m_pFileLoadForEditAction->setShortcut(Qt::ALT+Qt::Key_O);
    //connect( m_pFileLoadForEditAction, SIGNAL( triggered() ), this, SLOT( sltAskLoadForEditDoc() ) );
    m_pFileSaveAction = new QAction( aSaveIcon, tr( "&Save" ), this );
    m_pFileSaveAction->setStatusTip( tr( "Save presentation" ) );
    m_pFileSaveAction->setShortcut(Qt::CTRL+Qt::Key_S);
    pTools->addAction(m_pFileSaveAction);
    pTools->addSeparator();
    connect( m_pFileSaveAction, SIGNAL( triggered() ), m_pControler, SLOT( sltSaveDoc() ) );
    m_pFileSaveAsAction = new QAction( tr( "Save &as..." ), this );
    m_pFileSaveAsAction->setStatusTip( tr( "Save presentation as" ) );
    connect( m_pFileSaveAsAction, SIGNAL( triggered() ), this, SLOT( sltAskSaveAsDoc() ) );
    m_pFileMakeRelPathsAction = new QAction( tr( "Make &relative paths" ), this );
    m_pFileMakeRelPathsAction->setStatusTip( tr( "Convert absolute paths to reative paths" ));
    connect( m_pFileMakeRelPathsAction, SIGNAL( triggered() ), this, SLOT( sltMakeRelPaths() ) );
    m_pFileMakeAbsPathsAction = new QAction( tr( "Make a&bsolute paths..." ), this );
    m_pFileMakeAbsPathsAction->setStatusTip( tr( "Convert relative paths to absolute paths" ));
    connect( m_pFileMakeAbsPathsAction, SIGNAL( triggered() ), this, SLOT( sltMakeAbsPaths() ) );
    m_pFileImportXMLAction = new QAction( tr( "&Import XML..." ), this );
    m_pFileImportXMLAction->setStatusTip( tr( "Import presentation from a XML file" ) );
    connect( m_pFileImportXMLAction, SIGNAL( triggered() ), this, SLOT( sltImportXMLDoc() ) );
    m_pFileExportXMLAction = new QAction( tr( "E&xport XML..." ), this );
    m_pFileExportXMLAction->setStatusTip( tr( "Export presentation as XML file" ) );
    connect( m_pFileExportXMLAction, SIGNAL( triggered() ), this, SLOT( sltExportXMLDoc() ) );
    m_pFileExportAction = new QAction(  tr( "Expo&rt..." ), this );
    m_pFileExportAction->setStatusTip( tr( "Export presentation" ) );
    connect( m_pFileExportAction, SIGNAL( triggered() ), this, SLOT( sltExportDoc() ) );
    m_pFileExportDynGraphAction = new QAction( tr( "Export dyn. &graphics..." ), this );
    m_pFileExportDynGraphAction->setStatusTip( tr( "Export dynamic graphic data" ) );
    connect( m_pFileExportDynGraphAction, SIGNAL( triggered() ), this, SLOT( sltExportDynGraphData() ) );
    m_pFileImportDynGraphAction = new QAction( tr( "Import d&yn. graphics..." ), this );
    m_pFileImportDynGraphAction->setStatusTip( tr( "Import dynamic graphic data" ) );
    connect( m_pFileImportDynGraphAction, SIGNAL( triggered() ), this, SLOT( sltImportDynGraphData() ) );
    m_pFilePrintAction = new QAction( tr( "&Print..." ), this );
    m_pFilePrintAction->setStatusTip( tr( "Print data" ) );
    connect( m_pFilePrintAction, SIGNAL( triggered() ), this, SLOT( sltPrintDoc() ) );
    m_pFileExportAVIAction = new QAction( tr( "Cr&eate Movie..." ), this );
    m_pFileExportAVIAction->setShortcut(Qt::CTRL+Qt::Key_E);
    m_pFileExportAVIAction->setStatusTip( tr( "Export presentation as movie" ) );
    connect( m_pFileExportAVIAction, SIGNAL( triggered() ), this, SLOT( sltExportAVI() ) );
    m_pFileExitAction = new QAction( tr( "E&xit" ), this );
    m_pFileExitAction->setStatusTip( tr( "Exit application" ) );
    m_pFileExitAction->setShortcut(Qt::ALT+Qt::Key_F4);
    connect( m_pFileExitAction, SIGNAL( triggered() ), this, SLOT( close() ) );
    //connect( m_pFileExitAction, SIGNAL( triggered() ), qApp, SLOT( quit() ) );

	// ** create menu item for last opend files
    m_pLastFilesSubMenu = new QMenu( tr( "Las&t files" ), m_pFile );
	connect( m_pLastFilesSubMenu, SIGNAL( aboutToShow() ), this, SLOT( sltUpdateLastFilesMenu() ) );

    m_pImportExportFilesSubMenu = new QMenu( tr( "&Import/Export" ), m_pFile );
    m_pImportExportFilesSubMenu->addAction(m_pFileImportXMLAction);
    m_pImportExportFilesSubMenu->addAction(m_pFileExportXMLAction);
    m_pImportExportFilesSubMenu->addSeparator();
    m_pImportExportFilesSubMenu->addAction(m_pFileImportDynGraphAction);
    m_pImportExportFilesSubMenu->addAction(m_pFileExportDynGraphAction);
    m_pImportExportFilesSubMenu->addSeparator();
    m_pImportExportFilesSubMenu->addAction(m_pFileExportAction);

    m_pFile->addAction(m_pFileNewAction);
    m_pFile->addSeparator();
    m_pFile->addAction(m_pFileLoadAction);
    if( m_pFileLoadForEditAction ) { m_pFile->addAction(m_pFileLoadForEditAction); }
    m_pFile->addAction(m_pFileSaveAction);
    m_pFile->addAction(m_pFileSaveAsAction);
    m_pFile->addSeparator();
    m_pFile->addAction(m_pFileMakeRelPathsAction);
    m_pFile->addAction(m_pFileMakeAbsPathsAction);
    m_pFile->addSeparator();
    m_pFile->addAction(m_pFileExportAVIAction);
    m_pFile->addAction(m_pFilePrintAction);
    m_pFile->addSeparator();
    m_pFile->addMenu( m_pImportExportFilesSubMenu );
    m_pFile->addMenu( m_pLastFilesSubMenu );
    m_pFile->addSeparator();
    m_pFile->addAction(m_pFileExitAction);

	// *** submenu: edit ***
    m_pEditUndoAction = new QAction( tr( "&Undo" ), this );
    m_pEditUndoAction->setStatusTip( tr( "Undo last operation" ) );
    m_pEditUndoAction->setShortcut(Qt::ALT+Qt::Key_Backspace);
    connect( m_pEditUndoAction, SIGNAL( triggered() ), m_pControler, SLOT( sltUndoOperation() ) );
	m_pEditUndoAction->setEnabled( false );
    m_pEditRedoAction = new QAction( tr( "&Redo" ), this );
    m_pEditRedoAction->setStatusTip( tr( "Redo last operation" ) );
    m_pEditRedoAction->setShortcut(Qt::CTRL+Qt::Key_Backspace);
    connect( m_pEditRedoAction, SIGNAL( triggered() ), m_pControler, SLOT( sltRedoOperation() ) );
	m_pEditRedoAction->setEnabled( false );

    m_pEditCutAction = new QAction( tr( "Cu&t" ), this );
    m_pEditCutAction->setStatusTip( tr( "Cut to clipboard" ) );
    m_pEditCutAction->setShortcut(Qt::CTRL+Qt::Key_X);
    connect( m_pEditCutAction, SIGNAL( triggered() ), m_pControler, SLOT( sltCutClipboard() ) );
    m_pEditCopyAction = new QAction( tr( "&Copy" ), this );
    m_pEditCopyAction->setStatusTip( tr( "Copy to clipboard" ) );
    m_pEditCopyAction->setShortcut(Qt::CTRL+Qt::Key_C);
    connect( m_pEditCopyAction, SIGNAL( triggered() ), m_pControler, SLOT( sltCopyClipboard() ) );
    m_pEditPasteAction = new QAction( tr( "&Paste" ), this );
    m_pEditPasteAction->setStatusTip( tr( "Paste from clipboard" ) );
    m_pEditPasteAction->setShortcut(Qt::CTRL+Qt::Key_V);
    connect( m_pEditPasteAction, SIGNAL( triggered() ), m_pControler, SLOT( sltPasteClipboard() ) );
    m_pEditSelectAllAction = new QAction( tr( "Select &all" ), this );
    m_pEditSelectAllAction->setStatusTip( tr( "Select all dias" ) );
    m_pEditSelectAllAction->setShortcut(Qt::CTRL+Qt::Key_A);
    connect( m_pEditSelectAllAction, SIGNAL( triggered() ), m_pControler, SLOT( sltSelectAllClipboard() ) );
    m_pEditNewDiaAction = new QAction( tr( "Append d&ia" ), this );
    m_pEditNewDiaAction->setStatusTip( tr( "Append dia at the end of the presentation" ) );
    m_pEditNewDiaAction->setShortcut(Qt::CTRL+Qt::Key_I);
    connect( m_pEditNewDiaAction, SIGNAL( triggered() ), this, SLOT( sltNewItem() ) );
    m_pEditAddDiaAction = new QAction( tr( "Add &dia" ), this );
    m_pEditAddDiaAction->setStatusTip( tr( "Add dia at current position" ) );
    m_pEditAddDiaAction->setShortcut(Qt::ALT+Qt::Key_I);
    connect( m_pEditAddDiaAction, SIGNAL( triggered() ), this, SLOT( sltAddItem() ) );
    
    m_pEditDeleteAction = new QAction( tr( "D&elete dia(s)" ), this );
    m_pEditDeleteAction->setStatusTip( tr( "Delete selected dia(s)" ) );
    m_pEditDeleteAction->setShortcut(Qt::Key_Delete);    
    connect( m_pEditDeleteAction, SIGNAL( triggered() ), m_pControler, SLOT( sltDeleteSelectedItems() ) );

    m_pEditFindAction = new QAction( tr( "&Search..." ), this );
    m_pEditFindAction->setStatusTip( tr( "Search for text" ) );
    m_pEditFindAction->setShortcut(Qt::CTRL+Qt::Key_F);
    connect( m_pEditFindAction, SIGNAL( triggered() ), this, SLOT( sltFindItem() ) );
    m_pEditFindNextAction = new QAction( tr( "Search &next" ), this );
    m_pEditFindNextAction->setStatusTip( tr( "Search for next occurence of search text" ) );
    m_pEditFindNextAction->setShortcut(Qt::Key_F3);
    connect( m_pEditFindNextAction, SIGNAL( triggered() ), this, SLOT( sltFindNextItem() ) );

    m_pExtrasModifyItemAction = new QAction( tr( "&Modify dia data..." ), this );
    m_pExtrasModifyItemAction->setStatusTip( tr( "Show dialog to modify data of the selected dia" ) );
    m_pExtrasModifyItemAction->setShortcut(Qt::CTRL+Qt::Key_M);
    m_pExtrasModifyItemAction->setCheckable(true);
    connect( m_pExtrasModifyItemAction, SIGNAL( triggered() ), this, SLOT( sltDoModifyItem() ) );

    m_pExtrasAdjustTimesToFitMusicAction = new QAction( tr( "Ad&just dia times to fit sound..." ), this );
    m_pExtrasAdjustTimesToFitMusicAction->setStatusTip( tr( "Adjust all dia show times so that the presentation fits with the sound" ) );
    m_pExtrasAdjustTimesToFitMusicAction->setShortcut(Qt::CTRL+Qt::Key_J);
    connect( m_pExtrasAdjustTimesToFitMusicAction, SIGNAL( triggered() ), this, SLOT( sltAdjustDiaTimeToFitSound() ) );

    m_pExtrasAdjustTimesAction = new QAction( tr( "Adjust dia &times..." ), this );
    m_pExtrasAdjustTimesAction->setStatusTip( tr( "Adjust all dia show times so that the presentation fits with the sound" ) );
    m_pExtrasAdjustTimesAction->setShortcut(Qt::ALT+Qt::Key_J);
    connect( m_pExtrasAdjustTimesAction, SIGNAL( triggered() ), this, SLOT( sltAdjustDiaTime() ) );

    m_pEditUpdateAction = new QAction( tr( "Re&fresh" ), this );
    m_pEditUpdateAction->setStatusTip( tr("Refresh the view") );
    m_pEditUpdateAction->setShortcut(Qt::Key_F5);
    connect( m_pEditUpdateAction, SIGNAL( triggered() ), this, SLOT( sltUpdate() ) );
    m_pEditTestAction = new QAction( tr( "&Test" ), this );
    m_pEditTestAction->setStatusTip( tr("For testing only !") );
    m_pEditTestAction->setShortcut(Qt::Key_F6);
    connect( m_pEditTestAction, SIGNAL( triggered() ), this, SLOT( sltTest() ) );

    m_pEdit->addAction( m_pEditUndoAction );
    m_pEdit->addAction( m_pEditRedoAction );
    m_pEdit->addSeparator();
    m_pEdit->addAction( m_pEditCutAction );
    m_pEdit->addAction( m_pEditCopyAction );
    m_pEdit->addAction( m_pEditPasteAction );
    m_pEdit->addAction( m_pEditSelectAllAction );
    m_pEdit->addSeparator();
    m_pEdit->addAction( m_pEditFindAction );
    m_pEdit->addAction( m_pEditFindNextAction );
    m_pEdit->addSeparator();
    m_pEdit->addAction( m_pEditNewDiaAction );
    m_pEdit->addAction( m_pEditAddDiaAction );
    m_pEdit->addAction( m_pEditDeleteAction );
    m_pEdit->addSeparator();
    m_pEdit->addAction( m_pExtrasModifyItemAction );
    m_pEdit->addAction( m_pExtrasAdjustTimesAction );
    m_pEdit->addAction( m_pExtrasAdjustTimesToFitMusicAction );
    m_pEdit->addSeparator();
    m_pEdit->addAction( m_pEditUpdateAction );
    //for Test only: m_pEdit->addAction( m_pEditTestAction );

    // *** submenu: play ***
    m_pPlayStartAction = new QAction( aRunIcon, tr( "Sta&rt" ), this );
    m_pPlayStartAction->setStatusTip( tr( "Start presentation" ) );
    m_pPlayStartAction->setShortcut(Qt::CTRL+Qt::Key_R);
    connect( m_pPlayStartAction, SIGNAL( triggered() ), m_pControler, SLOT( sltPlayPresentation() ) );
    pTools->addAction(m_pPlayStartAction);
    m_pPlayPauseAction = new QAction( aPauseIcon, tr( "&Pause" ), this );
    m_pPlayPauseAction->setStatusTip( tr( "Pause presentation" ) );
    m_pPlayPauseAction->setShortcut(Qt::CTRL+Qt::Key_P);
    connect( m_pPlayPauseAction, SIGNAL( triggered() ), m_pControler, SLOT( sltPausePresentation() ) );
    pTools->addAction(m_pPlayPauseAction);
    m_pPlayStopAction = new QAction( aStopIcon, tr( "S&top" ), this );
    m_pPlayStopAction->setStatusTip( tr( "Stop presentation" ) );
    m_pPlayStopAction->setShortcut(Qt::CTRL+Qt::Key_T);
    connect( m_pPlayStopAction, SIGNAL( triggered() ), m_pControler, SLOT( sltStopPresentation() ) );
    pTools->addAction(m_pPlayStopAction);
    // ** go to position and start from that position:
    m_pPlayStartFromAction = new QAction( tr( "Start &from selected" ), this );
    m_pPlayStartFromAction->setStatusTip( tr( "Start from selected position (goto position and start)" ));
    m_pPlayStartFromAction->setShortcut(Qt::ALT+Qt::Key_R);
    connect( m_pPlayStartFromAction, SIGNAL( triggered() ), this, SLOT( sltPlayFromSelected() ) );
    // ** start from already selected position (dia is already positioned correctly):
    m_pPlayStartAtAction = new QAction( tr( "Start &at selected" ), this );
    m_pPlayStartAtAction->setStatusTip( tr( "Start at selected position (just start at position)" ) );
    m_pPlayStartAtAction->setShortcut(Qt::CTRL+Qt::ALT+Qt::Key_R);
    connect( m_pPlayStartAtAction, SIGNAL( triggered() ), this, SLOT( sltPlayAtSelected() ) );
    // ** set a comment mark at actual time-position
    m_pPlayAddSoundCommentAction = new QAction( tr( "A&dd sound comment" ), this );
    m_pPlayAddSoundCommentAction->setStatusTip( tr( "Add sound comment (play presentation to enable)" ) );
    m_pPlayAddSoundCommentAction->setShortcut(Qt::CTRL+Qt::Key_B);
    connect( m_pPlayAddSoundCommentAction, SIGNAL( triggered() ), m_pControler, SLOT( sltAddSoundComment() ) );
    // ** set a dynamic graphic operation mark at actual time-position
    m_pPlayAddGraphicOpAction = new QAction( tr( "Add &graphic operation" ), this );
    m_pPlayAddGraphicOpAction->setStatusTip( tr( "Add graphic operation (play presentation to enable)" ) );
    m_pPlayAddGraphicOpAction->setShortcut(Qt::CTRL+Qt::Key_G);
    connect( m_pPlayAddGraphicOpAction, SIGNAL( triggered() ), m_pControler, SLOT( sltAddGraphicOperation() ) );

    m_pPlayEditFadeTimeAction = new QAction( tr( "D&efault dissolve time..." ), this );
    m_pPlayEditFadeTimeAction->setStatusTip( tr( "Edit default dissolve time" ) );
    connect( m_pPlayEditFadeTimeAction, SIGNAL( triggered() ), this, SLOT( sltEditFadeInTime() ) );
    m_pPlayEditShowTimeAction = new QAction( tr( "Default &show time..." ), this );
    m_pPlayEditShowTimeAction->setStatusTip( tr( "Edit default show time" ) );
    connect( m_pPlayEditShowTimeAction, SIGNAL( triggered() ), this, SLOT( sltEditShowTime() ) );
    m_pPlayFadeInAction = new QAction( tr( "&Fade in test" ), this );
    m_pPlayFadeInAction->setStatusTip( tr( "Fade in test (select two dias to enable)" ) );
    m_pPlayFadeInAction->setShortcut(Qt::ALT+Qt::Key_F);
    connect( m_pPlayFadeInAction, SIGNAL( triggered() ), this, SLOT( sltFadeInTest() ) );
    m_pPlayFadeOutAction = new QAction( tr( "Fade &out test" ), this );
    m_pPlayFadeOutAction->setStatusTip( tr( "Fade out test (select two dias to enable)" ) );
    m_pPlayFadeOutAction->setShortcut(Qt::ALT+Qt::Key_O);
    connect( m_pPlayFadeOutAction, SIGNAL( triggered() ), this, SLOT( sltFadeOutTest() ) );
    m_pExtrasPlayStatusAction = new QAction( aPlayInfoIcon, tr( "Pla&y screen..." ), this );
    pTools->addSeparator();
    pTools->addAction(m_pExtrasPlayStatusAction);
    m_pExtrasPlayStatusAction->setStatusTip( tr( "Show dialog to show the dia presentation" ) );
    m_pExtrasPlayStatusAction->setShortcut(Qt::CTRL+Qt::Key_Y);
    m_pExtrasPlayStatusAction->setCheckable(true);
    connect( m_pExtrasPlayStatusAction, SIGNAL( triggered() ), this, SLOT( sltDoPlayInfos() ) );

    m_pPlay->addAction( m_pPlayStartFromAction );
    m_pPlay->addAction( m_pPlayStartAtAction );
    m_pPlay->addSeparator();
    m_pPlay->addAction( m_pPlayStartAction );
    m_pPlay->addAction( m_pPlayPauseAction );
    m_pPlay->addAction( m_pPlayStopAction );
    m_pPlay->addSeparator();
    m_pPlay->addAction( m_pPlayAddSoundCommentAction );
    m_pPlay->addAction( m_pPlayAddGraphicOpAction );
    m_pPlay->addSeparator();
    m_pPlay->addAction( m_pPlayEditFadeTimeAction );
    m_pPlay->addAction( m_pPlayEditShowTimeAction );
    m_pPlay->addAction( m_pPlayFadeInAction );
    m_pPlay->addAction( m_pPlayFadeOutAction );
    m_pPlay->addSeparator();
    m_pPlay->addAction( m_pExtrasPlayStatusAction );

    // *** submenu: extras ***
    m_pExtrasConfigAction = new QAction( tr( "&Configuration..." ), this );
    m_pExtrasConfigAction->setStatusTip( tr( "Show dialog for projector configuration" ) );
    m_pExtrasConfigAction->setShortcut(Qt::ALT+Qt::Key_C);
    connect( m_pExtrasConfigAction, SIGNAL( triggered() ), this, SLOT( sltDoConfiguration() ) );
    m_pExtrasControlProjectorAction = new QAction( tr( "Co&ntrol..." ), this );
    m_pExtrasControlProjectorAction->setStatusTip( tr( "Show dialog for projector control" ) );
    m_pExtrasControlProjectorAction->setShortcut(Qt::ALT+Qt::Key_N);
    m_pExtrasControlProjectorAction->setCheckable(true);
    connect( m_pExtrasControlProjectorAction, SIGNAL( triggered() ), this, SLOT( sltDoControlProjector() ) );
    m_pExtrasLoggingAction = new QAction( tr( "&Logging..." ), this );
    m_pExtrasLoggingAction->setStatusTip( tr( "Show dialog for logging" ) );
    m_pExtrasLoggingAction->setShortcut(Qt::ALT+Qt::Key_L);
    m_pExtrasLoggingAction->setCheckable(true);
    connect( m_pExtrasLoggingAction, SIGNAL( triggered() ), this, SLOT( sltDoLogging() ) );

    m_pProjector->addAction( m_pExtrasConfigAction );
    m_pProjector->addAction( m_pExtrasControlProjectorAction );
    m_pProjector->addAction( m_pExtrasLoggingAction );
	
    m_pExtrasPresentationDataAction = new QAction( tr( "Presentation &data..." ), this );
    m_pExtrasPresentationDataAction->setStatusTip( tr( "Show dialog to modify the presentation data" ) );
    m_pExtrasPresentationDataAction->setShortcut(Qt::CTRL+Qt::Key_D);
    connect( m_pExtrasPresentationDataAction, SIGNAL( triggered() ), this, SLOT( sltDoPresentationData() ) );
    m_pExtrasPresentationEventsAction = 0;
//	m_pExtrasPresentationEventsAction = new QAction( /*tr( "Presentation events" ),*/ tr( "Presentation &events..." ), Qt::CTRL+Qt::Key_E, this, "presentationevents"/*, TRUE*/ );
//    connect( m_pExtrasPresentationEventsAction, SIGNAL( triggered() ), this, SLOT( sltDoPresentationEvents() ) );
    m_pExtrasSoundDataAction = new QAction( tr( "So&und data..." ), this );
    m_pExtrasSoundDataAction->setStatusTip( tr( "Show dialog to modify the sound data" ) );
    m_pExtrasSoundDataAction->setShortcut(Qt::CTRL+Qt::Key_U);
    connect( m_pExtrasSoundDataAction, SIGNAL( triggered() ), this, SLOT( sltDoSoundData() ) );
    m_pExtrasSoundCommentAction = new QAction( tr( "Sou&nd comments..." ), this );
    m_pExtrasSoundCommentAction->setStatusTip( tr( "Show dialog to modify sound comments (blue text in timeline view)" ) );
    m_pExtrasSoundCommentAction->setShortcut(Qt::CTRL+Qt::Key_Z);
    connect( m_pExtrasSoundCommentAction, SIGNAL( triggered() ), this, SLOT( sltDoSoundComment() ) );
    m_pExtrasPlotCommentAction = new QAction( tr( "&Plot comments..." ), this );
    m_pExtrasPlotCommentAction->setStatusTip( tr( "Show dialog to modify the plot comments (internal comments for presentation, yellow text in timeline view)" ) );
    m_pExtrasPlotCommentAction->setShortcut(Qt::ALT+Qt::Key_Z);
    connect( m_pExtrasPlotCommentAction, SIGNAL( triggered() ), this, SLOT( sltDoPlotComment() ) );
    m_pExtrasDynGraphOpAction = new QAction( tr( "D&yn. text..." ), this );
    m_pExtrasDynGraphOpAction->setStatusTip( tr( "Show dialog to modify the dynamic texts" ) );
    m_pExtrasDynGraphOpAction->setShortcut(Qt::ALT+Qt::Key_G);
    connect( m_pExtrasDynGraphOpAction, SIGNAL( triggered() ), this, SLOT( sltDoDynGraphicOp() ) );

    m_pPresentation->addAction( m_pExtrasPresentationDataAction );
//	m_pPresentation->addAction( m_pExtrasPresentationEventsAction );
    m_pPresentation->addAction( m_pExtrasSoundDataAction );
    m_pPresentation->addAction( m_pExtrasSoundCommentAction );
    m_pPresentation->addAction( m_pExtrasPlotCommentAction );
    m_pPresentation->addAction( m_pExtrasDynGraphOpAction );

    // *** submenu: scripts ***
    // will be filled in the dll-module !

    // *** submenu: help ***
    QAction * helpAboutAction = new QAction( tr( "&About..." ), this );
    helpAboutAction->setStatusTip( tr( "Infos about this application" ) );
    helpAboutAction->setShortcut(Qt::CTRL+Qt::Key_F1);
    connect( helpAboutAction, SIGNAL( triggered() ), this, SLOT( sltShowAbout() ) );
    QAction * helpAboutQtAction = new QAction( tr( "About &Qt..." ), this );
    helpAboutQtAction->setStatusTip( tr( "About Qt" ) );
    connect( helpAboutQtAction, SIGNAL( triggered() ), this, SLOT( sltShowQtAbout() ) );
    QAction * helpAction = new QAction( tr( "&Help..." ), this );
    helpAction->setStatusTip( tr( "Help for this application" ) );
// do not use short key for help --> otherwise F1 keypress will not be delivered to the non-modal windows in Mac-OS
//#if !defined(Q_WS_MAC)
//    helpAction->setShortcut(Qt::Key_F1);
//#endif
    connect( helpAction, SIGNAL( triggered() ), this, SLOT( sltShowHelpForMe() ) );

    m_pHelp->addAction( helpAction );
    m_pHelp->addSeparator();
    //helpLicenseAction->addTo( m_pHelp );
    m_pHelp->addAction( helpAboutAction );
    m_pHelp->addAction( helpAboutQtAction );


	// *** connect signals to slots ***
    connect( this, SIGNAL( sigLoadDocFromFile(const QString &,bool) ), m_pControler, SLOT( sltLoadDoc(const QString &,bool) ) );
    connect( this, SIGNAL( sigSaveDocToFile(const QString &) ), m_pControler, SLOT( sltSaveAsDoc(const QString &) ) );
    connect( this, SIGNAL( sigSaveDocAsXML(const QString &) ), m_pControler, SLOT( sltSaveDocAsXML(const QString &) ) );
    connect( this, SIGNAL( sigQuit() ), qApp, SLOT( quit() ) );
    connect( this, SIGNAL( sigStartPresentation() ), m_pControler, SLOT( sltPlayPresentation() ) );
	connect( this, SIGNAL( sigShowPlayInfos() ), this, SLOT( sltDoPlayInfos() ) );

	connect( m_pEdit, SIGNAL( aboutToShow() ), this, SLOT( sltUpdateEditMenu() ) );
	connect( m_pFile, SIGNAL( aboutToShow() ), this, SLOT( sltUpdateFileMenu() ) );
	connect( m_pPlay, SIGNAL( aboutToShow() ), this, SLOT( sltUpdatePlayMenu() ) );
    connect( m_pProjector, SIGNAL( aboutToShow() ), this, SLOT( sltUpdateExtrasMenu() ) );
	// ** script-menu will be handled in dll
	//connect( m_pPlugins, SIGNAL( aboutToShow() ), this, SLOT( sltUpdateScriptsMenu() ) );

    connect( this, SIGNAL( sigDialogHelp(const QString &) ), this, SLOT( sltShowHelp(const QString &) ) );

    connect( m_pTimeLineView, SIGNAL( sigPlayMarkChanged(double) ), this, SLOT( sltPlayMarkChanged(double) ) );

    connect( QApplication::clipboard(), SIGNAL( dataChanged() ), this, SLOT( sltClipboardDataChanged() ) );
}

void MinDiaWindow::customEvent(QEvent * pEvent)
{
    if( pEvent->type()==QEvent::User )
    {
        sltUpdate();
    }
    if( pEvent->type()==QEvent::User+1 )
    {
        sltTest();
    }
    if( pEvent->type()==_USER_EVENT_GET_SOUND_LENGTH )
    {
        GetSoundLengthEvent * pSoundEvent = (GetSoundLengthEvent *)pEvent;
        miniSound & aSoundInfo = GetDocument()->GetSoundInfo();
        aSoundInfo.AsyncGetTotalLengthForFile(pSoundEvent->GetFileName(),pSoundEvent->GetRequester());
    }
    if( pEvent->type()==c_iCustomEvent_ShowStatus )
    {
        MyCustomEvent<QString> * pCustomEvent = (MyCustomEvent<QString> *)pEvent;
        sltShowStatusBarMessage( pCustomEvent->data() );
    }
}

void MinDiaWindow::CreateChildWidgets()
{
	const int iInitWidth = 795;
	const int iInitHeight1 = 260;
	const int iInitHeight2 = 260;

	// *** create now all the widgets
	m_pHBox			= new QWidget( this );

	m_pTargetBox	= new QWidget( m_pHBox );
//	m_pSourceBox	= 0; //new QWidget( m_pHBox );

	m_pSlideView	= new HItemView( m_pTargetBox, iInitWidth, iInitHeight1, this, m_pControler, &m_pControler->GetPresentation() );
	m_pTimeLineView	= new TimeLineView( m_pTargetBox, iInitWidth, iInitHeight2, this, m_pControler, &m_pControler->GetPresentation() );

    // TODO --> ggf. optionale Anzeige als Text-Tabelle --> QTableView

/*
	m_pDiaPultView	= new IconItemView( m_pSourceBox, 200, iInitHeight1 );
	m_pDiaPultView->setSizeHint( QSize( 100, 100 ) );
	m_pAudioPultView = new IconItemView( m_pSourceBox, 200, iInitHeight2 );
	m_pAudioPultView->setSizeHint( QSize( 100, 100 ) );
*/
//	m_pDiaStateView = 0; //new DiaStateView( m_pSourceBox, 200, iInitHeight );
	//m_pDiaStateView->setSizeHint( QSize( 100, 100 ) );

//	m_pDiaPultView = 0;
//	m_pAudioPultView = 0;

    QHBoxLayout * pLayout = new QHBoxLayout;
    pLayout->setContentsMargins(0,0,0,0);
    pLayout->addWidget(m_pTargetBox);
    //pLayout->addWidget(m_pSourceBox);
    m_pHBox->setLayout(pLayout);
    
    QVBoxLayout * pVLayout = new QVBoxLayout;
    pVLayout->setContentsMargins(0,0,0,0);
    pVLayout->addWidget(m_pSlideView);
    pVLayout->addWidget(m_pTimeLineView);
    m_pTargetBox->setLayout(pVLayout);
        
	setCentralWidget( m_pHBox );

	// ** create statusbar **
    /*QStatusBar * pStatusBar */ statusBar();

	m_pStatusBarMsg = new QLabel( statusBar() );
	m_pStatusBarModus = new QLabel( statusBar() );
	m_pStatusBarTime = new QLabel( statusBar() );
    statusBar()->addWidget( m_pStatusBarMsg, 5/*, FALSE*/ );
    statusBar()->addWidget( m_pStatusBarModus, 1/*, FALSE*/ );
    statusBar()->addWidget( m_pStatusBarTime, 3/*, FALSE*/ );
	//m_pStatusBarMsg->setText( tr( "message" ) );
	//m_pStatusBarModus->setText( tr( "modus" ) );
	//m_pStatusBarTime->setText( tr( "time" ) );
	
	// ** start status-update timer **
	m_pStatusUpdateTimer = new QTimer( this );
	connect( m_pStatusUpdateTimer, SIGNAL( timeout() ), this, SLOT( sltStatusUpdateTimerEvent() ) );
	m_pStatusUpdateTimer->start( c_ulStatusBarTimer );
}

void MinDiaWindow::sltDoConfiguration()
{
    ConfigurationDlgImpl * pConfigurationDialog = new ConfigurationDlgImpl( m_pControler, this );
    pConfigurationDialog->setModal(true);

    if( m_aConfigDialogGeometry.count()>0 )
    {
        pConfigurationDialog->restoreGeometry(m_aConfigDialogGeometry);
    }

    int iRet = pConfigurationDialog->exec();

    if( iRet == 1 )
    {
        // ** ok
        // ** nothing to do, all things will be done in the dialog !
    }

    m_aConfigDialogGeometry = pConfigurationDialog->saveGeometry();

    // ** destroy modal dialog ! **
    delete pConfigurationDialog;
}

void MinDiaWindow::sltDoModifyItem()
{
	// ** at this point the state of the action is allready toggled !
	// ** so we need the 'negative' logic !
    if( !m_pExtrasModifyItemAction->isChecked() )
	{
		if( m_pDiaInfoDialog )
		{
			m_pDiaInfoDialog->hide();

			//delete m_pDiaInfoDialog;
			//m_pDiaInfoDialog = 0;
		}
    }
	else
	{
		if( !m_pDiaInfoDialog )
		{
			m_pDiaInfoDialog = new DiaInfoDlgImpl( m_pSlideView, this );
			m_pDiaInfoDialog->move( 450, 330 );
            m_pDiaInfoDialog->setWindowFlags(m_pDiaInfoDialog->windowFlags() | Qt::WindowStaysOnTopHint);
		}

		m_pDiaInfoDialog->show();

		// ** update the data for the selected item
		sltItemSelected( m_iCount, m_pFirstSelectedItem, m_iFirstSelectedItemNo, 0 );
	}
}

void MinDiaWindow::sltAdjustDiaTimeToFitSound()
{
    // hole gesamtdauer der musik
    // hole gesamtdauer der praesentation
    // berechne skalierungsfaktor
    // aktualisiere alle dia stand- und ueberblend-zeiten mit dem skalierungsfaktor --> nur stand-zeit anpassen !
    // aktualisiere view

    double dPresentationTime = m_pControler->GetPresentation().GetTotalTime()*1000.0;   // in seconds
    double dMusicTime = m_pControler->GetPresentation().GetSoundInfoData().GetTotalPlayLength();    // im milli seconds
    if( dPresentationTime!=0.0 )
    {
        double dScaleFactor = dMusicTime / dPresentationTime;
        m_pControler->GetPresentation().ScaleAllDiaShowTimes( dScaleFactor );
        m_pControler->sltDataChanged();
        sltUpdate();
    }
}

void MinDiaWindow::sltAdjustDiaTime()
{
    bool ok = false;
    double dScaleFactor = QInputDialog::getDouble( this, tr("Adjust dia show time"), tr("Enter scale factor:"), 1.0, 0.0, 1000.0, 2, &ok );
    if( ok )
    {
        m_pControler->GetPresentation().ScaleAllDiaShowTimes( dScaleFactor );
        m_pControler->sltDataChanged();
        sltUpdate();
    }
}

void MinDiaWindow::sltShowModifyItem()
{
    if( !m_pExtrasModifyItemAction->isChecked() )
	{
        m_pExtrasModifyItemAction->setChecked( true );
		sltDoModifyItem();
	}
	m_pDiaInfoDialog->show();
    m_pDiaInfoDialog->raise();  // see: http://stackoverflow.com/questions/6087887/bring-window-to-front-raise-show-activatewindow-don-t-work
}

void MinDiaWindow::sltDoSoundData()
{
    SoundInfoDlgImpl * pSoundInfoDialog = new SoundInfoDlgImpl( &m_pControler->GetPresentation().GetSoundInfoData(), this );
    pSoundInfoDialog->setModal(true);

    pSoundInfoDialog->exec();

    // ** destroy modal dialog ! **
    delete pSoundInfoDialog;
}

void MinDiaWindow::sltShowSoundData()
{
	sltDoSoundData();
}

void MinDiaWindow::sltDoSoundComment()
{
    CommentDlgImpl * pSoundCommentDialog = new CommentDlgImpl( &m_pControler->GetPresentation().GetSoundCommentData(), this );
    pSoundCommentDialog->setModal(true);

    pSoundCommentDialog->exec();

    // ** destroy modal dialog ! **
    delete pSoundCommentDialog;
}

void MinDiaWindow::sltShowSoundComment()
{
	sltDoSoundComment();
}

void MinDiaWindow::sltDoPlotComment()
{
    CommentDlgImpl * pPlotCommentDialog = new CommentDlgImpl( &m_pControler->GetPresentation().GetPlotCommentData(), this );
    pPlotCommentDialog->setModal(true);

    pPlotCommentDialog->exec();

    // ** destroy modal dialog ! **
    delete pPlotCommentDialog;
}

void MinDiaWindow::sltDoDynGraphicOp()
{
    CommentDlgImpl * pDynGraphicOpDialog = new CommentDlgImpl( &m_pControler->GetPresentation().GetDynGraphicData(), this );
    pDynGraphicOpDialog->setModal(true);

    pDynGraphicOpDialog->exec();

    // ** destroy modal dialog ! **
    delete pDynGraphicOpDialog;
}

void MinDiaWindow::sltShowPlotComment()
{
	sltDoPlotComment();
}

void MinDiaWindow::CreatePlayInfoDlg()
{
    if( !m_pPlayInfoDialog )
    {
        m_pPlayInfoDialog = new PlayInfoDlgImpl( m_pControler, this );
        m_pPlayInfoDialog->setWindowFlags(m_pPlayInfoDialog->windowFlags() | Qt::WindowStaysOnTopHint);
        m_pPlayInfoDialog->move( 10, 350 );

        if( m_aPlayInfoDialogGeometry.count()>0 )
        {
            m_pPlayInfoDialog->restoreGeometry(m_aPlayInfoDialogGeometry);
        }

        if( m_iPosX>0 && m_iPosY>0 )
        {
            m_pPlayInfoDialog->SetPos( m_iPosX, m_iPosY );
        }

        if( m_iScreenX>0 && m_iScreenY>0 )
        {
            m_pPlayInfoDialog->SetSize( m_iScreenX, m_iScreenY );
        }

        if( m_bExpandImage )
        {
            m_pPlayInfoDialog->SetExpandImage( true );
        }

        // update the states of the run, pause and stop buttons in this dialog
        sltUpdatePlayMenu();
    }
}

void MinDiaWindow::sltDoPlayInfos()
{
    if( m_pPlayInfoDialog && !m_pExtrasPlayStatusAction->isChecked() )
	{
		if( m_pPlayInfoDialog )
		{
			m_pPlayInfoDialog->hide();
		}
	}
	else
	{
        m_pPlayInfoDialog->SetImageRatio( GetCurrentImageRatio() );
        m_pPlayInfoDialog->show();

        // ** update the data for the selected item
        sltItemSelected( m_iCount, m_pFirstSelectedItem, m_iFirstSelectedItemNo, 0 );
    }
}

void MinDiaWindow::sltShowPlayStatus()
{
    if( !m_pExtrasPlayStatusAction->isChecked() )
	{
        m_pExtrasPlayStatusAction->setChecked( true );
		sltDoPlayInfos();
	}
	m_pPlayInfoDialog->show();
}

void MinDiaWindow::sltDoPresentationData()
{
    PresentationDataDlgImpl * pPresentationDataDialog = new PresentationDataDlgImpl( &(m_pControler->GetPresentation()), this );
    pPresentationDataDialog->setModal(true);

    pPresentationDataDialog->exec();

    // ** destroy modal dialog ! **
    delete pPresentationDataDialog;
}

//void MinDiaWindow::sltDoPresentationEvents()
//{
///*
//	if( !m_pExtrasPresentationEventsAction->isChecked() )
//	{
//		if( m_pPresentationEventsDialog )
//		{
//			m_pPresentationEventsDialog->hide();
//		}
//	}
//	else
//*/	{
//		if( !m_pPresentationEventsDialog )
//		{
//			m_pPresentationEventsDialog = new EventMapDlgImpl( &(m_pControler->GetPresentation().GetScriptEnvironment()), this, "presentationevents", /*modal*/TRUE );
//			//m_pPresentationEventsDialog->move( 450, 10 );
//		}

//		m_pPresentationEventsDialog->exec();

//		// ** destroy modal dialog ! **
//		delete m_pPresentationEventsDialog;
//		m_pPresentationEventsDialog = 0;

//		// ** update the document state, scripts maybe changed
//		sltDoDocumentStateUpdate();
//	}
//}

void MinDiaWindow::sltDoControlProjector()
{
    if( !m_pExtrasControlProjectorAction->isChecked() )
	{
		if( m_pProjectorControlDialog )
		{
			m_pProjectorControlDialog->hide();
		}
	}
	else
	{
		if( !m_pProjectorControlDialog )
		{
			m_pProjectorControlDialog = new ProjectorControlDlgImpl( &(m_pControler->GetDiaCom()), &(m_pControler->GetPresentation()), this );
			m_pProjectorControlDialog->move( 450, 10 );
            
            if( m_aProjectorControlDialogGeometry.count()>0 )
            {            
                m_pProjectorControlDialog->restoreGeometry(m_aProjectorControlDialogGeometry);
            }
		}

		m_pProjectorControlDialog->show();
	}
}

void MinDiaWindow::sltDoLogging()
{
    if( !m_pExtrasLoggingAction->isChecked() )
	{
		m_pLoggingDialog->hide();
	}
	else
	{
        if( m_aLoggingDialogGeometry.count()>0 )
        {            
            m_pLoggingDialog->restoreGeometry(m_aLoggingDialogGeometry); 
        }
        
		m_pLoggingDialog->show();                
	}
}

void MinDiaWindow::sltShowAbout()
{
    AboutExtDlg * pAboutDlg = new AboutExtDlg( this );

	string sText;
	bool bOk = ReadTextFile( "COPYING", sText );

	QString sLicense = tr( "This program is released under the GPL Version 2:\n\n" );

	if( bOk )
	{
        sLicense += ToQString( sText );
	}
	else
	{
		sLicense += "http://www.fsf.org/licenses/gpl.html";
	}

    pAboutDlg->m_pLicenseEdit->setPlainText( sLicense );
    pAboutDlg->setModal(true);
	
	pAboutDlg->exec();

	delete pAboutDlg;
}

void MinDiaWindow::sltShowQtAbout()
{
	QMessageBox::aboutQt( this, "MinDia Qt Version" );
}

void MinDiaWindow::sltModifyItemDialogClosed()
{
    m_aDiaInfoDialogGeometry = m_pDiaInfoDialog->saveGeometry();
    m_pExtrasModifyItemAction->setChecked( false );
}

void MinDiaWindow::sltPlayInfoDialogClosed()
{
    m_aPlayInfoDialogGeometry = m_pPlayInfoDialog->saveGeometry();            
    m_pExtrasPlayStatusAction->setChecked( false );
}

void MinDiaWindow::sltProjectorControlDialogClosed()
{
    m_aProjectorControlDialogGeometry = m_pProjectorControlDialog->saveGeometry();
    m_pExtrasControlProjectorAction->setChecked( false );
}

void MinDiaWindow::sltLoggingDialogClosed()
{
    m_aLoggingDialogGeometry = m_pLoggingDialog->saveGeometry();            
    m_pExtrasLoggingAction->setChecked( false );
}

// TODO --> not used, because this is a modal dialog !
void MinDiaWindow::sltSoundDataDialogClosed()
{
    m_pExtrasSoundDataAction->setChecked( false );
}

// TODO --> not used, because this is a modal dialog !
void MinDiaWindow::sltSoundCommentDialogClosed()
{
    m_pExtrasSoundCommentAction->setChecked( false );
}

// TODO --> not used, because this is a modal dialog !
void MinDiaWindow::sltPlotCommentDialogClosed()
{
    m_pExtrasPlotCommentAction->setChecked( false );
}

// TODO --> not used, because this is a modal dialog !
void MinDiaWindow::sltPresentationDataDialogClosed()
{
    m_pExtrasPresentationDataAction->setChecked( false );
}

// TODO --> not used, because this is a modal dialog !
void MinDiaWindow::sltPresentationEventsDialogClosed()
{
    m_pExtrasPresentationEventsAction->setChecked( false );
}

// TODO --> not used, because this is a modal dialog !
void MinDiaWindow::sltConfigurationDialogClosed()
{
    m_pExtrasConfigAction->setChecked( false );
}

void MinDiaWindow::sltAskNewDoc()
{
    int iRet = QMessageBox::Yes;
	// ask user before deleting the actual document...
    if( m_pControler->IsChanged() )
    {
        iRet = QMessageBox::question( 0, tr( "MinDia - Question" ),
                                         tr( "Really create a new document and lose all changes ?" ) );
    }
    if( iRet == QMessageBox::Yes )
	{
		m_pControler->sltNewDoc();
        sltDoPresentationData();
	}
}

void MinDiaWindow::sltLoadDoc( const QString & sFileName, bool bExecuteEvent )
{
    if( !sFileName.isEmpty() )
	{
    	sltCheckCloseApplication();

        m_sLastFileName = sFileName;
        
		emit sigLoadDocFromFile( sFileName, bExecuteEvent );

		// ** select the first slide after reading the file
		sltSelectItem( 0, 0 );
	}
    else
	{
        statusBar()->showMessage( tr( "Loading aborted" ), c_ulStatusTimeout );
	}
}

void MinDiaWindow::sltAskLoadDoc()
{
    QString sFileName = QFileDialog::getOpenFileName( this, tr("Open"), ToQString( m_pControler->GetName() ), DIA_EXTENSION );

	sltLoadDoc( sFileName, /*bExecuteEvent*/true );
}

void MinDiaWindow::sltAskLoadForEditDoc()
{
    QString sFileName = QFileDialog::getOpenFileName( this, tr( "Open for edit" ), ToQString( m_pControler->GetName() ), DIA_EXTENSION );

	sltLoadDoc( sFileName, /*bExecuteEvent*/false );
}

void MinDiaWindow::sltAskSaveAsDoc()
{    
    QString sFileName = QFileDialog::getSaveFileName( this, tr( "Save as" ), ToQString( m_pControler->GetName() ), DIA_EXTENSION );

    if( !sFileName.isEmpty() )
	{
        m_sLastFileName = sFileName;

		emit sigSaveDocToFile( sFileName );
	}
    else
	{
        statusBar()->showMessage( tr( "Save was aborted" ), c_ulStatusTimeout );
	}
}

void MinDiaWindow::sltMakeRelPaths()
{
	m_pControler->GetPresentation().MakeRelativePaths();
	
	sltDoUpdateAllViews();
}

void MinDiaWindow::sltMakeAbsPaths()
{
    QString sDirInput = QInputDialog::getText(this,"Convert to absolute path","Directory:");

    if( !sDirInput.isEmpty() )
    {
        string sDir = ToStdString( sDirInput );
        m_pControler->GetPresentation().MakeAbsolutePaths(sDir);

        sltDoUpdateAllViews();
    }
}

void MinDiaWindow::sltPrintDoc()
{
    QPrintDialog aPrinter;

    bool bOk = aPrinter.exec();

	if( bOk )
	{
		sltShowErrorMessage( tr( "Sorry, printing not implemented yet !" ) );
	}
}

void MinDiaWindow::sltImportXMLDoc()
{
    // TODO: not supported yet !
	sltShowErrorMessage( tr( "Not implemented yet !" ) );
}

void MinDiaWindow::sltExportXMLDoc()
{
    QString sFileName = QFileDialog::getSaveFileName( this, tr( "Export as XML" ), QString::null, /*QString::null*/"*.xml" );

    if( !sFileName.isEmpty() )
	{
		emit sigSaveDocAsXML( sFileName );
	}
    else
	{
        statusBar()->showMessage( tr( "Export to XML was aborted" ), c_ulStatusTimeout );
	}
}

void MinDiaWindow::sltExportDoc()
{
	// use script interface for exporting data in other formats (i.e. csv)
	sltShowErrorMessage( tr( "Use scripts to export the presentation-data for other applications." ) );
}

void MinDiaWindow::sltExportAVI()
{
    CreateMovieDlg4 * pDlg = new CreateMovieDlg4(m_pControler,m_pControler->GetPresentation().GetTotalTime()*1000,this);

    if( m_aCreateMovieDialogGeometry.count()>0 )
    {
        pDlg->restoreGeometry(m_aCreateMovieDialogGeometry);
    }

    pDlg->restoreSettings();

    unsigned long ulWidth, ulHeight;
    m_pControler->GetPresentation().GetImageSize(ulWidth,ulHeight);
    pDlg->setOutputSizeIfPossible(ulWidth,ulHeight);

	int iRet = pDlg->exec();

    m_aCreateMovieDialogGeometry = pDlg->saveGeometry();

	if( iRet == 1 )
	{
        pDlg->saveSettings();
	}

	delete pDlg;

}

void MinDiaWindow::sltExportDynGraphData()
{
    QString sFileName = QFileDialog::getSaveFileName( this, tr( "Export dynamic graphic data" ), QString::null, /*QString::null*/"*.dat" );

    if( !sFileName.isEmpty() )
	{
		m_pControler->sltExportDynGraphicData( sFileName );
	}
}

void MinDiaWindow::sltImportDynGraphData()
{
    QString sFileName = QFileDialog::getOpenFileName( this, tr( "Import dynamic graphic data" ), QString::null, /*QString::null*/"*.dat" );

    if( !sFileName.isEmpty() )
	{
		m_pControler->sltImportDynGraphicData( sFileName );
	}
}

void MinDiaWindow::sltCheckCloseApplication()
{
	if( m_pControler->IsChanged() )
	{
		int iRet = QMessageBox::warning( this, tr( "MinDia - Warning" ), tr( "Write changed data ?" ), tr( "Yes" ), tr( "No" ) );

		if( iRet == 0 )
		{
			bool bOk;
			m_pControler->sltSaveDoc( bOk );
		}
	}
}

void MinDiaWindow::sltUpdateEditMenu()
{
    bool bIsPlaying = m_pControler->IsPlayModus();
    bool bIsPause = m_pControler->IsPauseModus();
    bool bEdit = !(bIsPlaying || bIsPause);
	bool bValue = bEdit;

    m_pEditUndoAction->setEnabled( false );
	m_pEditRedoAction->setEnabled( false );
	m_pEditCutAction->setEnabled( bValue );
	m_pEditCopyAction->setEnabled( bValue );
	m_pEditPasteAction->setEnabled( bValue );
	m_pEditDeleteAction->setEnabled( bValue );
	m_pEditSelectAllAction->setEnabled( bValue );
	m_pEditNewDiaAction->setEnabled( bValue );
	m_pEditAddDiaAction->setEnabled( bValue );
	m_pEditFindAction->setEnabled( bValue );
	m_pEditFindNextAction->setEnabled( bValue );

    if( bEdit )
	{
		if( IsValidClipboardData() )
		{
			m_pEditPasteAction->setEnabled( true );
		}
		else
		{
			m_pEditPasteAction->setEnabled( false );
		}

        if( m_pSlideView->GetSelectedCount()>0 )
		{
			m_pEditCutAction->setEnabled( true );
			m_pEditCopyAction->setEnabled( true );
			m_pEditDeleteAction->setEnabled( true );
		}
		else
		{
			m_pEditCutAction->setEnabled( false );
			m_pEditCopyAction->setEnabled( false );
			m_pEditDeleteAction->setEnabled( false );
		}
    }
}

void MinDiaWindow::sltUpdateFileMenu()
{
	bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	bool bEdit = !(bIsPlaying || bIsPause);
	bool bValue = bEdit;

	m_pFileNewAction->setEnabled( bValue );
	m_pFileLoadAction->setEnabled( bValue );
    if( m_pFileLoadForEditAction ) { m_pFileLoadForEditAction->setEnabled( bValue ); }
	m_pFileSaveAction->setEnabled( bValue );
	m_pFileSaveAsAction->setEnabled( bValue );
	m_pFileExportAction->setEnabled( bValue );
	m_pFilePrintAction->setEnabled( bValue );

	m_pLastFilesSubMenu->setEnabled( bValue );
	m_pImportExportFilesSubMenu->setEnabled( bValue );
}

void MinDiaWindow::sltUpdatePlayMenu()
{
	bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	bool bEdit = !(bIsPlaying || bIsPause);
	bool bDiaSelected = (m_pSlideView->GetSelectedCount() == 1);

	m_pPlayStartFromAction->setEnabled( bEdit && bDiaSelected );
	m_pPlayStartAtAction->setEnabled( bEdit && bDiaSelected );
	m_pPlayStartAction->setEnabled( !bIsPlaying );
	m_pPlayPauseAction->setEnabled( bIsPlaying && !bIsPause );
	m_pPlayStopAction->setEnabled( bIsPlaying || bIsPause );
    m_pPlayAddSoundCommentAction->setEnabled( !bEdit );
    m_pPlayAddGraphicOpAction->setEnabled( !bEdit );

	if( m_pPlayInfoDialog )
	{
		m_pPlayInfoDialog->UpdateStatus( bIsPlaying, bIsPause );
	}

	bool bTwoDiasSelected = (m_pSlideView->GetSelectedCount() == 2);
	m_pPlayFadeInAction->setEnabled( bTwoDiasSelected );
	m_pPlayFadeOutAction->setEnabled( bTwoDiasSelected );
}

void MinDiaWindow::sltUpdateExtrasMenu()
{
	bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	bool bEdit = !(bIsPlaying || bIsPause);

	m_pExtrasConfigAction->setEnabled( bEdit );
	m_pExtrasPresentationDataAction->setEnabled( bEdit );
    if( m_pExtrasPresentationEventsAction )
    {
        m_pExtrasPresentationEventsAction->setEnabled( bEdit );
    }
	m_pExtrasSoundDataAction->setEnabled( bEdit );
	m_pExtrasSoundCommentAction->setEnabled( bEdit );
	m_pExtrasPlotCommentAction->setEnabled( bEdit );
}

void MinDiaWindow::sltUpdateLastFilesMenu()
{
	m_pLastFilesSubMenu->clear();

    QStringList aFileHistory = m_pControler->GetFileHistoryList();
    int iMax = aFileHistory.size();
    
	for( int i=iMax-1; i>=0; i-- )
	{
		// ** show filename with number 
		QString s;

		s.sprintf( "&%d ", iMax-i );
        s += aFileHistory[i];

        QAction * pAction = new QAction(s,this);
        pAction->setData(QVariant(i));
        m_pLastFilesSubMenu->addAction( pAction );
        connect( m_pLastFilesSubMenu, SIGNAL( triggered(QAction *) ), this, SLOT( sltLastFilesMenuActivated(QAction *) ) );
	}
}

void MinDiaWindow::sltLastFilesMenuActivated( QAction * pAction )
{
    QStringList aFileHistory = m_pControler->GetFileHistoryList();
    int iIndex = pAction->data().toInt();

    if( iIndex>=0 && iIndex<aFileHistory.size() )
	{
        sltLoadDoc( aFileHistory[iIndex], /*bExecuteEvent*/true );
	}
}

/*
void MinDiaWindow::sltUpdateScriptsMenu()
{
	bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	bool bEdit = !(bIsPlaying || bIsPause);

	// ** enable script dialog only in edit modus and if there is a script-engine available
//	minClientHandle<IGeneralScriptEnvironment> hScriptEnv( g_IGeneralScriptEnvironmentID );
//	bool bEnabled = bEdit && hScriptEnv.IsValid() && hScriptEnv->IsOk();
//	m_pPlugins->setEnabled( bEnabled );
}
*/

void MinDiaWindow::sltShowErrorMessage( const QString & sMessage )
{
	QMessageBox::warning( this, tr( "MinDia - Error" ), sMessage );
}

void MinDiaWindow::sltShowWarningMessage( const QString & sMessage )
{
	QMessageBox::warning( this, tr( "MinDia - Warning" ), sMessage );
}

void MinDiaWindow::sltShowStatusBarMessage( const QString & sMsg )
{
	if( m_pStatusBarMsg )
	{
        m_pStatusBarMsg->setText( sMsg );

		// clear tooltips showed over the status bar
        statusBar()->clearMessage();
        //ggf.statusBar()->showMessage( sMsg, 3000 );
	}
}

void MinDiaWindow::SetHelpFile( HelpDlgImpl * pHelpDialog, const QString & sHelpTag ) const
{
	// ** help-file is language sensitive
    QString sHelp("qrc:/help/");
	sHelp += "mindia_";
	sHelp += m_sLanguage;
	sHelp += ".html";
    QUrl aUrl( sHelp );

	// ** and add the tag for the jump in the document
    aUrl.setFragment( sHelpTag );

	// ** possible Help tags are (see code: emit sigDialogHelp) **
	/*
		mindia
		LoggingDialog
		ConfigurationDialog
		DiaInfoDialog
		HItemView
		MinDiaWindow
		ProjectorControlDialog
		PresentationDataDialog
		PlayInfoDialog
		CommentDialog	(for Sound and Plot)
		SoundInfoDialog
		EventMapDialog

		ScriptModifyDialog
	*/

    pHelpDialog->m_pTextBrowser->setSource( aUrl );
}

void MinDiaWindow::sltShowModalHelp( QWidget * pParent, const QString & sHelpTag )
{
    HelpDlgImpl * pHelpDialog = new HelpDlgImpl( pParent );

    pHelpDialog->setModal(true);
	pHelpDialog->move( 470, 5 );

	SetHelpFile( pHelpDialog, sHelpTag );

	pHelpDialog->exec();

	delete pHelpDialog;
}

void MinDiaWindow::sltShowHelp( const QString & sHelpTag )
{
	SetHelpFile( m_pHelpDialog, sHelpTag );

	m_pHelpDialog->show();
}

void MinDiaWindow::sltShowHelpForMe()
{
	sltShowHelp( /*html-tag*/"mindia" );
}

void MinDiaWindow::sltDoDocumentStateUpdate()
{
	// *** update caption of the window, with file name ! ***
	QString sCaption = "MinDia - [";
    sCaption += ToQString(m_pControler->GetName());
	if( m_pControler->IsChanged() )
	{
		sCaption += " *";
	}
	sCaption += "] ";
    sCaption += ToQString( m_pControler->GetPlayModusStrg() );
    setWindowTitle( sCaption );
}

void MinDiaWindow::sltDoSyncAllViews()
{
	m_pSlideView->SyncViewWithData();
	m_pTimeLineView->SyncViewWithData();
}

void MinDiaWindow::sltStartAutoStartTimer( bool bAutoRun, bool bShowScreen, bool bExpandImage, bool bExitOnFinished, int iScreenX, int iScreenY, int iPosX, int iPosY )
{
	m_pAutoStartTimer = new QTimer( this );

	m_bAutoRun = bAutoRun;
	m_bShowScreen = bShowScreen;
	m_bExpandImage = bExpandImage;
	m_bExitOnFinished = bExitOnFinished;
	m_iScreenX = iScreenX;
	m_iScreenY = iScreenY;
	m_iPosX = iPosX;
	m_iPosY = iPosY;

	connect( m_pAutoStartTimer, SIGNAL( timeout() ), this, SLOT( sltAutoStartTimerEvent() ) );

	m_pAutoStartTimer->start( c_ulAutoStartTimeout );
}

void MinDiaWindow::sltAutoStartTimerEvent()
{
	m_pAutoStartTimer->stop();

	if( m_bShowScreen )
	{
		emit sigShowPlayInfos();
	}
	if( m_bAutoRun )
	{
		emit sigStartPresentation();
	}

	delete m_pAutoStartTimer;
	m_pAutoStartTimer = 0;
}

void MinDiaWindow::sltStatusUpdateTimerEvent()
{
	m_pStatusUpdateTimer->stop();

	double dActTime = m_pControler->GetPresentation().GetActPlayTime();

	QString sPlay;
	QString sTotal;
	QString sCountDown;

	QTime aPlayTime;
	aPlayTime = aPlayTime.addMSecs( (int)(dActTime*1000) );
	QTime aTotalTime;
	aTotalTime = aTotalTime.addMSecs( (int)(m_pControler->GetPresentation().GetTotalTime()*1000) );

	double dCountDown = m_pControler->GetPresentation().GetCountDownTime();
	sCountDown.sprintf( "%.1f", dCountDown );

	sPlay.sprintf( "%d:%02d", aPlayTime.minute(), aPlayTime.second() );
	sTotal.sprintf( "%d:%02d", aTotalTime.minute(), aTotalTime.second() );

	bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	bool bIsEdit = m_pControler->IsEditModus();
	QString sMsg;
	if( !bIsEdit )
	{
		sMsg = tr( "play" ) + " / ";
	}
	sMsg += tr( "total time: " );
	if( !bIsEdit )
	{
		sMsg += sPlay + " / ";
	}
	sMsg += sTotal + tr( " min" );

	if( bIsPlaying || bIsPause )
	{
		sMsg += tr( " *** next: " ) + sCountDown + tr( " s" );
	}
	else
	{
		QString sCount;
		sCount.setNum( m_pControler->GetPresentation().GetDiaCount() );
		sMsg += tr( " *** count: " ) + sCount;
	}

	{
		QtMTLock aMTLock;

		m_pStatusBarTime->setText( sMsg );
        m_pStatusBarModus->setText( ToQString( m_pControler->GetPlayModusStrg() ) );

		// clear tooltips showed over the status bar
		// --> not here because than tooltips are never seen
		// because the status-bar is updated every few ms
		//statusBar()->clear();

		if( !bIsEdit )
		{
			// ** update play mark in TimeLineView **
			m_pTimeLineView->SetPlayMark( dActTime );
		}
		else
		{
//disabled since live play mark support (14.3.2013):			m_pTimeLineView->SetPlayMark( -1 );
		}
	}

	// ** restart the timer again
	m_pStatusUpdateTimer->start( c_ulStatusBarTimer );
}

void MinDiaWindow::sltDoUpdateAllViews()
{
	m_pSlideView->sltUpdateView();
	m_pTimeLineView->sltUpdateView();
}

void MinDiaWindow::sltDoDataChanged()
{
    //sltDoSyncAllViews();
	sltDoUpdateAllViews();
	sltDoDocumentStateUpdate();

    // update content of play info dialog (clipping area):
    sltItemSelected( m_iCount, m_pFirstSelectedItem, m_iFirstSelectedItemNo, 0 );
}

void MinDiaWindow::sltPlayFromSelected()
{
	int iSelectedIndex = m_pSlideView->GetLastSelectedItemIndex();

	m_pControler->sltGotoPosition( iSelectedIndex );
	m_pControler->sltPlayPresentationAt( iSelectedIndex );
}

void MinDiaWindow::sltPlayAtSelected()
{
	int iSelectedIndex = m_pSlideView->GetLastSelectedItemIndex();

	m_pControler->sltPlayPresentationAt( iSelectedIndex );
}

void MinDiaWindow::sltItemSelected( int iCount, HItem * pFirstSelectedItem, int iFirstSelectedItemNo, int iDissolveTimeInMS )
{
	m_iCount = iCount;
	m_pFirstSelectedItem = pFirstSelectedItem;
	m_iFirstSelectedItemNo = iFirstSelectedItemNo;

	if( m_pDiaInfoDialog )
	{
		if( iCount==1 )
		{
			m_pDiaInfoDialog->sltUpdateData( pFirstSelectedItem, m_pControler->IsEditModus() );
		}
		else
		{
			m_pDiaInfoDialog->sltDisableDialog( true );
		}
	}

    if( m_pPlayInfoDialog && m_pPlayInfoDialog->isVisible() )   // do not update play info window if not visible !
	{
		bool bIsPlaying = m_pControler->IsPlayModus();

		if( iCount==1 )
		{
            minHandle<DiaInfo> hDiaInfo = pFirstSelectedItem->GetInfoData();
            QString sFileName = ToQString( hDiaInfo->GetImageFile() );
            CopyImageAreaAsyncAndPostResult( m_pPlayInfoDialog, bIsPlaying, iDissolveTimeInMS, sFileName, hDiaInfo );
		}
		else
		{
            CopyImageAreaAsyncAndPostResult( m_pPlayInfoDialog, bIsPlaying, iDissolveTimeInMS, "white",  minHandle<DiaInfo>( 0 ) );
		}
	}

	// ** pass info of selected item to sub-views
	m_pSlideView->sltItemSelected( iCount, iFirstSelectedItemNo );
	m_pTimeLineView->sltItemSelected( iCount, iFirstSelectedItemNo );

	// ** update the edit menu, because here are some accelerator-keys used...
	sltUpdateEditMenu();
	sltUpdatePlayMenu();
}

void MinDiaWindow::sltPlayMarkChanged( double dTimePosInSec )
{
    if( m_pPlayInfoDialog && m_pPlayInfoDialog->isVisible() )
    {
// TODO --> Performance Optimierung: besser Bilder erst skalieren und dann setzen ?
        // let the play info dialog decide which output format is needed...
        QImage aImage = m_pControler->GetPresentation().GetSlideForTime( dTimePosInSec*1000.0, -1, -1, false );
        m_pPlayInfoDialog->SetCurrentImage( aImage, /*bForceSet*/true );
    }
    sltShowStatusBarMessage( QString(tr("play mark %1 sec")).arg(dTimePosInSec) );
}

void MinDiaWindow::sltSelectItem( int iIndex, int iDissolveTimeInMS )
{
	m_pSlideView->sltSelectItem( iIndex, iDissolveTimeInMS );
	m_pTimeLineView->sltSelectItem( iIndex, iDissolveTimeInMS );
}

void MinDiaWindow::sltNewItem()
{
    m_pSlideView->sltNewItem( m_dDissolveTime, m_dShowTime );
}

void MinDiaWindow::sltAddItem()
{
    m_pSlideView->AddNewItemAfterSelected( m_dDissolveTime, m_dShowTime );
}

void MinDiaWindow::sltFindItem()
{
    bool ok;
    QString sFind = QInputDialog::getText(this,tr("Search"),tr("search for:"),/*QLineEdit::EchoMode mode=*/QLineEdit::Normal,/*text=*/m_sFindText,&ok,/*Qt::WindowFlags flags =*/0);
    if( ok && !sFind.isEmpty() )
    {
		m_sFindText = sFind;

		m_pControler->sltSelectItemWithText( m_sFindText );
    }
}

void MinDiaWindow::sltFindNextItem()
{
	if( m_sFindText.isEmpty() )
	{
		sltFindItem();
	}
	else
	{
		m_pControler->sltSelectNextItemWithText( m_sFindText );
	}
}

void MinDiaWindow::sltUpdate()
{
    m_pTimeLineView->sltUpdateView();
    m_pSlideView->sltUpdateView();
}

void MinDiaWindow::sltTest()
{
    // for testing only
}

void MinDiaWindow::sltEditShowTime()
{
    bool ok;
    double dValue = QInputDialog::getDouble(this,tr("Enter new default show time"),tr("default show time [s]:"),(double)m_dShowTime,-2147483647,2147483647,1,&ok,/*Qt::WindowFlags flags =*/0);
    if( ok )
    {
        m_dShowTime = dValue;
    }
}

void MinDiaWindow::sltEditFadeInTime()
{
    bool ok;	
    double dValue = QInputDialog::getDouble(this,tr("Enter new default dissolve time"),tr("default dissolve time [s]:"),(double)m_dDissolveTime,-2147483647,2147483647,1,&ok,/*Qt::WindowFlags flags =*/0);
    if( ok )
    {
        m_dDissolveTime = dValue;
    }
}

void MinDiaWindow::sltFadeInTest()
{
	if( m_pPlayInfoDialog )
	{
		HItem * pItem1 = 0;
		HItem * pItem2 = 0;

		if( m_pSlideView->GetTwoSelectedItems( pItem1, pItem2 ) )
		{
            minHandle<DiaInfo> hDiaInfo1 = pItem1->GetInfoData();
            QString sFileName1 = ToQString( hDiaInfo1->GetImageFile() );
            CopyImageAreaAsyncAndPostResult( m_pPlayInfoDialog, /*bIsPlaying*/false, 0, sFileName1, hDiaInfo1 );

			int iDissolveTimeInMS = (int)(m_dDissolveTime * 1000.0);
            minHandle<DiaInfo> hDiaInfo2 = pItem2->GetInfoData();
            QString sFileName2 = ToQString( hDiaInfo2->GetImageFile() );
            CopyImageAreaAsyncAndPostResult( m_pPlayInfoDialog, /*bIsPlaying*/true, iDissolveTimeInMS, sFileName2, hDiaInfo2 );
        }
	}
}

void MinDiaWindow::sltFadeOutTest()
{
	if( m_pPlayInfoDialog )
	{
		HItem * pItem1 = 0;
		HItem * pItem2 = 0;

		if( m_pSlideView->GetTwoSelectedItems( pItem2, pItem1 ) )
		{
            minHandle<DiaInfo> hDiaInfo1 = pItem1->GetInfoData();
            QString sFileName1 = ToQString( hDiaInfo1->GetImageFile() );
            CopyImageAreaAsyncAndPostResult( m_pPlayInfoDialog, /*bIsPlaying*/false, 0, sFileName1, hDiaInfo1 );

			int iDissolveTimeInMS = (int)(m_dDissolveTime * 1000.0);
            minHandle<DiaInfo> hDiaInfo2 = pItem2->GetInfoData();
            QString sFileName2 = ToQString( hDiaInfo2->GetImageFile() );
            CopyImageAreaAsyncAndPostResult( m_pPlayInfoDialog, /*bIsPlaying*/true, iDissolveTimeInMS, sFileName2, hDiaInfo2 );
        }
	}
}

void MinDiaWindow::sltModusIsSwitched()
{
    sltUpdateFileMenu();
    sltUpdateEditMenu();
    sltUpdatePlayMenu();
    sltUpdateExtrasMenu();
	//sltUpdateScriptsMenu();

    sltDoDocumentStateUpdate();

    bool bIsPlaying = m_pControler->IsPlayModus();
	bool bIsPause = m_pControler->IsPauseModus();
	//bool bIsEdit = m_pControler->IsEditModus();

	if( m_pDiaInfoDialog )
	{
		if( bIsPlaying || bIsPause )
		{
			m_pDiaInfoDialog->sltDisableDialog( true );
		}
		else
		{
			m_pDiaInfoDialog->sltUpdateData( m_pFirstSelectedItem, m_pControler->IsEditModus() );
		}
	}

    // ** disable Dialog in play modus !
	if( m_pProjectorControlDialog && m_pProjectorControlDialog->isVisible() )
	{
		m_pProjectorControlDialog->sltCheckMode();
	}
}

void MinDiaWindow::sltPlayFinished()
{
	if( m_bExitOnFinished )
	{
		close();
	}
}

void MinDiaWindow::sltCopyClipboard()
{
	QClipboard * pClip = QApplication::clipboard();

	QString sData = "";
	if( pClip && m_pSlideView->GetActClipboardData( sData ) )
	{
 		pClip->setText( sData );
	}

	// ** update the state of the edit-menu, because now there is data in the clipboard
	sltUpdateEditMenu();
}

void MinDiaWindow::sltCutClipboard()
{
	sltCopyClipboard();
	sltDeleteSelectedItems();
}

void MinDiaWindow::sltPasteClipboard()
{
	QClipboard * pClip = QApplication::clipboard();

	if( pClip )
	{
		QString sData = pClip->text();

		if( !sData.isNull() )
		{
            // try to process as an internal Item format
            // or as a image path
            m_pSlideView->SetFromClipboardData( sData, false );
		}
	}

	// ** update the state of the edit-menu 
	sltUpdateEditMenu();
}

void MinDiaWindow::sltClipboardDataChanged()
{
    // update state of paste menu item
    sltUpdateEditMenu();
}

void MinDiaWindow::sltSelectAllClipboard()
{
	m_pSlideView->sltSelectAllItems( true, true );
}

void MinDiaWindow::sltDeleteSelectedItems()
{
	m_pSlideView->sltDeleteAllSlectedItems();
}

#include <QMimeData>

bool MinDiaWindow::IsValidClipboardData()
{
    QClipboard * pClip = QApplication::clipboard();

    if( pClip && pClip->mimeData() && pClip->mimeData()->hasText())
	{
        QString sData = pClip->text();

		if( !sData.isNull() )
		{
            if( IsImageFile(sData) )
            {
                return true;
            }
            else
            {
                // ** you need at leas one valid item, to enable paste
                return m_pSlideView->GetCountValidClipboardData( sData ) > 0;
            }
        }
	}

    return false;
}

void MinDiaWindow::closeEvent( QCloseEvent * pCloseEvent )
{
	sltCheckCloseApplication();

	if( m_pControler->IsEditModus() )
	{
		pCloseEvent->accept();
	}
	else
	{
		/*int iRet =*/ QMessageBox::warning( this, tr( "MinDia - Warning" ), tr( "Can't close the application, because the presentation is still playing !" ) );

		pCloseEvent->ignore();
	}
}

void MinDiaWindow::keyPressEvent( QKeyEvent * pEvent )
{
	if( (pEvent->key() == Qt::Key_F1) )
	{
		emit sigDialogHelp( "MinDiaWindow" );
	}
	else
	{
		QMainWindow::keyPressEvent( pEvent );
	}
}

bool MinDiaWindow::IsValid() const
{
	return ( m_pPlayInfoDialog != 0 );
}

bool MinDiaWindow::Show()
{
	sltShowPlayStatus();
	return m_pPlayInfoDialog->isVisible(); 
}

bool MinDiaWindow::Hide()
{
	if( m_pPlayInfoDialog )
	{
		m_pPlayInfoDialog->hide();
		return m_pPlayInfoDialog->isVisible(); 
	}
	return true;
}

bool MinDiaWindow::Maximize()
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->Maximize();
	}
	return false;
}

bool MinDiaWindow::Restore()
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->Restore();
	}
	return false;
}

int MinDiaWindow::GetWidth() const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetWidth();
	}
	return 0;
}

int MinDiaWindow::GetHeight() const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetHeight();
	}
	return 0;
}

bool MinDiaWindow::SetSize( int iWidth, int iHeight )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetSize( iWidth, iHeight );
	}
	return false;
}

bool MinDiaWindow::SetPos( int iXPos, int iYPos )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetPos( iXPos, iYPos );
	}
	return false;
}

int MinDiaWindow::GetDrawWidth() const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetDrawWidth();
	}
	return 0;
}

int MinDiaWindow::GetDrawHeight() const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetDrawHeight();
	}
	return 0;
}

bool MinDiaWindow::Clear()
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->Clear();
	}
	return false;
}

bool MinDiaWindow::SetColor( int iRed, int iGreen, int iBlue )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetColor( iRed, iGreen, iBlue );
	}
	return false;
}

bool MinDiaWindow::SetFont( const string & sFontName )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetFont( sFontName );
	}
	return false;
}

bool MinDiaWindow::SetFontSize( int iSizeInPixel, bool bBold, bool bItalic )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetFontSize( iSizeInPixel, bBold, bItalic );
	}
	return false;
}

int MinDiaWindow::SetTextXY( int x, int y, const string & sText )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetTextXY( x, y, sText );
	}
	return -1;
}

int MinDiaWindow::GetTextCount() const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetTextCount();
	}
	return 0;
}

bool MinDiaWindow::MoveText( int iTextID, int x, int y )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->MoveText( iTextID, x, y );
	}
	return false;
}

int	MinDiaWindow::GetTextX( int iTextID ) const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetTextX( iTextID );
	}
	return 0;
}

int	MinDiaWindow::GetTextY( int iTextID ) const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetTextY( iTextID );
	}
	return 0;
}

int MinDiaWindow::GetTextWidth( int iTextID ) const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetTextWidth( iTextID );
	}
	return 0;
}

int	MinDiaWindow::GetTextHeight( int iTextID ) const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetTextHeight( iTextID );
	}
	return 0;
}

bool MinDiaWindow::SetTextColor( int iTextID, int iRed, int iGreen, int iBlue )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->SetTextColor( iTextID, iRed, iGreen, iBlue );
	}
	return false;
}

IColor MinDiaWindow::GetTextColor( int iTextID ) const
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->GetTextColor( iTextID );
	}

	return IColor();
}

bool MinDiaWindow::DeleteText( int iTextID )
{
	if( m_pPlayInfoDialog )
	{
		return m_pPlayInfoDialog->DeleteText( iTextID );
	}
	return false;
}

QGraphicsScene * MinDiaWindow::GetCanvas()
{
    if( m_pPlayInfoDialog )
    {
        return m_pPlayInfoDialog->GetCanvas();
    }
    return 0;
}

void MinDiaWindow::SaveSettings()
{
    QSettings aSettings;
    // settings could be found for Mac plattform: $home/Library/Preferences/de.mneuroth.mindia.plist
    
    aSettings.setValue("App/DefaultDissolveTime",m_dDissolveTime);
    aSettings.setValue("App/DefaultShowTime",m_dShowTime);
    aSettings.setValue("App/DataFileName",m_sLastFileName);
    aSettings.setValue("App/HistoryFileNames",m_pControler->GetFileHistoryList());
    aSettings.setValue("App/WindowState",saveState());
    aSettings.setValue("App/WindowGeometry",saveGeometry());       
    // update all non modal dialog geometries when exiting application 
    if( m_pPlayInfoDialog )
    {
        m_aPlayInfoDialogGeometry = m_pPlayInfoDialog->saveGeometry();            
        aSettings.setValue("App/GeometryPlayInfoDlg",m_aPlayInfoDialogGeometry);
    }    
    if( m_pLoggingDialog )
    {
        m_aLoggingDialogGeometry = m_pLoggingDialog->saveGeometry();            
        aSettings.setValue("App/GeometryLoggingDlg",m_aLoggingDialogGeometry);
    }    
    if( m_pProjectorControlDialog )
    {
        m_aProjectorControlDialogGeometry = m_pProjectorControlDialog->saveGeometry();            
        aSettings.setValue("App/GeometryProjectorControlDlg",m_aProjectorControlDialogGeometry);
    }
    if( m_pDiaInfoDialog )        
    {
        m_aDiaInfoDialogGeometry = m_pDiaInfoDialog->saveGeometry();
        aSettings.setValue("App/GeometryDiaInfoDlg",m_aDiaInfoDialogGeometry);
    }
    if( m_aConfigDialogGeometry.count()>0 )
    { 
        aSettings.setValue("App/GeometryConfigDlg",m_aConfigDialogGeometry);
    }
    if( m_aCreateMovieDialogGeometry.count()>0 )
    {
        aSettings.setValue("App/GeometryCreateMovieDlg",m_aCreateMovieDialogGeometry);
    }
	aSettings.setValue("App/EnableScript",m_pControler->GetPresentation().GetScriptEnvironment().IsEnabled());
// TODO: ggf. save data for other dialogs...
    // presentation data
    // presentation events
    // sound data
    // sound comments
    // plot comments
    // ggf. dissolve time dialog 
    // ggf. find dialog
}

void MinDiaWindow::LoadSettings()
{
    QSettings aSettings;

    m_dDissolveTime = aSettings.value("App/DefaultDissolveTime",m_dDissolveTime).toDouble();
    m_dShowTime = aSettings.value("App/DefaultShowTime",m_dShowTime).toDouble();
    m_sLastFileName = aSettings.value("App/DataFileName",m_sLastFileName).toString();
    m_pControler->SetFileHistoryList(aSettings.value("App/HistoryFileNames",QVariant(QStringList())).toStringList());
    restoreState(aSettings.value("App/WindowState").toByteArray());
    restoreGeometry(aSettings.value("App/WindowGeometry").toByteArray());
    m_aPlayInfoDialogGeometry = aSettings.value("App/GeometryPlayInfoDlg",QVariant(QByteArray())).toByteArray();
    m_aLoggingDialogGeometry = aSettings.value("App/GeometryLoggingDlg",QVariant(QByteArray())).toByteArray();
    m_aProjectorControlDialogGeometry = aSettings.value("App/GeometryProjectorControlDlg",QVariant(QByteArray())).toByteArray();    
    m_aDiaInfoDialogGeometry = aSettings.value("App/GeometryDiaInfoDlg",QVariant(QByteArray())).toByteArray();
    m_aConfigDialogGeometry = aSettings.value("App/GeometryConfigDlg",QVariant(QByteArray())).toByteArray();
    m_aCreateMovieDialogGeometry = aSettings.value("App/GeometryCreateMovieDlg",QVariant(QByteArray())).toByteArray();
    
	m_pControler->GetPresentation().GetScriptEnvironment().SetEnabled( aSettings.value("App/EnableScript",true).toBool() );
}

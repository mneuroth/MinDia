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

// see: http://ffmpeg.org/trac/ffmpeg/wiki/Create%20a%20video%20slideshow%20from%20images

// http://stackoverflow.com/questions/11579952/add-multiple-audio-files-to-video-at-specific-points-using-ffmpeg

// http://superuser.com/questions/409987/how-to-embed-multiple-audio-files-to-one-video-file-with-timeline-offset
// http://ffmpeg.org/trac/ffmpeg/wiki/How%20to%20concatenate%20(join,%20merge)%20media%20files
// http://zoid.cc/ffmpeg-audio-video/

// https://docs.google.com/file/d/0BxRalAlWlsgjclFzOTVKLTBGUnM/edit?usp=drive_web&pli=1

// concat files with ffpmeg: http://ffmpeg.org/trac/ffmpeg/wiki/How%20to%20concatenate%20(join,%20merge)%20media%20files
// ffmpeg -f concat -i test.txt -c copy outffmpeg.mp3
// ffmpeg -i "concat:WhistleTheme.mp3|01_WalkThroughTheWorld.mp3" -c copy outputff.mp3

// ffmpeg -i video.mp4 -i sound.mp3 -qscale 0 videowithsound.mp4

// audio beschneiden: ffmpeg -i output_audio.aac -ss 00:00:30:00 -acodec copy -t 00:00:30:10 output_audio_cut.aac
// --> http://www.admindu.de/wordpress/?p=187
// ffmpeg -i sound.mp3 -ss 0 -t 10 -acodec copy out.mp3     // beschneiden auf 10 sec laenge !

// ffmpeg -i input.aac -f wav - | sox -t wav - -t wav - fade t 0 32 3 | ffmpeg -f wav -i - -acodec libfaac -ab 128k -ac 2 output.aac

// http://randykarels.com/blog/batch-create-audio-clips-with-fades/

// output formats: *.mpg, *.avi, *.mp4, *.mov, *.h264


#include "CreateMovieDlg4.h"

#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QSettings>
#include <QTextStream>

#include "doccontroler.h"
#include "diapresentation.h"

#include "misctools.h"

QApplication * GetApplication();

#if defined(Q_OS_WIN32)
#define CMD_SHELL       "cmd.exe"
#define CMD_SHELL_ARG   "/c"
#define CMD_TEST        "dir >d:\\outdir.txt"
#else
#define CMD_SHELL       "sh"
#define CMD_SHELL_ARG   "-c"
//#define CMD_TEST        "pwd >/Users/min/Documents/home/Entwicklung/projects/mindia/MinDia.zip/examples/outpwd.txt" // ls -lrt >/Users/min/Documents/home/Entwicklung/projects/mindia/MinDia.zip/examples/outdir.txt"
#define CMD_TEST        "/opt/local/bin/mpg123 -q /Users/min/Documents/home/Entwicklung/projects/mindia/MinDia.zip/examples/01_WalkThroughTheWorld.mp3" // ls -lrt >/Users/min/Documents/home/Entwicklung/projects/mindia/MinDia.zip/examples/outdir.txt"
#endif

// for resolutions see: http://de.wikipedia.org/wiki/Standard_Definition_Television
const QString g_sDefaultSize1 = QObject::tr("400:304");
const QString g_sDefaultSize2 = QObject::tr("576:400");
// 640x480   // NTSC (VGA)      4:3
// 800x600
// 1024x768
// 720x480   // NTSC (DV)       3:2
// 768x576   // PAL (DVB)       4:3
// 1024x576  // PAL-wide (DVB)  16:9
// 704x576   // PAL (DVD)
// 720x576   // PAL (DV)
// 960x720   // HD720 HDTV      4:3
// 1440x1080 // HD1080 HDTV     4:3
const QString g_sDefaultSize3 = QObject::tr("1280:720");        // 720p:  hd ready
const QString g_sDefaultSize4 = QObject::tr("1920:1080");       // 1080p: full hd
const QString g_sDefaultSize5 = QObject::tr("1920:1280");       // 3:2
const QString g_sSizeOfFirstImage = QObject::tr("size of first image");
const QString g_sUserValue = QObject::tr("user value");

// *************************************************************************

CreateMovieDlg4::CreateMovieDlg4(DocumentAndControler * pDocControler, double dTotalTimeMS, QWidget * parent)
: QDialog(parent),
  m_pDocControler(0),
  m_pProcess(0),
  m_bAutoDeleteProcess( false )
{
    ui.setupUi(this);

    m_dTotalTimeMS = dTotalTimeMS;
    m_pDocControler = pDocControler;

    unsigned long iWidth, iHeight;
    pDocControler->GetPresentation().GetImageSize( iWidth, iHeight );
    ui.m_pImageWidth->setText( QString("%1").arg(iWidth) );
    ui.m_pImageHeight->setText( QString("%1").arg(iHeight) );
    ui.m_pImageWidth->setEnabled( false );
    ui.m_pImageHeight->setEnabled( false );

    ui.m_pImagesPerSecond->setMinimum(1);
    ui.m_pImagesPerSecond->setMaximum(100);
    ui.m_pImagesPerSecond->setValue(25);

    ui.m_pImageRatio->insertItem( 0, g_sDefaultSize1 );
    ui.m_pImageRatio->insertItem( 1, g_sDefaultSize2 );
    ui.m_pImageRatio->insertItem( 2, g_sDefaultSize3 );
    ui.m_pImageRatio->insertItem( 3, g_sDefaultSize4 );
    ui.m_pImageRatio->insertItem( 4, g_sDefaultSize5 );
    ui.m_pImageRatio->insertItem( 5, g_sUserValue );
    ui.m_pImageRatio->insertItem( 6, g_sSizeOfFirstImage );

    ui.m_pMovieExtension->addItem("avi");
    ui.m_pMovieExtension->addItem("mov");
    ui.m_pMovieExtension->addItem("mp4");
    ui.m_pMovieExtension->addItem("mpg");

    ui.m_pImageExtension->addItem("jpg");
    ui.m_pImageExtension->addItem("bmp");
    ui.m_pImageExtension->addItem("png");
}

CreateMovieDlg4::~CreateMovieDlg4()
{
    DeleteProcess();
}

void CreateMovieDlg4::sltImageRatioSelected( const QString & sValue )
{
    if( sValue==g_sDefaultSize1 )
    {
        ui.m_pImageWidth->setText( sValue.split(":")[0] );
        ui.m_pImageHeight->setText( sValue.split(":")[1] );
        ui.m_pImageWidth->setEnabled( false );
        ui.m_pImageHeight->setEnabled( false );
    }
    else if( sValue==g_sDefaultSize2 )
    {
        ui.m_pImageWidth->setText( sValue.split(":")[0] );
        ui.m_pImageHeight->setText( sValue.split(":")[1] );
        ui.m_pImageWidth->setEnabled( false );
        ui.m_pImageHeight->setEnabled( false );
    }
    else if( sValue==g_sDefaultSize3 )
    {
        ui.m_pImageWidth->setText( sValue.split(":")[0] );
        ui.m_pImageHeight->setText( sValue.split(":")[1] );
        ui.m_pImageWidth->setEnabled( false );
        ui.m_pImageHeight->setEnabled( false );
    }
    else if( sValue==g_sDefaultSize4 )
    {
        ui.m_pImageWidth->setText( sValue.split(":")[0] );
        ui.m_pImageHeight->setText( sValue.split(":")[1] );
        ui.m_pImageWidth->setEnabled( false );
        ui.m_pImageHeight->setEnabled( false );
    }
    else if( sValue==g_sDefaultSize5 )
    {
        ui.m_pImageWidth->setText( sValue.split(":")[0] );
        ui.m_pImageHeight->setText( sValue.split(":")[1] );
        ui.m_pImageWidth->setEnabled( false );
        ui.m_pImageHeight->setEnabled( false );
    }
    else if( sValue==g_sUserValue )
    {
        ui.m_pImageWidth->setText( "" );
        ui.m_pImageHeight->setText( "" );
        ui.m_pImageWidth->setEnabled( true );
        ui.m_pImageHeight->setEnabled( true );
    }
    else if( sValue==g_sSizeOfFirstImage )
    {
        ui.m_pImageWidth->setText( "?" );
        ui.m_pImageHeight->setText( "?" );
        ui.m_pImageWidth->setEnabled( false );
        ui.m_pImageHeight->setEnabled( false );
    }
}

void CreateMovieDlg4::sltSelectOutputDirectory()
{
    QString sDir = QFileDialog::getExistingDirectory( this, tr("Select directory"), ui.m_pDirectoryName->text() );

    if( !sDir.isEmpty() )
    {
        ui.m_pDirectoryName->setText( sDir );
        UpdateCmds();
    }
}

void CreateMovieDlg4::sltCreateImages()
{
    sltDisable();

    QString sName = ui.m_pImageNameOffset->text();
    QString sDir = ui.m_pDirectoryName->text();
    int iImagesPerSecond = ui.m_pImagesPerSecond->text().toInt();
    int iWidth = ui.m_pImageWidth->text().toInt();
    int iHeight = ui.m_pImageHeight->text().toInt();

    double dDeltaInMS = 1/((double)iImagesPerSecond)*1000.0;

    // create temporary path if not existing...
    QDir aTempDir(ui.m_pDirectoryName->text());
    aTempDir.setFilter(QDir::Files);
    aTempDir.mkpath(ui.m_pDirectoryName->text());
    if( aTempDir.count()>0 )
    {
        if( QMessageBox::question(this,tr("Question"),tr("Temporary directory is not empty, remove all files in temporary directory?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
        {
            sltDeleteTempFiles( false );
        }
    }

// TODO gulp: Optimierung: referenzen auf Images erzeugen, wenn sich bild nicht geaendert hat ! --> Optimierung bei Image schreiben !
// TODO gulp: ggf. ffmpeg mit named pipes ansteuern

//http://stackoverflow.com/questions/5571566/give-input-to-ffmpeg-using-named-pipes
//
//ffmpeg -i \\.\pipe\vbam
//ffmpeg -f video4linux2 -i /dev/video0 -i record.ssa -scodec ass foo.mkv
//
//http://stackoverflow.com/questions/3861948/how-do-i-read-a-fifo-named-pipe-line-by-line-from-a-c-qt-linux-app
//
//http://www.developer.nokia.com/Community/Wiki/Named_Pipes_Example

    int iCount = m_pDocControler->CreateImagesForMovie( this,
              ToStdString( sDir ), ToStdString( sName ), ToStdString( QDir::separator() ), ToStdString( ui.m_pImageExtension->currentText() ),
              iWidth, iHeight,
              0, m_pDocControler->GetPresentation().GetTotalTime()*1000, dDeltaInMS );

    QString sOutput;
    QTextStream(&sOutput) << tr("Created ") << iCount << tr(" images");
    ui.m_pOutput->append( sOutput );

    sltEnable();
}

void CreateMovieDlg4::sltCreateAVI()
{
    RunCommands( ui.m_pGeneratorCmd->document()->toPlainText() );
}

void CreateMovieDlg4::sltAddSound()
{
    RunCommands( ui.m_pSoundGeneratorCmd->document()->toPlainText() );
}

void CreateMovieDlg4::sltMakeShow()
{
    sltCreateImages();
    sltCreateAVI();
    sltAddSound();
}

void CreateMovieDlg4::sltEnable()
{
    EnableDialog( true );
}

void CreateMovieDlg4::sltDisable()
{
    EnableDialog( false );
}

void CreateMovieDlg4::sltDeleteTempFiles( bool bAsk )
{
    QDir aTempDir(ui.m_pDirectoryName->text());
    QStringList lstAllFiles = aTempDir.entryList(QDir::Files);
    if( lstAllFiles.length()>0 && (!bAsk || QMessageBox::question(this,tr("Question"),tr("Really delete all files in temporary directory?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes) )
    {
        foreach( const QString & sFileName, lstAllFiles )
        {
            aTempDir.remove( sFileName );
        }
    }
}

void CreateMovieDlg4::sltChangeMjpegToolsDirectory()
{
    QString sDir = QFileDialog::getExistingDirectory(this,tr("Select directory"),ui.m_pMjpegtoolsDirectory->text() );

    if( !sDir.isEmpty() )
    {
        ui.m_pMjpegtoolsDirectory->setText( sDir );
    }
}

void CreateMovieDlg4::sltImagesPerSecondsChanged( const QString & /*sValue*/ )
{
    UpdateCmds();
}

void CreateMovieDlg4::sltImageExtensionChanged( const QString & /*sValue*/ )
{
    UpdateCmds();
}

void CreateMovieDlg4::sltImageOutputChanged( const QString & /*sValue*/ )
{
    UpdateCmds();
}

void CreateMovieDlg4::sltMovieOutputChanged( const QString & /*sValue*/ )
{
    UpdateCmds();
}

void CreateMovieDlg4::sltMovieExtensionChanged( const QString & /*sValue*/ )
{
    UpdateCmds();
}

void CreateMovieDlg4::sltOutputDirectoryChanged( const QString & /*sValue*/ )
{
    UpdateCmds();
}

void CreateMovieDlg4::sltSetToTemp()
{
    ui.m_pDirectoryName->setText(QDir::tempPath());
}

void CreateMovieDlg4::sltReadyReadStdOutput()
{
    if( m_pProcess )
    {
        ui.m_pOutput->append(m_pProcess->readAllStandardOutput());
    }
}

void CreateMovieDlg4::sltReadyReadStdError()
{
    if( m_pProcess )
    {
        ui.m_pOutput->append(m_pProcess->readAllStandardError());
    }
}

void CreateMovieDlg4::sltProcessFinished( int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/ )
{
    ui.m_pOutput->append("done.");

    if( m_bAutoDeleteProcess )
    {
        DeleteProcess();
    }
}

void CreateMovieDlg4::UpdateCmds()
{
    QString sName = ui.m_pImageNameOffset->text();
    QString sMovieOutput = ui.m_pMovieFileName->text();
    QString sMovieExtension = "."+ui.m_pMovieExtension->currentText();
    QString sSeparator = QDir::separator();
    QString sRate = QString("%1").arg( ui.m_pImagesPerSecond->value() );
    QString sFfmpegDir = ui.m_pMjpegtoolsDirectory->text();
    QString sImageExtension = ui.m_pImageExtension->currentText();
    QString sTempDir = ui.m_pDirectoryName->text();
    QString sNoSound = "nosound_";
    QString sTempSoundFile = sTempDir+sSeparator+"_temp_sound_.mp3";
    QString sCmd = QString("%6%2ffmpeg -f image2 -i %1%2%8.%7 -r %5 %1%2%3%4").arg( sTempDir ).arg( sSeparator ).arg( sNoSound+sMovieOutput ).arg( sMovieExtension ).arg( sRate ).arg( sFfmpegDir ).arg( sImageExtension ).arg( sName );

    ui.m_pGeneratorCmd->setText( sCmd );

    QString sSoundFiles = QString( "%1%2ffmpeg -i \"concat:" ).arg( sFfmpegDir ).arg( sSeparator );
    double dPresentationLength = m_pDocControler->GetPresentation().GetTotalTime();
    SoundInfoContainer aSoundContainer = m_pDocControler->GetPresentation().GetSoundInfoData();
    SoundInfoContainer::const_iterator aIter = aSoundContainer.begin();
    double dCurrentPos = 0.0;
    while( aIter!=aSoundContainer.end() )
    {
        double dCurrentLength = (double)(*aIter)->GetTotalLength()/1000.0;
        sSoundFiles += ToQString( (*aIter)->GetFileName() );
        aIter++;
        if( aIter != aSoundContainer.end() )
        {
            sSoundFiles += "|";
        }
        dCurrentPos += dCurrentLength;
    }
    sSoundFiles += "\" -c copy "+sTempSoundFile;

    QString sSounds = QString( "-i %1 -strict -2 -t %2 " ).arg( sTempSoundFile ).arg( dPresentationLength );

    sCmd = QString("%2 -y;\n%6%4ffmpeg -i %3%4%1%8 %7 -qscale 0 %3%4%5%8 -y").arg( sNoSound+sMovieOutput ).arg( sSoundFiles ).arg( sTempDir ).arg( sSeparator ).arg( sMovieOutput ).arg( sFfmpegDir ).arg( sSounds ).arg( sMovieExtension );

    if( aSoundContainer.size()>0 )
    {
        ui.m_pSoundGeneratorCmd->setText( sCmd );
    }
    else
    {
        ui.m_pSoundGeneratorCmd->setText( "<no sound found>" );
    }
}

void CreateMovieDlg4::saveSettings()
{
    QSettings aSettings;

    aSettings.setValue("CreateMovieDlg/OutputDir",ui.m_pDirectoryName->text());
    aSettings.setValue("CreateMovieDlg/OutputName",ui.m_pImageNameOffset->text());
    aSettings.setValue("CreateMovieDlg/OutputMovieName",ui.m_pMovieFileName->text());
    aSettings.setValue("CreateMovieDlg/MjpegToolsDir",ui.m_pMjpegtoolsDirectory->text());
    aSettings.setValue("CreateMovieDlg/ImagesPerSeconds",ui.m_pImagesPerSecond->value());
    aSettings.setValue("CreateMovieDlg/ImageSizeItem",ui.m_pImageRatio->currentIndex());
    aSettings.setValue("CreateMovieDlg/ImageExtension",ui.m_pImageExtension->currentIndex());
    aSettings.setValue("CreateMovieDlg/MovieExtension",ui.m_pMovieExtension->currentIndex());
}


void CreateMovieDlg4::restoreSettings()
{
    QSettings aSettings;

    QString sTempImagePath = QDir::tempPath()+QDir::separator()+"mindia_movie";
    ui.m_pDirectoryName->setText(aSettings.value("CreateMovieDlg/OutputDir",sTempImagePath).toString());
    QString sTempImageName("image%05d");
    ui.m_pImageNameOffset->setText(aSettings.value("CreateMovieDlg/OutputName",sTempImageName).toString());
    QString sTempMovieName("movie");
    ui.m_pMovieFileName->setText(aSettings.value("CreateMovieDlg/OutputMovieName",sTempMovieName).toString());
    // ffmpeg will be delivered from installation as default...
    QString sMjpegToolsDir = QCoreApplication::applicationDirPath()+QDir::separator()+"files";    // "/opt/local/bin";
    ui.m_pMjpegtoolsDirectory->setText(aSettings.value("CreateMovieDlg/MjpegToolsDir",sMjpegToolsDir).toString());
    ui.m_pImagesPerSecond->setValue(aSettings.value("CreateMovieDlg/ImagesPerSeconds",10).toInt());
    ui.m_pImageRatio->setCurrentIndex(aSettings.value("CreateMovieDlg/ImageSizeItem",0).toInt());
    ui.m_pImageExtension->setCurrentIndex(aSettings.value("CreateMovieDlg/ImageExtension",0).toInt());
    ui.m_pMovieExtension->setCurrentIndex(aSettings.value("CreateMovieDlg/MovieExtension",0).toInt());

    UpdateCmds();
}

void CreateMovieDlg4::CreateProcess( bool bAutoDelete )
{
    m_bAutoDeleteProcess = bAutoDelete;
    m_pProcess = new QProcess(this);

    connect(m_pProcess,SIGNAL( readyReadStandardOutput() ), this, SLOT( sltReadyReadStdOutput() ) );
    connect(m_pProcess,SIGNAL( readyReadStandardError() ), this, SLOT( sltReadyReadStdError() ) );
    connect(m_pProcess,SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( sltProcessFinished( int, QProcess::ExitStatus ) ) );
}

void CreateMovieDlg4::DeleteProcess()
{
    delete m_pProcess;
    m_pProcess = 0;
}

void CreateMovieDlg4::EnableDialog( bool value )
{
    ui.m_pCreateAVI->setEnabled( value );
    ui.m_pCreateImages->setEnabled( value );
    ui.m_pAddSound->setEnabled( value );
    ui.m_pDeleteTemp->setEnabled( value );
    ui.m_pMakeShow->setEnabled( value );
    ui.m_pChangeMjpegToolsDirectory->setEnabled( value );
    ui.m_pDirectoryName->setEnabled( value );
    ui.m_pImagesPerSecond->setEnabled( value );
    ui.m_pSelectDirectory->setEnabled( value );
    ui.m_pSetToTemp->setEnabled( value );
    ui.m_pImageExtension->setEnabled( value );
    ui.m_pMovieExtension->setEnabled( value );
    ui.m_pImageRatio->setEnabled( value );
}

void CreateMovieDlg4::RunCommands( const QString & sCmds )
{
    sltDisable();

    // switch to logging output tab page
    ui.tabWidgetOps->setCurrentIndex(1);

    ProcessCommands( sCmds );

    sltEnable();
}

void CreateMovieDlg4::ProcessCommands( const QString & sCmdsIn )
{
    QStringList aCmdList = sCmdsIn.split(';');

    if( m_pProcess==0 )
    {
        CreateProcess( false );

        foreach( const QString & sCmd, aCmdList)
        {
            ui.m_pOutput->append( sCmd );

            QString sProg = CMD_SHELL;
            QStringList aArgs;
#ifdef Q_WS_WIN
            // see: http://stackoverflow.com/questions/12769321/qprocess-and-command-line-c-argument
            aArgs << CMD_SHELL_ARG;
            m_pProcess->setNativeArguments(sCmd);
#else
            aArgs << CMD_SHELL_ARG << sCmd;
#endif
            m_pProcess->start(sProg,aArgs);

            while( m_pProcess->state()!=QProcess::NotRunning )
            {
                GetApplication()->processEvents();
            }
            m_pProcess->waitForFinished();
        }

        DeleteProcess();
    }
}



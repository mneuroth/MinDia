
#include "CreateMovieDlg4.h"

#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QSettings>
#include <QTextStream>

#include "doccontroler.h"

#include "misctools.h"

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
// 768x576   // PAL (DVB)
// 704x576   // PAL (DVD)
// 720x576   // PAL (DV)
const QString g_sDefaultSize3 = QObject::tr("1280:720");        // 720p:  hd ready
const QString g_sDefaultSize4 = QObject::tr("1920:1080");       // 1080p: full hd
const QString g_sSizeOfFirstImage = QObject::tr("size of first image");
const QString g_sUserValue = QObject::tr("user value");

CreateMovieDlg4::CreateMovieDlg4(DocumentAndControler * pDocControler, double dTotalTimeMS, QWidget * parent)
: QDialog(parent),
  m_pDocControler(0),
  m_pProcess(0)
{
    ui.setupUi(this);

    m_dTotalTimeMS = dTotalTimeMS;
    m_pDocControler = pDocControler;

    ui.m_pImagesPerSecond->setMinimum(1);
    ui.m_pImagesPerSecond->setMaximum(100);
    ui.m_pImagesPerSecond->setValue(25);

    ui.m_pImageRatio->insertItem( 0, g_sDefaultSize1 );
    ui.m_pImageRatio->insertItem( 1, g_sDefaultSize2 );
    ui.m_pImageRatio->insertItem( 2, g_sDefaultSize3 );
    ui.m_pImageRatio->insertItem( 3, g_sDefaultSize4 );
    ui.m_pImageRatio->insertItem( 4, g_sUserValue );
    ui.m_pImageRatio->insertItem( 5, g_sSizeOfFirstImage );

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
    delete m_pProcess;
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
    QString sDir = QFileDialog::getExistingDirectory( this, tr("Open Directory"), ui.m_pDirectoryName->text() );

    if( !sDir.isEmpty() )
    {
        ui.m_pDirectoryName->setText( sDir );
        UpdateCmds();
    }
}

void CreateMovieDlg4::sltCreateImages()
{
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

    int iCount = m_pDocControler->CreateImagesForMovie( this,
              ToStdString( sDir ), ToStdString( sName ), ToStdString( QDir::separator() ), ToStdString( ui.m_pImageExtension->currentText() ),
              iWidth, iHeight,
              0, m_pDocControler->GetPresentation().GetTotalTime()*1000, dDeltaInMS );

    QString sOutput;
    QTextStream(&sOutput) << tr("Created ") << iCount << tr(" images");
    ui.m_pOutput->append( sOutput );
}

void CreateMovieDlg4::sltCreateAVI()
{
    // http://superuser.com/questions/409987/how-to-embed-multiple-audio-files-to-one-video-file-with-timeline-offset
    // http://ffmpeg.org/trac/ffmpeg/wiki/How%20to%20concatenate%20(join,%20merge)%20media%20files
    // http://zoid.cc/ffmpeg-audio-video/

    // https://docs.google.com/file/d/0BxRalAlWlsgjclFzOTVKLTBGUnM/edit?usp=drive_web&pli=1

    // concat files with ffpmeg: http://ffmpeg.org/trac/ffmpeg/wiki/How%20to%20concatenate%20(join,%20merge)%20media%20files
    // ffmpeg -f concat -i test.txt -c copy outffmpeg.mp3
    // ffmpeg -i "concat:WhistleTheme.mp3|01_WalkThroughTheWorld.mp3" -c copy outputff.mp3

    // ffmpeg -f image2 -i image%05d.jpg output.mpg
    //QString sName = ui.m_pImageNameOffset->text();
    //QString sOutput = ui.m_pMovieFileName->text();
    //QDir aTempDir(ui.m_pDirectoryName->text());
// TODO: check if sOutput eine extension besitzt, falls nicht eine dran haengen
// TODO --> pfade korrekt setzen etc...
    QString sCmd; // = QString("C:\\usr\\neurothmi\\Android\\mindia-1.0.2\\src\\debug\\ffmpeg -f image2 -i %1/%2.jpg %3").arg(aTempDir.absolutePath()).arg( sName ).arg( sOutput );

    // TODO: hier muss %05d stehen, noch nicht beim image erzeugen !
    //QString sCmd("%1%2jpeg2yuv -I p -f %3 -j %6%2%4%7.jpg | %1%2yuv2lav -f avi -o %6%2%5.avi");
    //sCmd = sCmd.arg(ui.m_pMjpegtoolsDirectory->text(),QString(QDir::separator()),ui.m_pImagesPerSecond->text(),ui.m_pImageNameOffset->text(),ui.m_pMovieFileName->text(),ui.m_pDirectoryName->text(),QString("%05d") );

    sCmd = ui.m_pGeneratorCmd->text();

    ui.m_pOutput->append( sCmd );

    //QString sProg = CMD_SHELL;
    //QStringList aArgs;
    //aArgs << CMD_SHELL_ARG << sCmd;
    //bool bOk = QProcess::execute(sProg,aArgs);

    if( m_pProcess==0 )
    {
        m_pProcess = new QProcess(this);

        connect(m_pProcess,SIGNAL( readyReadStandardOutput() ), this, SLOT( sltReadyReadStdOutput() ) );
        connect(m_pProcess,SIGNAL( readyReadStandardError() ), this, SLOT( sltReadyReadStdError() ) );
        connect(m_pProcess,SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( sltProcessFinished( int, QProcess::ExitStatus ) ) );

        QString sProg = CMD_SHELL;
        QStringList aArgs;
        aArgs << CMD_SHELL_ARG << sCmd;
        m_pProcess->start(sProg,aArgs);
//        bool bOk = QProcess::execute(sProg,aArgs);
//        QString sOut;
//        sOut.sprintf("started %d",(int)bOk);
//        ui.m_pOutput->append(sOut);
    }
}

void CreateMovieDlg4::sltAddSound()
{
    // ffmpeg -i video.mp4 -i sound.mp3 -qscale 0 videowithsound.mp4

    QString sCmdIn = ui.m_pSoundGeneratorCmd->text();
    QStringList aCmdList = sCmdIn.split(';');

    if( m_pProcess==0 )
    {
        m_pProcess = new QProcess(this);

        connect(m_pProcess,SIGNAL( readyReadStandardOutput() ), this, SLOT( sltReadyReadStdOutput() ) );
        connect(m_pProcess,SIGNAL( readyReadStandardError() ), this, SLOT( sltReadyReadStdError() ) );
        connect(m_pProcess,SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( sltProcessFinished( int, QProcess::ExitStatus ) ) );

        foreach( const QString & sCmd, aCmdList)
        {
            ui.m_pOutput->append( sCmd );

            QString sProg = CMD_SHELL;
            QStringList aArgs;
            aArgs << CMD_SHELL_ARG << sCmd;
            m_pProcess->start(sProg,aArgs);
            m_pProcess->waitForFinished();
        }
//        bool bOk = QProcess::execute(sProg,aArgs);
//        QString sOut;
//        sOut.sprintf("started %d",(int)bOk);
//        ui.m_pOutput->append(sOut);
    }

    // audio beschneiden: ffmpeg -i output_audio.aac -ss 00:00:30:00 -acodec copy -t 00:00:30:10 output_audio_cut.aac
    // --> http://www.admindu.de/wordpress/?p=187
    // ffmpeg -i sound.mp3 -ss 0 -t 10 -acodec copy out.mp3     // beschneiden auf 10 sec laenge !

    // ffmpeg -i input.aac -f wav - | sox -t wav - -t wav - fade t 0 32 3 | ffmpeg -f wav -i - -acodec libfaac -ab 128k -ac 2 output.aac

    // http://randykarels.com/blog/batch-create-audio-clips-with-fades/

    // output formats: *.mpg, *.avi, *.mp4, *.mov, *.h264

//    if( m_pProcess )
//    {
//        delete m_pProcess;
//        m_pProcess = 0;
//    }

//    m_pProcess = new QProcess( this );

//// TODO --> nur fuer Windows !!!
//    m_pProcess->addArgument("cmd.exe");
//    m_pProcess->addArgument("/k");
//    m_pProcess->addArgument("dir");
//    m_pProcess->addArgument("/od");
//    m_pProcess->addArgument("|");
//    m_pProcess->addArgument("grep");
//    m_pProcess->addArgument("INSTALL");

//    connect( m_pProcess, SIGNAL(readyReadStdout()), this, SLOT(sltReadFromStdout()) );

//    m_pProcess->start();
}

void CreateMovieDlg4::sltCreateVCD()
{
    QMessageBox::warning(this,"Warning","Not implemented yet !");
}

void CreateMovieDlg4::sltMakeShow()
{
    QMessageBox::warning(this,"Warning","Not implemented yet !");
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
    QString sDir = QFileDialog::getExistingDirectory(this,tr("Existing Directory"),ui.m_pMjpegtoolsDirectory->text() );

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

    delete m_pProcess;
    m_pProcess = 0;
}

void CreateMovieDlg4::UpdateCmds()
{
    // see: http://ffmpeg.org/trac/ffmpeg/wiki/Create%20a%20video%20slideshow%20from%20images

    QString sName = ui.m_pImageNameOffset->text();
    QString sMovieOutput = ui.m_pMovieFileName->text();
    QString sMovieExtension = "."+ui.m_pMovieExtension->currentText();
    QString sSeparator = QDir::separator();
    QString sRate = QString("%1").arg( ui.m_pImagesPerSecond->value() );
    QString sFfmpegDir = ui.m_pMjpegtoolsDirectory->text();
    QString sImageExtension = ui.m_pImageExtension->currentText();
    QString sNoSound = "nosound_";
    QString sTempSoundFile = "_temp_sound_.mp3";
    //QDir aTempDir(ui.m_pDirectoryName->text());
    QString sTempDir = ui.m_pDirectoryName->text();
    QString sCmd = QString("%6%2ffmpeg -f image2 -i %1%2%8.%7 -r %5 %1%2%3%4").arg( sTempDir ).arg( sSeparator ).arg( sNoSound+sMovieOutput ).arg( sMovieExtension ).arg( sRate ).arg( sFfmpegDir ).arg( sImageExtension ).arg( sName );

    ui.m_pGeneratorCmd->setText( sCmd );

    // ffmpeg -i video.mp4 -i sound.mp3 -qscale 0 videowithsound.mp4

    QString sSoundFiles = QString( "%1%2ffmpeg -i \"concat:" ).arg( sFfmpegDir ).arg( sSeparator );
    double dPresentationLength = m_pDocControler->GetPresentation().GetTotalTime();
    SoundInfoContainer aSoundContainer = m_pDocControler->GetPresentation().GetSoundInfoData();
    SoundInfoContainer::const_iterator aIter = aSoundContainer.begin();
    double dCurrentPos = 0.0;
    while( aIter!=aSoundContainer.end() )
    {
        double dCurrentLength = (double)(*aIter)->GetTotalLength()/1000.0;
        //sSoundFiles += QString( "-itsoffset %1 " ).arg( dCurrentPos );
        //sSoundFiles += "-i "+ToQString( (*aIter)->GetFileName() )+" ";
        //sSoundFiles += QString( "-ss %1 " ).arg( 0 );              // start always at beginning
        //sSoundFiles += QString( "-t %1 " ).arg( dCurrentLength );
        sSoundFiles += ToQString( (*aIter)->GetFileName() );
        aIter++;
        if( aIter != aSoundContainer.end() )
        {
            sSoundFiles += "|";
        }
        dCurrentPos += dCurrentLength;
    }
    sSoundFiles += "\" -c copy "+sTempSoundFile;

    QString sSounds = QString( "-i %1 -t %2 " ).arg( sTempSoundFile ).arg( dPresentationLength );

// http://stackoverflow.com/questions/11579952/add-multiple-audio-files-to-video-at-specific-points-using-ffmpeg

    //QString sSoundFile = "sound.mp3";
// TODO --> start und stop fuer soundfile angeben, soundfiles loopen ueber alle verfuegbare...
    sCmd = QString("%2; %6%4ffmpeg -i %3%4%1%8 %7 -qscale 0 %3%4%5%8").arg( sNoSound+sMovieOutput ).arg( sSoundFiles ).arg( sTempDir ).arg( sSeparator ).arg( sMovieOutput ).arg( sFfmpegDir ).arg( sSounds ).arg( sMovieExtension );

    if( aSoundContainer.size()>0 )
    {
        ui.m_pSoundGeneratorCmd->setText( sCmd );
    }
    else
    {
        ui.m_pSoundGeneratorCmd->setText( "<no sound found>" );
    }

    //QString sCmd;
    //sCmd.sprintf( "jpeg2yuv -I p -f %d -j movie\\image%05d.jpg | yuv2lav -o movie\\movie.avi", iImagesPerSecond );

    // for mac: use mjpegtools 2.0.0 (1.9.0 funktioniert nicht --> Segmentation fault) aber ggf. ist die Qualitaet nicht so gut !?

// TODO --> pfade anpassen an Plattform !!! Qt immer / ?
//    ui.m_pGeneratorCmd->setText( QString("jpeg2yuv -I p -f %1 -j <output_dir>%2%3.jpg | yuv2lav -f avi -o <output_dir>%4%5.avi").arg(ui.m_pImagesPerSecond->value()).arg(QDir::separator()).arg(ui.m_pImageNameOffset->text()).arg(QDir::separator()).arg(ui.m_pMovieFileName->text()));

    //ui.m_pGeneratorCmd->setText( QString("<mjpegtools_dir>%2jpeg2yuv -I p -f %1 -j <output_dir>%2%3%6.jpg | <mjpegtools_dir>%2yuv2lav -f avi -o <output_dir>%4%5.avi").arg(ui.m_pImagesPerSecond->text(),QString(QDir::separator()),ui.m_pImageNameOffset->text(),QString(QDir::separator()),ui.m_pMovieFileName->text(),QString("%05d") ) );
    //ui.m_pSoundGeneratorCmd->setText( QString("<mjpegtools_dir>%1lavaddwav <output_dir>%1%2.avi <sound_for_movie>.wav <output_dir>%1%2.avi").arg(QString(QDir::separator()),ui.m_pMovieFileName->text()) );

    // convert mp3 to wav: mpg123 -w 01_WalkThroughTheWorld.wav 01_WalkThroughTheWorld.mp3
    // cat mp3file1.mp3 >movie.mp3
    // cat mp3file2.mp3 >>movie.mp3     --> geht nicht !

    // lavaddwav mit mehreren wav files moeglich? --> wohl nicht !

    // mpg123 -w sound.wav sound.mp3
    // sox sound1.wav sound2.wav sound_out.wav

    // http://sox.sourceforge.net/Main/HomePage


    //ui.m_pVCDGeneratorCmd->setText( QString("<mjpegtools_dir>%1lav2yuv <output_dir>%1%2.avi | <mjpegtools_dir>%1yuvscaler -O VCD | <mjpegtools_dir>%1mpeg2enc -f 1 -r 16 -o <output_dir>%1%2.mpg").arg(QString(QDir::separator()),ui.m_pMovieFileName->text()) );

}

void CreateMovieDlg4::saveSettings()
{
    QSettings aSettings;

    aSettings.setValue("CreateMoveDlg/OutputDir",ui.m_pDirectoryName->text());
    aSettings.setValue("CreateMoveDlg/OutputName",ui.m_pImageNameOffset->text());    
    aSettings.setValue("CreateMoveDlg/OutputMovieName",ui.m_pMovieFileName->text());
    aSettings.setValue("CreateMoveDlg/MjpegToolsDir",ui.m_pMjpegtoolsDirectory->text());
    aSettings.setValue("CreateMoveDlg/ImagesPerSeconds",ui.m_pImagesPerSecond->value());
    aSettings.setValue("CreateMoveDlg/ImageSizeItem",ui.m_pImageRatio->currentIndex());
    aSettings.setValue("CreateMoveDlg/ImageExtension",ui.m_pImageExtension->currentIndex());
    aSettings.setValue("CreateMoveDlg/MovieExtension",ui.m_pMovieExtension->currentIndex());
}


void CreateMovieDlg4::restoreSettings()
{
    QSettings aSettings;

    QString sTempImagePath = QDir::tempPath()+QDir::separator()+"mindia_movie";
    ui.m_pDirectoryName->setText(aSettings.value("CreateMoveDlg/OutputDir",sTempImagePath).toString());
    QString sTempImageName("image%05d");
    ui.m_pImageNameOffset->setText(aSettings.value("CreateMoveDlg/OutputName",sTempImageName).toString());
    QString sTempMovieName("movie");
    ui.m_pMovieFileName->setText(aSettings.value("CreateMoveDlg/OutputMovieName",sTempMovieName).toString());
    QString sMjpegToolsDir = "/opt/local/bin";
    ui.m_pMjpegtoolsDirectory->setText(aSettings.value("CreateMoveDlg/MjpegToolsDir",sMjpegToolsDir).toString());
    ui.m_pImagesPerSecond->setValue(aSettings.value("CreateMoveDlg/ImagesPerSeconds",10).toInt());
    ui.m_pImageRatio->setCurrentIndex(aSettings.value("CreateMoveDlg/ImageSizeItem",0).toInt());
    ui.m_pImageExtension->setCurrentIndex(aSettings.value("CreateMoveDlg/ImageExtension",0).toInt());
    ui.m_pMovieExtension->setCurrentIndex(aSettings.value("CreateMoveDlg/MovieExtension",0).toInt());

    UpdateCmds();
}

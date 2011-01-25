
#include "CreateMovieDlg4.h"

#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include <QSettings>

#include "doccontroler.h"

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

const QString g_sDefaultSize1 = QObject::tr("400:304");
const QString g_sDefaultSize2 = QObject::tr("576:400");
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

    ui.m_pImagesPerSecond->setMinValue(1);
    ui.m_pImagesPerSecond->setMaxValue(100);
    ui.m_pImagesPerSecond->setValue(10);

    ui.m_pImageRatio->insertItem( g_sDefaultSize1 );
    ui.m_pImageRatio->insertItem( g_sDefaultSize2 );
    ui.m_pImageRatio->insertItem( g_sUserValue );
    ui.m_pImageRatio->insertItem( g_sSizeOfFirstImage );
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
    if( sValue==g_sDefaultSize2 )
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
    QString sDir = QFileDialog::getExistingDirectory(ui.m_pDirectoryName->text() );

    if( !sDir.isEmpty() )
    {
        ui.m_pDirectoryName->setText( sDir );
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
    aTempDir.mkpath(ui.m_pDirectoryName->text());
    if( aTempDir.count()>0 )
    {
        if( QMessageBox::question(this,tr("Question"),tr("Temporary directory is not empty, remove all files in temporary directory?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
        {
            // TODO --> temporary directory loeschen
        }
    }

    int iCount = m_pDocControler->CreateImagesForMovie(
             (const char *)sDir, (const char *)sName,
              iWidth, iHeight,
              0, m_pDocControler->GetPresentation().GetTotalTime()*1000, dDeltaInMS );

    QString sOutput;
    sOutput.sprintf( tr("Created %d images"), iCount );
    ui.m_pOutput->append( sOutput );
}

void CreateMovieDlg4::sltCreateAVI()
{
    // TODO: hier muss %05d stehen, noch nicht beim image erzeugen !
    QString sCmd("%1%2jpeg2yuv -I p -f %3 -j %6%2%4%7.jpg | %1%2yuv2lav -f avi -o %6%2%5.avi");
    sCmd = sCmd.arg(ui.m_pMjpegtoolsDirectory->text(),QString(QDir::separator()),ui.m_pImagesPerSecond->text(),ui.m_pImageNameOffset->text(),ui.m_pMovieFileName->text(),ui.m_pDirectoryName->text(),QString("%05d") );

    ui.m_pOutput->append( sCmd );

    cout << "CREATE AVI " <<(const char *)sCmd << endl;

    QString sProg = CMD_SHELL;
    QStringList aArgs;
    aArgs << CMD_SHELL_ARG << sCmd;
    bool bOk = QProcess::execute(sProg,aArgs);
    cout << "execute = " << bOk << endl;

    if( m_pProcess==0 )
    {
        m_pProcess = new QProcess(this);

        QString sProg = CMD_SHELL;
        QStringList aArgs;
        aArgs << CMD_SHELL_ARG << sCmd;
        m_pProcess->start(sProg,aArgs);
//        bool bOk = QProcess::execute(sProg,aArgs);
//        QString sOut;
//        sOut.sprintf("started %d",(int)bOk);
//        ui.m_pOutput->append(sOut);
        connect(m_pProcess,SIGNAL( readyReadStandardOutput() ), this, SLOT( sltReadyReadStdOutput() ) );
        connect(m_pProcess,SIGNAL( readyReadStandardError() ), this, SLOT( sltReadyReadStdError() ) );
        connect(m_pProcess,SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( sltProcessFinished( int, QProcess::ExitStatus ) ) );
    }
}

void CreateMovieDlg4::sltAddSound()
{
//    if( m_pProcess )
//    {
//        delete m_pProcess;
//        m_pProcess = 0;
//    }

//    m_pProcess = new Q3Process( this );

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

void CreateMovieDlg4::sltDeleteTempFiles()
{
    QMessageBox::warning(this,"Warning","Not implemented yet !");
}

void CreateMovieDlg4::sltChangeMjpegToolsDirectory()
{
    QString sDir = QFileDialog::getExistingDirectory(ui.m_pMjpegtoolsDirectory->text() );

    if( !sDir.isEmpty() )
    {
        ui.m_pMjpegtoolsDirectory->setText( sDir );
    }
}

void CreateMovieDlg4::sltImagesPerSecondsChanged( const QString & sValue )
{
    UpdateCmds();
}


void CreateMovieDlg4::sltImageOutputChanged( const QString & sValue )
{
    UpdateCmds();
}

void CreateMovieDlg4::sltMovieOutputChanged( const QString &)
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

void CreateMovieDlg4::sltProcessFinished( int exitCode, QProcess::ExitStatus exitStatus )
{
    ui.m_pOutput->append("done.");

    delete m_pProcess;
    m_pProcess = 0;
}

void CreateMovieDlg4::UpdateCmds()
{
    QString sCmd;
    //sCmd.sprintf( "jpeg2yuv -I p -f %d -j movie\\image%05d.jpg | yuv2lav -o movie\\movie.avi", iImagesPerSecond );

    // for mac: use mjpegtools 2.0.0 (1.9.0 funktioniert nicht --> Segmentation fault) aber ggf. ist die Qualitaet nicht so gut !?

// TODO --> pfade anpassen an Plattform !!! Qt immer / ?
//    ui.m_pGeneratorCmd->setText( QString("jpeg2yuv -I p -f %1 -j <output_dir>%2%3.jpg | yuv2lav -f avi -o <output_dir>%4%5.avi").arg(ui.m_pImagesPerSecond->value()).arg(QDir::separator()).arg(ui.m_pImageNameOffset->text()).arg(QDir::separator()).arg(ui.m_pMovieFileName->text()));


    ui.m_pGeneratorCmd->setText( QString("<mjpegtools_dir>%2jpeg2yuv -I p -f %1 -j <output_dir>%2%3%6.jpg | <mjpegtools_dir>%2yuv2lav -f avi -o <output_dir>%4%5.avi").arg(ui.m_pImagesPerSecond->text(),QString(QDir::separator()),ui.m_pImageNameOffset->text(),QString(QDir::separator()),ui.m_pMovieFileName->text(),QString("%05d") ) );
    ui.m_pSoundGeneratorCmd->setText( QString("<mjpegtools_dir>%1lavaddwav <output_dir>%1%2.avi <sound_for_movie>.wav <output_dir>%1%2.avi").arg(QString(QDir::separator()),ui.m_pMovieFileName->text()) );

    // convert mp3 to wav: mpg123 -w 01_WalkThroughTheWorld.wav 01_WalkThroughTheWorld.mp3
    // cat mp3file1.mp3 >movie.mp3
    // cat mp3file2.mp3 >>movie.mp3     --> geht nicht !

    // lavaddwav mit mehreren wav files moeglich? --> wohl nicht !

    // mpg123 -w sound.wav sound.mp3
    // sox sound1.wav sound2.wav sound_out.wav

    // http://sox.sourceforge.net/Main/HomePage


    ui.m_pVCDGeneratorCmd->setText( QString("<mjpegtools_dir>%1lav2yuv <output_dir>%1%2.avi | <mjpegtools_dir>%1yuvscaler -O VCD | <mjpegtools_dir>%1mpeg2enc -f 1 -r 16 -o <output_dir>%1%2.mpg").arg(QString(QDir::separator()),ui.m_pMovieFileName->text()) );

}

void CreateMovieDlg4::saveSettings()
{
    QSettings aSettings;

    aSettings.setValue("CreateMoveDlg/OutputDir",ui.m_pDirectoryName->text());
    aSettings.setValue("CreateMoveDlg/OutputName",ui.m_pImageNameOffset->text());
    aSettings.setValue("CreateMoveDlg/OutputMovieName",ui.m_pMovieFileName->text());
    aSettings.setValue("CreateMoveDlg/MjpegToolsDir",ui.m_pMjpegtoolsDirectory->text());
    aSettings.setValue("CreateMoveDlg/ImagesPerSeconds",ui.m_pImagesPerSecond->value());
    aSettings.setValue("CreateMoveDlg/ImageSizeItem",ui.m_pImageRatio->currentItem());
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
    ui.m_pImageRatio->setCurrentItem(aSettings.value("CreateMoveDlg/ImageSizeItem",0).toInt());

    UpdateCmds();
}

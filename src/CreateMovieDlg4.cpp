
#include "CreateMovieDlg4.h"

#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>

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
    ui.m_pImageRatio->setCurrentItem( 0 );
    sltImageRatioSelected( g_sDefaultSize1 );

    ui.m_pImageNameOffset->setText( "image" );

    QString sTempImagePath = QDir::tempPath()+QDir::separator()+"mindia_movie";
    ui.m_pDirectoryName->setText( sTempImagePath );

    UpdateCmds();
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
   // if( m_pProcess==0 )
    {
        //m_pProcess = new QProcess(this);

        QString sProg = CMD_SHELL;
        QStringList aArgs;
        //aArgs << "/c" << "notepad.exe d:\\home\\no_error.txt";
        aArgs << CMD_SHELL_ARG << CMD_TEST;
        bool bOk = QProcess::execute(sProg,aArgs);
        QString sOut;
        sOut.sprintf("started %d",(int)bOk);
        ui.m_pOutput->append(sOut);

    }
}

void CreateMovieDlg4::UpdateCmds()
{
    QString sCmd;
    //sCmd.sprintf( "jpeg2yuv -I p -f %d -j movie\\image%05d.jpg | yuv2lav -o movie\\movie.avi", iImagesPerSecond );

    // for mac: use mjpegtools 2.0.0 (1.9.0 funktioniert nicht --> Segmentation fault) aber ggf. ist die Qualitaet nicht so gut !?

// TODO --> pfade anpassen an Plattform !!! Qt immer / ?
    ui.m_pGeneratorCmd->setText( "jpeg2yuv -I p -f 10 -j movie\\image%05d.jpg | yuv2lav -f avi -o movie\\movie.avi" );
    ui.m_pSoundGeneratorCmd->setText( "lavaddwav movie\\movie.avi movie.wav movie\\movie.avi" );
    ui.m_pVCDGeneratorCmd->setText( "lav2yuv movie\\movie.avi | yuvscaler -O VCD | mpeg2enc -f 1 -r 16 -o movie\\movie.mpg" );
}

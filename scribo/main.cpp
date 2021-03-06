/**
 * @mainpage Scribo
 *
 * A word processor with encrypt and decrypt functions
 *
 * You are able to create or load files
 *
 * Format the text whatever you want
 *
 * Administrate the server with the server software
 *
 * @author Rune Krauss
 *
 * @since 2015
 *
 * @copyright (c) by Rune Krauss
 *
 * @version 0.1
 */

/**
 * @file main.cpp
 * The main window is loaded and main settings are set
 */

#ifdef OSX
#ifdef WIN
#error "Error: It is only allowed to compile the code for one operating system."
#endif
#endif

#include "mainwindow.h"
#include "database.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

// If defined, the messages are logged in a file
#define LOGFILE

#ifdef LOGFILE

/**
 * Text stream for logging
 */
QTextStream *out = 0;

/**
 * Register a message handler for a better logging in a logfile
 *
 * @param type Message type like WARNING or CRITICAL
 * @param context Context for logging
 * @param msg Message respectively a log text
 */
void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString debugdate = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
        switch (type)
        {
        case QtDebugMsg:
            debugdate += " [D]";
            break;
        case QtWarningMsg:
            debugdate += " [W]";
            break;
        case QtCriticalMsg:
            debugdate += " [C]";
            break;
        case QtFatalMsg:
            debugdate += " [F]";
        }
        (*out) << debugdate << " " << msg << endl;

        if (QtFatalMsg == type)
        {
            abort();
        }
}

#endif

/**
 * Start point of the program which executes the main window
 * @param argc Slots of command line parameter
 * @param argv Values of command line parameter
 * @return The executing main window
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #ifdef LOGFILE
    QString fileName = QCoreApplication::applicationFilePath() + ".log";
    QFile *log = new QFile(fileName);
    if (log->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        out = new QTextStream(log);
        qInstallMessageHandler(messageOutput);
    }
    else
    {
        qDebug() << "Error opening log file '" << fileName << "'. All debug output redirected to console.";
    }
    #endif

    // Check language
    QTranslator translator;
    QString current = QLocale::system().name();
    if ( current.contains("de_DE") ) {
        translator.load("scribo_de", ":/translations");
        a.installTranslator(&translator);
    }

    // Load splash screen
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap( QPixmap(":/img/splash.jpg") );
    splash->show();

    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->setStyleSheet("font-size: 10px;");

    splash->showMessage(QObject::tr("Loading modules..."),
                               topRight, Qt::white);

    QCoreApplication::setApplicationName("Scribo");
    QCoreApplication::setApplicationVersion("0.1");

    // Create directories
    QString scriboDir = QDir::home().absolutePath() + QDir::separator() + "scribo";

    if ( !QDir(scriboDir).exists() )
        QDir().mkdir(scriboDir);

    splash->showMessage(QObject::tr("Establishing connections..."),
                               topRight, Qt::white);

    // Create database connection
    Database db;

    if ( !db.open() ) {
        qDebug() << "Failed to open the database.\n"
                    "Please check the database settings";
    } else {
        qDebug() << "Connected to the database";
    }

    splash->showMessage(QObject::tr("Loading..."),
                               topRight, Qt::white);
    MainWindow w;

    QTimer::singleShot(3000, splash, SLOT(close()));
    QTimer::singleShot(3000, &w, SLOT(show()));
    //w.show();

    qDebug() << "Application started...";

    return a.exec();
}

#include "mainwindow.h"
#include "database.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap( QPixmap(":/img/splash.jpg") );
    splash->show();

    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->setStyleSheet("font-size: 10px;");

    splash->showMessage(QObject::tr("Loading modules..."),
                               topRight, Qt::white);

    QString scriboDir = QDir::home().absolutePath() + QDir::separator() + "scribo";

    if ( !QDir(scriboDir).exists() )
        QDir().mkdir(scriboDir);

    splash->showMessage(QObject::tr("Establishing connections..."),
                               topRight, Qt::white);
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

    return a.exec();
}

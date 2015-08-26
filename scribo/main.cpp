#include "mainwindow.h"
#include "database.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QString scriboDir = QDir::home().absolutePath() + QDir::separator() + "scribo";

    if ( !QDir(scriboDir).exists() )
        QDir().mkdir(scriboDir);

    Database db;

    if ( !db.open() ) {
        qDebug() << "Failed to open the database.\n"
                    "Please check the database settings";
    } else {
        qDebug() << "Connected to the database";
    }

    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}

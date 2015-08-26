/** @file database.cpp
 * Configures the database settings.
 * Moreover, initializes a database connection.
 */

#include "database.h"
#include <QDebug>
#include <QFile>

/**
 * Driver of the database
 */
QString Database::driver = "QSQLITE";
/**
 * Name of the database
 */
QString Database::name = "scribo.db";

/**
 * Initializes the database object.
 * At first, calls the method setDatabaseSettings().
 * Second, configures different fields like the password.
 */
Database::Database()
{
    db = QSqlDatabase::addDatabase(driver);
    db.setDatabaseName(QDir::homePath() + QDir::separator() + "scribo" + QDir::separator() + name);
}

/**
 * Destroys the database object.
 * Closes and removes the database.
 */
Database::~Database(){
    if (QSqlDatabase::database().isOpen()) {
        QSqlDatabase::database().close();
    }
    QString cn = QSqlDatabase::database().connectionName();
    QSqlDatabase::removeDatabase(cn);
}

/**
 * Checks whether the database is open.
 * @return Status of the database
 */
bool Database::open() {
    return QSqlDatabase::database().open();
}

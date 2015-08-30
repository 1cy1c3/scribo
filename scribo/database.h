/**
 * @file database.h
 * @brief Represents the database and its operations.
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QDir>
#include <QDebug>
#include <QFile>

/**
 * Realises the database connection.
 *
 * @author Rune Krauss
 */
class Database
{
private:
    /**
     * @brief Database object for the access to the database
     */
    QSqlDatabase db;
    /**
     * @brief Name of the database
     */
    static QString name;
    /**
     * @brief Driver of the database
     */
    static QString driver;
public:
    /**
     * @brief Initializes the database object.
     */
    Database();
    /**
     * @brief Destroys the database object.
     */
    ~Database();
    /**
     * @brief Checks whether the database is open.
     * @return Status of the database connection
     */
    bool open();
};

#endif // DATABASE_H

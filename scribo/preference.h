/**
 * @file preference.h
 * @brief Represents the preferences in this program
 */

#ifndef PREFERENCE_H
#define PREFERENCE_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "mainwindow.h"

namespace Ui {
class Preference;
}

/**
 * Realises the preferencees dialog
 *
 * @author Rune Krauss
 */
class Preference : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Initializes the preferences object
     * @param parent Pointer to the super class of objects
     */
    explicit Preference(QWidget *parent = 0);
    /**
     * @brief Destroys the preferences object
     */
    ~Preference();

signals:
    /**
     * @brief Signal which is emitted if the preferences was changed
     */
    void preferencesChanged(); // const QString &name

private slots:
    /**
     * @brief Starts saving process regarding preferences
     */
    void on_pushButton_preference_save_clicked();
    /**
     * @brief Closes preferences dialog
     */
    void on_pushButton_preference_cancel_clicked();

private:
    /**
     * @brief Dynamic object for access to the graphical elements
     */
    Ui::Preference *ui;
    /**
     * @brief Database object for database operations
     */
    QSqlDatabase db;
    /**
     * @brief Query object for queries regarding DDL and DML
     */
    QSqlQuery query;
    /**
     * @brief Saves preferences
     */
    void savePreferences();
    /**
     * @brief Loads preferences
     */
    void loadPreferences();
    /**
     * @brief Checks preferences with regex
     * @return
     */
    bool checkPreferences();
};

#endif // PREFERENCE_H

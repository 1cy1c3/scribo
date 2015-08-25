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

class Preference : public QDialog
{
    Q_OBJECT

public:
    explicit Preference(QWidget *parent = 0);
    ~Preference();

signals:
    void preferencesChanged(); // const QString &name

private slots:
    void on_pushButton_preference_save_clicked();
    void on_pushButton_preference_cancel_clicked();

private:
    Ui::Preference *ui;
    QSqlDatabase db;
    QSqlQuery query;
    void savePreferences();
    void loadPreferences();
    bool checkPreferences();
};

#endif // PREFERENCE_H

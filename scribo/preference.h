#ifndef PREFERENCE_H
#define PREFERENCE_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
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
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Preference *ui;
    void savePreferences();
    void loadPreferences();
    bool checkPreferences();
    void accept();
    void reject();
};

#endif // PREFERENCE_H

#include "preference.h"
#include "ui_preference.h"

Preference::Preference(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preference)
{
    ui->setupUi(this);
    loadPreferences();
}

Preference::~Preference()
{
    delete ui;
}

void Preference::savePreferences() {
    QSettings setting("rk", "scribo");
    setting.beginGroup("writing");
    setting.setValue( "marginLeft", ui->spinBox_preference_marginLeft->value() );
    setting.setValue( "marginRight", ui->spinBox_preference_marginRight->value() );
    setting.endGroup();
}

void Preference::loadPreferences() {
    QSettings setting("rk", "scribo");
    setting.beginGroup("writing");
    ui->spinBox_preference_marginLeft->setValue( setting.value("marginLeft", 0).toInt() );
    ui->spinBox_preference_marginRight->setValue( setting.value("marginRight", 0).toInt() );
    setting.endGroup();
}

void Preference::on_buttonBox_accepted()
{
    if ( checkPreferences() ) {
        accept();
    } else {
        QMessageBox::critical(this, tr("Error"), tr("An error has ocurred. Please revise your entries."));
    }
}

bool Preference::checkPreferences()
{
    savePreferences();
    emit preferencesChanged();
    return true;
}

void Preference::accept() {
    QDialog::accept();
}

void Preference::reject() {
    QDialog::reject();
}

void Preference::on_buttonBox_rejected()
{
    reject();
}

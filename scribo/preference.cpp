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
    setting.setValue( "marginTop", ui->spinBox_preference_marginTop->value() );
    setting.setValue( "marginRight", ui->spinBox_preference_marginRight->value() );
    setting.setValue( "marginBottom", ui->spinBox_preference_marginBottom->value() );
    setting.setValue( "marginLeft", ui->spinBox_preference_marginLeft->value() );
    setting.setValue( "paddingTop", ui->spinBox_preference_paddingTop->value() );
    setting.setValue( "paddingRight", ui->spinBox_preference_paddingRight->value() );
    setting.setValue( "paddingBottom", ui->spinBox_preference_paddingBottom->value() );
    setting.setValue( "paddingLeft", ui->spinBox_preference_paddingLeft->value() );
    setting.endGroup();
}

void Preference::loadPreferences() {
    QSettings setting("rk", "scribo");
    setting.beginGroup("writing");
    ui->spinBox_preference_marginTop->setValue( setting.value("marginTop", 0).toInt() );
    ui->spinBox_preference_marginRight->setValue( setting.value("marginRight", 0).toInt() );
    ui->spinBox_preference_marginBottom->setValue( setting.value("marginBottom", 0).toInt() );
    ui->spinBox_preference_marginLeft->setValue( setting.value("marginLeft", 0).toInt() );
    ui->spinBox_preference_paddingTop->setValue( setting.value("paddingTop", 0).toInt() );
    ui->spinBox_preference_paddingRight->setValue( setting.value("paddingRight", 0).toInt() );
    ui->spinBox_preference_paddingBottom->setValue( setting.value("paddingBottom", 0).toInt() );
    ui->spinBox_preference_paddingLeft->setValue( setting.value("paddingLeft", 0).toInt() );
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

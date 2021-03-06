/** @file preference.cpp
 * Represents the preferences in this program
 * Chooses a password and sets layout parameters
 */

#include "preference.h"
#include "ui_preference.h"
#include "regex.h"

/**
 * Initializes the preferences object
 * @param parent Pointer to the super class of objects
 * Sets a fixed size
 * Loads actual preferences
 */
Preference::Preference(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preference)
{
    ui->setupUi(this);
    setFixedSize( size() );
    loadPreferences();
}

/**
 * Destroys the preferences object
 */
Preference::~Preference()
{
    delete ui;
}

/**
 * Saves preferences like layout parameters and the password
 */
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
    setting.setValue( "paddingLeft", ui->spinBox_preference_paddingLeft->value() );
    if ( ui->checkBox_preference_password->isChecked() )
        setting.setValue("passwordUsed", true );
    else
        setting.setValue("passwordUsed", false );
    setting.endGroup();
}

/**
 * Laods preferences like layout parameters and the password
 */
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
    ui->checkBox_preference_password->setChecked( setting.value("passwordUsed", false).toBool() );
    setting.endGroup();

    query.prepare("SELECT `Password` FROM `password`;");
    query.exec();
    QString passwordBase;
    while ( query.next() )
    {
        passwordBase = query.value(0).toString();
    }
    if ( passwordBase.isEmpty() )
    {
        ui->lineEdit_preference_password->setText("");
    }
    else
    {
        QByteArray password = QByteArray::fromBase64( passwordBase.toUtf8() );
        ui->lineEdit_preference_password->setText( QString(password) );
    }
}

/**
 * Checks whether preferences are valid.
 * @return Status of preferences
 */
bool Preference::checkPreferences()
{
    Regex regex;
    QRegExp rePassword( regex.getPassword() );

    QString password = ui->lineEdit_preference_password->text();

    if ( password.contains(rePassword) || password.isEmpty() )
    {
        if ( password.isEmpty() && ui->checkBox_preference_password->isChecked() )
        {
            return false;
        }
        else if ( !password.isEmpty() )
        {
            QByteArray passwordBase = password.toUtf8();

            query.prepare("SELECT COUNT(*) FROM `password`;");
            query.exec();
            while ( query.next() )
            {
                QString count = query.value(0).toString();
                if (count != "0")
                {
                    query.prepare("DELETE FROM `password`;");
                    query.exec();
                }
            }

            db.transaction();
            query.prepare("INSERT INTO `password` (`Password`) "
                          "VALUES (?);");
            query.bindValue( 0, passwordBase.toBase64() );
            if ( query.exec() )
            {
                db.commit();
                qDebug() << "The password has been saved successfully";
            }
            else
            {
                db.rollback();
                qDebug() << "The password could not be saved";
                qDebug() << query.lastError();
                return false;
            }
        }
        else if ( password.isEmpty() )
        {
            query.prepare("SELECT COUNT(*) FROM `password`;");
            query.exec();
            while ( query.next() )
            {
                QString count = query.value(0).toString();
                if (count != "0")
            {
                query.prepare("DELETE FROM `password`;");
                query.exec();
            }
        }
    }
    } else
    {
        return false;
    }
    return true;
}

/**
 * Starts validation regarding preferences
 * After validation, saves preferences or opens a message box
 */
void Preference::on_pushButton_preference_save_clicked()
{
    if ( checkPreferences() ) {
        savePreferences();
        qDebug() << "The settings have been saved successfully.";
        emit preferencesChanged();
        close();
    } else {
        QMessageBox::critical(this, tr("Error"), tr("An error has ocurred. Please revise your entries and note the following:\n"
                                                    "A Password must contain at least one upper case letter.\n"
                                                    "A Password must contain at least one lower case letter.\n"
                                                    "A Password must contain at least one numeric digit."));
        qDebug() << "Settings could not be saved because the password requirements are incorrect.";
    }
}

/**
 * Closes this dialog box
 */
void Preference::on_pushButton_preference_cancel_clicked()
{
    close();
}

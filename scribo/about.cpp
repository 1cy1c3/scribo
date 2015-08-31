/** @file about.cpp
 * Represents the infos about this program
 * Furthermore, shows the license
 */

#include "about.h"
#include "ui_about.h"

/**
 * Version of this program
 */
QString About::version = "Version ";
/**
 * Author of this program
 */
QString About::author = "Rune Krauss";

/**
 * Constructor of the about object
 * Sets name and version of this program
 * Moreover, shows the license and creates a link to the author website
 * @param parent Pointer to the super class of objects
 */
About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    setFixedSize( size() );

    ui->label_about_program->setText( QCoreApplication::applicationName() );
    ui->label_about_version->setText( version + QCoreApplication::applicationVersion() );
    ui->label_about_author->setOpenExternalLinks(true);
    ui->label_about_author->setTextFormat(Qt::RichText);
    ui->label_about_author->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_about_author->setText("&copy; <a href=\"http://www.runekrauss.com\">" + author + "</a>");
}

/**
 * Destructor of the about object
 */
About::~About()
{
    delete ui;
}

/**
 * Button which closes this dialog after the click event
 */
void About::on_pushButton_about_ok_clicked()
{
    close();
}

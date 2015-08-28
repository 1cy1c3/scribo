#include "about.h"
#include "ui_about.h"

QString About::version = "Version ";
QString About::author = "Rune Krauss";

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    ui->label_about_program->setText( QCoreApplication::applicationName() );
    ui->label_about_version->setText( version + QCoreApplication::applicationVersion() );
    ui->label_about_author->setOpenExternalLinks(true);
    ui->label_about_author->setTextFormat(Qt::RichText);
    ui->label_about_author->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_about_author->setText("&copy; <a href=\"http://www.runekrauss.com\">" + author + "</a>");
}

About::~About()
{
    delete ui;
}

void About::on_pushButton_about_ok_clicked()
{
    close();
}

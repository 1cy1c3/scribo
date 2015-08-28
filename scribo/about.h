#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

private slots:
    void on_pushButton_about_ok_clicked();

private:
    Ui::About *ui;
    static QString version;
    static QString author;
};

#endif // ABOUT_H

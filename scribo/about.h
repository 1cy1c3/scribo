/**
 * @file about.h
 * @brief Represents the infos about this program
 */

#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
class About;
}

/**
 * Realises the info dialog
 *
 * @author Rune Krauss
 */
class About : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Initializes the about object
     * @param parent Pointer to the super class of objects
     */
    explicit About(QWidget *parent = 0);
    /**
      * @brief Destroys the about object
     */
    ~About();

private slots:
    /**
     * @brief Button for closing this dialog
     */
    void on_pushButton_about_ok_clicked();

private:
    /**
     * @brief Dynamic object for access to the graphical elements
     */
    Ui::About *ui;
    /**
     * @brief Version of this program
     */
    static QString version;
    /**
     * @brief Author of this program
     */
    static QString author;
};

#endif // ABOUT_H

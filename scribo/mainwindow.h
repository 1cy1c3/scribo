/**
 * @file mainwindow.h
 * @brief Presents the main window of this program
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileDialog>
#include <QColor>
#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPrinter>
#include <QPrintDialog>
#include <QAction>
#include <QMimeData>

class Preference;

namespace Ui {
class MainWindow;
}

/**
 * @brief Presents the main window of this program
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Initializes the about object
     * @param parent Pointer to the super class of objects
     */
    explicit MainWindow(QWidget *parent = 0);
    /**
     * @brief Destroys the about object
     */
    ~MainWindow();

signals:
    /**
     * @brief Signal which changes the background.
     */
    void backgroundChanged();

protected:
    /**
     * @brief Event which is sent to a widget when a drag and drop action enters it
     * @param event Event
     */
    void dragEnterEvent(QDragEnterEvent *event);
    /**
     * @brief Event that is sent to a widget when a drag and drop action leaves it
     * @param event Event
     */
    void dragLeaveEvent(QDragLeaveEvent *event);
    /**
     * @brief Event which is sent while a drag and drop action is in progress
     * @param event Event
     */
    void dragMoveEvent(QDragMoveEvent *event);
    /**
     * @brief Event which is sent when a drag and drop action is completed
     * @param event Event
     */
    void dropEvent(QDropEvent *event);

private slots:
    /**
     * @brief Creates a new document
     */
    void on_actionNew_triggered();

    /**
     * @brief Opens the help dialog
     */
    void on_actionScribo_Help_triggered();

    /**
     * @brief Opens a document
     */
    void on_actionOpen_triggered();

    /**
     * @brief Saves a document
     */
    void on_actionSave_triggered();

    /**
     * @brief Saves a document in a specific location
     */
    void on_actionSave_As_triggered();

    /**
     * @brief Copies text
     */
    void on_actionCopy_triggered();

    /**
     * @brief Cuts out text
     */
    void on_actionCut_triggered();

    /**
     * @brief Undoes an action
     */
    void on_actionUndo_triggered();

    /**
     * @brief Redoes an action
     */
    void on_actionRedo_triggered();

    /**
     * @brief Pastes text at one point
     */
    void on_actionPaste_triggered();

    /**
     * @brief Checks for updates
     */
    void on_actionCheck_for_updates_triggered();

    /**
     * @brief Opens the about dialog
     */
    void on_actionAbout_triggered();

    /**
     * @brief Opens the preferences dialog
     */
    void on_actionSettings_triggered();

    /**
     * @brief Quits this program
     */
    void on_actionClose_triggered();

    /**
     * @brief Standardizes the text
     */
    void on_actionNormal_triggered();

    /**
     * @brief Makes the text bold
     */
    void on_actionBold_triggered();

    /**
     * @brief Format the text italic
     */
    void on_actionItalic_triggered();

    /**
     * @brief Makes the text underlined
     */
    void on_actionUnderline_triggered();

    /**
     * @brief Aligns the text left
     */
    void on_actionAlign_text_left_triggered();

    /**
     * @brief Centers the text
     */
    void on_actionCenter_text_triggered();

    /**
     * @brief Aligns the text right
     */
    void on_actionAlign_text_right_triggered();

    /**
     * @brief Creates a justification regarding a selected text
     */
    void on_actionJustification_triggered();

    /**
     * @brief Opens the font dialog
     */
    void on_actionFont_triggered();

    /**
     * @brief Highlights the text
     */
    void on_actionHighlighting_triggered();

    /**
     * @brief Opens the background dialog
     */
    void on_actionBackground_triggered();

    /**
     * @brief Opens a submenu
     */
    void on_actionFont_2_triggered();

    /**
     * @brief Updates preferences
     */
    void updatePreferences(); // const QString &name

    /**
     * @brief Encrypts data usng key and plaintext
     */
    void on_actionEncrypt_triggered();

    /**
     * @brief Decrypts data using key and cipher text
     */
    void on_actionDecrypt_triggered();

    /**
     * @brief Inserts an image
     */
    void on_actionImage_triggered();

    /**
     * @brief Opens the printer dialog
     */
    void on_actionPrint_triggered();

private:
    /**
     * @brief Dynamic object for access to the graphical elements
     */
    Ui::MainWindow *ui;
    /**
     * @brief Path of the specific file
     */
    QString fileName;
    /**
     * @brief Cursor position regarding a selected text
     */
    QTextCursor cursor;
    /**
     * @brief Font color
     */
    QColor color;
    /**
     * @brief Font object
     */
    QFont font;
    /**
     * @brief Objekt for queries
     */
    QSqlQuery query;
    /**
     * @brief Password of the user
     */
    QString password;
    /**
     * @brief Encrypted text
     */
    QByteArray encrypted;
    /**
     * @brief Decrypted text
     */
    QByteArray decrypted;
    /**
     * @brief Status of the password
     */
    bool passwordUsed;
    /**
     * @brief Preference object
     */
    Preference *preference;
    /**
     * @brief Database object for the access to the database
     */
    QSqlDatabase db;
    /**
     * @brief Name of the database
     */
    QString name;
    /**
     * @brief Driver of the database
     */
    QString driver;
    /**
     * @brief Merges format on a word or selection
     * @param format Format
     */
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    /**
     * @brief Encrypts a text using password
     * @param password Password
     * @return Encrypted text
     */
    QByteArray encrypt(QString password);
    /**
     * @brief Decrypts a text using password
     * @param password Password
     * @return Decrypted text
     */
    QByteArray decrypt(QString password);
    /**
     * @brief Adds a context menu
     */
    void addContextMenu();
    /**
     * @brief Gets file content
     * @param file File path
     * @return Content of a file
     */
    QString getFileContent(QString file);
};

#endif // MAINWINDOW_H

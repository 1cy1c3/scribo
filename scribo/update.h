/**
 * @file update.h
 * @brief Represents the update checker in this program
 */

#ifndef UPDATE_H
#define UPDATE_H

#include <QDialog>
#include <QEvent>
#include <QTcpSocket>
#include <QMessageBox>

namespace Ui {
class Update;
}

/**
 * Realises the update checker with sockets
 *
 * @author Rune Krauss
 */
class Update : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Initializes the update object
     * @param parent Pointer to the super class of objects
     */
    explicit Update(QWidget *parent = 0);
    /**
     * @brief Destroys the update object
     */
    ~Update();

signals:

public slots:
    /**
     * @brief Event listener for connected events.
     */
    void connected();
    /**
     * @brief Event listener for disconnected events.
     */
    void disconnected();
    /**
     * @brief Event listener for reading messages from the client.
     */
    void readyRead();

private slots:
    /**
     * @brief Display errors regarding socket operations.
     * @param socketError Error of socket operations.
     */
    void displayError (QAbstractSocket::SocketError socketError);

    /**
     * @brief Closes the dialog
     */
    void on_pushButton_clicked();

private:
    /**
     * @brief Dynamic object for access to the graphical elements
     */
    Ui::Update *ui;
    /**
     * @brief Bidirectional interface
     */
    QTcpSocket *clientSocket;
    /**
     * @brief Port of the server
     */
    static int port;
    /**
     * @brief Adress of the host
     */
    static QString hostAddress;
};

#endif // UPDATE_H

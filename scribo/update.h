#ifndef UPDATE_H
#define UPDATE_H

#include <QDialog>
#include <QEvent>
#include <QTcpSocket>
#include <QMessageBox>

namespace Ui {
class Update;
}

class Update : public QDialog
{
    Q_OBJECT

public:
    explicit Update(QWidget *parent = 0);
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

    void on_pushButton_clicked();

private:
    Ui::Update *ui;
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

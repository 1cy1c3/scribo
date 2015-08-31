/**
 * @file client.h
 * @brief Describes the client of the server
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThreadPool>
#include <QtXml>

/**
 * Describes the client of the server
 *
 * @author Rune Krauss
 */
class Client : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Initializes the client object
     * @param parent Pointer to the super class of objects
     */
    explicit Client(QObject *parent = 0);
    /**
     * @brief Set the bidirectional interface for interprocess communication
     * @param Descriptor of the socket
     */
    void setSocket(qintptr descriptor);
    /**
     * @brief Configures the program infos
     */
    void setProgramInfos();

signals:

public slots:
    /**
     * @brief Event listener for connected events
     */
    void connected();
    /**
     * @brief Event listener for disconnected events
     */
    void disconnected();
    /**
     * @brief Event listener for reading messages from the client
     */
    void readyRead();

    /**
     * @brief Event listener for sending messages to the client
     * @param number Number of the task regarding the thread
     */
    void taskResult(int number);

private:
    /**
     * @brief Bidirectional interface for interprocess communication
     */
    QTcpSocket *socket;

    /**
     * @brief Version of the program regarding client
     */
    int clientVersion;
    /**
     * @brief Name of the program
     */
    static QString name;
    /**
     * @brief Version of the program
     */
    static int version;
    /**
     * @brief Configuration path regarding the ressource of the info
     */
    static char confPath[];

};

#endif // CLIENT_H

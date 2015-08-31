/**
 * @file scriboserver.h
 * @brief Represents the server operations
 */

#ifndef SCRIBOSERVER_H
#define SCRIBOSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>

/**
 * Represents the server operations
 *
 * @author Rune Krauss
 */
class ScriboServer : public QTcpServer
{
    Q_OBJECT
public:
    /**
     * @brief Initializes the server object
     * @param parent Pointer to the super class of objects
     */
    explicit ScriboServer(QObject *parent = 0);
    /**
     * @brief Starts the server
     */
    void startServer();

protected:
    /**
     * @brief Method for incoming connections in terms of clients
     * @param socketDescriptor Descriptor of the socket
     */
    void incomingConnection(qintptr socketDescriptor);

signals:

public slots:

private:
    /**
     * @brief Port of the server
     */
    static int port;
};

#endif // SCRIBOSERVER_H

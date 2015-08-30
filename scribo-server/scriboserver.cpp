/** @file scriboserver.cpp
 * Represents the server operations.
 * The server deals with incomming connections from the clients.
 */

#include "scriboserver.h"
#include "client.h"

/**
 * Port of the server
 */
int ScriboServer::port = 11001;

/**
 * @brief Constructor of the server, initializes it.
 * @param parent Pointer to the super class of objects
 */
ScriboServer::ScriboServer(QObject *parent) :
    QTcpServer(parent)
{
}

/**
 * Starts the server based on host address and port.
 */
void ScriboServer::startServer()
{
    if(listen(QHostAddress::Any, port))
    {
        qDebug() << "Server started...";
    }
    else
    {
        qDebug() << "Server not started...";
    }
}

/**
 * @brief Deals with incoming connections and sets the socket regarding a client.
 * @param socketDescriptor Descriptor of the socket
 */
void ScriboServer::incomingConnection(qintptr socketDescriptor)
{
    Client *client = new Client(this);
    client->setSocket(socketDescriptor);
}

/** @file client.cpp
 * Implements the client of the server.
 * The client sends and receives messages between server and users.
 */

#include "client.h"
#include "task.h"

/**
 * Name of the program
 */
QString Client::name = "";
/**
 * Version of the program
 */
int Client::version = 0;
/**
 * Config path of the info
 */
char Client::confPath[] = ":/conf/config.xml";

/**
 * Constructor of the clients, initializes them.
 * @param parent Pointer to the super class of objects
 */
Client::Client(QObject *parent) :
    QObject(parent)
{
    setProgramInfos();
    QThreadPool::globalInstance()->setMaxThreadCount(5);
}

/**
 * Makes a new socket and registers the signals and slots.
 * Moreover, sets a socket descriptor based on the descriptor parameter.
 * @param descriptor Descriptor of the socket
 */
void Client::setSocket(qintptr descriptor)
{
    // Make a new socket
    socket = new QTcpSocket(this);

    qDebug() << "A new socket created...";

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket->setSocketDescriptor(descriptor);

    qDebug() << " Client connected at " << descriptor << "...";
}

/**
 * Event listener for connected signals.
 * Runs seperately from the thread which is created.
 */
void Client::connected()
{
    qDebug() << "Client connected...";
}

/**
 * Event listener for disconnected signals.
 * It's an asynchronous process.
 */
void Client::disconnected()
{
    qDebug() << "Client disconnected...";
}

/**
 * This is the main thread of execution and happening via main thread.
 * The code is running in the current thread, not in another threads.
 * For this reason, getting the thread from the pool.
 */
void Client::readyRead()
{
    qDebug() << "Ready to read...";

    clientVersion = socket->readAll().toInt();

    // Time consumer
    Task *task = new Task();
    task->setAutoDelete(true);

    // Using queued connection
    connect(task, SIGNAL(result(int)), this, SLOT(taskResult(int)), Qt::QueuedConnection);

    qDebug() << "Starting a new task using a thread from the pool...";

    // Returns global pool instance
    QThreadPool::globalInstance()->start(task);

}

/**
 * After a task performed a time consuming task, it's grabs the result here and sends it to the client.
 * Furthermore, makes the server fully asynchronous by doing time consuming task.
 * @param number Number of the task
 */
void Client::taskResult(int number)
{
    QByteArray buffer;
    bool update = false;
    if (version > clientVersion)
        update = true;
    //buffer.append("\r\nTask result = ");
    buffer.append(QString::number(update));
    socket->write(buffer);
}

/**
 * Method for document object model manipulation.
 * Opens the info.xml and reads out the specific values.
 */
void Client::setProgramInfos() {
    QDomDocument document;

    QFile file(confPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qDebug() << "Failed to open file for info settings.";
    } else {
        if ( !document.setContent(&file) ) {
            qDebug() << "Failed to load document for info settings.";
        }
        file.close();
    }

    // Get root element
    QDomElement rootNode = document.documentElement();

    // Get connection element
    QDomElement configNode = rootNode.firstChild().toElement();

    while ( !configNode.isNull() ) {
        if ( configNode.tagName() == "info" ) {

            QDomElement configChild = configNode.firstChild().toElement();

            // Read each child of the connection node
            while ( !configChild.isNull() ) {
                if (configChild.tagName() == "name")
                    name = configChild.firstChild().toText().data();
                if (configChild.tagName() == "version")
                    version = configChild.firstChild().toText().data().toInt();
                // Next child
                configChild = configChild.nextSibling().toElement();
            }
        }
        // Next connection
        configNode = configNode.nextSibling().toElement();
    }
}

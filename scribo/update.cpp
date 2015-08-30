#include "update.h"
#include "ui_update.h"

/**
 * Adress of the host
 */
QString Update::hostAddress = "127.0.0.1";

/**
 * Port of the server
 */
int Update::port = 11001;

Update::Update(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Update)
{
    ui->setupUi(this);

    clientSocket = new QTcpSocket(this);

    clientSocket->connectToHost(hostAddress, port);

    connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(clientSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    clientSocket->write( QCoreApplication::applicationVersion().toUtf8() );
}

Update::~Update()
{
    delete ui;
}

/**
 * Error handling for the bidirectional communication.
 * @param socketError Error of socket operations.
 */
void Update::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("Client"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            ui->label_update_status->setText(tr("An error has occurred. Please try it again later."));
            qDebug() << "The host was not found.";
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running "
                                    "and check that the host name and port "
                                    "settings are correct."));
            ui->label_update_status->setText(tr("An error has occurred. Please try it again later."));
            qDebug() << "The connection was refused by the peer.";
            break;
        default:
            QMessageBox::information(this, tr("Client"),
                                 tr("The following error ocurred: %1.")
                                     .arg(clientSocket->errorString()));
            ui->label_update_status->setText(tr("An error has occurred. Please try it again later."));
            qDebug() << "The following error ocurred: " << clientSocket->errorString();
    }
}

/**
 * Event listener for connected signals.
 * Runs seperately from the thread which is created.
 */
void Update::connected()
{
    qDebug() << "Client connected...";
}

/**
 * Event listener for disconnected signals.
 * It's an asynchronous process.
 */
void Update::disconnected()
{
    qDebug() << "Client disconnected...";
}

/**
 * This is the main thread of execution and happening via main thread.
 * The code is running in the current thread, not in another threads.
 * For this reason, getting the thread from the pool.
 */
void Update::readyRead()
{
    qDebug() << "Ready to read...";
    QString update = QString ( clientSocket->readAll() );
    if ( update.contains("1") )
        ui->label_update_status->setText("There is an update available!");
    else {
        ui->label_update_status->setText("There is no update available!");
    }
}

void Update::on_pushButton_clicked()
{
    close();
}

#include "client_manager.hpp"

#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QHostInfo>
#include <QFile>

Client::Client(QObject *parent)
    : QObject(parent)
    , client_socket_(new QTcpSocket(this))
{
    connect(client_socket_, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(client_socket_, &QAbstractSocket::errorOccurred, this, &Client::error);
    connect(client_socket_, &QTcpSocket::disconnected, this, [this]()->void{logged_in_ = false;});
}

Client::~Client()
{
    disconnectFromHost();
}


void Client::sendMessage(QString player_name, quint32 movement)
{
    QDataStream clientStream(client_socket_);
    clientStream.setVersion(QDataStream::Qt_6_4);

    // todo: proslediti podatke o kursoru
    QByteArray data = player_name.toLocal8Bit();
    data.append(movement);
    clientStream << data;
}

void Client::disconnectFromHost()
{
    client_socket_->disconnectFromHost();
}

void Client::dataReceived(const QByteArray &data)
{
    QString test = QString(data);

    // todo: hendlovati
    if (test == "tick")
    {
        emit serverTickReceived();
    }
    else
    {
        qDebug() << "sa servera: " << test;
    }
}

void Client::connectToServer(const QString &ipAdress, quint16 port)
{
    client_socket_->connectToHost(ipAdress, port);
}

void Client::onReadyRead()
{
    QByteArray data;
    QDataStream socketStream(client_socket_);
    socketStream.setVersion(QDataStream::Qt_6_4);
    for (;;)
    {
        socketStream.startTransaction();
        socketStream >> data;
        if (socketStream.commitTransaction())
        {
            dataReceived(data);
        } else
        {
            break;
        }
    }
}

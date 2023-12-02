#include "client.hpp"

#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QHostInfo>
#include <QFile>

Client::Client(QObject *parent)
    : QObject(parent)
    , client_socket_(new QTcpSocket(this))
    //, logged_in_(false)
    , player_(new Player("todo", 0, 0))
{
    connect(client_socket_, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(client_socket_, &QAbstractSocket::errorOccurred, this, &Client::error);
    connect(client_socket_, &QTcpSocket::disconnected, this, [this]()->void{logged_in_ = false;});
    connect(player_, &Player::positionChangedSignal, this, &Client::updatePosition);
}

Client::~Client()
{
    disconnectFromHost();
}

Player *Client::getPlayer()
{
    return player_;
}

void Client::sendMessage()
{
    QDataStream clientStream(client_socket_);
    clientStream.setVersion(QDataStream::Qt_6_4);
    QVariant variant = player_->toVariant();

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << variant;

    clientStream << data;
}

void Client::disconnectFromHost()
{
    client_socket_->disconnectFromHost();
}

void Client::dataReceived(QByteArray &data)
{
    QVariant variant;
    QDataStream stream(data);
    stream >> variant;

    Player *enemy = new Player();
    enemy->fromVariant(variant);
    qDebug() << enemy->getName() << ": " << enemy->x() << " " <<  enemy->y();

    emit signalDataReceived(enemy);
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
    for (;;) {
        socketStream.startTransaction();
        socketStream >> data;
        if (socketStream.commitTransaction()) {
            dataReceived(data);
        } else {
            break;
        }
    }
}

void Client::updatePosition()
{
    sendMessage();
}


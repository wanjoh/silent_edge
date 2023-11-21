#include "client.hpp"

#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QHostInfo>
#include <QFile>

float x_received;
float y_received;

Client::Client(QObject *parent)
    : QObject(parent)
    , client_socket_(new QTcpSocket(this))
    , logged_in_(false)
    , serializer_(new BinarySerializer())
    , player_(new Player())
{
    x_received = 0;
    y_received = 0;

    connect(client_socket_, &QTcpSocket::connected, this, &Client::connected);
    connect(client_socket_, &QTcpSocket::disconnected, this, &Client::disconnected);
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

void Client::sendMessage(const QString &text)
{
    if (text.isEmpty())
        return;
    QDataStream clientStream(client_socket_);
    clientStream.setVersion(QDataStream::Qt_6_4);

//    serializer_->save(*player_, "../data/binary/1.bin");

//    QFile file("../data/binary/1.bin");
//    if (file.open(QIODevice::ReadOnly))
//    {
//        QByteArray data = file.readAll();
//        file.close();
//        clientStream << data;
//    }

    clientStream << text.QString::toUtf8();
}

void Client::disconnectFromHost()
{
    client_socket_->disconnectFromHost();
}

void Client::dataReceived(const QByteArray &data)
{
//    Player *enemy_ = new Player();
//    serializer_->load(*enemy_, "../data/binary/1.bin");

//    x_received = enemy_->x();
//    y_received = enemy_->y();
    qDebug() << QString::fromUtf8(data);
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
    QString msg = QString("x:") + QString::number(player_->x())
                  + QString("; y:") + QString::number(player_->y());
    sendMessage(msg);
}


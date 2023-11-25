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
    , serializer_(new BinarySerializer())
    , player_(new Player("todo", 0, 0))
{
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

    serializer_->save(*player_, FILEPATH_FOR_SENDING);

    QFile file(FILEPATH_FOR_SENDING);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();

        file.close();
        clientStream << data;
    }
}

void Client::disconnectFromHost()
{
    //serializer_->deleteData(filepath);
    client_socket_->disconnectFromHost();
}

void Client::dataReceived(const QByteArray &data)
{
    Player *enemy = new Player("enemy", 0, 0);
    QFile file(FILEPATH_FOR_RECEIVING);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(data);
        file.close();
    }
    serializer_->load(*enemy, FILEPATH_FOR_RECEIVING);
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
    QString msg = QString("x:") + QString::number(player_->x())
                  + QString("; y:") + QString::number(player_->y());
    sendMessage(msg);
}


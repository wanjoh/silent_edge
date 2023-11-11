#include "client.hpp"

#include <QTcpSocket>
#include <QDataStream>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QHostAddress>
#include <QHostInfo>

float x_received;
float y_received;

Client::Client(QObject *parent)
    : QObject(parent)
    , client_socket_(new QTcpSocket(this))
    , logged_in_(false)
{
    x_received = 0;
    y_received = 0;

    connect(client_socket_, &QTcpSocket::connected, this, &Client::connected);
    connect(client_socket_, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(client_socket_, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(client_socket_, &QAbstractSocket::errorOccurred, this, &Client::error);
    connect(client_socket_, &QTcpSocket::disconnected, this, [this]()->void{logged_in_ = false;});
}

void Client::login(const QString &userName)
{

    if (client_socket_->state() == QAbstractSocket::ConnectedState) {
        QDataStream clientStream(client_socket_);
        clientStream.setVersion(QDataStream::Qt_6_4);
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("login");
        message[QStringLiteral("username")] = userName;
        clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
    }
}

Client::~Client()
{
    disconnectFromHost();
}

void Client::sendMessage(const QString &text)
{

    if (text.isEmpty())
        return;
    QDataStream clientStream(client_socket_);
    clientStream.setVersion(QDataStream::Qt_6_4);
    QJsonObject message;

    if(text != "Space")
    {
        message = {
            {"x", player_x_},
            {"y", player_y_},
        };
    }
    else if(text == "Space")
    {
        message = {
            {text, 9}
        };
    }
    clientStream << QJsonDocument(message).toJson();
}

void Client::disconnectFromHost()
{
    client_socket_->disconnectFromHost();
}

void Client::jsonReceived(const QJsonObject &docObj)
{

    QJsonValue xs = *docObj.find("x");
    QJsonValue ys = *docObj.find("y");

    x_received = xs.toDouble();
    y_received = ys.toDouble();
}

void Client::connectToServer(const QString &ipAdress, quint16 port)
{
    client_socket_->connectToHost(ipAdress, port);
}
void Client::onReadyRead()
{
    QByteArray jsonData;
    QDataStream socketStream(client_socket_);
    socketStream.setVersion(QDataStream::Qt_6_4);
    for (;;) {
        socketStream.startTransaction();
        socketStream >> jsonData;
        if (socketStream.commitTransaction()) {

            QJsonParseError parseError;
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if (parseError.error == QJsonParseError::NoError) {
                if (jsonDoc.isObject()){
                    jsonReceived(jsonDoc.object());
                }
            }
        } else {

            break;
        }
    }
}


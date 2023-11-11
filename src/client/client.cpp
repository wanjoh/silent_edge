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
    , m_clientSocket(new QTcpSocket(this))
    , m_loggedIn(false)
{
    x_received = 0;
    y_received = 0;

    connect(m_clientSocket, &QTcpSocket::connected, this, &Client::connected);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    #if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
        connect(m_clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::error);
    #else
        connect(m_clientSocket, &QAbstractSocket::errorOccurred, this, &Client::error);
    #endif
    connect(m_clientSocket, &QTcpSocket::disconnected, this, [this]()->void{m_loggedIn = false;});
}

void Client::login(const QString &userName)
{

    if (m_clientSocket->state() == QAbstractSocket::ConnectedState) {
        QDataStream clientStream(m_clientSocket);
        clientStream.setVersion(QDataStream::Qt_5_7);
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
    QDataStream clientStream(m_clientSocket);
    clientStream.setVersion(QDataStream::Qt_5_7);
    QJsonObject message;

    if(text != "Space")
    {
        message = {
            {"x", m_playerX},
            {"y", m_playerX},
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
    m_clientSocket->disconnectFromHost();
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
    m_clientSocket->connectToHost(ipAdress, port);
}
void Client::onReadyRead()
{
    QByteArray jsonData;
    QDataStream socketStream(m_clientSocket);
    socketStream.setVersion(QDataStream::Qt_5_7);
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


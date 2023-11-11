#include "server.hpp"

#include <QDebug>
#include <QTcpSocket>
#include <QJsonDocument>

GameServer::GameServer(QObject *parent) : QTcpServer(parent)
{

    if (!this->listen(HOST, PORT))
    {
        qDebug() << "Could not start the server";
    }
    else
    {
        qDebug() << "Server started on " << HOST <<  ", port" << PORT;
    }


}

GameServer::~GameServer()
{
    for (auto user : users_)
    {
        user->disconnectClient();
    }
    close();
}

void GameServer::startGame()
{

}

void GameServer::error(QTcpSocket::SocketError error)
{
    emit logMessage(QLatin1String("error"));
}

void GameServer::dataReceived(ConnectionThread* sender, const QJsonObject& msg)
{
    Q_ASSERT(sender);

    emit logMessage(QLatin1String("object received"));

    // todo: check if data is valid

    broadcast(msg, sender);
}

void GameServer::userDisconnected(ConnectionThread* user)
{
    Q_ASSERT(user);
    users_.removeAll(user);
    user->disconnectClient();
    user->deleteLater();
}

void GameServer::broadcast(const QJsonObject& msg, ConnectionThread *sender)
{
    for (auto user : users_)
    {
        Q_ASSERT(user);
        if (user != sender)
        {
            user->sendData(msg);
        }
    }
}

void GameServer::sendData(ConnectionThread *user, const QJsonObject& msg)
{
    Q_ASSERT(user);
    user->sendData(msg);
}

void GameServer::incomingConnection(qintptr socket_desc)
{
    if (users_.size() < MAX_USERS)
    {
        ConnectionThread *user = new ConnectionThread(socket_desc, this);

        //connect(user, &ConnectionThread::finished, user, &ConnectionThread::deleteLater);
        connect(user, &ConnectionThread::disconnectedFromClient,
                this, std::bind(&GameServer::userDisconnected, this, user));
        connect(user, &ConnectionThread::error,
                this, std::bind(&GameServer::error, this, std::placeholders::_1));
        connect(user, &ConnectionThread::dataReceived,
                this, std::bind(&GameServer::dataReceived, this, user, std::placeholders::_1));
        connect(user, &ConnectionThread::logMessage, this, &GameServer::logMessage);

        users_.push_back(user);
        qDebug() << "thread created";
        //user->start();
    }
}

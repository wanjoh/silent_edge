#include "server.hpp"
#include "qjsonobject.h"
#include <QDebug>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QTimer>

GameServer::GameServer(QObject *parent)
    : QTcpServer(parent)
    , ideal_thread_count_(qMax(QThread::idealThreadCount(), 1))
{
    available_threads_.reserve(ideal_thread_count_);
    threads_load_.reserve(ideal_thread_count_);

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
    for (QThread *singleThread : available_threads_) {
        singleThread->quit();
        singleThread->wait();
    }
}

void GameServer::startGame()
{

}

void GameServer::stopServer()
{
    emit stopAllClients();
    close();
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

void GameServer::userDisconnected(ConnectionThread* user, int threadIdx)
{
    --threads_load_[threadIdx];
    users_.removeAll(user);
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
    QTimer::singleShot(0, user, std::bind(&ConnectionThread::sendData, user, msg));
}

void GameServer::incomingConnection(qintptr socket_desc)
{
    if (users_.size() < MAX_USERS)
    {
        ConnectionThread *user = new ConnectionThread(socket_desc, this);

        int threadIdx = available_threads_.size();
        if (threadIdx < ideal_thread_count_) {
            available_threads_.append(new QThread(this));
            threads_load_.append(1);
            available_threads_.last()->start();
        } else {
            threadIdx = std::distance(threads_load_.cbegin(), std::min_element(threads_load_.cbegin(), threads_load_.cend()));
            ++threads_load_[threadIdx];
        }
        user->moveToThread(available_threads_.at(threadIdx));

        connect(available_threads_.at(threadIdx), &QThread::finished, user, &QObject::deleteLater);
        connect(user, &ConnectionThread::disconnectedFromClient, this, std::bind(&GameServer::userDisconnected, this, user, threadIdx));
        connect(user, &ConnectionThread::error, this, std::bind(&GameServer::error, this, std::placeholders::_1));
        connect(user, &ConnectionThread::dataReceived, this, std::bind(&GameServer::dataReceived, this, user, std::placeholders::_1));
        connect(user, &ConnectionThread::logMessage, this, &GameServer::logMessage);
        connect(this, &GameServer::stopAllClients, user, &ConnectionThread::disconnectedFromClient);

        users_.push_back(user);
        qDebug() << "thread created";
        //user->start();
    }
}

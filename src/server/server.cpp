#include "server.hpp"
#include <QDebug>
#include <QTcpSocket>
#include <QByteArray>
#include <QTimer>

GameServer::GameServer(QObject *parent)
    : QTcpServer(parent)
{
    available_threads_.reserve(MAX_USERS);
    threads_load_.reserve(MAX_USERS);

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

void GameServer::dataReceived(ConnectionThread* sender, const QByteArray& msg)
{
    Q_ASSERT(sender);

    emit logMessage(QLatin1String("object received"));

    // todo: check if data is valid

    broadcast(msg, sender);
}

void GameServer::userDisconnected(ConnectionThread* user, int thread_idx)
{
    Q_ASSERT(user);
    --threads_load_[thread_idx];
    users_.removeAll(user);

    user->disconnectClient();
    user->deleteLater();
}

void GameServer::broadcast(const QByteArray& msg, ConnectionThread *sender)
{
    for (auto user : users_)
    {
        Q_ASSERT(user);
        if (1 || user != sender)
        {
            user->sendData(msg);
        }
    }
}

void GameServer::sendData(ConnectionThread *user, const QByteArray& msg)
{
    Q_ASSERT(user);

    //QTimer::singleShot(0, user, std::bind(&ConnectionThread::sendData, user, msg));
}

void GameServer::incomingConnection(qintptr socket_desc)
{
    if (users_.size() < MAX_USERS)
    {
        // ovo treba da se razmotri, nismo sigurni da li će ovo praviti siročiće
        ConnectionThread *user = new ConnectionThread(socket_desc, nullptr);

        int thread_idx = available_threads_.size();
        if (thread_idx < MAX_USERS) {
            available_threads_.append(new QThread(this));
            available_threads_.last()->start();
        } else {
            //TODO
        }
        user->moveToThread(available_threads_.at(thread_idx));

        connect(available_threads_.at(thread_idx), &QThread::finished, user, &QObject::deleteLater);
        connect(user, &ConnectionThread::disconnectedFromClient, this, std::bind(&GameServer::userDisconnected, this, user, thread_idx));
        connect(user, &ConnectionThread::error, this, std::bind(&GameServer::error, this, std::placeholders::_1));
        connect(user, &ConnectionThread::dataReceived, this, std::bind(&GameServer::dataReceived, this, user, std::placeholders::_1));
        connect(user, &ConnectionThread::logMessage, this, &GameServer::logMessage);
        connect(this, &GameServer::stopAllClients, user, &ConnectionThread::disconnectedFromClient);

        users_.push_back(user);
        qDebug() << "thread created";
        //user->start();
    }
}

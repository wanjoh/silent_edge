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
    qDebug() << "server gotov";
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

void GameServer::dataReceived(Connection* sender, const QByteArray& msg)
{
    Q_ASSERT(sender);

    emit logMessage(QLatin1String("object received"));

    // todo: check if data is valid

    broadcast(msg, sender);
}

void GameServer::userDisconnected(Connection* user, int thread_idx)
{
    Q_ASSERT(user);
    // videti da li nam ovo treba i kako ga azurirati
//    --threads_load_[thread_idx];
    users_.removeAll(user);
}

void GameServer::broadcast(const QByteArray& msg, Connection *sender)
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

void GameServer::sendData(Connection *user, const QByteArray& msg)
{
    Q_ASSERT(user);

    QTimer::singleShot(0, user, std::bind(&Connection::sendData, user, msg));
}

void GameServer::incomingConnection(qintptr socket_desc)
{
    if (users_.size() < MAX_USERS)
    {
        // ovo treba da se razmotri, nismo sigurni da li će ovo praviti siročiće
        Connection *user = new Connection(socket_desc, nullptr);

        int thread_idx = available_threads_.size();

        if (thread_idx < MAX_USERS)
        {
            available_threads_.append(new QThread(this));
            available_threads_.last()->start();
        }
        else
        {
            // TODO: handle
        }

        user->moveToThread(available_threads_.at(thread_idx));

        connect(user, &Connection::disconnectedFromServer, this, std::bind(&GameServer::userDisconnected, this, user, thread_idx));
        connect(user, &Connection::error, this, std::bind(&GameServer::error, this, std::placeholders::_1));
        connect(user, &Connection::dataReceived, this, std::bind(&GameServer::dataReceived, this, user, std::placeholders::_1));
        connect(user, &Connection::logMessage, this, &GameServer::logMessage);
        connect(this, &GameServer::stopAllClients, user, &Connection::disconnectedFromServer); // popraviti

        connect(user, &Connection::disconnectedFromServer, available_threads_.last(), &QThread::quit);
        connect(user, &Connection::disconnectedFromServer, user, &Connection::deleteLater);
        connect(available_threads_.last(), &QThread::finished, user, &QObject::deleteLater);

        users_.push_back(user);
        qDebug() << "thread created";
    }
}

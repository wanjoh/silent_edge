#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client_(new Client())
{
    ui->setupUi(this);

    connect(ui->pushButtonCreateServer,&QPushButton::clicked,
            this,&MainWindow::onPbCreateServer_clicked);

    connect(ui->pushButtonJoinGame,&QPushButton::clicked,
            this,&MainWindow::onPbJoinGame_clicked);

    connect(ui->pushButtonBack_2,&QPushButton::clicked,
            this,&MainWindow::onPbBack_clicked);

    connect(ui->pushButtonConnect,&QPushButton::clicked,
            this,&MainWindow::onPbConnect_clicked);

    connect(ui->pushButtonSettings,&QPushButton::clicked,
            this,&MainWindow::onPbSettings_clicked);

    connect(ui->pushButtonBack,&QPushButton::clicked,
            this,&MainWindow::onPbBack_clicked);

    connect(ui->pushButtonDone,&QPushButton::clicked,
            this,&MainWindow::onPbDone_clicked);

    connect(ui->pushButtonQuit,&QPushButton::clicked,
            this,&MainWindow::onPbQuit_clicked);



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onPbCreateServer_clicked()
{

    ui->ip_label->setVisible(true);
    ui->ip_line->setVisible(true);
    ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::onPbJoinGame_clicked()
{

    ui->stackedWidget->setCurrentIndex(2);

}

void MainWindow::onPbConnect_clicked()
{

    QString server_address = ui->serverIP_line->text();
    client_->connectToServer(server_address, ServerConfig::PORT);

}

void MainWindow::onPbSettings_clicked()
{

    ui->ip_label->setVisible(false);
    ui->ip_line->setVisible(false);
    ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::onPbBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}

void MainWindow::onPbDone_clicked()
{

    if(ui->ip_line->isVisible())
    {

        QString server_address = ui->ip_line->text();

        server_ = new GameServer(server_address);

        connect(server_, &GameServer::playerJoined, this, &MainWindow::onPlayerJoined);
    }

    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onPbQuit_clicked()
{
    this->close();
}

void MainWindow::onPlayerJoined(const QString &playerName, Lobby* lobby)
{
    if (lobby)
    {
        if(!lobby->isVisible())
        {
            connect(lobby,&Lobby::closeConnection,this,&MainWindow::disconnectFromServer);
            connect(this,&MainWindow::updateLobbySignal,lobby,&Lobby::updateLobby);
            connect(lobby,&Lobby::startGameSignal,server_,&GameServer::broadcast);
            connect(client_, &Client::startGameSignal, this, &MainWindow::startGame);
            lobby_ = lobby;
            lobby->show();
            qDebug() << playerName << "joined the game!";
            emit updateLobbySignal(playerName);
        }
        else
        {
            emit updateLobbySignal(playerName);
        }
    }

}

void MainWindow::startGame(const QString &server_ip)
{
    Game *game = new Game("name", client_);
    game->startGame(server_ip);
    game->moveToThread(new QThread);
}


void MainWindow::disconnectFromServer()
{

    client_->disconnectFromHost();
}

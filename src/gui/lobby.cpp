#include "lobby.hpp"
#include "ui_lobby.h"

Lobby::Lobby(QString server_ip,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Lobby),
    server_ip_(server_ip)
{
    ui->setupUi(this);

    connect(ui->pushButtonExit,&QPushButton::clicked,this,&Lobby::onPbExit_clicked);
    connect(ui->pushButtonReady,&QPushButton::clicked,this,&Lobby::onPbReady_clicked);
}

Lobby::~Lobby()
{
    delete ui;
}

bool Lobby::isEmpty() const
{
    return ui->playerList->count() == 0;
}

void Lobby::onPbExit_clicked()
{
    this->close();
    emit closeConnection();
}

void Lobby::onPbReady_clicked()
{
    qDebug() << "Ready";

    QString string = "ip ";
    string.append(server_ip_);
    string.append("\0");

    emit startGameSignal(string.toUtf8());
}

void Lobby::updateLobby(const QString &playerName)
{
    ui->playerList->addItem(playerName);
}



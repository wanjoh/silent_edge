#include "lobby.hpp"
#include "ui_lobby.h"

Lobby::Lobby(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Lobby)
{
    ui->setupUi(this);

    connect(ui->pushButtonExit,&QPushButton::clicked,this,&Lobby::onPbExit_clicked);

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



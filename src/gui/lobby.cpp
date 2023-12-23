#include "lobby.hpp"
#include "ui_lobby.h"

Lobby::Lobby(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Lobby)
{
    ui->setupUi(this);
}

Lobby::~Lobby()
{
    delete ui;
}

/*QVariant Lobby::toVariant() const
{

}

void Lobby::fromVariant(const QVariant &variant)
{

}*/

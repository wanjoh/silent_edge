#include "mainwindow.hpp"
#include "ui_mainwindow.h"

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

    if(ui->lineUsername->text().trimmed()!="")
    {
        QString server_address = ui->serverIP_line->text();

        QString username = ui->lineUsername->text();

        Game *game = new Game(username, client_);
        game->startGame(server_address);
        game->moveToThread(new QThread);

    }
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

    }

    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onPbQuit_clicked()
{
    this->close();
}


void MainWindow::disconnectFromServer()
{

    client_->disconnectFromHost();
}

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

    QString ip = server_->server_address();

    qsizetype dot_position = ip.lastIndexOf('.');

    QString base_ip = ip.sliced(0,dot_position+1);



    for(int j=1;j<=255;j++)
    {
        QTcpSocket tcp_socket;
        QString ip_address = base_ip + QString::number(j);
        tcp_socket.connectToHost(QHostAddress(ip_address),6969);

        if(tcp_socket.waitForConnected(5))
        {
            qDebug() << "Server found at" << ip_address << "on port" << 6969;
            ui->serverList->addItem(ip_address);
            tcp_socket.disconnectFromHost();
        }
    }



}

void MainWindow::onPbConnect_clicked()
{

    QList<QListWidgetItem*> server_list = ui->serverList->selectedItems();
    QString server_address = server_list.first()->text();
    client_->connectToServer(server_address,6969);



    //client_->sendMessage(lobbies_[server_address]->toVariant());




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

        //list_of_servers[server_address] = new GameServer(server_address);

    }

    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onPbQuit_clicked()
{
    this->close();
}


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

QString MainWindow::getLocalIPv4Address()
{
    QList<QHostAddress> ip_addresses_list = QNetworkInterface::allAddresses();

    for (const QHostAddress &address : ip_addresses_list) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && !address.isLoopback()) {
            return address.toString();
        }
    }

    return QString();
}

void MainWindow::onPbCreateServer_clicked()
{

    ui->ip_label->setVisible(true);
    ui->ip_line->setVisible(true);
    ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::onPbJoinGame_clicked()
{

    //QString local_ip = getLocalIPv4Address();


    //ovo sluzi da testiram na svom racunaru
    QString local_ip = "192.168.1.2";
    //
    qsizetype dot_position = local_ip.lastIndexOf('.');

    QString base_ip = local_ip.sliced(0,dot_position+1);

    test_connection = true;

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


    ui->stackedWidget->setCurrentIndex(2);

}

void MainWindow::onPbConnect_clicked()
{

    test_connection = false;
    QList<QListWidgetItem*> server_list = ui->serverList->selectedItems();
    if (!server_list.isEmpty())
    {
        QString server_address = server_list.first()->text();
        client_->connectToServer(server_address, 6969);


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
    if (lobby && !test_connection)
    {
        connect(lobby,&Lobby::closeConnection,this,&MainWindow::disconnectFromServer);
        connect(this,&MainWindow::updateLobbySignal,lobby,&Lobby::updateLobby);
        connect(lobby,&Lobby::startGameSignal,this,&MainWindow::startGame);
        lobby_ = lobby;
        lobby->show();
        qDebug() << playerName << "joined the game!";
        test_connection = !test_connection;
        emit updateLobbySignal(playerName);
    }

}

void MainWindow::startGame(const QString& server_ip)
{
    Game *game = new Game("name");
    game->startGame(server_ip);
    game->moveToThread(new QThread);
}


void MainWindow::disconnectFromServer()
{

    client_->disconnectFromHost();
}

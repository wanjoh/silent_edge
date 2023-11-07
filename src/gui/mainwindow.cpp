#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButtonPlay,&QPushButton::clicked,
            this,&MainWindow::onPbPlay_clicked);

    connect(ui->pushButtonSettings,&QPushButton::clicked,
            this,&MainWindow::onPbSettings_clicked);

    connect(ui->pushButtonQuit,&QPushButton::clicked,
            this,&MainWindow::onPbQuit_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPbPlay_clicked()
{

    ui->stackedWidget->setCurrentIndex(2);

}

void MainWindow::onPbSettings_clicked()
{

    ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::onPbQuit_clicked()
{

    this->close();
}




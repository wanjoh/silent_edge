#include "results.hpp"
#include "ui_results.h"

Results::Results(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Results)
{
    ui->setupUi(this);
}

Results::~Results()
{
    delete ui;
}

void Results::readResultsFromFile()
{
    QString path = QDir::currentPath().mid(0, QDir::currentPath().lastIndexOf('/'));
    QFile resultsFile(path + "/silent-edge/resources/results.txt");

    if(!resultsFile.exists()){
        qDebug() << "File does not exist.";
        return ;
    }
    if(!resultsFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Opening failed";
        return ;
    }

    players_ = "";
    scores_ = "";
    QTextStream stream(&resultsFile);
    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        QStringList lineList = line.split(" ");

        const QString& name = lineList.at(0);
        const QString& score = lineList.at(1);

        players_ += name + "\n";
        scores_ += score + "\n";
    }

    resultsFile.close();
}

void Results::showResults()
{
    readResultsFromFile();

    ui->label_players->setText(players_);
    ui->label_scores->setText(scores_);

    this->show();
}

void Results::on_pb_Close_clicked()
{
    this->close();
}


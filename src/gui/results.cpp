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
    QFile results("../silent-edge/resources/results.txt");
    if(!results.exists())
    {
        qDebug() << "File does not exist.";
        return ;
    }

    results.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!results.isOpen())
    {
        qDebug() << "Opening failed";
        return ;
    }

    players_ = "";
    scores_ = "";
    QTextStream stream(&results);
    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        QStringList lineList = line.split(" ");

        QString name = lineList.at(0);
        QString score = (lineList.at(1));

        players_ += name + "\n";
        scores_ += score + "\n";
    }

    results.close();
}

void Results::showResults()
{
    readResultsFromFile();

    ui->label_players->setText(players_);
    ui->label_scores->setText(scores_);

    this->show();
}

void Results::on_pB_close_clicked()
{
    this->close();
}


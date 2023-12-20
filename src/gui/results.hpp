#pragma once

#include <QWidget>
#include <QDir>
#include <QFile>

namespace Ui {
class Results;
}

class Results : public QWidget
{
    Q_OBJECT

public:
    explicit Results(QWidget *parent = nullptr);
    ~Results();

    void readResultsFromFile();
    void showResults();

private slots:
    void on_pB_close_clicked();

private:
    Ui::Results *ui;
    QString players_;
    QString scores_;
};


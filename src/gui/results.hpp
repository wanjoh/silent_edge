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
    ~Results() override;

    void readResultsFromFile();
    void showResults();

private slots:
    void on_pb_Close_clicked();

private:
    Ui::Results *ui;
    QString players_;
    QString scores_;
};

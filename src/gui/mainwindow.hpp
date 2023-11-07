#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void onPbPlay_clicked();
    void onPbSettings_clicked();
    void onPbQuit_clicked();

private:
    Ui::MainWindow *ui;
};

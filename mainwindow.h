#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

struct User
{
    int id;
    std::string username;
};

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int socket;
    User user = {0, "client"};

public:
    MainWindow(QWidget *parent = nullptr, int socket = 0);
    ~MainWindow();

private slots:

    void on_LoginButton_clicked();

    void on_RegisterButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

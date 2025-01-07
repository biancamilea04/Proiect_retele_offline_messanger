#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "LoginDialog.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <arpa/inet.h>
#include <string>

// ./client 127.0.0.1 2025

extern int errno;

int port;

using namespace std;

int f(string ip, string port)
{
    int sd;
    struct sockaddr_in server;

    int port2 = atoi(port.c_str());

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket().\n");
        return errno;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(port2);

    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Eroare la connect().\n");
        return errno;
    }

    printf("[client]Welcome!.\n");

    return sd;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int sd = f("127.0.0.1", "2025");

    MainWindow mainWindow(nullptr, sd);
    mainWindow.setWindowTitle("Client Chat");
    mainWindow.show();

    return app.exec();
}

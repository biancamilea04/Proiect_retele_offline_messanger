#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "registerdialog.h"
#include "interfata.h"
#include <QMessageBox>
#include <string.h>

#include "include/biblioteci.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent, int socket)
    : QMainWindow(parent), ui(new Ui::MainWindow), socket(socket)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_LoginButton_clicked()
{

    LoginDialog *login = new LoginDialog(this, socket);
    if (login->exec() == QDialog::Accepted)
    {
        printf("Exec login");
    }

    if (login->flagLogin == 1)
    {

        interfata *Interfata = new interfata(this, socket, login->userName);
        if (Interfata->exec() == QDialog::Accepted)
        {
        }
        delete Interfata;
    }
    delete login;
}

void MainWindow::on_RegisterButton_clicked()
{
    RegisterDialog *registerDialog = new RegisterDialog(this, socket);
    if (registerDialog->exec() == QDialog::Accepted)
    {
        printf("Exec login");
    }
    delete registerDialog;
}

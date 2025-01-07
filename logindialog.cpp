#include "logindialog.h"
#include "interfata.h"
#include "ui_logindialog.h"
#include "include/biblioteci.h"
#include <QMessageBox>

using namespace std;

LoginDialog::~LoginDialog()
{
    delete ui;
}

LoginDialog::LoginDialog(QWidget *parent, int sd) : QDialog(parent), socket(sd), ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    usernameEdit = ui->username;
    passwordEdit = ui->password;
}

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

void LoginDialog::on_Login_clicked()
{

    userName = usernameEdit->text().toStdString();
    string password = passwordEdit->text().toStdString();
    string msg = "login " + userName + " " + password;
    if (fn_write(this->socket, const_cast<char *>(msg.c_str()), msg.size() + 1) <= 0)
    {
        printf("[client]Eroare la scrierea in socket\n");
        perror("[client]eroarea ");
        return;
    }

    printf("Am trimis catre server mesajul:%s\n", msg.c_str());

    char response[2024];
    bzero(response, 2024);
    int size;

    if (fn_read(this->socket, response, &size) <= 0)
    {
        printf("[client]Eroare la read() de la server.\n");
        return;
    }

    if (strncmp(response, "Succes login!", strlen("Succes login!")) == 0)
    {
        this->setWindowTitle(QString::fromStdString("Chat " + userName));

        string msgR = response;

        flagLogin = 1;
        hide();
    }
    else
    {
        QString errorMessage = QString::fromStdString(response);
        QMessageBox::critical(nullptr, "Login Failed", "Error: " + errorMessage);
        show();
    }
}

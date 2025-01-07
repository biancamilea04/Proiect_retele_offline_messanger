#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "include/biblioteci.h"

using namespace std;

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent),
                                                  ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
}

RegisterDialog::RegisterDialog(QWidget *parent, int sd) : QDialog(parent),
                                                          socket(sd),
                                                          ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    usernameEdit = ui->username;
    passwordEdit = ui->password;
    confirmPasswordEdit = ui->confirmpassword;
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_registerButton_clicked()
{
    string userName = usernameEdit->text().toStdString();
    string password = passwordEdit->text().toStdString();
    string confirmPassword = confirmPasswordEdit->text().toStdString();
    string msg = "register " + userName + " " + password + " " + confirmPassword;

    if (fn_write(this->socket, const_cast<char *>(msg.c_str()), msg.size() + 1) <= 0)
    {
        printf("[clientRegister]Eroare la scrierea in socket\n");
        perror("[clientReegister]eroarea ");
        return;
    }

    char response[2024];
    bzero(response, 2024);
    int size;

    // Citirea răspunsului de la server
    if (fn_read(this->socket, response, &size) <= 0)
    {
        printf("[client]Eroare la read() de la server.\n");
        return;
    }
    // Inregistrare reusita

    if (strncmp(response, "Inregistrare reusita", strlen("Inregistrare reusita")) == 0)
    {
        // Dacă login-ul este un succes
        show();

        string msg = response;

        this->setWindowTitle(QString::fromStdString("Chat " + userName));

        QMessageBox::information(nullptr, "Login Successful", "Welcome, " + usernameEdit->text() + "!");
    }
    else
    {
        // În cazul unui login eșuat, afișăm o fereastră de eroare, dar nu închidem fereastra de login
        QString errorMessage = QString::fromStdString(response);
        QMessageBox::critical(nullptr, "Login Failed", "Error: " + errorMessage);
        show();
    }
}

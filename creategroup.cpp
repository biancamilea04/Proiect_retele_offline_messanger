#include "creategroup.h"
#include "ui_creategroup.h"
#include "biblioteci.h"

#include <QMessageBox>

using namespace std;

createGroup::createGroup(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::createGroup)
{
    ui->setupUi(this);
}

createGroup::createGroup(QWidget *parent, int sd) : QDialog(parent),
                                                    socket(sd),
                                                    ui(new Ui::createGroup)
{
    ui->setupUi(this);
}

createGroup::~createGroup()
{
    delete ui;
}

void createGroup::on_create_clicked()
{
    string msg = "create " + ui->groupName->text().toStdString();

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

    string rez = response;
    if (rez != "Grupul a fost creat cu succes.")
    {
        QString errorMessage = QString::fromStdString(response);
        QMessageBox::critical(nullptr, "createGroup", "Error: " + errorMessage);
    }
    hide();
}

#include "interfata.h"
#include "ui_interfata.h"
#include "include/biblioteci.h"
#include "creategroup.h"
#include "addtogroup.h"

#include <QMessageBox>
#include <QTimer>
#include <QDebug>

interfata::interfata(QWidget *parent) : QDialog(parent),
                                        ui(new Ui::interfata)
{
    ui->setupUi(this);
}

interfata::interfata(QWidget *parent, int sd, string username) : QDialog(parent),
                                                                 socket(sd),
                                                                 userName(username),
                                                                 ui(new Ui::interfata)
{
    ui->setupUi(this);
    ui->groupBox->hide();
    ui->UsersBox->hide();
    this->setWindowTitle(QString::fromStdString(userName));
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &interfata::userUpdate);
    timer->start(1400);

    QTimer *timerGroup = new QTimer(this);
    connect(timerGroup, &QTimer::timeout, this, &interfata::groupsUpdate);
    timerGroup->start(1300);

    trimiteNotificari();
}

interfata::~interfata()
{
    string msg = "logout";

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
    delete ui;
}

void interfata::userUpdate()
{
    string msg = "users";
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

    QString responseStr = QString::fromUtf8(response);
    QStringList users = responseStr.split("\n", Qt::SkipEmptyParts);

    ui->listWidget->clear();

    for (const QString &user : users)
    {
        ui->listWidget->addItem(user.trimmed());
    }
}

void interfata::chatUpdate()
{
    string msg = "history " + userSend;

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

    QString responseStr = QString::fromUtf8(response);
    QStringList mesaje = responseStr.split("\n", Qt::SkipEmptyParts);

    ui->chat->clear();

    for (const QString &mesaj : mesaje)
    {
        ui->chat->addItem(mesaj);
    }
}

void interfata::mesajeNecitite()
{
    string msg = "notificareNecitita";

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

    string msgRasp = response;
    if (msgRasp != "\nNu ai mesaje necitite.")
    {
        QMessageBox::information(nullptr, "Mesaje necitite", msgRasp.c_str());
    }
}

void interfata::notificareMesaje()
{

    string msg = "notificare";

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

    // Citirea răspunsului de la server
    if (fn_read(this->socket, response, &size) <= 0)
    {
        printf("[client]Eroare la read() de la server.\n");
        return;
    }

    string msgRasp = response;
    string titlu = "[" + userName + "]" + "Notificare";
    if (msgRasp != "Nu ai notificari")
    {
        QMessageBox::information(nullptr, titlu.c_str(), msgRasp.c_str());
    }
}

void interfata::trimiteNotificari()
{

    mesajeNecitite();

    QTimer *timerNotificare = new QTimer(this);
    connect(timerNotificare, &QTimer::timeout, this, &interfata::notificareMesaje);
    timerNotificare->start(1170);
}

void interfata::groupsUpdate()
{

    string msg = "group";
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

    string msgRasp = response;
    if (msgRasp == "Nu esti in niciun grup.")
    {
        flagGroup = 0;
    }
    else
    {
        flagGroup = 1;
        QString responseStr = QString::fromUtf8(response);
        QStringList groups = responseStr.split("\n", Qt::SkipEmptyParts);

        ui->GroupNames->clear();

        for (const QString &group : groups)
        {
            ui->GroupNames->addItem(group);
        }
    }
}

void interfata::membersGroup()
{
    string msg = "groupMembers " + groupName;
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

    QString responseStr = QString::fromUtf8(response);
    QStringList groups = responseStr.split("\n", Qt::SkipEmptyParts);

    ui->MembersNames->clear();

    for (const QString &group : groups)
    {
        ui->MembersNames->addItem(group);
    }
}

void interfata::chatGroup()
{
    string msg = "historyGroup " + groupName;
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

    string res = response;
    res = res.substr(res.find(':') + 1);
    QString responseStr = QString::fromStdString(res);
    QStringList groups = responseStr.split("\n", Qt::SkipEmptyParts);

    ui->chatGroup->clear();

    for (const QString &group : groups)
    {
        ui->chatGroup->addItem(group);
    }
}

void interfata::sendGroup()
{
    string msg = "sendGroup " + groupName + ": " + ui->groupLine->text().toStdString();

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

    if (strcmp(response, "Mesaj trimis cu succes.") == 0)
    {
    }
    else
    {
        QString errorMessage = QString::fromStdString(response);
        QMessageBox::critical(nullptr, "Send", "Error: " + errorMessage);
    }
    ui->groupLine->clear();
}

void interfata::on_listWidget_clicked(const QModelIndex &index)
{
    QString selectedText = index.data(Qt::DisplayRole).toString();
    string userStatus = selectedText.toStdString();

    userSend = userStatus.substr(0, userStatus.find(' '));

    ui->UsersBox->show();
    ui->UsersBox->setTitle(userSend.c_str());

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &interfata::chatUpdate);
    timer->start(1100);
}

void interfata::on_sendButton_clicked()
{
    QLineEdit *mesajLine = ui->mesajLine;
    string mesaj = mesajLine->text().toStdString();

    string msg = "send " + userSend + ": " + mesaj;
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

    if (strcmp(response, "Mesaj trimis cu succes.") == 0)
    {
    }
    else
    {
        QString errorMessage = QString::fromStdString(response);
        QMessageBox::critical(nullptr, "Send", "Error: " + errorMessage);
    }
    ui->mesajLine->clear();
}

void interfata::on_LogoutButton_clicked()
{
    string msg = "logout";

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
    close();
}

void interfata::on_chat_clicked(const QModelIndex &index)
{
    QString selectedText = index.data(Qt::DisplayRole).toString();
    string mesajReply = selectedText.toStdString();
    string reply = mesajReply.substr(mesajReply.find('>') + 1);
    string userReplied = mesajReply.substr(0, mesajReply.find('>'));
    // userSend = userStatus.substr(0,userStatus.find(' '));
    string msgSend = "<replied " + userReplied + ": " + reply + ">";
    ui->mesajLine->setText(msgSend.c_str());
}

void interfata::on_mesajLine_returnPressed()
{
}

void interfata::on_GroupNames_clicked(const QModelIndex &index)
{
    QString selectedText = index.data(Qt::DisplayRole).toString();
    groupName = selectedText.toStdString();
    ui->groupBox->show();
    ui->groupBox->setTitle(groupName.c_str());

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &interfata::membersGroup);
    timer->start(1070);

    QTimer *timerChat = new QTimer(this);
    connect(timerChat, &QTimer::timeout, this, &interfata::chatGroup);
    timerChat->start(1050);
}

void interfata::on_groupSend_clicked()
{

    sendGroup();
}

void interfata::on_groupLine_returnPressed()
{
    sendGroup();
}

void interfata::on_chatGroup_clicked(const QModelIndex &index)
{
    QString selectedText = index.data(Qt::DisplayRole).toString();
    string mesajReply = selectedText.toStdString();
    string reply = mesajReply.substr(mesajReply.find('>') + 1);
    string userReplied = mesajReply.substr(0, mesajReply.find('>'));
    // userSend = userStatus.substr(0,userStatus.find(' '));
    string msgSend = "<replied " + userReplied + ": " + reply + ">";

    ui->groupLine->setText(msgSend.c_str());
}

void interfata::on_CreateGroup_clicked()
{
    createGroup *create = new createGroup(this, socket);
    if (create->exec() == QDialog::Accepted)
    {
    }
}

void interfata::on_pushButton_2_clicked()
{

    addToGroup *add = new addToGroup(this, socket, groupName);
    if (add->exec() == QDialog::Accepted)
    {
    }
}

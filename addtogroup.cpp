#include "addtogroup.h"
#include "ui_addtogroup.h"
#include "biblioteci.h"

#include <QMessageBox>

addToGroup::addToGroup(QWidget *parent) : QDialog(parent),
                                          ui(new Ui::addToGroup)
{
    ui->setupUi(this);
}

addToGroup::addToGroup(QWidget *parent, int sd, std::string groupname) : QDialog(parent),
                                                                         socket(sd),
                                                                         groupName(groupname),
                                                                         ui(new Ui::addToGroup)
{
    ui->setupUi(this);
    ui->members->clear();
    addMembers();
}

addToGroup::~addToGroup()
{
    delete ui;
}

string addToGroup::removeWords(std::string input)
{

    std::string result = input;

    const std::vector<std::string> wordsToRemove = {"online", "offline"};

    for (const auto &word : wordsToRemove)
    {
        size_t pos;

        while ((pos = result.find(word)) != std::string::npos)
        {
            result.erase(pos, word.length());

            if (pos < result.size() && result[pos] == ' ')
            {
                result.erase(pos, 1);
            }
            if (pos > 0 && result[pos - 1] == ' ')
            {
                result.erase(pos - 1, 1);
            }
        }
    }

    return result;
}

void addToGroup::addMembers()
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

    string rez = response;
    string rezMembers = this->removeWords(rez);
    QString myString = QString::fromStdString(rezMembers);

    QStringList elements = myString.split("\n");

    for (const QString &element : elements)
    {
        ui->members->addItem(element.trimmed());
    }
}

void addToGroup::on_pushButton_clicked()
{
    string username = ui->members->currentText().toStdString();
    string msg = "add " + groupName + " " + username;

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

    if (rez == "Utilizatorul a fost adăugat în grup.")
    {
        QMessageBox::information(nullptr, "Add", rez.c_str());
        hide();
    }
    else
    {
        QString errorMessage = QString::fromStdString(response);
        QMessageBox::critical(nullptr, "Add error", "Error: " + errorMessage);
    }
}

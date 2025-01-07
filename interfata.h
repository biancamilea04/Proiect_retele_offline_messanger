#ifndef INTERFATA_H
#define INTERFATA_H

#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QListView>
#include <QStringListModel>
#include <QListWidgetItem>

using namespace std;

namespace Ui
{
    class interfata;
}

class interfata : public QDialog
{
    Q_OBJECT

public:
    explicit interfata(QWidget *parent = nullptr);
    interfata(QWidget *parent = nullptr, int sd = 0, string userName = "");
    ~interfata();

    int socket;
    int id;
    int flagGroup = 1;
    string userName;
    string userSend;
    string groupName;

    QLineEdit *mesaj;
    QPushButton *loginButton;

    void userUpdate();
    void chatUpdate();
    void mesajeNecitite();
    void notificareMesaje();
    void trimiteNotificari();

    void groupsUpdate();
    void membersGroup();
    void chatGroup();
    void sendGroup();
private slots:

    void on_listWidget_clicked(const QModelIndex &index);

    void on_sendButton_clicked();

    void on_LogoutButton_clicked();

    void on_chat_clicked(const QModelIndex &index);

    void on_mesajLine_returnPressed();

    void on_GroupNames_clicked(const QModelIndex &index);

    void on_groupSend_clicked();

    void on_groupLine_returnPressed();

    void on_chatGroup_clicked(const QModelIndex &index);

    void on_CreateGroup_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::interfata *ui;
};

#endif // INTERFATA_H

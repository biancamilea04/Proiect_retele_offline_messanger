#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace Ui
{
    class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::LoginDialog *ui;

public:
    LoginDialog(QWidget *parent = nullptr);
    LoginDialog(QWidget *parent = nullptr, int sd = 0);
    ~LoginDialog();
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    int flagLogin = 0;
    std::string userName;
    int socket;
private slots:
    void on_Login_clicked();
};

#endif // LOGINDIALOG_H

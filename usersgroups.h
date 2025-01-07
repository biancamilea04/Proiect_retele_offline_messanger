#ifndef USERSGROUPS_H
#define USERSGROUPS_H

#include <QWidget>

namespace Ui
{
    class UsersGroups;
}

class UsersGroups : public QWidget
{
    Q_OBJECT

public:
    explicit UsersGroups(QWidget *parent = nullptr);
    ~UsersGroups();

private:
    Ui::UsersGroups *ui;
};

#endif // USERSGROUPS_H

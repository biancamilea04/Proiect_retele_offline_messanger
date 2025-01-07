#include "usersgroups.h"
#include "ui_usersgroups.h"

UsersGroups::UsersGroups(QWidget *parent) : QWidget(parent),
                                            ui(new Ui::UsersGroups)
{
    ui->setupUi(this);
}

UsersGroups::~UsersGroups()
{
    delete ui;
}

#ifndef CREATEGROUP_H
#define CREATEGROUP_H

#include <QWidget>
#include <QDialog>

namespace Ui
{
    class createGroup;
}

class createGroup : public QDialog
{
    Q_OBJECT

public:
    explicit createGroup(QWidget *parent = nullptr);
    createGroup(QWidget *parent = nullptr, int socket = 0);
    ~createGroup();

    int socket;
private slots:
    void on_create_clicked();

private:
    Ui::createGroup *ui;
};

#endif // CREATEGROUP_H

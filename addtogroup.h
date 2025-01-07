#ifndef ADDTOGROUP_H
#define ADDTOGROUP_H

#include <QWidget>
#include <QDialog>

namespace Ui
{
    class addToGroup;
}

class addToGroup : public QDialog
{
    Q_OBJECT

public:
    explicit addToGroup(QWidget *parent = nullptr);
    addToGroup(QWidget *parent = nullptr, int sd = 0, std::string groupName = "");
    ~addToGroup();

    int socket;
    std::string groupName;

    std::string removeWords(std::string input);
    void addMembers();

private slots:
    void on_pushButton_clicked();

private:
    Ui::addToGroup *ui;
};

#endif // ADDTOGROUP_H

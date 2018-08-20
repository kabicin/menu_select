#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include <QMainWindow>
#include "connection.h"

namespace Ui {
class AddWindow;
}

class AddWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddWindow(QWidget *parent = nullptr);
    ~AddWindow();

private:
    Ui::AddWindow *ui;
    connection con;
};

#endif // ADDWINDOW_H

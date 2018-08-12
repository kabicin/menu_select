#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include "connection.h"


namespace Ui {
class addDialog;
}

class addDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDialog(QWidget *parent = nullptr);
    ~addDialog();
    void setData(connection&);

private slots:
    void on_btnClrField_clicked();
    void on_btnAddToDatabase_clicked();
    void on_loadFromFile_clicked();

private:
    Ui::addDialog *ui;
    connection con;
};

#endif // ADDDIALOG_H

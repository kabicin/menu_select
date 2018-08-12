#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "connection.h"
#include "adddialog.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();
    void setData(connection &);

private slots:
    void on_pushButton_sdiag_apply_clicked();

private:
    Ui::SettingsDialog *ui;
    connection con;
};

#endif // SETTINGSDIALOG_H

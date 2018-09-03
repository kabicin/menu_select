#ifndef PRINTCONFIRMDIALOG_H
#define PRINTCONFIRMDIALOG_H

#include <QDialog>
#include <QObject>

namespace Ui {
class printConfirmDialog;
}

class printConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit printConfirmDialog(QWidget *parent = nullptr);
    ~printConfirmDialog();

private slots:

private:
    Ui::printConfirmDialog *ui;
};

#endif // PRINTCONFIRMDIALOG_H

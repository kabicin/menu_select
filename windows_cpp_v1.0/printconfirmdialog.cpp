#include "printconfirmdialog.h"
#include "ui_printconfirmdialog.h"

printConfirmDialog::printConfirmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::printConfirmDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->btnPrintConfirmAccept, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(ui->btnPrintConfirmReject, SIGNAL(clicked()), this, SLOT(reject()));
}

printConfirmDialog::~printConfirmDialog()
{
    delete ui;
}


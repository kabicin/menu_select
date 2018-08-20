#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setData(connection &c){
    con = c;
    con.createConnection();
    qDebug() << "[Database] Transferring connection to Settings Dialog";
}

void SettingsDialog::on_pushButton_sdiag_apply_clicked()
{
    // When applied settings
    QString gs = ui->lineEdit_sdiag_gsfp->text();
    QString db = ui->lineEdit_sdiag_dbfp->text();
    if(gs.size() > 0){
        changeGsName(gs);
    }

    if(db.size() > 0){
        con.changeFilepath(db);
    }

}


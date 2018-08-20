#include "adddialog.h"
#include "ui_adddialog.h"
#include "connection.h"
#include <QDebug>

addDialog::addDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDialog)
{
    ui->setupUi(this);
}

addDialog::~addDialog()
{
    delete ui;
}

void addDialog::setData(connection &c){
    con = c;
    con.createConnection();
    qDebug() << "[Database] Transferring connection to Add Dialog";
}

void addDialog::on_btnClrField_clicked()
{
    // clears the lineEdits of the dialog
    ui->lineEdit_id->setText("");
    ui->lineEdit_foodNameEng->setText("");
    ui->lineEdit_foodNameCh->setText("");
    ui->lineEdit_price->setText("");
    ui->labelAddMessage->setText("");

}

void addDialog::on_btnAddToDatabase_clicked()
{
    QString foodid = ui->lineEdit_id->text();
    QString foodnameeng = ui->lineEdit_foodNameEng->text();
    QString foodnamech = ui->lineEdit_foodNameCh->text();
    QString price = ui->lineEdit_price->text();

    // validates each line edit
    if(foodid.size() > 0 && foodnameeng.size() > 0 && foodnamech.size() > 0 && price.size() > 0){
        con.insertItem(foodid.toInt(), foodnameeng, foodnamech, price);
        ui->labelAddMessage->setText("Successfully added item to database!");
    }
}


void addDialog::on_loadFromFile_clicked()
{
    // test button
    QFile file(QDir::currentPath() + "/dhmenudata.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Could not open file!!!";
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList lines = line.split("|");
        lines[0].remove('\t');
        lines[1].remove('\t');
        lines[2].remove('\t');
        lines[3].remove('\t');
        qDebug() << "Inserted item number" << lines[0];
        con.insertItem(lines[0].toInt(), lines[1], lines[2], lines[3]);
    }
    file.close();
    in.flush();
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QIntValidator>
#include <QKeyEvent>
#include "adddialog.h"
#include "settingsdialog.h"
#include "connection.h"
#include <QFile>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QFontMetrics>
#include "printformat.h"
#include <QListWidget>
#include <QEventLoop>
#include <QObject>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // preconfigure search bar to only accept integers between 0-999
    ui->lineEditNum->setValidator(new QIntValidator(0, 999, this));

    // link the scroll bars to connect all listWidgets
    QScrollBar *main = ui->listWidget_2->verticalScrollBar();
    QScrollBar *other1 = ui->listWidget->verticalScrollBar();
    QScrollBar *other2 = ui->listWidget_3->verticalScrollBar();
    QScrollBar *other3 = ui->listWidget_4->verticalScrollBar();
    connect(main, SIGNAL(valueChanged(int)), other1, SLOT(setValue(int)));
    connect(main, SIGNAL(valueChanged(int)), other2, SLOT(setValue(int)));
    connect(main, SIGNAL(valueChanged(int)), other3, SLOT(setValue(int)));
    connect(other1, SIGNAL(valueChanged(int)), main, SLOT(setValue(int)));
    connect(other1, SIGNAL(valueChanged(int)), other2, SLOT(setValue(int)));
    connect(other1, SIGNAL(valueChanged(int)), other3, SLOT(setValue(int)));
    connect(other2, SIGNAL(valueChanged(int)), main, SLOT(setValue(int)));
    connect(other2, SIGNAL(valueChanged(int)), other1, SLOT(setValue(int)));
    connect(other2, SIGNAL(valueChanged(int)), other3, SLOT(setValue(int)));
    connect(other3, SIGNAL(valueChanged(int)), main, SLOT(setValue(int)));
    connect(other3, SIGNAL(valueChanged(int)), other1, SLOT(setValue(int)));
    connect(other3, SIGNAL(valueChanged(int)), other2, SLOT(setValue(int)));

    // stylesheet for list labels
    ui->labelQuantityAdd->setStyleSheet("QLabel{background-color: green; color: white;}");
    ui->labelItemIdMinus->setStyleSheet("QLabel{background-color: orange; color: white;}");
    ui->labelPriceDelete->setStyleSheet("QLabel{background-color: red; color: white;}");


    // Create database connection.
    con.createConnection();

    // by default disable the abc buttons
    ui->btn_A->setEnabled(false);
    ui->btn_B->setEnabled(false);
    ui->btn_C->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}



// Logic behind grid layout numbered buttons, each button has their corresponding integer x.
// The bounds are 3 digits (inclusive) so we check to see if there are less than or equal to 2 digits already.
void MainWindow::btnAdd(int x){
    QString prev = ui->lineEditNum->text();
    int prev_sz = prev.size();
    if(prev_sz <= 2){
        ui->lineEditNum->setText(prev + QString::number(x));
    }
    else{
        qDebug() << "Too many numbers entered...";
    }
    queryDashboard();
}


// queryDashboard() searches the database for the current number in the search bar and displays it on the dashboard
void MainWindow::queryDashboard(){
    QString searchbar = ui->lineEditNum->text();
    if(searchbar.size() > 0){
        QList<QVariant> x = con.findItem(searchbar.toInt());
        if(x[2].toString().size() <= 0){
            ui->labelFood_1->setText("");
            ui->labelFood_2->setText("Invalid entry!");
            ui->labelFood_3->setText("输入无效");
            ui->labelFood_4->setText("");
        }
        else{
            updateDashboard(searchbar, x[0].toString(), x[1].toString(), "");
            ui->labelFood_4->setText(x[2].toString());
        }

    }
}


//Multipurpose function to clear the search bar.
// - used for Clear button and Go button.
void MainWindow::clearSearchBar(){
    ui->lineEditNum->setText("");
}

void MainWindow::on_btnClear_clicked()
{
    ui->labelFood_1->setText("");
    ui->labelFood_2->setText("");
    ui->labelFood_3->setText("");
    ui->labelFood_4->setText("");
    clearSearchBar();
}

void MainWindow::dashboardToggle(bool t){
    // toggles the dashboard buttons on/off.
    // t = true means buttons on.
    // t = false means buttons off.
    ui->btnGo->setEnabled(t);
    //ui->btnClear->setEnabled(t);
    ui->btnOne->setEnabled(t);
    ui->btnTwo->setEnabled(t);
    ui->btnThree->setEnabled(t);
    ui->btnFour->setEnabled(t);
    ui->btnFive->setEnabled(t);
    ui->btnSix->setEnabled(t);
    ui->btnSeven->setEnabled(t);
    ui->btnEight->setEnabled(t);
    ui->btnNine->setEnabled(t);
    ui->btnZero->setEnabled(t);
    ui->listWidget->setEnabled(t);
    ui->listWidget_2->setEnabled(t);
    ui->listWidget_3->setEnabled(t);
    ui->listWidget_4->setEnabled(t);
    ui->btnBackspace->setEnabled(t);

}

void MainWindow::set1(){
    choice = 0;
}

void MainWindow::set2(){
    choice = 1;
}

void MainWindow::set3(){
    choice = 2;
}

// helper function to get the specific row to enter the
int MainWindow::getRowListWidgetIncreasing(int itemid){
    // listWidget_4 is the widget containing elements of item ids
    // O(n) - but it doesn't matter since generally people will order no more than 10 orders.
    QListWidget *x = ui->listWidget_4;
    if(x->count() == 0){
        return 0;
    }
    for(int i = 0; i < x->count(); ++i){
        if(x->item(i)->text().toInt() > itemid){
            return i;
        }
    }
    return x->count();

}

void MainWindow::on_btnGo_clicked()
{

    // set the text of lineEditNum to search AGAIN
    // it will only search if labelFood_1 already has an entry
    // the entry MUST START WITH  "項目 # ", hence the mid(5) substring call.
    if(ui->lineEditNum->text().size() == 0){
        ui->lineEditNum->setText(ui->labelFood_1->text().mid(5));
    }

    QString num = ui->lineEditNum->text();

    //QList<QVariant> x = con.findItem(num.toInt());


    // if the search bar has a selection
    // if(x[0].toString().size() > 0){
    if(ui->labelFood_4->text().size() > 0){
        // display the food selection

        //updateDashboard(num, x[0].toString(), x[1].toString(), x[2].toString());

        // Add to list widget
        //QStringList prices = x[2].toString().split(" ");
        QStringList prices = ui->labelFood_4->text().split(" ");
        QStringList::size_type numPrices = prices.size();

        // create QStrings to hold name and price values for input in the listwidget.
        QString itemId;
        QString addName;
        QString addPrice;
        QString currName;

        // if there is more than one price, we need to go into an event loop.
        // the event loop will ask for one of 2-3 button presses either btn_A, btn_B or btn_C.
        // A B C are all located underneath the lineEdit search bar.

        if(numPrices > 1){
            qDebug() << "Starting event loop for food with more than one price.";

            // if there is more than one number of prices
            // we must override the updateDashboard by simply
            // setting the fourth food label to equal the original data.
            //ui->labelFood_4->setText(x[2].toString());

            if(numPrices >= 2){
                // enable the buttons because by default they are disabled.
                ui->btn_A->setEnabled(true);
                ui->btn_B->setEnabled(true);

                // set the text to the specific category.
                // for example S means small, M means medium, L means large.
                ui->btn_A->setText(prices[0].left(1));
                ui->btn_B->setText(prices[1].left(1));
            }
            if(numPrices >= 3){
                // enable the third button.
                ui->btn_C->setEnabled(true);

                // set the text to specific category.
                ui->btn_C->setText(prices[2].left(1));
            }

            // disable all button interaction to only work for buttons A B and C
            dashboardToggle(false);

            // --------------------
            // Enter the event loop
            QEventLoop loop;

            connect(ui->btnClear, SIGNAL(clicked()), &loop, SLOT(quit()));

            if(numPrices >= 2){
                connect(ui->btn_A, SIGNAL(clicked()), this, SLOT(set1()));
                connect(ui->btn_B, SIGNAL(clicked()), this, SLOT(set2()));
                connect(ui->btn_A, SIGNAL(clicked()), &loop, SLOT(quit()));
                connect(ui->btn_B, SIGNAL(clicked()), &loop, SLOT(quit()));

            }
            if(numPrices == 3){
                connect(ui->btn_C, SIGNAL(clicked()), this, SLOT(set3()));
                connect(ui->btn_C, SIGNAL(clicked()), &loop, SLOT(quit()));
            }


            loop.exec();

            // End of event loop
            // ------------------

            QString category;
            if(choice != -1){
                if(choice == 0){
                    category = ui->btn_A->text();
                }
                else if(choice == 1){
                    category = ui->btn_B->text();
                }
                else if(choice == 2){
                    category = ui->btn_C->text();
                }

                //addName = x[1].toString() + " (" + category + ")";
                //addPrice = x[2].toString().split(" ")[choice].mid(2);
                //currName = x[1].toString() + " (" + category + ")";

                addName = ui->labelFood_3->text() + " (" + category + ")";
                addPrice = ui->labelFood_4->text().split(" ")[choice].mid(2);
                currName = ui->labelFood_3->text() + " (" + category + ")";


            }
            else{
                // change value to -2 so that code outside the loop won't run upon a bad choice;
                choice = -2;
            }


            // clear ABC button text.
            // I am using a space so that the buttons maintain their width cus I don't wanna bother with the designer.
            ui->btn_A->setText(" ");
            ui->btn_B->setText(" ");
            ui->btn_C->setText(" ");

            // re-disable the ABC buttons.
            ui->btn_A->setEnabled(false);
            ui->btn_B->setEnabled(false);
            ui->btn_C->setEnabled(false);

            // re-enable button all functions
            dashboardToggle(true);
            //
        }
        // otherwise if there is only one price given
        else{
            // set the name and price
            //addName = x[1].toString();
            //addPrice = ui->labelFood_4->text();
            //currName = x[1].toString();
            addName = ui->labelFood_3->text();
            addPrice = ui->labelFood_4->text();
            currName = ui->labelFood_3->text();
        }

        // only runs if it is a multi step process.
        if(choice != -2){
            // get the item number
            itemId = ui->labelFood_1->text().mid(5);
            int itemIdInt = itemId.toInt();
            itemId = QString::number(itemIdInt);

            // if a duplicate exists
            int dup = checkDuplicateInList(currName);
            if(dup != -1){
                int c = ui->listWidget_3->item(dup)->text().toInt();
                c++;
                double price = ui->listWidget_2->item(dup)->text().toDouble();
                price += addPrice.toDouble();
                addPrice = QString::number(price, 'f', 2);
                ui->listWidget_3->item(dup)->setText(QString::number(c));
                ui->listWidget_2->item(dup)->setText(addPrice);
                if(c >= 3){
                    ui->listWidget_3->item(dup)->setBackground(Qt::red);
                }
                else{
                    ui->listWidget_3->item(dup)->setBackground(Qt::yellow);
                }


            }
            // otherwise, if no duplicate exists. Notice we must maintain items in a sorted fashion..
            else{
                int row = getRowListWidgetIncreasing(itemId.toInt());
                ui->listWidget_3->insertItem(row, "1");
                ui->listWidget_4->insertItem(row, itemId);
                ui->listWidget->insertItem(row, addName);
                double price = addPrice.toDouble();
                addPrice = QString::number(price, 'f', 2);
                ui->listWidget_2->insertItem(row, addPrice);
                if(itemId.toInt() > 68){
                    // not dim sum
                    ui->listWidget->item(row)->setForeground(Qt::darkGreen);
                    ui->listWidget_2->item(row)->setForeground(Qt::darkGreen);
                    ui->listWidget_3->item(row)->setForeground(Qt::darkGreen);
                    ui->listWidget_4->item(row)->setForeground(Qt::darkGreen);

                }
            }
            on_btnTestTotal_clicked();

            qDebug() << "[Database] Found entry in column \"foodid\" with value of" << num;
        }

        // change choice back to -1
        choice = -1;

    }
    // if the search bar does not have any selection
    else{
        // call for an invalid entry
        ui->labelFood_1->setText("");
        ui->labelFood_4->setText("");
        ui->labelFood_2->setText("Invalid entry!");
        ui->labelFood_3->setText("输入无效");
        qDebug() << "[Database] No entry found in column \"foodid\" with value of" << num;
    }

    //ui->btn_A->setText(" ");
    //ui->btn_B->setText(" ");
    //ui->btn_C->setText(" ");
    clearSearchBar();
    ui->lineEditNum->setFocus();

}

// helper function to check if a duplicate exists in the listwidget..
int MainWindow::checkDuplicateInList(QString name){
    for(int i = 0; i < ui->listWidget->count(); ++i){
        if (ui->listWidget->item(i)->text() == name){
            qDebug() << "[ListWidget] Found duplicate in list.";
            return i;
        }
    }
    qDebug() << "[ListWidget] No duplicates in list.";
    return -1;
}

void MainWindow::on_btnOne_clicked()
{
    btnAdd(1);
}

void MainWindow::on_btnTwo_clicked()
{
    btnAdd(2);
}

void MainWindow::on_btnThree_clicked()
{
    btnAdd(3);
}

void MainWindow::on_btnFour_clicked()
{
    btnAdd(4);
}

void MainWindow::on_btnFive_clicked()
{
    btnAdd(5);
}

void MainWindow::on_btnSix_clicked()
{
    btnAdd(6);
}

void MainWindow::on_btnSeven_clicked()
{
    btnAdd(7);
}

void MainWindow::on_btnEight_clicked()
{
    btnAdd(8);
}

void MainWindow::on_btnNine_clicked()
{
    btnAdd(9);
}

void MainWindow::on_btnZero_clicked()
{
    btnAdd(0);
}

void MainWindow::on_actionAdd_Item_triggered()
{
    addDialog aDialog;
    aDialog.setModal(true);
    aDialog.setData(con);
    aDialog.exec();
}

void MainWindow::keyPressEvent(QKeyEvent* e){
    if(ui->btnGo->isEnabled() && (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)){
        on_btnGo_clicked();
    }
}
/*
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    // pass
}

void MainWindow::on_listWidget_2_itemDoubleClicked(QListWidgetItem *item)
{
    // pass
}
*/


// on listWidget_3 (quantity) clicked
void MainWindow::on_listWidget_3_itemClicked(QListWidgetItem *item)
{   
    // ADD QUANTITY BY ONE
    int row = ui->listWidget_3->row(item);
    QString chineseName = ui->listWidget->item(row)->text().split(" ")[0];
    if(ui->listWidget->item(row)->text().split(" ").size() > 1 && ui->listWidget->item(row)->text().split(" ")[1].left(1) == "["){
        // if it is a square bracket descripted item, it must be used in the search name
        chineseName += " " + ui->listWidget->item(row)->text().split(" ")[1];
    }
    QList<QVariant> x = con.findItemByChName(chineseName); // contains number, foodnameeng, foodnamech, foodprice
    updateDashboard(x[0].toString(), x[1].toString(), chineseName, x[2].toString());
    // if there is more than one price, we need to override it
    // so that the fourth food label shows multiple prices
    if(x[2].toString().split(" ").size() > 1){
        ui->labelFood_4->setText(x[2].toString());
    }
    on_btnGo_clicked();
}



void MainWindow::on_btnTestTotal_clicked()
{
    double subtotal = 0.0;
    double taxpayable = 0.0;
    double total;
    const double tax = 0.13;
    for(int i = 0; i < ui->listWidget_2->count(); ++i){
        subtotal += ui->listWidget_2->item(i)->text().toDouble();
    }

    taxpayable += subtotal * tax;
    QString taxpayablestr = QString::number(taxpayable, 'f', 2);
    taxpayable = taxpayablestr.toDouble();
    total = subtotal + taxpayable;
    ui->label_subtotal->setText("$" + QString::number(subtotal, 'f', 2));
    ui->label_tax->setText("$" + taxpayablestr);
    ui->label_total->setText("$" + QString::number(total, 'f', 2));
}


/*
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    int row = ui->listWidget->row(item);
    QString chineseName = ui->listWidget->item(row)->text().split(" ")[0];
    QList<QVariant> x = con.findItemByChName(chineseName); // contains number, foodnameeng, foodnamech, foodprice
    updateDashboard(x[0].toString(), x[1].toString(), chineseName, x[2].toString());
    // if there is more than one price, we need to override it
    // so that the fourth food label shows multiple prices
    if(x[2].toString().split(" ").size() > 1){
        ui->labelFood_4->setText(x[2].toString());
    }
}
*/


void MainWindow::updateDashboard(QString x1, QString x2, QString x3, QString x4){
    ui->labelFood_1->setText("項目 # " + x1);
    ui->labelFood_2->setText(x2);
    ui->labelFood_3->setText(x3);

    // round the price two decimals because it is not originally 2 decimals
    // in the database.
    double x4d = x4.toDouble();
    x4 = QString::number(x4d, 'f', 2);

    ui->labelFood_4->setText(x4);
}




void MainWindow::on_listWidget_2_itemClicked(QListWidgetItem *item)
{
    // DELETE ALL LOGIC
    int row = ui->listWidget_2->row(item);
    ui->listWidget->takeItem(row);
    ui->listWidget_2->takeItem(row);
    ui->listWidget_3->takeItem(row);
    ui->listWidget_4->takeItem(row);
    on_btnTestTotal_clicked();

}

void MainWindow::on_lineEditNum_textEdited(const QString &arg1)
{
    queryDashboard();
}

void MainWindow::on_btnBackspace_clicked()
{
    QString curr_entry = ui->lineEditNum->text();

    if(curr_entry.size() > 0){
        ui->lineEditNum->setText(curr_entry.left(curr_entry.size() - 1));
        queryDashboard();
    }
}

void MainWindow::on_listWidget_4_itemClicked(QListWidgetItem *item)
{
    // DECREASE QUANTITY BY ONE
    int index = ui->listWidget_4->row(item);
    int count = ui->listWidget_3->item(index)->text().toInt();
    double price = ui->listWidget_2->item(index)->text().toDouble();
    price -= price / count;
    ui->listWidget_3->item(index)->setText(QString::number(count - 1));
    ui->listWidget_2->item(index)->setText(QString::number(price, 'f', 2));
    if(count == 3){
        // turn it into 2
        ui->listWidget_3->item(index)->setBackground(Qt::yellow);
    }
    else if(count == 2){
        // turn it into no colour for 1
        ui->listWidget_3->item(index)->setBackground(Qt::transparent);
    }
    count--;
    if(count <= 0){
        delete ui->listWidget->takeItem(index);
        delete ui->listWidget_2->takeItem(index);
        delete ui->listWidget_3->takeItem(index);
        delete ui->listWidget_4->takeItem(index);
        qDebug() << "Removed row at index " << index;
    }

    on_btnTestTotal_clicked();
}

void MainWindow::on_btnPrint_clicked()
{
    QPrinter printer;
    printer.setPrinterName(printerName);
    printer.setPaperSize(QPrinter::EnvelopeC7);

    QPainter painter;
    QFont font("simsun", 16);
    painter.setFont(font);
    painter.drawText(0, 0, "hello");
    QFontMetrics fm(font);

    PrintFormat x(fm, &printer, &painter);

    if(!painter.begin(&printer)){
        qDebug() << "failed to open file";
    }

    x.printNewline();
    x.changeFont(16);

    for(int i = 0; i < ui->listWidget->count(); ++i){
        QString item;
        if(ui->listWidget_3->item(i)->text().toInt() > 1){
            item = ui->listWidget->item(i)->text() + " x" + ui->listWidget_3->item(i)->text();
        }
        else{
            item = ui->listWidget->item(i)->text();
        }
        x.printLeftText(item);
        if(i != ui->listWidget->count()){
            x.printSpacing(20);
        }
    }

    // THIS IS SO SLOW
    for(int i = 0; i < 10 - ui->listWidget->count(); ++i){
        x.printNewline();
    }

    x.printNewline();
    x.changeFont(5);
    x.printCentreText(".");
    painter.end();
}

void MainWindow::on_btnPrintKitchen_clicked()
{
    QPrinter printer;
    printer.setPrinterName(printerName);
    printer.setPaperSize(QPrinter::EnvelopeC7);

    QPainter painter;
    QFont font("simsun", 16);
    painter.setFont(font);
    painter.drawText(0, 0, "hello");
    QFontMetrics fm(font);

    PrintFormat x(fm, &printer, &painter);

    if(!painter.begin(&printer)){
        qDebug() << "failed to open file";
    }

    x.printNewline();
    x.changeFont(16);
    int i = 0;
    // use 68 because dim sum items go up to 68;
    while(i < ui->listWidget->count() && ui->listWidget_4->item(i)->text().toInt() <= 68){
        QString item;
        if(ui->listWidget_3->item(i)->text().toInt() > 1){
            item = ui->listWidget->item(i)->text() + " x" + ui->listWidget_3->item(i)->text();
        }
        else{
            item = ui->listWidget->item(i)->text();
        }
        x.printLeftText(item);
        if(i != ui->listWidget->count()){
            x.printSpacing(20);
        }
    }

    for(int c = 0; c < 10 - ui->listWidget->count(); ++c){
        x.printNewline();
    }
    x.printNewline();
    x.changeFont(5);
    x.printCentreText(".");

    printer.newPage();

    // if we printed everything..
    if(i == ui->listWidget->count())
        return;


    // otherwise continue printing
    while(i < ui->listWidget->count() && ui->listWidget_4->item(i)->text().toInt() > 68){
        QString item;
        if(ui->listWidget_3->item(i)->text().toInt() > 1){
            item = ui->listWidget->item(i)->text() + " x" + ui->listWidget_3->item(i)->text();
        }
        else{
            item = ui->listWidget->item(i)->text();
        }
        x.printLeftText(item);
        if(i != ui->listWidget->count()){
            x.printSpacing(20);
        }
    }

    for(int c = 0; c < 10 - ui->listWidget->count(); ++c){
        x.printNewline();
    }

    x.printNewline();
    x.changeFont(5);
    x.printCentreText(".");
    painter.end();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    // UPDATES THE DASHBOARD
    QString chineseName = item->text().split(" ")[0];
    if(item->text().split(" ").size() > 1 && item->text().split(" ")[1].left(1) == "["){
        // if it is a square bracket descripted item, it must be used in the search name
        chineseName += " " + item->text().split(" ")[1];
    }
    QList<QVariant> x = con.findItemByChName(chineseName); // contains number, foodnameeng, foodnamech, foodprice
    updateDashboard(x[0].toString(), x[1].toString(), chineseName, x[2].toString());
    // if there is more than one price, we need to override it
    // so that the fourth food label shows multiple prices
    if(x[2].toString().split(" ").size() > 1){
        ui->labelFood_4->setText(x[2].toString());
    }
}

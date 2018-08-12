#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connection.h"
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    const QString printerName = "EPSON TM-T20II Receipt";

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void btnAdd(int);
    void clearSearchBar();
    void on_btnGo_clicked();
    void on_btnOne_clicked();
    void on_btnTwo_clicked();
    void on_btnThree_clicked();
    void on_btnFour_clicked();
    void on_btnFive_clicked();
    void on_btnSix_clicked();
    void on_btnSeven_clicked();
    void on_btnEight_clicked();
    void on_btnNine_clicked();
    void on_btnZero_clicked();
    void on_btnClear_clicked();
    void on_actionAdd_Item_triggered();
    //void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    //void on_listWidget_2_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_3_itemClicked(QListWidgetItem *item);
    void set1();
    void set2();
    void set3();
    void on_btnTestTotal_clicked();
    void on_btnPrint_clicked();
    void updateDashboard(QString, QString, QString, QString);
    void on_listWidget_2_itemClicked(QListWidgetItem *item);
    void queryDashboard();
    void on_lineEditNum_textEdited(const QString &arg1);
    void on_btnBackspace_clicked();
    void on_listWidget_4_itemClicked(QListWidgetItem *item);
    int getRowListWidgetIncreasing(int);
    void on_btnPrintKitchen_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    connection con;
    void keyPressEvent(QKeyEvent*);
    int checkDuplicateInList(QString);
    void dashboardToggle(bool);
    int choice = -1;

};

#endif // MAINWINDOW_H

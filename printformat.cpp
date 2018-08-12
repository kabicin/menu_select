#include "printformat.h"
#include <QFontMetrics>
#include <QPrinter>
#include <QPainter>
#include <QDebug>

PrintFormat::PrintFormat(const QFontMetrics &fm, QPrinter *print, QPainter *paint)
{
    fontMet = new QFontMetrics(fm);
    printer = print;
    painter = paint;
    height = fontMet->height();
}

void PrintFormat::adjCursor(){
    cursorPos += height;
}

void PrintFormat::changeFont(int pt){
    QFont font("simsun", pt);
    QFontMetrics fm(font);
    painter->setFont(font);
    fontMet = new QFontMetrics(fm);
}

int PrintFormat::getWidth(){
    return printer->width() - 25;
}

int PrintFormat::getWidth(const QString &text){
    return fontMet->width(text);
}

void PrintFormat::printLeftText(QString display){
    int x = 0;
    int y = cursorPos;
    painter->drawText(x, y, display);
    adjCursor();
}

void PrintFormat::printRightText(QString display){
    int x = (getWidth() - getWidth(display));
    int y = cursorPos;
    painter->drawText(x, y, display);
    adjCursor();
}

void PrintFormat::printCentreText(QString display){
    int x = (getWidth() - getWidth(display)) / 2;
    int y = cursorPos;
    qDebug() << "Current printing: " << display;
    qDebug() << "Cursor Pos adjusted: " << fontMet->height();
    painter->drawText(x, y, display);
    adjCursor();
}

void PrintFormat::printNewline(){
    adjCursor();
}

void PrintFormat::printLeftRightText(QString display_1, QString display_2){
    int x1 = 0;
    int x2 = getWidth() - getWidth(display_2);
    int y = cursorPos;
    painter->drawText(x1, y, display_1);
    painter->drawText(x2, y, display_2);
    adjCursor();
}

void PrintFormat::printSpacing(int pixels){
    cursorPos += pixels;
}

void PrintFormat::printUnderline(){
    changeFont(6);
    printCentreText("------------------------------------------------------------------------------------");
    printSpacing(5);
    changeFont(8);
}


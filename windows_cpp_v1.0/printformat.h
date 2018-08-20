#ifndef PRINTFORMAT_H
#define PRINTFORMAT_H
#include <QString>
#include <QFontMetrics>
#include <QPrinter>
#include <QPainter>

class PrintFormat
{
    int cursorPos = 0;
    QFontMetrics *fontMet;
    QPrinter *printer;
    QPainter *painter;
    int height;
    int getWidth();
    int getWidth(const QString &);
    void adjCursor();
public:
    PrintFormat(const QFontMetrics &, QPrinter *, QPainter *);
    void printCentreText(QString);
    void printLeftText(QString);
    void printRightText(QString);
    void printLeftRightText(QString, QString);
    void printNewline();
    void setPointSize(int);
    void changeFont(int);
    void printSpacing(int);
    void printUnderline();
    void reset();
    void printLines(int);
};

#endif // PRINTFORMAT_H

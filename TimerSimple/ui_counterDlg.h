/********************************************************************************
** Form generated from reading UI file 'counterDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COUNTERDLG_H
#define UI_COUNTERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Counter
{
public:
    QPushButton *button;
    QLCDNumber *lcdNumber;
    QPushButton *showTimeButton;
    QLCDNumber *lcdNumberPeriod;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QSlider *horizontalSlider;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *Counter)
    {
        if (Counter->objectName().isEmpty())
            Counter->setObjectName(QString::fromUtf8("Counter"));
        Counter->resize(508, 521);
        button = new QPushButton(Counter);
        button->setObjectName(QString::fromUtf8("button"));
        button->setGeometry(QRect(130, 340, 251, 71));
        lcdNumber = new QLCDNumber(Counter);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(30, 40, 441, 91));
        showTimeButton = new QPushButton(Counter);
        showTimeButton->setObjectName(QString::fromUtf8("showTimeButton"));
        showTimeButton->setGeometry(QRect(130, 430, 251, 71));
        lcdNumberPeriod = new QLCDNumber(Counter);
        lcdNumberPeriod->setObjectName(QString::fromUtf8("lcdNumberPeriod"));
        lcdNumberPeriod->setGeometry(QRect(180, 170, 141, 61));
        lineEdit = new QLineEdit(Counter);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(180, 140, 141, 31));
        lineEdit_2 = new QLineEdit(Counter);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(210, 10, 91, 31));
        horizontalSlider = new QSlider(Counter);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(50, 250, 401, 41));
        horizontalSlider->setMaximum(2000);
        horizontalSlider->setValue(1000);
        horizontalSlider->setOrientation(Qt::Horizontal);
        plainTextEdit = new QPlainTextEdit(Counter);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(70, 290, 361, 41));

        retranslateUi(Counter);

        QMetaObject::connectSlotsByName(Counter);
    } // setupUi

    void retranslateUi(QWidget *Counter)
    {
        Counter->setWindowTitle(QApplication::translate("Counter", "Counter", nullptr));
        button->setText(QApplication::translate("Counter", "STOP / START", nullptr));
        showTimeButton->setText(QApplication::translate("Counter", "SHOW REAL TIME", nullptr));
        lineEdit->setText(QApplication::translate("Counter", "Millisecond per tick", nullptr));
        lineEdit_2->setText(QApplication::translate("Counter", "TIME CLOCK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Counter: public Ui_Counter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COUNTERDLG_H

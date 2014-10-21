/********************************************************************************
** Form generated from reading UI file 'monitorclient.ui'
**
** Created: Tue Sep 23 19:11:39 2014
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MONITORCLIENT_H
#define UI_MONITORCLIENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_monitorclient
{
public:
    QWidget *centralWidget;
    QLineEdit *lineEdit;
    QLineEdit *username;
    QPushButton *LandingButton;
    QLineEdit *lineEdit_2;
    QPushButton *RegisterButton;
    QLineEdit *userpasswd;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *monitorclient)
    {
        if (monitorclient->objectName().isEmpty())
            monitorclient->setObjectName(QString::fromUtf8("monitorclient"));
        monitorclient->resize(607, 402);
        centralWidget = new QWidget(monitorclient);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(130, 100, 71, 31));
        username = new QLineEdit(centralWidget);
        username->setObjectName(QString::fromUtf8("username"));
        username->setGeometry(QRect(200, 100, 231, 31));
        LandingButton = new QPushButton(centralWidget);
        LandingButton->setObjectName(QString::fromUtf8("LandingButton"));
        LandingButton->setGeometry(QRect(210, 200, 101, 27));
        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(130, 140, 71, 31));
        RegisterButton = new QPushButton(centralWidget);
        RegisterButton->setObjectName(QString::fromUtf8("RegisterButton"));
        RegisterButton->setGeometry(QRect(330, 200, 101, 27));
        userpasswd = new QLineEdit(centralWidget);
        userpasswd->setObjectName(QString::fromUtf8("userpasswd"));
        userpasswd->setGeometry(QRect(200, 140, 231, 31));
        monitorclient->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(monitorclient);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 607, 25));
        monitorclient->setMenuBar(menuBar);
        mainToolBar = new QToolBar(monitorclient);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        monitorclient->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(monitorclient);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        monitorclient->setStatusBar(statusBar);

        retranslateUi(monitorclient);

        QMetaObject::connectSlotsByName(monitorclient);
    } // setupUi

    void retranslateUi(QMainWindow *monitorclient)
    {
        monitorclient->setWindowTitle(QApplication::translate("monitorclient", "monitorclient", 0, QApplication::UnicodeUTF8));
        lineEdit->setText(QApplication::translate("monitorclient", "\347\231\273\345\275\225\350\264\246\346\210\267", 0, QApplication::UnicodeUTF8));
        LandingButton->setText(QApplication::translate("monitorclient", "\347\231\273\345\275\225", 0, QApplication::UnicodeUTF8));
        lineEdit_2->setText(QApplication::translate("monitorclient", "\347\231\273\345\275\225\345\257\206\347\240\201", 0, QApplication::UnicodeUTF8));
        RegisterButton->setText(QApplication::translate("monitorclient", "\346\263\250\345\206\214", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class monitorclient: public Ui_monitorclient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MONITORCLIENT_H

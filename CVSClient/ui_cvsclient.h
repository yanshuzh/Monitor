/********************************************************************************
** Form generated from reading UI file 'cvsclient.ui'
**
** Created: Mon Sep 8 05:10:39 2014
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CVSCLIENT_H
#define UI_CVSCLIENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CvsClient
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CvsClient)
    {
        if (CvsClient->objectName().isEmpty())
            CvsClient->setObjectName(QString::fromUtf8("CvsClient"));
        CvsClient->resize(871, 563);
        centralWidget = new QWidget(CvsClient);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 10, 640, 480));
        CvsClient->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CvsClient);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 871, 25));
        CvsClient->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CvsClient);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        CvsClient->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CvsClient);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        CvsClient->setStatusBar(statusBar);

        retranslateUi(CvsClient);

        QMetaObject::connectSlotsByName(CvsClient);
    } // setupUi

    void retranslateUi(QMainWindow *CvsClient)
    {
        CvsClient->setWindowTitle(QApplication::translate("CvsClient", "CvsClient", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CvsClient", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CvsClient: public Ui_CvsClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CVSCLIENT_H

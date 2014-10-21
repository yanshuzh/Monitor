#ifndef MONITORCLIENT_H
#define MONITORCLIENT_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>
#include <QDebug>
#include "myrecvthread.h"
namespace Ui {
class monitorclient;
}
struct MessageData{
    char flag;
    char username[10];
    char userpasswd[20];
    int userport;
};
class monitorclient : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit monitorclient(QWidget *parent = 0);
    ~monitorclient();
    void conncet2server();
    void sendMessage(QString str);
private slots:
    void landing();
    void new_register();
    void displayError(QAbstractSocket::SocketError);
    void readMessage();
    
private:
    Ui::monitorclient *ui;
    QTcpSocket *TcpClientSocket;
    myrecvthread *recvthread;
};

#endif // MONITORCLIENT_H

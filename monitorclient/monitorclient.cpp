#include "monitorclient.h"
#include "ui_monitorclient.h"


#define H264 96
#define MY_VIDEO_WIDTH 352
#define MY_VIDEO_HEIGHT 288

monitorclient::monitorclient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::monitorclient)
{
    ui->setupUi(this);
    TcpClientSocket =new QTcpSocket(this);
    connect(TcpClientSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(TcpClientSocket,SIGNAL(error(QAbstractSocket::SocketError)),
             this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(ui->LandingButton,SIGNAL(clicked()),this,SLOT(landing()));
    connect(ui->RegisterButton,SIGNAL(clicked()),this,SLOT(new_register()));
}

monitorclient::~monitorclient()
{

   TcpClientSocket->close();
    delete ui;
}

void monitorclient::landing()
{
    conncet2server();
    MessageData message;
    strcpy(message.username,ui->username->text().toLatin1().data());
    strcpy(message.userpasswd,ui->userpasswd->text().toLatin1().data());
    message.userport=2312;
    message.flag=1;
//    message.username=ui->username->text();
//    message.userpasswd=ui->userpasswd->text();
    qDebug()<<message.username;
    qDebug()<<message.userpasswd;
    TcpClientSocket->write((char *)&message,sizeof(message));
    //sendMessage((char *)&message);
    qDebug()<<"landing successfully!!";
    recvthread = new myrecvthread();
    recvthread->start();

}
//注册新用户
void monitorclient::new_register()
{
    conncet2server();
    MessageData message;
    message.flag=2;
    strcpy(message.username,ui->username->text().toLatin1().data());
    strcpy(message.userpasswd,ui->userpasswd->text().toLatin1().data());
}

void monitorclient::conncet2server()
{
    TcpClientSocket->abort();
    TcpClientSocket->connectToHost("127.0.0.1",7766);
}
void monitorclient::displayError(QAbstractSocket::SocketError)
{
    qDebug() << TcpClientSocket->errorString(); //输出错误信息
}
void monitorclient::readMessage()
{

}
void monitorclient::sendMessage(QString str)
{
    QByteArray byte;
    byte=str.toAscii();
    TcpClientSocket->write(byte);
}



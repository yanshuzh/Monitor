#include "myrecvthread.h"
myrecvthread::myrecvthread(QObject *parent) :
    QThread(parent)
{


}
void myrecvthread::run()
{
    myrtprecv sess;
    RTPUDPv4TransmissionParams transparams;
    RTPSessionParams sessparams;
    sessparams.SetOwnTimestampUnit(1.0/90000.0);

    transparams.SetPortbase(2312);
    int status = sess.Create(sessparams,&transparams);
    sess.checkerror(status);
    qDebug()<<"begin";
    RTPTime::Wait(RTPTime(800000,0));
    qDebug()<<"yes";
    sess.BYEDestroy(RTPTime(10,0),0,0);
}

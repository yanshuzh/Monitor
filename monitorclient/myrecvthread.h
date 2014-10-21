#ifndef MYRECVTHREAD_H
#define MYRECVTHREAD_H

#include <QThread>
#include "myrtprecv.h"
#include <QDebug>
class myrecvthread : public QThread
{
    Q_OBJECT
public:
    explicit myrecvthread(QObject *parent = 0);
    void run();

    
signals:
    
public slots:
    
};

#endif // MYRECVTHREAD_H

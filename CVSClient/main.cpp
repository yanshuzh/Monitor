#include <QApplication>
#include "cvsclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CvsClient w;
    w.show();
    
    return a.exec();
}

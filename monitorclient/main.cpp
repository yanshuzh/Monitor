#include <QApplication>
#include "monitorclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    monitorclient w;
    w.show();
    
    return a.exec();
}

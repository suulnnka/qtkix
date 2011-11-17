#include <QtGui/QApplication>
#include "qtkix.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qtkix w;
    w.show();

    return a.exec();
}

#include <QCoreApplication>
#include "EWTheme.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    EWTheme* t = new EWTheme("D:/Lydia/c++/EW/data1.csv","D:/Lydia/c++/EW/outdata2.csv");

    t->RunModel();

    delete t;
    t = NULL;

    return a.exec();
}

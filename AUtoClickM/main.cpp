#include "AUtoClickM.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AUtoClickM w;
    w.show();
    return a.exec();
}

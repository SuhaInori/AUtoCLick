#include "AutoClick.h"
#include <QtWidgets/QApplication>
//#include "GlobaQApplication.h"
//#if _MSC_VER >= 1600
//#pragma execution_character_set("utf-8")
//#endif

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    AutoClick w;
    w.setWindowFlags(w.windowFlags() & ~
        Qt::WindowMaximizeButtonHint);
    w.show();
    return a.exec();
}

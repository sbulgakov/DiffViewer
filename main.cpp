#include "diffview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    diffview w;
    w.show();
    
    return a.exec();
}

#include "plotter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    plotter w;
    w.show();

    return a.exec();
}

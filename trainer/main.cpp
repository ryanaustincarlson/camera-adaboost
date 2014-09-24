#include <QtGui/QApplication>
#include "trainergui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrainerGui w;
    w.show();
    return a.exec();
}

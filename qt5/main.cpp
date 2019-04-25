#include "frameimage.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    frameimage w;
    w.show();

    return a.exec();
}

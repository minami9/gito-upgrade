#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if (argc != 2)
        return -1;

    w.setFileSize(QString::fromUtf8(argv[2]).toULongLong());
    w.show();
    w.doUpgrade(QUrl(argv[1]));
    return a.exec();
}

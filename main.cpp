#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(900, 600);
    w.setWindowTitle("Task 3 - Encryptor/Decryptor");
    w.show();
    return a.exec();
}

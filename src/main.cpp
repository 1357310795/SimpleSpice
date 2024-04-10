#include "ui/mymainwindow.h"
#include <QApplication>

int main(int argc, char **args)
{
    QApplication app(argc, args);

    MyMainWindow *mainwindow = new MyMainWindow;
    mainwindow->show();

    return app.exec();
}

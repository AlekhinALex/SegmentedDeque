#include <QApplication>
#include "views/mainwindow.h"

int main(int argc, char *argv[])
{
    QLocale::setDefault(QLocale::C);
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}

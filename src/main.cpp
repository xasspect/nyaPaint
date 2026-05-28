#include <QApplication>
#include "app/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("Nya Paint");

    MainWindow window;
    window.show();

    return app.exec();
}

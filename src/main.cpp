#include <QApplication>
#include "app/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("Nya Paint");
    app.setApplicationVersion("1.0.0");

    app.setWindowIcon(QIcon(":/icon.ico"));

    MainWindow window;
    window.show();

    return app.exec();
}

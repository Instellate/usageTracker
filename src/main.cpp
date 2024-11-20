#include <QApplication>
#include <QtQuick>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QQmlEngine e;
    QQmlComponent component(&e);
    component.loadUrl(QUrl("qrc:/qt/qml/tracker/main.qml"));
    component.create();

    return QApplication::exec();
}

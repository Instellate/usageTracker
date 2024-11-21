#include <QApplication>
#include <QtQuick>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QSettings settings("instellate", "usageTracker");
    int32_t refreshRate = settings.value("processes/refreshRate", 500).toInt();

    QQmlEngine e;
    QQmlComponent component(&e);
    component.loadUrl(QUrl("qrc:/qt/qml/UsageTracker/main.qml"));
    component.createWithInitialProperties({
        {"refreshRate", refreshRate}
    });

    return QApplication::exec();
}

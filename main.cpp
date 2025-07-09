#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "src/GameLogic.h"
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Fusion"); // Устанавливаем стиль Fusion

    QQmlApplicationEngine engine;
    qmlRegisterType<GameLogic>("Game", 1, 0, "GameLogic");
    engine.addImportPath("qrc:/qml");
    qputenv("QML_IMPORT_TRACE", "1");

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "src/networklogic.h"
#include "src/GameLogic.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Fusion"); // Устанавливаем стиль Fusion

    QQmlApplicationEngine engine;
    qmlRegisterType<NetworkLogic>("Game", 1, 0, "NetworkLogic");
    qmlRegisterType<GameLogic>("Game", 1, 0, "GameLogic");
    engine.addImportPath("qrc:/qml");
    qputenv("QML_IMPORT_TRACE", "1");

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            qCritical() << "Ошибка: Не удалось загрузить" << url;
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    QObject::connect(&engine, &QQmlApplicationEngine::warnings,
                     &app, [](const QList<QQmlError> &warnings) {
        for (const QQmlError &warning : warnings) {
            qWarning() << "QML предупреждение:" << warning.toString();
        }
    });

    engine.load(url);
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Ошибка: Не найдены корневые объекты QML";
        return -1;
    }

    return app.exec();
}
QT += quick qml gui multimedia

CONFIG += c++17
CONFIG += qmltypes
CONFIG += resources_big

SOURCES += \
    main.cpp \
    src/GameLogic.cpp

HEADERS += \
    src/GameLogic.h

RESOURCES += \
    resources.qrc

TARGET = buckshotroulette
TEMPLATE = app

QML_IMPORT_PATH = qml
QML_DESIGNER_IMPORT_PATH = qml
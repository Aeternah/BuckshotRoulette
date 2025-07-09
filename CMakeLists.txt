cmake_minimum_required(VERSION 3.16)

project(BuckshotRoulette VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Находим необходимые модули Qt6
find_package(Qt6 REQUIRED COMPONENTS 
    Quick 
    QuickControls2 
    QuickLayouts
    Core
    Multimedia  # Добавляем Multimedia для SoundEffect
)

set(SOURCES
    main.cpp
    src/GameLogic.cpp
)

set(HEADERS
    src/GameLogic.h
)

qt_add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

# Ресурсы QML
qt_add_resources(${PROJECT_NAME} "qml"
    PREFIX "/qml"
    BASE ${CMAKE_CURRENT_SOURCE_DIR}/qml
    FILES
        qml/main.qml
        qml/Menu.qml
        qml/Game.qml
        qml/Shotgun.qml
)





# Ресурсы изображений
qt_add_resources(${PROJECT_NAME} "images"
    PREFIX "/images"
    BASE ${CMAKE_CURRENT_SOURCE_DIR}/images
    FILES
        images/background.png
        images/menu_bg.png
        images/game_bg.png
        images/enemy.png
        images/player.jpg
        images/shotgun.png
        images/bullet1.png
        images/bullet2.png
        images/heart1.png
        images/heart2.png
)

# Ресурсы звуков
qt_add_resources(${PROJECT_NAME} "sounds"
    PREFIX "/sounds"
    BASE ${CMAKE_CURRENT_SOURCE_DIR}/sounds
    FILES
        sounds/shot.wav
        sounds/miss.wav
        sounds/prepare_for_shot.wav
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::QuickLayouts
    Qt6::Core
    Qt6::Multimedia  # Добавляем Multimedia
)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Отладочные сообщения
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message("Fonts base: ${CMAKE_CURRENT_SOURCE_DIR}/fonts")
message("Checking file: ${CMAKE_CURRENT_SOURCE_DIR}/fonts/Minecraftia.ttf")
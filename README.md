# Buckshot Roulette - Установка и запуск

## Описание
Buckshot Roulette - это игра в стиле русской рулетки с элементами стратегии, реализованная с использованием Qt6 и QML.

## Системные требования
- Windows 10/11 или Ubuntu 20.04/22.04
- Qt6 (минимум 6.2)
- Компилятор C++ с поддержкой C++17

## Установка на Windows

### 1. Установка зависимостей
1. Установите [Qt6](https://www.qt.io/download) с компонентами:
   - Qt Quick
   - Qt Quick Controls 2
   - Qt Multimedia
   - Qt Network

2. Установите [CMake](https://cmake.org/download/) версии 3.16 или выше

3. Установите компилятор (например, MinGW или MSVC через Visual Studio)

### 2. Сборка проекта
```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:\Qt\6.5.0\mingw_64" ..
cmake --build .
```

### 3. Запуск
После сборки исполняемый файл будет находиться в папке `build` или `build/Debug`.

## Установка на Ubuntu

### 1. Установка зависимостей
```bash
sudo apt update
sudo apt install -y build-essential cmake qt6-base qt6-declarative-dev qt6-quickcontrols2-6 qt6-multimedia-dev libx11-dev
```

### 2. Сборка проекта
```bash
mkdir build
cd build
cmake ..
make
```

### 3. Запуск
```bash
./BuckshotRoulette
```

## Управление в игре
- Используйте мышь для взаимодействия с интерфейсом

## Проблемы и решения
Если возникают проблемы с ресурсами (изображения, звуки):
- Убедитесь, что файлы ресурсов находятся в правильных папках
- Проверьте пути в файле `CMakeLists.txt`

Для проблем с аудио на Linux:
```bash
sudo apt install libqt6multimedia6-plugins
```


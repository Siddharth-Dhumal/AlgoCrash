QT       += core gui widgets

CONFIG   += c++17

TARGET = AlgoCrash
TEMPLATE = app

# point to your sources
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

# point to your headers
HEADERS += \
    include/mainwindow.h

# point to the .ui in the ui/ folder
FORMS += \
    ui/mainwindow.ui

INCLUDEPATH += include

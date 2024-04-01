TEMPLATE = app
TARGET = test
INCLUDEPATH += $$PWD/src

QT += widgets printsupport
QT += core gui qml svg charts

CONFIG += debug
CONFIG += c++2a
#CONFIG += release

CONFIG(debug, release|debug) {
    message("Compiling in debug mode.")
}

CONFIG(release, release|debug) {
    message("Compiling in release mode.")
}

macx{
    CONFIG -= sdk_no_version_check
    CONFIG -= app_bundle
    message("Compiling on macOS.")
}

MOC_DIR     = build
OBJECTS_DIR = build
RCC_DIR     = build
UI_DIR      = build

HEADERS += src/mainwindow.h \
    src/SpiceHighlighter.hpp \
    src/calc/stamp.hpp \
    src/chart/mychart.h \
    src/circuit/circuit.hpp \
    src/circuit/circuit_node.h \
    src/cmds/ac_dc.hpp \
    src/cmds/option.hpp \
    src/cmds/pulse.hpp \
    src/devices/all_devices.hpp \
    src/devices/base_device.hpp \
    src/devices/source_device.hpp \
    src/global.hpp \
    src/myconsole.h \
    src/mymainwindow.h \
    src/parser/location.hh \
    src/parser/parser.hpp \
    src/parser/position.hh \
    src/parser/scanner.hpp \
    src/parser/sp.l \
    src/parser/sp.y \
    src/parser/stack.hh \
    src/qss/QmlStyleUrlInterceptor.h \
    src/qss/QtAdvancedStylesheet.h \
    src/qss/acss_globals.h

SOURCES += src/main.cpp \
           src/chart/mychart.cpp \
           src/mainwindow.cpp \
           src/myconsole.cpp \
           src/mymainwindow.cpp \
           src/parser/parser.cpp \
           src/parser/scanner.cpp \
           src/qss/QmlStyleUrlInterceptor.cpp \
           src/qss/QtAdvancedStylesheet.cpp

RESOURCES += src/mainwindow.qrc

FORMS += \
    mainwindow2.ui \
    mychart.ui

INCLUDEPATH += /usr/include/eigen3

DEFINES += "STYLES_DIR=$$PWD/styles"

DISTFILES += \
    src/parser/parser.output

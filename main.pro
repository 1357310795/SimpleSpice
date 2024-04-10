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

HEADERS += \
    src/calc/analyze_context.h \
    src/calc/analyze_entry.h \
    src/calc/stamp.hpp \
    src/chart/mychart.h \
    src/chart/plot_context.h \
    src/circuit/circuit.h \
    src/circuit/circuit_node.h \
    src/cmds/ac_dc.hpp \
    src/cmds/option.hpp \
    src/cmds/pulse.hpp \
    src/console/myconsole.h \
    src/devices/item/capacitor.h \
    src/devices/item/cccs.h \
    src/devices/item/ccvs.h \
    src/devices/item/diode.h \
    src/devices/item/inductor.h \
    src/devices/item/isource.h \
    src/devices/item/resistor.h \
    src/devices/item/vccs.h \
    src/devices/item/vcvs.h \
    src/devices/item/vsource.h \
    src/devices/nonlinear_device.h \
    src/devices/base_device.h \
    src/devices/dynamic_device.h \
    src/devices/source_device.h \
    src/global/global.h \
    src/ui/mymainwindow.h \
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
    src/utils/SpiceHighlighter.hpp

SOURCES += src/main.cpp \
           src/calc/analyze_context.cpp \
           src/chart/mychart.cpp \
           src/chart/plot_context.cpp \
           src/console/myconsole.cpp \
           src/devices/item/capacitor.cpp \
           src/devices/item/cccs.cpp \
           src/devices/item/ccvs.cpp \
           src/devices/item/diode.cpp \
           src/devices/item/inductor.cpp \
           src/devices/item/isource.cpp \
           src/devices/item/resistor.cpp \
           src/devices/item/vccs.cpp \
           src/devices/item/vcvs.cpp \
           src/devices/item/vsource.cpp \
           src/devices/nonlinear_device.cpp \
           src/devices/base_device.cpp \
           src/devices/dynamic_device.cpp \
           src/devices/source_device.cpp \
           src/global/global.cpp \
           src/ui/mymainwindow.cpp \
           src/parser/parser.cpp \
           src/parser/scanner.cpp \
           src/qss/QmlStyleUrlInterceptor.cpp \
           src/qss/QtAdvancedStylesheet.cpp

RESOURCES += src/ui/mainwindow.qrc \
    src/ui/mainwindow.qrc

FORMS += \
    mainwindow2.ui \
    mychart.ui

INCLUDEPATH += /usr/include/eigen3

DEFINES += "STYLES_DIR=$$PWD/styles"

DISTFILES += \
    src/parser/parser.output

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    ../common/my_net_stack.cpp

mac {
    PKG_CONFIG = /usr/local/bin/pkg-config
}

INCLUDEPATH += /usr/local/include

CONFIG += link_pkgconfig
PKGCONFIG += \
    libparsley \
    parsly

HEADERS += \
    ../common/my_net_stack.h



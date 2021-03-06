QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    celipsa.cpp \
    ckrug.cpp \
    clinija.cpp \
    cpoligon.cpp \
    ctacka.cpp \
    cxosa.cpp \
    cyosa.cpp \
    geom_graphics_view.cpp \
    main.cpp \
    mainwindow.cpp \
    "../Cons/afin.cpp" \
    "../Cons/geom.cpp" \
    "../Cons/oblik.cpp" \
    "../Cons/tacka.cpp"

HEADERS += \
    Boje.hpp \
    crtanje.hpp \
    geom_graphics_view.hpp \
    mainwindow.h \
    "../Cons/afin.hpp" \
    "../Cons/alijasi.hpp" \
    "../Cons/geom.hpp" \
    "../Cons/oblik.hpp" \
    "../Cons/tacka.hpp" \
    "../Cons/util.hpp"

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

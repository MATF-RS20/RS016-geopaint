TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        afin_test.cpp \
        geom_test.cpp \
        main.cpp \
        util_test.cpp \
        "../Main/afin.cpp" \
        "../Main/geom.cpp" \
        "../Main/tacka.cpp"

HEADERS += \
        catch.hpp

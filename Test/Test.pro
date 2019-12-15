TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        geom_test.cpp \
        main.cpp \
        util_test.cpp \
        "../Main/geom.cpp" \
        "../Main/tacka.cpp"

HEADERS += \
        catch.hpp

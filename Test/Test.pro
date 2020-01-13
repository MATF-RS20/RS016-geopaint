TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        afin_test.cpp \
        geom_test.cpp \
        main.cpp \
        oblik_test.cpp \
        tacka_test.cpp \
        util_test.cpp \
        "../Cons/afin.cpp" \
        "../Cons/geom.cpp" \
        "../Cons/oblik.cpp" \
        "../Cons/tacka.cpp"

HEADERS += \
        catch.hpp \
        "../Cons/geom.hpp"

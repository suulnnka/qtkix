#-------------------------------------------------
#
# Project created by QtCreator 2011-10-10T12:12:57
#
#-------------------------------------------------

QT       += core gui

TARGET = qtkix
TEMPLATE = app
QMAKE_CFLAGS_RELEASE = -O3


SOURCES += main.cpp\
        qtkix.cpp \
    gui_board.cpp \
    othellov3.c \
    kix/eval.c \
    kix/bit.c \
    kix/search.c \
    kix/midsearch.c \
    kix/endsearch.c \
    kix/flip.c

HEADERS  += qtkix.h \
    gui_board.h \
    ai_data.h \
    kix/eval.h \
    kix/endsearch.h \
    kix/midsearch.h \
    kix/search.h \
    kix/bit.h \
    kix/base.h \
    kix/flip.h

FORMS    += qtkix.ui















































































































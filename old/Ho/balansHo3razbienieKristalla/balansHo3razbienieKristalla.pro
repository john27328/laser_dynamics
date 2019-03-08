#-------------------------------------------------
#
# Project created by QtCreator 2017-01-30T17:27:41
#
#-------------------------------------------------

QT       += core gui
 QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = balansHo3razbienieKristalla
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        widget.cpp \
    constants.cpp \
    balansrk.cpp \
    grafsouce.cpp \
    grafwidget.cpp \
    const.cpp \
    fyk.cpp \
    p_gen.cpp

HEADERS  += widget.h \
    constants.h \
    balansrk.h \
    grafsource.h \
    grafwidget.h

FORMS    += widget.ui

DISTFILES +=

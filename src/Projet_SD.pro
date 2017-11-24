#-------------------------------------------------
#
# Project created by QtCreator 2015-05-07T11:55:04
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projet_SD
TEMPLATE = app

SOURCES += main.cpp \
        mainwindow.cpp \
    glwidget.cpp \
    vector.cpp \
    individu.cpp \
    village.cpp \
    xdr_adn_connection.cpp \
    rpc_client.cpp \
    infoconsole.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    vector.h \
    individu.h \
    village.h \
    xdr_adn_connection.h \
    rpc_client.h \
    infoconsole.h \
    ui_infoconsole.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/include/tirpc

LIBS += -lglut -ltirpc -lpthread

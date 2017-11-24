TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt
QT += opengl

SOURCES += main_client.cpp \
    glwidget.cpp \
    vector.cpp \
    individu.cpp \
    village.cpp \
    xdr_adn_connection.cpp \
    rpc_client.cpp \

HEADERS  += glwidget.h \
    vector.h \
    individu.h \
    village.h \
    xdr_adn_connection.h \
    rpc_client.h

include(deployment.pri)
qtcAddDeployment()

INCLUDEPATH += /usr/include/tirpc

LIBS += -lglut -ltirpc

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    largestnumbersmodulo.cpp \
    main.cpp \
    mainwindow.cpp \
    calculationproducer.cpp \
    filehandler.cpp \
    renderer.cpp \
    saprelement.cpp \
    workspace.cpp

HEADERS += \
    largestnumbersmodulo.h \
    mainwindow.h \
    calculationproducer.h \
    filehandler.h \
    renderer.h \
    saprelement.h \
    workspace.h

FORMS += \
    largestnumbersmodulo.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    TODO

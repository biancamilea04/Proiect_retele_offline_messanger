QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -lsqlite3

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addtogroup.cpp \
    creategroup.cpp \
    interfata.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    $$files(./src/*.cpp) \
    $$files(./src/comunic/*.cpp) \
    $$files(./src/functii/*.cpp) \
    registerdialog.cpp \
    usersgroups.cpp

HEADERS += \
    addtogroup.h \
    creategroup.h \
    interfata.h \
    logindialog.h \
    mainwindow.h \
    ./include/*.h \
    ./include/comunic/*.h \
    ./include/functii/*.h \
    registerdialog.h \
    usersgroups.h

INCLUDEPATH += \
    ./include \
    ./include/comunic \
    ./include/functii

LIBS += -lsqlite3



FORMS += \
    addtogroup.ui \
    creategroup.ui \
    interfata.ui \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui \
    usersgroups.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

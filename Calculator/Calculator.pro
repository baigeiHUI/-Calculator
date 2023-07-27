QT       += core gui
RC_ICONS =title.ico
QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
QMAKE_CXXFLAGS_STL_ON = /EHa

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG +=resources_big

CONFIG += exception

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    history_recording.cpp \
    main.cpp \
    mainwindow.cpp \
    sub_window.cpp

HEADERS += \
    history_recording.h \
    mainwindow.h \
    sub_window.h

FORMS += \
    history_recording.ui \
    mainwindow.ui \
    sub_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

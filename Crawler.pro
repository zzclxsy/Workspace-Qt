QT       += core gui network printsupport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
TARGET_DIR=$$PWD/output

CONFIG(debug, debug|release){
    DESTDIR = $$TARGET_DIR/debug
}else{
    DESTDIR = $$TARGET_DIR/release
}
MOC_DIR = $$TARGET_DIR/moc/
RCC_DIR = $$TARGET_DIR/rcc/
UI_DIR = $$TARGET_DIR/ui/
OBJECTS_DIR = $$TARGET_DIR/obj/

SOURCES += \
    HttpLib/http.cpp \
    QCustomPlot/qcustomplot.cpp \
    control.cpp \
    crawlerthread.cpp \
    custombars.cpp \
    dataacquire.cpp \
    database.cpp \
    main.cpp \
    mycustomplot.cpp \
    openimage.cpp \
    showpage.cpp

HEADERS += \
    HttpLib/http.h \
    QCustomPlot/qcustomplot.h \
    control.h \
    crawlerthread.h \
    custombars.h \
    dataacquire.h \
    database.h \
    mycustomplot.h \
    openimage.h \
    showpage.h

FORMS += \
    showpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

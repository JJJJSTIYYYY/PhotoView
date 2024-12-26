QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = title.ico

SOURCES += \
    aidescrible.cpp \
    imagedownloader.cpp \
    imageprocessingthread.cpp \
    main.cpp \
    openaiAPI.cpp \
    pic_view.cpp \
    setting.cpp \
    ui_beautify/labelbutton.cpp \
    ui_beautify/menu.cpp \
    ui_beautify/popwid.cpp \
    widget.cpp

HEADERS += \
    ScreenshotWidget.h \
    aidescrible.h \
    imagedownloader.h \
    imageprocessingthread.h \
    openaiAPI.h \
    pic_view.h \
    setting.h \
    ui_beautify/labelbutton.h \
    ui_beautify/menu.h \
    ui_beautify/popwid.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    1.qrc

QT       += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_UIC += -utf8

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = title.ico

SOURCES += \
    aidescrible.cpp \
    gridboard.cpp \
    imagedownloader.cpp \
    imageprocessingthread.cpp \
    main.cpp \
    morefunc.cpp \
    openaiAPI.cpp \
    pic_view.cpp \
    setting.cpp \
    table.cpp \
    ui_beautify/ctrlbar.cpp \
    ui_beautify/guidebtn.cpp \
    ui_beautify/labelbutton.cpp \
    ui_beautify/menu.cpp \
    ui_beautify/pointbutton.cpp \
    ui_beautify/popwid.cpp \
    ui_beautify/zoomlabel.cpp \
    widget.cpp

HEADERS += \
    ScreenshotWidget.h \
    aidescrible.h \
    gridboard.h \
    imagedownloader.h \
    imageprocessingthread.h \
    morefunc.h \
    openaiAPI.h \
    pic_view.h \
    setting.h \
    table.h \
    ui_beautify/ctrlbar.h \
    ui_beautify/guidebtn.h \
    ui_beautify/labelbutton.h \
    ui_beautify/menu.h \
    ui_beautify/pointbutton.h \
    ui_beautify/popwid.h \
    ui_beautify/zoomlabel.h \
    widget.h

FORMS += \
    gridboard.ui \
    table.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    1.qrc

INCLUDEPATH += $$PWD

win32: {
    QT += axcontainer
#    LIBS += -L$$PWD/lib/ -licom
#    CONFIG += qaxcontainer
} else: {
}


DEPENDPATH += $$PWD


HEADERS += \
    $$PWD/qexcel.h \
    $$PWD/excel_savethread.h

SOURCES += \
    $$PWD/qexcel.cpp \
    $$PWD/excel_savethread.cpp


INCLUDEPATH += $$PWD

QT       += serialport

HEADERS +=  \
    $$PWD/serialstatuswid.h \
    $$PWD/serialport.h \
    $$PWD/serialportwid.h \
    $$PWD/serialportdlg.h
    

SOURCES +=   \
    $$PWD/serialstatuswid.cpp \
    $$PWD/serialport.cpp \
    $$PWD/serialportwid.cpp \
    $$PWD/serialportdlg.cpp


FORMS += \
    $$PWD/serialportwid.ui \
    $$PWD/serialportdlg.ui \
    $$PWD/serialstatuswid.ui

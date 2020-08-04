
INCLUDEPATH += $$PWD

include(users/users.pri)

HEADERS +=  \
    $$PWD/setup_mainwid.h \
    $$PWD/versiondlg.h
SOURCES +=  \
    $$PWD/setup_mainwid.cpp \
    $$PWD/versiondlg.cpp

FORMS +=  \
    $$PWD/setup_mainwid.ui \
    $$PWD/versiondlg.ui






INCLUDEPATH += $$PWD

include(users/users.pri)

HEADERS +=  \
    $$PWD/setup_mainwid.h \
    $$PWD/readmedlg.h
	
SOURCES +=  \
    $$PWD/setup_mainwid.cpp \
    $$PWD/readmedlg.cpp

FORMS +=  \
    $$PWD/setup_mainwid.ui \
    $$PWD/readmedlg.ui





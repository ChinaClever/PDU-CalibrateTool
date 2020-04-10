
INCLUDEPATH += $$PWD

include(cfgcom/cfgcom.pri)
include(msgcom/msgcom.pri)
include(tabcom/tabcom.pri)
include(datapacket/datapacket.pri)
include(serialport/serialport.pri)
include(backcolour/backcolour.pri)
include(qtsingleapplication/qtsingleapplication.pri)

HEADERS += \	
    $$PWD/common.h
	
SOURCES += \	
    $$PWD/common.cpp

FORMS += \





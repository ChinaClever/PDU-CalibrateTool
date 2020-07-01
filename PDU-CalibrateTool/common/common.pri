
INCLUDEPATH += $$PWD

include(excel/excel.pri)
include(dbcom/dbcom.pri)
include(sqlcom/sqlcom.pri)
include(logcom/logcom.pri)
include(cfgcom/cfgcom.pri)
include(msgcom/msgcom.pri)
include(tabcom/tabcom.pri)
include(datapacket/datapacket.pri)
include(serialport/serialport.pri)
include(backcolour/backcolour.pri)
include(qtsingleapplication/qtsingleapplication.pri)
include(json/json.pri)

HEADERS += \	
    $$PWD/common.h
	
SOURCES += \	
    $$PWD/common.cpp

FORMS += \





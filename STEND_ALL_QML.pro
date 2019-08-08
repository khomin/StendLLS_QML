
#QT += quick quickcontrols2 serialport positioning location opengl qml network quick sql concurrent
QT += qml network quickcontrols2 quick serialport charts sql #opengl #concurrent

CONFIG += c++14

TRANSLATIONS += \
        translate/Language_en.ts \
        translate/Language_ru.ts \
        translate/Language_cn.ts

SOURCES += \
        clipboard.cpp \
        connection/interfaces/interfaceEth.cpp \
        connection/logModel/logModel.cpp \
        connection/logModel/logModelItem.cpp \
        connection/connection.cpp \
        connection/interfaces/interfacesAbstract.cpp \
        connection/log.cpp \
        findStend/findStendItem.cpp \
        findStend/findStendModel.cpp \
        findStend/findstend.cpp \
        main.cpp \
        scanerQr/interfaces/interfaceSerial.cpp \
        settings.cpp \
        stend/dataBase.cpp \
        stend/stendApi.cpp \
        stend/stendFirmware/programming.cpp \
        stend/stendTestStatus.cpp \
        storeMisc.cpp \
        translate/translation.cpp \
        view.cpp

RESOURCES += qml.qrc \
    font.qrc

HEADERS += \
    clipboard.h \
    connection/interfaces/interfaceEth.h \
    connection/logModel/logModel.h \
    connection/logModel/logModelItem.h \
    connection/connection.h \
    connection/interfaces/interfacesAbstract.h \
    connection/log.h \
    findStend/findStendItem.h \
    findStend/findStendModel.h \
    findStend/findstend.h \
    globals.h \
    scanerQr/interfaces/interfaceSerial.h \
    settings.h \
    stend/dataBase.h \
    stend/stendApi.h \
    stend/stendFirmware/programming.h \
    stend/stendProperty.h \
    stend/stendTestStatus.h \
    storeMisc.h \
    translate/translation.h \
    view.h

RC_ICONS = ./resources/logo.ico

DISTFILES += \
    ico.rc

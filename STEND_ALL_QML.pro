
QT += qml network quickcontrols2 quick serialport charts sql #opengl

CONFIG += c++17

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
        dataBase/dataBase.cpp \
        dataBase/dataBaseProperty.cpp \
        findStend/findStendItem.cpp \
        findStend/findStendModel.cpp \
        findStend/findstend.cpp \
        globals.cpp \
        main.cpp \
        scanerQr/interfaces/interfaceSerial.cpp \
        scanerQr/qrScaner.cpp \
        selectLlsTestType/item.cpp \
        selectLlsTestType/selectLlsTestModel.cpp \
        selectLlsTestType/selectLlsTesttype.cpp \
        settings.cpp \
        stend/stendApi.cpp \
        stend/stendFirmware/programming.cpp \
        stend/stendTestStatus.cpp \
        stend/validateQchValues.cpp \
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
    dataBase/dataBase.h \
    dataBase/dataBaseProperty.h \
    findStend/findStendItem.h \
    findStend/findStendModel.h \
    findStend/findstend.h \
    globals.h \
    scanerQr/interfaces/interfaceSerial.h \
    scanerQr/qrScaner.h \
    selectLlsTestType/item.h \
    selectLlsTestType/selectLlsTestModel.h \
    selectLlsTestType/selectLlsTesttype.h \
    settings.h \
    stend/stendApi.h \
    stend/stendFirmware/programming.h \
    stend/stendProperty.h \
    stend/stendTestStatus.h \
    stend/validateQchValues.h \
    storeMisc.h \
    translate/translation.h \
    view.h

RC_ICONS = ./resources/logo.ico

DISTFILES += \
    ico.rc

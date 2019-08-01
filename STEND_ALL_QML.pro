
#QT += quick quickcontrols2 serialport positioning location opengl qml network quick sql concurrent
QT += qml network quickcontrols2 quick serialport charts

# Workaround for QTBUG-38735
#QT_FOR_CONFIG += location-private
#qtConfig(geoservices_mapboxgl): QT += sql opengl
#qtConfig(geoservices_osm): QT += concurrent

CONFIG += c++14

TRANSLATIONS += \
        translate/Language_en.ts \
        translate/Language_ru.ts \
        translate/Language_cn.ts

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
        settings.cpp \
        stend/stendApi.cpp \
        storeMisc.cpp \
        translate/translation.cpp \
        view.cpp

RESOURCES += qml.qrc \
    font.qrc \
    resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

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
    settings.h \
    stend/stendApi.h \
    stend/stendProperty.h \
    storeMisc.h \
    translate/translation.h \
    view.h

RC_ICONS = ./resources/logo.ico

DISTFILES += \
    ico.rc

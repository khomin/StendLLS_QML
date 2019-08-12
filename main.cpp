#include <QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickItem>
#include <QQmlContext>
#include <QQuickStyle>
#include "translate/translation.h"
#include "view.h"
#include"clipboard.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    application.setWindowIcon(QIcon("../resources/logo.png"));

    QVariantMap parameters;
    QStringList args(QCoreApplication::arguments());

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    Translation translater(&engine);
    View viewControl;

    engine.rootContext()->setContextProperty("viewControl", &viewControl);
    engine.rootContext()->setContextProperty("translate", &translater);
    qmlRegisterSingletonType<Settings>("Settings", 1, 0, "Settings", Settings::qmlInstance);

    engine.rootContext()->setContextProperty("stendProp", QVariant::fromValue(viewControl.getStendProp()));
    engine.rootContext()->setContextProperty("stendInterface", QVariant::fromValue(viewControl.getStendInterface()));

    engine.rootContext()->setContextProperty("qrScaner", QVariant::fromValue(viewControl.getQrScanerProp()));
    engine.rootContext()->setContextProperty("qrScanerInterface", QVariant::fromValue(viewControl.getQrScanerInterface()));

    engine.rootContext()->setContextProperty("stendFind", QVariant::fromValue(viewControl.getFindStend()));
    engine.rootContext()->setContextProperty("stendFindModel", QVariant::fromValue(viewControl.getFindStend()->getModel()));

    engine.rootContext()->setContextProperty("logStendModel", QVariant::fromValue(viewControl.getStendLogModel()));

    engine.addImportPath(QStringLiteral(":/imports"));

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    engine.rootContext()->setContextProperty("clipboard", new Clipboard(QApplication::clipboard()));

    QObject::connect(&engine, &QQmlApplicationEngine::quit, &QApplication::quit);

    QObject *item = engine.rootObjects().first();
    Q_ASSERT(item);

    return application.exec();
}

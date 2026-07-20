#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "assetservice.h"
#include "vehiclemodel.h"
#include "versionmodel.h"


int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    AssetService assetService;
    VehicleModel vehicleModel;
    VersionModel versionModel;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("assetService", &assetService);
    engine.rootContext()->setContextProperty("vehicleModel", &vehicleModel);
    engine.rootContext()->setContextProperty("versionModel", &versionModel);

    const QUrl url(u"qrc:/Main/Main.qml"_qs);
    engine.load(url);

    return app.exec();
}
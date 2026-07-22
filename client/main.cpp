#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "assetservice.h"
#include "vehiclemodel.h"
#include "versionmodel.h"
#include "globalpartmodel.h"
#include "componentmodel.h"
#include "searchproxymodel.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    AssetService assetService;
    VehicleModel vehicleModel;
    VersionModel versionModel;
    GlobalPartModel globalPartModel;
    ComponentModel componentModel;

    SearchProxyModel searchModel;
    searchModel.setSourceModel(&vehicleModel);
    searchModel.setFilterRole(VehicleModel::NameRole);

    QObject::connect(&assetService, &AssetService::globalPartsFetched,
                     &globalPartModel, [&globalPartModel](const QList<GlobalPart> &parts) {
        globalPartModel.clear();
        for (const auto &part : parts) {
            globalPartModel.addGlobalPart(part);
        }
    });

    QObject::connect(&assetService, &AssetService::componentsFetched,
                     &componentModel, [&componentModel](const QList<ComponentData> &components) {
        componentModel.clear();
        for (const auto &comp : components) {
            componentModel.addComponent(comp);
        }
    });

    QObject::connect(&assetService, &AssetService::versionsLoaded,
                     &versionModel, [&versionModel](const QString &data) {
        versionModel.updateData(data.toUtf8());
    });

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("assetService", &assetService);

    engine.rootContext()->setContextProperty("rawVehicleModel", &vehicleModel);

    engine.rootContext()->setContextProperty("vehicleModel", &searchModel);

    engine.rootContext()->setContextProperty("versionModel", &versionModel);
    engine.rootContext()->setContextProperty("globalPartModel", &globalPartModel);
    engine.rootContext()->setContextProperty("componentModel", &componentModel);

    const QUrl url(u"qrc:/Main/Main.qml"_qs);
    engine.load(url);

    return app.exec();
}
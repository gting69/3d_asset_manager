#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "assetservice.h"
#include "vehiclemodel.h"
#include "versionmodel.h"
#include "globalpartmodel.h"
#include "componentmodel.h"
#include "searchproxymodel.h" // Підключаємо нашу нову Proxy модель

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    // Ініціалізація всіх сервісів та базових моделей
    AssetService assetService;
    VehicleModel vehicleModel;
    VersionModel versionModel;
    GlobalPartModel globalPartModel;
    ComponentModel componentModel;

    // --- НАЛАШТУВАННЯ ПОШУКУ (Proxy Model) ---
    SearchProxyModel searchModel;
    searchModel.setSourceModel(&vehicleModel);
    // Вказуємо, що фільтрація буде відбуватися за полем NameRole
    searchModel.setFilterRole(VehicleModel::NameRole);

    // --- НАЛАШТУВАННЯ ЗВ'ЯЗКІВ (Сигнали та Слоти) ---
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

    // Коли AssetService завантажує Версії -> оновлюємо versionModel
    QObject::connect(&assetService, &AssetService::versionsLoaded,
                     &versionModel, [&versionModel](const QString &data) {
        versionModel.updateData(data.toUtf8()); // Виправляємо тип на QByteArray
    });

    QQmlApplicationEngine engine;

    // Реєструємо всі об'єкти для доступу з QML
    engine.rootContext()->setContextProperty("assetService", &assetService);

    // 1. Експортуємо ОРИГІНАЛЬНУ модель для оновлення даних (запису)
    engine.rootContext()->setContextProperty("rawVehicleModel", &vehicleModel);

    // 2. Експортуємо PROXY модель для відображення у списку та пошуку (читання)
    engine.rootContext()->setContextProperty("vehicleModel", &searchModel);

    engine.rootContext()->setContextProperty("versionModel", &versionModel);
    engine.rootContext()->setContextProperty("globalPartModel", &globalPartModel);
    engine.rootContext()->setContextProperty("componentModel", &componentModel);

    const QUrl url(u"qrc:/Main/Main.qml"_qs);
    engine.load(url);

    return app.exec();
}
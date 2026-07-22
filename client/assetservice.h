#ifndef ASSETSERVICE_H
#define ASSETSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "globalpartmodel.h"  // Підключаємо структуру GlobalPart
#include "componentmodel.h"   // Підключаємо структуру ComponentData

class AssetService : public QObject {
    Q_OBJECT
public:
    explicit AssetService(QObject *parent = nullptr);

    // Існуючі методи
    Q_INVOKABLE void fetchVehicles();
    Q_INVOKABLE void fetchVersions(int vehicleId);
    Q_INVOKABLE void downloadFile(int fileId, const QString &savePath);

    // НОВІ методи для завантаження деталей та компонентів
    Q_INVOKABLE void fetchGlobalParts(int versionId);
    Q_INVOKABLE void fetchComponents(int globalPartId);

    signals:
        // Існуючі сигнали
        void vehiclesLoaded(const QString &data);
    void versionsLoaded(const QString &data);

    // НОВІ сигнали, які передають готові розпарсені списки
    void globalPartsFetched(const QList<GlobalPart> &parts);
    void componentsFetched(const QList<ComponentData> &components);

private:
    QNetworkAccessManager *networkManager;
};

#endif // ASSETSERVICE_H
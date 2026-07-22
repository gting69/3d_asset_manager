#ifndef ASSETSERVICE_H
#define ASSETSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "globalpartmodel.h"
#include "componentmodel.h"

class AssetService : public QObject {
    Q_OBJECT
public:
    explicit AssetService(QObject *parent = nullptr);

    Q_INVOKABLE void fetchVehicles();
    Q_INVOKABLE void fetchVersions(int vehicleId);
    Q_INVOKABLE void downloadFile(int fileId, const QString &savePath);

    Q_INVOKABLE void fetchGlobalParts(int versionId);
    Q_INVOKABLE void fetchComponents(int globalPartId);

    signals:
        void vehiclesLoaded(const QString &data);
    void versionsLoaded(const QString &data);

    void globalPartsFetched(const QList<GlobalPart> &parts);
    void componentsFetched(const QList<ComponentData> &components);

private:
    QNetworkAccessManager *networkManager;
};

#endif
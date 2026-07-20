#ifndef ASSETSERVICE_H
#define ASSETSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class AssetService : public QObject {
    Q_OBJECT
public:
    explicit AssetService(QObject *parent = nullptr);

    Q_INVOKABLE void fetchVehicles();
    Q_INVOKABLE void fetchVersions(int vehicleId);
    Q_INVOKABLE void downloadFile(int fileId, const QString &savePath);

    signals:
    void vehiclesLoaded(const QString &data);
    void versionsLoaded(const QString &data);

private:
    QNetworkAccessManager *networkManager;
};

#endif
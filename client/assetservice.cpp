#include "assetservice.h"
#include <QJsonDocument>
#include <QFile>
#include <QIODevice>
#include <QDebug>

AssetService::AssetService(QObject *parent) : QObject(parent) {
    networkManager = new QNetworkAccessManager(this);
}

void AssetService::fetchVehicles() {
    QUrl url("http://127.0.0.1:8000/vehicles");
    QNetworkReply *reply = networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit vehiclesLoaded(QString::fromUtf8(reply->readAll()));
        }
        reply->deleteLater();
    });
}

void AssetService::fetchVersions(int vehicleId) {
    QUrl url(QString("http://127.0.0.1:8000/vehicles/%1/versions").arg(vehicleId));
    QNetworkReply *reply = networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, [this, reply, vehicleId]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            emit versionsLoaded(QString::fromUtf8(data));
            qDebug() << "Versions for ID received" << vehicleId << ":" << data;
        } else {
            qDebug() << "Error retrieving versions:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void AssetService::downloadFile(int versionId, const QString &savePath) {
    QUrl url(QString("http://127.0.0.1:8000/download/%1").arg(versionId));
    QNetworkReply *reply = networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, [this, reply, savePath]() {
        if (reply->error() == QNetworkReply::NoError) {
            QFile file(savePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(reply->readAll());
                file.close();
                qDebug() << "File successfully saved to:" << savePath;
            } else {
                qDebug() << "ERROR: Could not open file for writing:" << file.errorString();
            }
        } else {
            qDebug() << "ERROR: File upload:" << reply->errorString();
        }
        reply->deleteLater();
    });
}
#include "assetservice.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
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

// НОВИЙ МЕТОД: Завантаження Global Parts
void AssetService::fetchGlobalParts(int versionId) {
    QUrl url(QString("http://127.0.0.1:8000/versions/%1/global-parts").arg(versionId));
    QNetworkReply *reply = networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QList<GlobalPart> partsList;

            if (jsonDoc.isArray()) {
                QJsonArray jsonArray = jsonDoc.array();
                for (int i = 0; i < jsonArray.size(); ++i) {
                    QJsonObject obj = jsonArray[i].toObject();
                    GlobalPart part;
                    part.id = obj["id"].toInt();
                    part.name = obj["name"].toString();
                    partsList.append(part);
                }
            }
            emit globalPartsFetched(partsList);
            qDebug() << "Global parts fetched successfully. Count:" << partsList.size();
        } else {
            qDebug() << "Error retrieving global parts:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

// НОВИЙ МЕТОД: Завантаження Components
void AssetService::fetchComponents(int globalPartId) {
    QUrl url(QString("http://127.0.0.1:8000/global-parts/%1/components").arg(globalPartId));
    QNetworkReply *reply = networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QList<ComponentData> componentsList;

            if (jsonDoc.isArray()) {
                QJsonArray jsonArray = jsonDoc.array();
                for (int i = 0; i < jsonArray.size(); ++i) {
                    QJsonObject obj = jsonArray[i].toObject();
                    ComponentData comp;
                    comp.id = obj["id"].toInt();
                    comp.name = obj["name"].toString();
                    componentsList.append(comp);
                }
            }
            emit componentsFetched(componentsList);
            qDebug() << "Components fetched successfully. Count:" << componentsList.size();
        } else {
            qDebug() << "Error retrieving components:" << reply->errorString();
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
            qDebug() << "ERROR: File download:" << reply->errorString();
        }
        reply->deleteLater();
    });
}
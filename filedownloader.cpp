#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl url, QObject *parent) :
    QObject(parent)
{
    connect(&manager, &QNetworkAccessManager::finished, this, &FileDownloader::fileDownloaded);

    QNetworkRequest request(url);
    manager.get(request);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* reply) {

    qDebug() << "Request result" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    byteArray = reply->readAll();
    reply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const {
    return byteArray;
}

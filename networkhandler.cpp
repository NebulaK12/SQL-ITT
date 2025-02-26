#include "networkhandler.h"
#include <QUrl>
#include <QEventLoop>
#include <QDebug>

NetworkHandler::NetworkHandler(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
}

QString NetworkHandler::fetchNormalResponse(const QString &url)
{
    QUrl requestUrl(url);  // Ensure URL is properly formed
    QNetworkRequest request(requestUrl);  // Properly create QNetworkRequest
    QNetworkReply *reply = networkManager->get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString responseText;
    if (reply->error() == QNetworkReply::NoError) {
        responseText = QString::fromUtf8(reply->readAll());  // Convert QByteArray to QString properly
    } else {
        qDebug() << "Error fetching normal response:" << reply->errorString();
        responseText = QString();  // Return an empty QString on error
    }

    reply->deleteLater();
    return responseText;
}

QString NetworkHandler::testSQLInjection(const QString &url, const QString &payload)
{
    QString fullUrl = url + (url.contains("?") ? "&" : "?") + "cat=" + QUrl::toPercentEncoding(payload);
    return fetchNormalResponse(fullUrl);
}

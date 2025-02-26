#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class NetworkHandler : public QObject
{
    Q_OBJECT

public:
    explicit NetworkHandler(QObject *parent = nullptr);
    QString fetchNormalResponse(const QString &url);
    QString testSQLInjection(const QString &url, const QString &payload);

private:
    QNetworkAccessManager *networkManager;
};

#endif // NETWORKHANDLER_H

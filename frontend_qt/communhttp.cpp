#include "communhttp.h"

communhttp::communhttp(QObject *parent) : QObject (parent)
{
    manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished,
            this, &communhttp::replyfinished);
}

void communhttp::replyfinished(QNetworkReply *reply){
    if (reply->error()){
        qDebug() << reply->errorString();
        return;
    }
    QString answer = reply->readAll();
    qDebug() << answer;
}

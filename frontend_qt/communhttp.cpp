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

QNetworkReply* communhttp::http_get(QNetworkRequest request){
    return manager->get(request);
}

QNetworkReply* communhttp::http_post(QNetworkRequest request){
    return nullptr;
}

QNetworkReply* communhttp::http_head(QNetworkRequest request){
    return nullptr;
}

QNetworkReply* communhttp::http_delete(QNetworkRequest request){
    return nullptr;
}

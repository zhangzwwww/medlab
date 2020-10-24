#ifndef COMMUNHTTP_H
#define COMMUNHTTP_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class communhttp : public QObject{
    Q_OBJECT
private:
    // the network manager handling the communication
    QNetworkAccessManager * manager;
    // the request sent to server: GET, POST, etc.
    QNetworkRequest request;

private slots:
    // after reply is finished -- do the following
    void replyfinished(QNetworkReply *reply);

public:
    explicit communhttp(QObject *parent = nullptr);
};

#endif // COMMUNHTTP_H

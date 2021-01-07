#ifndef PREDICTION_H
#define PREDICTION_H

#include <QString>
#include <QVector>
#include <QNetworkAccessManager>
#include <QObject>
#include <QDir>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

#include "communhttp.h"
#include "urlbase.h"

class prediction
{
private:
    static QString token;

public:
    prediction();

    // set the token for communication
    static void set_token(QString t);

    // predict tumor class of an image
    static QString predictTumor(communhttp* requester, QString filepath);
};

#endif // PREDICTION_H

#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QString>
#include <QVector>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QProgressDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

#include "communhttp.h"
#include "urlbase.h"


class ProgressDialog : public QProgressDialog{
    Q_OBJECT

public:
    explicit ProgressDialog(const QUrl &url, QWidget *parent=nullptr);
    ~ProgressDialog();

public slots:
    void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);
};


class imageInfo : public QDialog
{
    Q_OBJECT

private:
    // the path where user save the image data
    static QString save_path;
    // got from user object or server
    static QString token;

private slots:
    void downloadFile();
    void cancelDownload();
    void httpFinished();
    void httpReadyRead();

public:
    // constructor and destructor
    explicit imageInfo(QWidget *parent=nullptr);
    ~imageInfo();

    // set file path that will save images
    static void setFilePath(QString path);
    // set user token used in http communication
    static void setToken(QString token);

    // Asynchronously get info and files through http requests
    // INPUT: patientID and ctime needed for searching data
    static void getImagesHttp(QString patientID, QString ctime);
};

#endif // IMAGEINFO_H

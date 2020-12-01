#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QString>
#include <QVector>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QDir>
#include <QFileDialog>
#include <QUrlQuery>
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

    // the communicator
    QNetworkAccessManager qnam;
    // the folder path for a patient and ctime
    QString folder_path;

private slots:
    void downloadFile(QNetworkReply*);
    // void uploadFile();
    void cancelDownload();
    void httpFinished(QNetworkReply*);
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
    void getImagesHttp(QString patientID, QString ctime);

    // Upload image data to the server
    // INPUT: patientId and ctime
};

#endif // IMAGEINFO_H

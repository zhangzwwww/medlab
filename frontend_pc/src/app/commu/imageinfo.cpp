#include "imageinfo.h"


// construct a progress window to show downloading progress
ProgressDialog::ProgressDialog(const QUrl &url, QWidget *parent) : QProgressDialog (parent){
    setWindowTitle(tr("Download Progress"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setLabelText(tr("Downloading %1.").arg(url.toDisplayString()));
    setMinimum(0);
    setValue(0);
    setMinimumDuration(0);
    setMinimumSize(QSize(400, 75));
}

ProgressDialog::~ProgressDialog(){

}

void ProgressDialog::networkReplyProgress(qint64 bytesRead, qint64 totalBytes){
    setMaximum(totalBytes);
    setValue(bytesRead);
}


QString imageInfo::token = "";
QString imageInfo::save_path = "";

imageInfo::imageInfo(QWidget* parent)
{
    setWindowFlags(windowFlags());
    setWindowTitle(tr("Http"));
}

imageInfo::~imageInfo(){

}

void imageInfo::setFilePath(QString path){
    save_path = path;
}

void imageInfo::setToken(QString t){
    token = t;
}

void imageInfo::getImagesHttp(QString patientID, QString ctime){
    // construct a directory to save files
    // TODO: check whether they contain files
    QDir dir(save_path);
    if (!dir.exists()){
        qDebug() << "invalid saving path";
        return;
    }
    dir.mkdir(patientID + "_" + ctime);
    if (!dir.cd(patientID + "_" + ctime)){
        qDebug() << "create directory fails";
        return;
    }
    folder_path = dir.absolutePath();
    // write in the metadata
    QString fullpath = dir.filePath("meta_data");
    QFile afile(fullpath);
    if (!afile.open(QIODevice::WriteOnly)){
        qDebug() << "Open file fails";
        return;
    }
    afile.write(patientID.toUtf8(), patientID.length());
    afile.write("\r\n");
    afile.write(ctime.toUtf8(), ctime.length());

    // construct url and query item
    QUrl url(urlbase["base2"] + urlbase["image"]);
    QUrlQuery query;
    query.addQueryItem("patientId", patientID);
    query.addQueryItem("ctime", ctime);
    url.setQuery(query.query());

//    QByteArray post_data;
//    post_data.append("patientId="+patientID);
//    post_data.append("ctime="+ctime);

    // construct request
    // connect(&qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFile(QNetworkReply*)));
    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("X-Auth-Token", token.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    // send request
    QNetworkReply* reply = qnam.get(request);

    // construct the dialog window
    ProgressDialog *progressDialog = new ProgressDialog(url, this);
    progressDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(progressDialog, &QProgressDialog::canceled, this, &imageInfo::cancelDownload);
    connect(reply, &QNetworkReply::downloadProgress, progressDialog, &ProgressDialog::networkReplyProgress);
    connect(reply, &QNetworkReply::finished, progressDialog, &ProgressDialog::hide);
    progressDialog->show();

    // Debug: use straight way to handle reply
    QEventLoop eventloop;
    connect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
    eventloop.exec();
    downloadFile(reply);
}

void imageInfo::downloadFile(QNetworkReply* reply){
    // handle the error
    if (reply->error()){
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }
    // get the status of reply
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    if (status == 401){
        // unauthorized
        qDebug() << "Token unauthorized";
        reply->deleteLater();
        return;
    }
    if (status == 200){
        QDir dir(folder_path);
        // normal case
        QByteArray resp = reply->readAll();
        QJsonParseError jerror;
        QJsonDocument json = QJsonDocument::fromJson(resp, &jerror);
        if (jerror.error == QJsonParseError::NoError && !json.isNull() && !json.isEmpty()){
            // parse each patient
            QVariantList list = json.toVariant().toList();
            for (int i = 0; i < list.count(); i++){
                // get each data and save file
                QString filename = QString("%1").arg(i, 6, 10, QLatin1Char('0')) + ".dcm";
                QVariantMap map = list[i].toMap();
                // save image
                QString filepath = dir.filePath(filename);
                QFile afile(filepath);
                if (!afile.open(QIODevice::WriteOnly)){
                    qDebug() << "Open file fails";
                    return;
                }
                // TODO: check how to write the file
                QString content = map["content"].toString();
                // qDebug() << "NEW FIG";
                // qDebug() << content.toLatin1();
                afile.write(content.toLatin1(), content.length());
            }
        }
    }
    reply->deleteLater();
}

void imageInfo::cancelDownload(){

}

void imageInfo::httpFinished(QNetworkReply* reply){

}

void imageInfo::httpReadyRead(){

}

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
    QString url = urlbase["base3"] + urlbase["image"];
    // construct request
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("X-Auth-Token", token.toUtf8());

}

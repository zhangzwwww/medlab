#include "test_window.h"
#include "ui_test_window.h"

#include "communhttp.h"
#include <QNetworkRequest>
#include <QNetworkReply>

test_window::test_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::test_window)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->plainTextEdit);

    QNetworkRequest request;
    request.setUrl(QUrl("https://www.baidu.com"));
    request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

    // declare a communicator for http request
    communhttp commuicator;
    QNetworkReply* reply = commuicator.http_get(request);
    qDebug() << reply->readAll() << request.rawHeader("User-Agent");

    ui->plainTextEdit->setPlainText(reply->readAll());
    ui->plainTextEdit->appendPlainText(request.rawHeader("User-Agent"));
}

test_window::~test_window()
{
    delete ui;
}

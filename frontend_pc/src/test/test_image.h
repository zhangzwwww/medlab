#ifndef TEST_IMAGE_H
#define TEST_IMAGE_H

#include <QTest>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QVector>

#include "../app/commu/userinfo.h"
#include "../app/commu/patient.h"
#include "../app/commu/imageinfo.h"

class testImage : public QObject
{
    Q_OBJECT

public:
    testImage();
    ~testImage();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
    void test_case2();
    void test_case3();
    void test_case4();

private:
    userinfo usr;
    communhttp requester;
};

#endif // TEST_IMAGE_H

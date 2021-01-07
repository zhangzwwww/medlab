#ifndef TEST_LOGIN_H
#define TEST_LOGIN_H

#include <QTest>

#include "../app/login/loginchecker.h"
#include "../app/commu/communhttp.h"
#include "../app/commu/userinfo.h"

class TestLogin : public QObject
{
    Q_OBJECT

public:
    TestLogin();
    ~TestLogin();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
    void test_id();
    void test_pwd();

private:
    LoginChecker m_login_checker;
};



#endif // TEST_LOGIN_H

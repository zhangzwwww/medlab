#include "test_login.h"

TestLogin::TestLogin()
{

}

TestLogin::~TestLogin()
{

}

void TestLogin::initTestCase()
{

}

void TestLogin::cleanupTestCase()
{

}

void TestLogin::test_case1()
{
    QVERIFY(m_login_checker.SignIn("1", "1") == OK);
}

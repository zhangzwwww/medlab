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
    communhttp commun;
    userinfo user;
    QVERIFY(m_login_checker.SignIn("test@test.com", "hello123", &commun, &user) == OK);
    QVERIFY(user.is_logged_in() == true);
    QVERIFY(user._email() == "test@test.com");
}

void TestLogin::test_id()
{
    QVERIFY(m_login_checker.IsValidID("") == false);
    QVERIFY(m_login_checker.IsValidID("testwhenidistoolongtoolonglonglonglogngmorethan32word") == false);
    QVERIFY(m_login_checker.IsValidID("testbadsymbol#.com") == false);
    QVERIFY(m_login_checker.IsValidID("test@test.com") == true);
    QVERIFY(m_login_checker.IsValidID("test124@test.com") == true);
}

void TestLogin::test_pwd()
{
    QVERIFY(m_login_checker.IsValidPwd("") == false);
    QVERIFY(m_login_checker.IsValidPwd("testwhenpasswordistoolongmorethan30word") == false);
    QVERIFY(m_login_checker.IsValidPwd("testbadsymbol\\") == false);
    QVERIFY(m_login_checker.IsValidPwd("testbadsymbol/") == false);
    QVERIFY(m_login_checker.IsValidPwd("hwhhh12") == true);
}

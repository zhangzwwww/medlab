#include <QtTest>
#include <QCoreApplication>

#include "test_login.h"

// add necessary includes here
#define TEST_CLASS(TestObject) { TestObject tc; QTest::qExec(&tc); }

//QTEST_MAIN(TestLoginChecker)

int main(void) {
    TEST_CLASS(TestLogin);
    return 0;
}

//#include "tst_testloginchecker.moc"

#include <QtTest>
#include <QCoreApplication>

#include "test_login.h"
#include "test_commun.h"

// add necessary includes here
#define TEST_CLASS(TestObject) { TestObject tc; QTest::qExec(&tc); }

//QTEST_MAIN(TestCommun)

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    TEST_CLASS(TestLogin);
    TEST_CLASS(TestCommun);
    return 0;
}

//#include "tst_testloginchecker.moc"

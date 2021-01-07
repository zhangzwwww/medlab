#include <QtTest>
#include <QCoreApplication>
#include <QApplication>

#include "test_login.h"
#include "test_commun.h"
#include "test_patient.h"
#include "test_image.h"

// add necessary includes here
#define TEST_CLASS(TestObject) { TestObject tc; QTest::qExec(&tc); }

//QTEST_MAIN(TestCommun)

int main(int argc, char* argv[]) {
    // QCoreApplication app(argc, argv);
    QApplication a(argc, argv);
    TEST_CLASS(testImage);
//    TEST_CLASS(TestLogin);
//    TEST_CLASS(TestCommun);
//    TEST_CLASS(TestPatient);
    return 0;
}

//#include "tst_testloginchecker.moc"

#ifndef TEST_MAINWINDOWUI_H
#define TEST_MAINWINDOWUI_H

#include <QTest>

class TestMainwindowUI : public QObject
{
    Q_OBJECT
public:
    TestMainwindowUI();
    ~TestMainwindowUI();

private slots:
    void initTestCase();
    void cleanupTestCase();

};

#endif // TEST_MAINWINDOWUI_H

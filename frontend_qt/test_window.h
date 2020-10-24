#ifndef TEST_WINDOW_H
#define TEST_WINDOW_H

#include <QMainWindow>

namespace Ui {
class test_window;
}

class test_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit test_window(QWidget *parent = nullptr);
    ~test_window();

private:
    Ui::test_window *ui;
};

#endif // TEST_WINDOW_H

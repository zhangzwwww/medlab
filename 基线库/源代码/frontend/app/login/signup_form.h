#ifndef SignUpForm_H
#define SignUpForm_H

#include <QDialog>

#include "loginchecker.h"
#include "commu/communhttp.h"
#include "commu/userinfo.h"

namespace Ui {
class SignUpForm;
}

class SignUpForm : public QDialog
{
    Q_OBJECT

public:
    // objects needed for http communicator
    communhttp *commnicator;
    userinfo *user;

    explicit SignUpForm(communhttp *commu, userinfo *user_info, QWidget *parent = nullptr);
    ~SignUpForm();

private slots:
    void on_sign_up_button_clicked();

private:
    Ui::SignUpForm *ui;
    LoginChecker login_checker;
};

#endif // SignUpForm_H

#include "signup_form.h"
#include "ui_signup_form.h"

#include <QMessageBox>
#include <QSizePolicy>

using std::string;

SignUpForm::SignUpForm(communhttp *commu, userinfo *user_info, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUpForm)
{
    ui->setupUi(this);
    ui->id_input->setPlaceholderText("email address");
    ui->pwd_input->setPlaceholderText("password");
    ui->id_input->setFocus();

    // initialize communication object
    this->user = user_info;
    this->commnicator = commu;
}

SignUpForm::~SignUpForm()
{
    delete ui;
}

void SignUpForm::on_sign_up_button_clicked()
{
    QString id = ui->id_input->text();
    QString pwd = ui->pwd_input->text();
    if (login_checker.IsValidID(id) == false) {
        QMessageBox::warning(this, tr("w"), tr("Invalid ID!"), QMessageBox::Yes);
        return;
    }
    if (login_checker.IsValidPwd(pwd) == false) {
        QMessageBox::warning(this, tr("w"), tr("Invalid Password!"), QMessageBox::Yes);
        return;
    }
    ERROR_CODE ret = login_checker.SignUp(id, pwd, this->commnicator, this->user);
    if (ret == OK){
        QMessageBox::information(this, tr("w"), tr("Registration Succeeds!"), QMessageBox::Yes);
        accept();
    }
    else if (ret == LOGIN_ID_NOT_FOUND){
        QMessageBox::warning(this, tr("w"), tr("ID cannot be empty!"), QMessageBox::Yes);
    }
    else if (ret == LOGIN_ERROR_PWD){
        QMessageBox::warning(this, tr("w"), tr("Password cannot be empty!"), QMessageBox::Yes);
    }
    else if (ret == PARAM_INVALID){
        QMessageBox::warning(this, tr("w"), tr("ID already exists!"), QMessageBox::Yes);
    }
    else {
        QMessageBox::warning(this, tr("w"), tr("Registration ERROR!"), QMessageBox::Yes);
    }
}


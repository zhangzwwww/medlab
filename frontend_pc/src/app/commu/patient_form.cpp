#include "patient_form.h"
#include "ui_patient_form.h"

#include <QMessageBox>

PatientForm::PatientForm(PatientFormParams &params, QWidget *parent) :
    QDialog (parent),
    ui(new Ui::PatientForm)
{
    ui->setupUi(this);
    user_info_ = params.user_info;
//    patient_.set_token(user_info_._token());
}

PatientForm::~PatientForm()
{
    delete ui;
}

void PatientForm::on_addPatientBtn_clicked()
{
    QString name = ui->nameEdit->text();
    bool gender = bool(ui->genderSelector->currentIndex());
    QString birth = ui->dateSelector->date().toString("yyyy-MM-dd");
    int age = ui->ageSelector->value();
    qDebug()<<QString("add patient: name[%1],gender[%2],date[%3],age[%4]").arg(name).arg(gender).arg(birth).arg(age);
    patient pat;
    pat.set_token(user_info_._token());
    bool ret = pat.http_create_patient(&communicator_, name, gender, birth, age);
    if (ret == false) {
        QMessageBox::warning(this, "w", "Please input image name!", QMessageBox::Yes);
    } else {
        QMessageBox::information(this, "i", "Succeed to add patient, id:?", QMessageBox::Yes);
        accept();
    }
//    TODO: update parent patient list
}

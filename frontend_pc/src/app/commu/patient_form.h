#ifndef PATIENT_FORM_H
#define PATIENT_FORM_H

#include <QDialog>
#include <QComboBox>

#include "commu/communhttp.h"
#include "commu/patient.h"
#include "commu/userinfo.h"

namespace Ui {
class PatientForm;
}

struct PatientFormParams {
    userinfo user_info;
};

class PatientForm : public QDialog
{
    Q_OBJECT

public:
    explicit PatientForm(PatientFormParams &params, QWidget *parent = nullptr);
    ~PatientForm();

private slots:

    void on_addPatientBtn_clicked();

private:
    Ui::PatientForm *ui;
    communhttp communicator_;
    userinfo user_info_;
};

#endif // UPLOAD_FORM_H

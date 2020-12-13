#ifndef UPLOAD_FORM_H
#define UPLOAD_FORM_H

#include <QDialog>
#include <QComboBox>
#include <QVector>

#include "commu/communhttp.h"
#include "commu/patient.h"
#include "commu/userinfo.h"

namespace Ui {
class UploadForm;
}

struct UploadFormParams {
    communhttp communicator;
    userinfo user_info;
    QVector<patient> patients;
};

class UploadForm : public QDialog
{
    Q_OBJECT

public:
    explicit UploadForm(QWidget *parent = nullptr);
    explicit UploadForm(QComboBox *patient_id_combo, QWidget *parent = nullptr);
    explicit UploadForm(UploadFormParams &params, QWidget *parent = nullptr);
    ~UploadForm();

private slots:
    void on_openFileBtn_clicked();

    void on_uploadFileBtn_clicked();

private:
    Ui::UploadForm *ui;
    communhttp communicator_;
    userinfo user_info_;
    QVector<patient> patients_;
};

#endif // UPLOAD_FORM_H

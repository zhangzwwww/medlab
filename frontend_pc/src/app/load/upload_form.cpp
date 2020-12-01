#include "upload_form.h"
#include "ui_upload_form.h"

#include <QFileDialog>
#include <QMessageBox>

#include "struct_define.h"
#include "utils/general_util.h"

UploadForm::UploadForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadForm)
{
    ui->setupUi(this);
}

UploadForm::UploadForm(QComboBox *patient_id_combo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadForm)
{
    ui->setupUi(this);
    int count = int(patient_id_combo->count());
    for (int i = 0; i < count; i++) {
        ui->patientIDSelector->addItem(patient_id_combo->itemText(i));
    }
}

UploadForm::UploadForm(UploadFormParams &params, QWidget *parent) :
    QDialog (parent),
    ui(new Ui::UploadForm)
{
    ui->setupUi(this);
    user_info_ = params.user_info;
    patients_ = params.patients;
    if (patients_.empty()) {
        patient temp_patient;
        temp_patient.set_token(user_info_._token());
        patients_ = temp_patient.http_get_all_patient(&communicator_);
    }
    for (patient &pat:patients_) {
        ui->patientIDSelector->addItem(QString("[%1]%2").arg(pat._name()).arg(pat._id()));
    }
}

UploadForm::~UploadForm()
{
    delete ui;
}

void UploadForm::on_openFileBtn_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, QString(tr("Open DICOM Image")));

    if (fileName.isEmpty() == true)
        return;

    ui->filePathEdit->setText(fileName);
    ui->imageNameEdit->setText(GetFileName(fileName));
}

void UploadForm::on_uploadFileBtn_clicked()
{
//    TODO: upload file
    if (ui->filePathEdit->text().isEmpty()) {
        QMessageBox::warning(this, "w", "Please select folder to upload!", QMessageBox::Yes);
        return;
    }
    if (ui->imageNameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "w", "Please input image name!", QMessageBox::Yes);
        return;
    }
    UploadFileParams params;
    params.file_path = ui->filePathEdit->text().toStdString();
    params.patient_id = ui->patientIDSelector->currentText().toStdString();
    params.image_name = ui->imageNameEdit->text().toStdString();
//    if upload succeed
    accept();
}

#include "download_form.h"
#include "ui_download_form.h"

#include <QFileDialog>
#include <QMessageBox>

#include "struct_define.h"
#include "utils/general_util.h"

DownloadForm::DownloadForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadForm)
{
    ui->setupUi(this);
}

DownloadForm::DownloadForm(QComboBox *patient_id_combo, QComboBox *image_name_combo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadForm)
{
    ui->setupUi(this);
    int count = int(patient_id_combo->count());
    for (int i = 0; i < count; i++) {
        ui->patientIDSelector->addItem(patient_id_combo->itemText(i));
    }
    count = int(image_name_combo->count());
    for (int i = 0; i < count; i++) {
        ui->imageNameSelector->addItem(image_name_combo->itemText(i));
    }
}

DownloadForm::DownloadForm(DownloadFormParams &params, QWidget *parent) :
    QDialog (parent),
    ui(new Ui::DownloadForm)
{
    ui->setupUi(this);
    user_info_ = params.user_info;
    patients_ = params.patients;
    if (patients_.empty()) {
        patient pat;
        pat.set_token(user_info_._token());
        patients_ = pat.http_get_all_patient(&communicator_);
    }
    for (patient &pat: patients_) {
        ui->patientIDSelector->addItem(QString("[%1]%2").arg(pat._name()).arg(pat._id()));
    }
}

DownloadForm::~DownloadForm()
{
    delete ui;
}

void DownloadForm::on_openFileBtn_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, QString(tr("Select DICOM Image Save Folder")));

    if (fileName.isEmpty() == true)
        return;

    ui->filePathEdit->setText(fileName);
}

void DownloadForm::on_downloadFileBtn_clicked()
{
//    TODO: upload file
    if (ui->filePathEdit->text().isEmpty()) {
        QMessageBox::warning(this, "w", "Please select folder to upload!", QMessageBox::Yes);
        return;
    }
    DownloadFileParams params;
    params.save_path = ui->filePathEdit->text().toStdString();
    params.patient_id = ui->patientIDSelector->currentText().toStdString();
    params.image_name = ui->imageNameSelector->currentText().toStdString();
//    if upload succeed
    accept();
}

void DownloadForm::on_patientIDSelector_currentIndexChanged(int index)
{
//    TODO: update image name list
}

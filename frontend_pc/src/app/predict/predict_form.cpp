#include "predict_form.h"
#include "ui_predict_form.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#include "struct_define.h"
#include "utils/general_util.h"

PredictForm::PredictForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PredictForm)
{
    ui->setupUi(this);
}

PredictForm::PredictForm(PredictFormParams &params, QWidget *parent) :
    QDialog (parent),
    ui(new Ui::PredictForm)
{
    ui->setupUi(this);
    user_info_ = params.user_info;
    prediction_manager_.set_token(user_info_._token());
}

PredictForm::~PredictForm()
{
    delete ui;
}

void PredictForm::on_openFileBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("open a file."),
            "./",
            tr("Image Files(*.jpg *.png *.jpeg)"));
    if (fileName.isEmpty() == true)
        return;

    ui->filePathEdit->setText(fileName);
}

void PredictForm::on_predictBtn_clicked()
{
    QString file_path = ui->filePathEdit->text();
    if (file_path.isEmpty()) {
        QMessageBox::warning(this, "error", "Please select file to predict!", QMessageBox::Yes);
        return;
    }
    QString prediction = prediction_manager_.predictTumor(&communicator_, file_path);
    ui->predictResultEdit->setText(prediction);
}

#ifndef UPLOAD_FORM_H
#define UPLOAD_FORM_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class UploadForm;
}

class UploadForm : public QDialog
{
    Q_OBJECT

public:
    explicit UploadForm(QWidget *parent = nullptr);
    explicit UploadForm(QComboBox *patient_id_combo, QWidget *parent = nullptr);
    ~UploadForm();

private slots:
    void on_openFileBtn_clicked();

    void on_uploadFileBtn_clicked();

private:
    Ui::UploadForm *ui;
};

#endif // UPLOAD_FORM_H

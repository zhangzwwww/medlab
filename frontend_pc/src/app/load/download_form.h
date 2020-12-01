#ifndef DOWNLOAD_FORM_H
#define DOWNLOAD_FORM_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class DownloadForm;
}

class DownloadForm : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadForm(QWidget *parent = nullptr);
    explicit DownloadForm(QComboBox *patient_id_combo, QComboBox *image_name_combo, QWidget *parent = nullptr);
    ~DownloadForm();

private slots:
    void on_openFileBtn_clicked();

    void on_downloadFileBtn_clicked();

private:
    Ui::DownloadForm *ui;
};

#endif // UPLOAD_FORM_H

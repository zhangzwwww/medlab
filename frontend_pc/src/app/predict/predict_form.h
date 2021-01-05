#ifndef PREDICT_FORM_H
#define PREDICT_FORM_H

#include <QDialog>
#include <QComboBox>
#include <QVector>

#include "commu/communhttp.h"
#include "commu/userinfo.h"
#include "commu/prediction.h"

namespace Ui {
class PredictForm;
}

struct PredictFormParams {
    userinfo user_info;
};

class PredictForm : public QDialog
{
    Q_OBJECT

public:
    explicit PredictForm(QWidget *parent = nullptr);
    explicit PredictForm(PredictFormParams &params, QWidget *parent = nullptr);
    ~PredictForm();

private slots:
    void on_openFileBtn_clicked();

    void on_predictBtn_clicked();

private:
    Ui::PredictForm *ui;
    communhttp communicator_;
    userinfo user_info_;
    prediction prediction_manager_;
};

#endif // PREDICT_FORM_H

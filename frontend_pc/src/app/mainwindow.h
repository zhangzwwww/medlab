#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "struct_define.h"

#include <QMainWindow>
#include <QTreeWidgetItem>

#include <vector>

#include <vtkSmartPointer.h>
#include <itkImage.h>

#include "commu/communhttp.h"
#include "commu/userinfo.h"
#include "utils/general_util.h"

using std::vector;

class vtkImageViewer2;
class vtkRenderer;
class vtkImageData;
class vtkVolume;
class vtkImageStack;
class vtkActor;

struct ImageDataItem {
    QString image_name;
    vtkSmartPointer<vtkImageData> image_data;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // define a http communicator
    communhttp communicator;
    // define the user object
    userinfo user;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void init_views();
    void show_image();
    void clean_view4();
    void update_data_manager();
    

private slots:
    void load_image();
    void volume_rendering(bool status);
    void view_zoom_to_fit();
    void view_full_screen(bool full_status);
    void view_change_slice();

    void generate_surface();
    void clean_actors();

    void image_threshold(vtkImageData* input_image, vtkImageData* output_image, ThresholdingParams params);

    void on_pushButton_4_clicked();

    void on_in_fusion_2_opacity_valueChanged(int value);

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_start_smoothing_button_clicked();

    void on_detect_edge_button_clicked();

    void on_start_thresholding_button_clicked();

    void on_data_manager_itemClicked(QTreeWidgetItem *item, int column);

    void on_addPatientBtn_clicked();

    void on_action_upload_file_triggered();

    void on_action_download_file_triggered();

private:
    vtkSmartPointer<vtkImageViewer2> riw_[3];
    vtkSmartPointer<vtkRenderer> renderer3D_;

    itk::Image<float, 3>::Pointer image_itk_;
    vtkSmartPointer<vtkImageData> image_vtk_;
    vtkSmartPointer<vtkVolume> volume_;


    vtkSmartPointer<vtkRenderer> m_Renderer2D[3];
    vtkSmartPointer<vtkImageStack> m_ImageStack2D[3];
    //int dims_[3];

    vector<vector<ImageDataItem>> image_tree_;
    int cur_selected_image_ind[2];

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "struct_define.h"

#include <QMainWindow>
// QT
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>


// VTK
#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);


#include "vtkSmartPointer.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageViewer2.h"
#include "vtkCamera.h"
#include "vtkAxisActor.h"
#include "vtkNIFTIImageReader.h"

#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkRendererCollection.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>


#include <stdio.h>
#include <stdlib.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include "vtkResliceImageViewer.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkResliceCursorThickLineRepresentation.h"
#include "vtkResliceCursorWidget.h"
#include "vtkResliceCursorActor.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkResliceCursor.h"
#include "vtkDICOMImageReader.h"
#include "vtkMetaImageReader.h"
#include "vtkCellPicker.h"
#include "vtkProperty.h"
#include "vtkPlane.h"
#include "vtkImageData.h"
#include "vtkCommand.h"
#include "vtkPlaneSource.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkInteractorStyleImage.h"
#include "vtkImageSlabReslice.h"
#include "vtkBoundedPlanePointPlacer.h"
#include "vtkDistanceWidget.h"
#include "vtkDistanceRepresentation.h"
#include "vtkHandleRepresentation.h"
#include "vtkResliceImageViewerMeasurements.h"
#include "vtkDistanceRepresentation2D.h"
#include "vtkPointHandleRepresentation3D.h"
#include "vtkPointHandleRepresentation2D.h"

//
#include <vtkImageThreshold.h>
#include <vtkPolyDataMapper.h>

#include "struct_define.h"
#include "registration/RegistrationWorker.h"
#include "voxel2mesh/Voxel2Mesh.h"

// ITK
#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>
#include <itkVTKImageToImageFilter.h>

#include <vtkImageSliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkImageStack.h>

#include <vtkSmartPointer.h>
#include <itkImage.h>

#include "commu/communhttp.h"
#include "commu/userinfo.h"

//class vtkImageViewer2;
//class vtkRenderer;
//class vtkImageData;
//class vtkVolume;
//class vtkImageStack;


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

    // utility class
    class vtkSharedWindowLevelCallback : public vtkCommand{
    public:
        static vtkSharedWindowLevelCallback* New();
        void Execute(vtkObject* caller, unsigned long ev, void* callData);
        vtkSharedWindowLevelCallback();
        vtkImageViewer2* view[3];
    };

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void init_views();
    void show_image();
    void clean_view4();
    

private slots:
    void load_image();
    void volume_rendering();
    void view_zoom_to_fit();
    void view_full_screen(bool full_status);
    void view_change_slice();

    void generate_surface();


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

private:
    vtkSmartPointer<vtkImageViewer2> riw_[3];
    vtkSmartPointer<vtkRenderer> renderer3D_;

    itk::Image<float, 3>::Pointer image_itk_;
    vtkSmartPointer<vtkImageData> image_vtk_;
    vtkSmartPointer<vtkVolume> volume_;

    vtkSmartPointer<vtkRenderer> m_Renderer2D[3];
    vtkSmartPointer<vtkImageStack> m_ImageStack2D[3];
    //int dims_[3];

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"


/*--------------------- Definition for subclass ------------------*/
MainWindow::vtkSharedWindowLevelCallback* MainWindow::vtkSharedWindowLevelCallback::New(){
    return new vtkSharedWindowLevelCallback;
}

void MainWindow::vtkSharedWindowLevelCallback::Execute(vtkObject *caller, unsigned long ev, void *callData){
    if (ev == vtkCommand::WindowLevelEvent)
    {
        vtkInteractorStyleImage* style =
            dynamic_cast<vtkInteractorStyleImage*>(caller);

        if (style)
        {
            if (style == this->view[0]->GetInteractorStyle())
            {
                view[1]->SetColorLevel(view[0]->GetColorLevel());
                view[1]->SetColorWindow(view[0]->GetColorWindow());
                view[2]->SetColorLevel(view[0]->GetColorLevel());
                view[2]->SetColorWindow(view[0]->GetColorWindow());
            }
            else if (style == this->view[1]->GetInteractorStyle())
            {
                view[0]->SetColorLevel(view[1]->GetColorLevel());
                view[0]->SetColorWindow(view[1]->GetColorWindow());
                view[2]->SetColorLevel(view[1]->GetColorLevel());
                view[2]->SetColorWindow(view[1]->GetColorWindow());
            }
            else if (style == this->view[2]->GetInteractorStyle())
            {
                view[0]->SetColorLevel(view[2]->GetColorLevel());
                view[0]->SetColorWindow(view[2]->GetColorWindow());
                view[1]->SetColorLevel(view[2]->GetColorLevel());
                view[1]->SetColorWindow(view[2]->GetColorWindow());
            }
        }

        for (int i = 0; i < 3; i++)
        {
            this->view[i]->Render();
        }

        return;
    }
}

MainWindow::vtkSharedWindowLevelCallback::vtkSharedWindowLevelCallback(){

}
/*--------------------- End definition for subclass --------------*/



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    image_itk_(nullptr), image_vtk_(nullptr)
{
    ui->setupUi(this);
    ui->data_manager->clear();
//    ui->patientSelector->setFocus();
//    TODO: getAllPatient and update ui, suppose we input a vector<QString> vec, use ui->patientSelector->addItem(vec[i]) to update

    //const char* path = ":/resources/qssfile/dark_theme.qss";
    //QFile qssfile(path);
    //qssfile.open(QFile::ReadOnly);
    //QString qss;
    //qss = qssfile.readAll();
    //this->setStyleSheet(qss);


    this->ui->view1->hide();
    this->ui->view2->hide();
    this->ui->view3->hide();

    this->init_views();

    connect(ui->action_open_file, SIGNAL(triggered()), this, SLOT(load_image()));
    connect(ui->action_visualization, SIGNAL(triggered(bool)), this, SLOT(volume_rendering(bool)));

    connect(ui->zoomBtn1, SIGNAL(clicked()), this, SLOT(view_zoom_to_fit()));
    connect(ui->zoomBtn2, SIGNAL(clicked()), this, SLOT(view_zoom_to_fit()));
    connect(ui->zoomBtn3, SIGNAL(clicked()), this, SLOT(view_zoom_to_fit()));
    connect(ui->fullScreenBtn1, SIGNAL(clicked(bool)), this, SLOT(view_full_screen(bool)));
    connect(ui->fullScreenBtn2, SIGNAL(clicked(bool)), this, SLOT(view_full_screen(bool)));
    connect(ui->fullScreenBtn3, SIGNAL(clicked(bool)), this, SLOT(view_full_screen(bool)));
    connect(ui->fullScreenBtn4, SIGNAL(clicked(bool)), this, SLOT(view_full_screen(bool)));
    connect(ui->ScrollBar1, SIGNAL(valueChanged(int)), this, SLOT(view_change_slice()));
    connect(ui->ScrollBar2, SIGNAL(valueChanged(int)), this, SLOT(view_change_slice()));
    connect(ui->ScrollBar3, SIGNAL(valueChanged(int)), this, SLOT(view_change_slice()));

    connect(ui->voxel2meshBtn, SIGNAL(clicked()), this, SLOT(generate_surface()));

    connect(ui->clean_actors_btn, SIGNAL(clicked()), this, SLOT(clean_actors()));


//    TODO: make sure the number of spinbox/lineedit is legal
//    AlgorithmParams
//    FusionParams
//    RegistrationParams
//    ProcessingParams
//    MedianFilterParams
//    GaussianParams
//    PolygonSmoothingParams


}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::init_views()
{

    for (int i = 0; i < 3; i++)
    {
        riw_[i] = vtkSmartPointer< vtkImageViewer2 >::New();
    }

    riw_[0]->SetRenderWindow(this->ui->view1->GetRenderWindow());
    riw_[1]->SetRenderWindow(this->ui->view2->GetRenderWindow());
    riw_[2]->SetRenderWindow(this->ui->view3->GetRenderWindow());

    riw_[0]->SetupInteractor(
        this->ui->view1->GetRenderWindow()->GetInteractor());
    riw_[1]->SetupInteractor(
        this->ui->view2->GetRenderWindow()->GetInteractor());
    riw_[2]->SetupInteractor(
        this->ui->view3->GetRenderWindow()->GetInteractor());

    // Image Stack
    //for (int crntViewLabel = 0; crntViewLabel < 3; crntViewLabel++)
    //{
    //	// vtkImageStack
    //	m_ImageStack2D[crntViewLabel] = vtkSmartPointer<vtkImageStack>::New();
    //	// vtkRenderer
    //	m_Renderer2D[crntViewLabel] = vtkSmartPointer<vtkRenderer>::New();
    //	m_Renderer2D[crntViewLabel]->AddViewProp(m_ImageStack2D[crntViewLabel]);
    //	m_Renderer2D[crntViewLabel]->GetActiveCamera()->ParallelProjectionOn(); // ƽ��ͶӰ
    //
    //}

    //this->ui->view1->GetRenderWindow()->AddRenderer(m_Renderer2D[0]);
    //this->ui->view2->GetRenderWindow()->AddRenderer(m_Renderer2D[1]);
    //this->ui->view3->GetRenderWindow()->AddRenderer(m_Renderer2D[2]);


    renderer3D_ = vtkSmartPointer<vtkRenderer>::New();
    renderer3D_->SetBackground(1, 1, 1);
    renderer3D_->SetBackground2(0.5, 0.5, 0.5);
    renderer3D_->SetGradientBackground(1);

//    this->ui->view4->GetRenderWindow()->AddRenderer(renderer3D_);

    this->ui->view1->show();
    this->ui->view2->show();
    this->ui->view3->show();

}

void MainWindow::load_image()
{
    QString fileName = QFileDialog::getExistingDirectory(this, QString(tr("Open DICOM Image")));

    if (fileName.isEmpty() == true)
        return;

    QByteArray ba = fileName.toLocal8Bit();
    const char* fileName_str = ba.data();

    RegistrationWorker worker;
    image_itk_ = worker.readImageDICOM(fileName_str);

    if (image_itk_ == nullptr)
    {
        QMessageBox::warning(nullptr,
            tr("Read Image Error"),
            tr("Read image error, Please read the correct Image."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

        return;
    }

    using FilterType = itk::ImageToVTKImageFilter<itk::Image<float, 3>>;
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(image_itk_);

    try
    {
        filter->Update();
    }
    catch (itk::ExceptionObject & error)
    {
        QMessageBox::warning(nullptr,
            tr("Read Image Error"),
            tr(error.GetDescription()),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
    }

    image_vtk_ = filter->GetOutput();
    ImageDataItem image_item;
    image_item.image_name = GetFileName(fileName);
    image_item.image_data = image_vtk_;
    image_tree_.push_back(vector<ImageDataItem>{image_item});
    image_item.image_name = "test";
    image_tree_.back().push_back(image_item);
    update_data_manager();

    // clean the current volume
    this->clean_view4();
    this->show_image();
}


void MainWindow::show_image()
{
    vtkSmartPointer< vtkSharedWindowLevelCallback > sharedWLcbk =
        vtkSmartPointer< vtkSharedWindowLevelCallback >::New();

    double range[2];
    int dims[3];
    image_vtk_->GetScalarRange(range);
    image_vtk_->GetDimensions(dims);


    //vtkSmartPointer<vtkImageSliceMapper> imageSliceMapper =
    //	vtkSmartPointer<vtkImageSliceMapper>::New();
    //imageSliceMapper->SetInputData(image_vtk_);
    //vtkSmartPointer<vtkImageSlice> imageSlice = vtkSmartPointer<vtkImageSlice>::New();
    //imageSlice->SetMapper(imageSliceMapper);
    //for (int crntViewLabel = 0; crntViewLabel < 3; crntViewLabel++)
    //{
    //	m_ImageStack2D[crntViewLabel]->AddImage(imageSlice);
    //}


    for (int i = 0; i < 3; i++)
    {
        riw_[i]->SetInputData(image_vtk_);
        riw_[i]->SetSliceOrientation(i);
        riw_[i]->SetSlice(dims[i] / 2);

        riw_[i]->SetColorWindow((range[1] - range[0]));
        riw_[i]->SetColorLevel((range[0] + range[1]) / 2.0);

        sharedWLcbk->view[i] = riw_[i];
        riw_[i]->GetInteractorStyle()->AddObserver(vtkCommand::WindowLevelEvent, sharedWLcbk);
    }

    this->ui->ScrollBar1->setMaximum(dims[0]);
    this->ui->ScrollBar2->setMaximum(dims[1]);
    this->ui->ScrollBar3->setMaximum(dims[2]);
    this->ui->ScrollBar1->setSliderPosition(dims[0] / 2 - 1);
    this->ui->ScrollBar2->setSliderPosition(dims[1] / 2 - 1);
    this->ui->ScrollBar3->setSliderPosition(dims[2] / 2 - 1);

    this->view_zoom_to_fit();
}

void MainWindow::volume_rendering(bool status)
{
    if (status == false)
    {
        if (volume_ != nullptr)
        {
            renderer3D_->RemoveVolume(volume_);
            this->ui->view4->GetRenderWindow()->Render();
        }

        return;
    }
    else {

        if (image_vtk_ == nullptr)
        {
            QMessageBox::warning(nullptr,
                tr("Error"),
                tr("Please read the image first."),
                QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

            ui->action_visualization->setChecked(false);
            return;
        }

        if (volume_ == nullptr)
        {
            volume_ = vtkSmartPointer<vtkVolume>::New();
        }

        ui->action_visualization->setEnabled(0);

        vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper =
            vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
        volumeMapper->SetInputData(image_vtk_);

        volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance() / 4);
        volumeMapper->SetAutoAdjustSampleDistances(0);
        volumeMapper->SetImageSampleDistance(2);

        vtkSmartPointer<vtkVolumeProperty> volumeProperty =
            vtkSmartPointer<vtkVolumeProperty>::New();
        volumeProperty->SetInterpolationTypeToLinear();
        volumeProperty->ShadeOn();
        volumeProperty->SetAmbient(.1);
        volumeProperty->SetDiffuse(.9);
        volumeProperty->SetSpecular(.2);
        volumeProperty->SetSpecularPower(10);

        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
            vtkSmartPointer<vtkPiecewiseFunction>::New();
        vtkSmartPointer<vtkColorTransferFunction> colorFun =
            vtkSmartPointer<vtkColorTransferFunction>::New();

        compositeOpacity->AddPoint(-3024, 0, 0.5, 0.0);
        compositeOpacity->AddPoint(-16, 0, .49, .61);
        compositeOpacity->AddPoint(641, .72, .5, 0.0);
        compositeOpacity->AddPoint(3071, .71, 0.5, 0.0);

        colorFun->AddRGBPoint(-3024, 0, 0, 0, 0.5, 0.0);
        colorFun->AddRGBPoint(-16, 0.73, 0.25, 0.30, 0.49, .61);
        colorFun->AddRGBPoint(641, .90, .82, .56, .5, 0.0);
        colorFun->AddRGBPoint(3071, 1, 1, 1, .5, 0.0);

        volumeProperty->SetScalarOpacity(compositeOpacity);
        volumeProperty->SetColor(colorFun);


        volume_->SetMapper(volumeMapper);
        volume_->SetProperty(volumeProperty);

        renderer3D_->AddVolume(volume_);
        renderer3D_->ResetCamera();
        this->ui->view4->GetRenderWindow()->Render();

        ui->action_visualization->setEnabled(1);
    }
}


void MainWindow::clean_view4()
{
    //if (volume_ != nullptr)
    //{
    //	renderer3D_->RemoveVolume(volume_);
    //}

    //if (mesher_ != nullptr)
    //{
    //	renderer3D_->RemoveActor(mesher_);
    //}

    this->ui->view4->GetRenderWindow()->RemoveRenderer(renderer3D_);

    renderer3D_ = vtkSmartPointer<vtkRenderer>::New();
    renderer3D_->SetBackground(1, 1, 1);
    renderer3D_->SetBackground2(0.5, 0.5, 0.5);
    renderer3D_->SetGradientBackground(1);

    this->ui->view4->GetRenderWindow()->AddRenderer(renderer3D_);
    this->ui->view4->GetRenderWindow()->Render();
}

void MainWindow::view_zoom_to_fit()
{
    for (int i = 0; i < 3; i++)
    {
        riw_[i]->GetRenderer()->ResetCamera();
        riw_[i]->Render();
    }

}

void MainWindow::view_full_screen(bool full_status)
{
    QObject* obj = sender();

    if (obj == this->ui->fullScreenBtn1)
    {
        if (full_status)
        {
            this->ui->view1Widget->show();

            this->ui->view2Widget->hide();
            this->ui->view3Widget->hide();
            this->ui->view4Widget->hide();
        }
        else {
            this->ui->view1Widget->show();
            this->ui->view2Widget->show();
            this->ui->view3Widget->show();
            this->ui->view4Widget->show();
        }
    }
    if (obj == this->ui->fullScreenBtn2)
    {
        if (full_status)
        {
            this->ui->view2Widget->show();

            this->ui->view1Widget->hide();
            this->ui->view3Widget->hide();
            this->ui->view4Widget->hide();
        }
        else {
            this->ui->view1Widget->show();
            this->ui->view2Widget->show();
            this->ui->view3Widget->show();
            this->ui->view4Widget->show();
        }
    }
    if (obj == this->ui->fullScreenBtn3)
    {
        if (full_status)
        {
            this->ui->view3Widget->show();

            this->ui->view2Widget->hide();
            this->ui->view1Widget->hide();
            this->ui->view4Widget->hide();
        }
        else {
            this->ui->view1Widget->show();
            this->ui->view2Widget->show();
            this->ui->view3Widget->show();
            this->ui->view4Widget->show();
        }
    }
    if (obj == this->ui->fullScreenBtn4)
    {
        if (full_status)
        {
            this->ui->view4Widget->show();

            this->ui->view2Widget->hide();
            this->ui->view3Widget->hide();
            this->ui->view1Widget->hide();
        }
        else {
            this->ui->view1Widget->show();
            this->ui->view2Widget->show();
            this->ui->view3Widget->show();
            this->ui->view4Widget->show();
        }
    }

}


void MainWindow::view_change_slice()
{
    QObject* obj = sender();
    if (obj == this->ui->ScrollBar1)
    {
        QScrollBar* tempScroll = qobject_cast<QScrollBar*>(obj);
        riw_[0]->SetSlice(tempScroll->value());
        riw_[0]->Render();

        //this->ui->label_slicenum1->
        //	setText(QString::number(tempScroll->value() + 1) + "  of  " + QString::number(dims[2]));

    }
    if (obj == this->ui->ScrollBar2)
    {
        QScrollBar* tempScroll = qobject_cast<QScrollBar*>(obj);
        riw_[1]->SetSlice(tempScroll->value());
        riw_[0]->Render();

        //this->ui->label_slicenum2->
        //	setText(QString::number(tempScroll->value() + 1) + "  of  " + QString::number(dims[1]));

    }
    if (obj == this->ui->ScrollBar3)
    {
        QScrollBar* tempScroll = qobject_cast<QScrollBar*>(obj);
        riw_[2]->SetSlice(tempScroll->value());
        riw_[0]->Render();

        //this->ui->label_slicenum3->
        //	setText(QString::number(tempScroll->value() + 1) + "  of  " + QString::number(dims[0]));

    }
}

void MainWindow::generate_surface()
{
    Voxel2Mesh voxel2mesh_filter;

    voxel2mesh_filter.SetUseGuassianSmoothing(ui->medianGroup->isChecked());
    voxel2mesh_filter.SetMedianKernelSize(ui->kernelXSpinBox->value(),
        ui->kernelYSpinBox->value(), ui->kernelZSpinBox->value());

    voxel2mesh_filter.SetUseGuassianSmoothing(ui->gaussGroup->isChecked());
    voxel2mesh_filter.SetGaussianStandardDeviation(ui->deviationSpinBox->value());
    voxel2mesh_filter.SetGaussianRadius(ui->radiusSpinBox->value());

    voxel2mesh_filter.SetPolygonSmoothing(ui->smoothingGroup->isChecked());
    voxel2mesh_filter.SetIteration(ui->iterationSpinBox->value());
    voxel2mesh_filter.SetRelaxationFactor(ui->relaxationSpinBox->value());

    voxel2mesh_filter.SetIsovalue(ui->isovalueSpinBox->value());

    if (image_vtk_ == nullptr)
    {
        QMessageBox::warning(nullptr,
            tr("Error"),
            tr("No Image."),
            QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

    }

    voxel2mesh_filter.SetInputData(image_vtk_);
    voxel2mesh_filter.Update();

    vtkPolyData* mesh = voxel2mesh_filter.GetOutput();

    vtkSmartPointer<vtkPolyDataMapper> mesh_mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mesh_mapper->SetInputData(mesh);
    mesh_mapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkActor> mesh_actor =
        vtkSmartPointer<vtkActor>::New();
    mesh_actor->SetMapper(mesh_mapper);
    mesh_actor->GetProperty()->SetColor(250 / 250.0, 187 / 250.0, 124 / 250.0);
    mesh_actor->GetProperty()->SetOpacity(0.9);

    renderer3D_->AddActor(mesh_actor);
    renderer3D_->ResetCamera();

    this->ui->view4->GetRenderWindow()->Render();

}

void MainWindow::clean_actors()
{

    clean_view4();

    ui->action_visualization->setChecked(false);

}

void MainWindow::image_threshold(vtkImageData* input_image,
    vtkImageData* output_image, ThresholdingParams params)
{
    vtkSmartPointer<vtkImageThreshold> thresholdFilter = vtkSmartPointer<vtkImageThreshold>::New();

    thresholdFilter->SetInputData(input_image);
    thresholdFilter->ThresholdBetween(params.lower_value, params.upper_value);

    //thresholdFilter->ReplaceInOn();//��ֵ�ڵ�����ֵ�滻
    thresholdFilter->ReplaceOutOn();//��ֵ�������ֵ�滻

    thresholdFilter->SetInValue(1);//��ֵ������ֵȫ���滻��1
    thresholdFilter->SetOutValue(0);//��ֵ������ֵȫ���滻��0

    thresholdFilter->Update();

    output_image = thresholdFilter->GetOutput();
}



void MainWindow::on_pushButton_4_clicked()
{
    // TODO: start fusion
    FusionParams fusion_params;
    //fusion_params.img0 = ui->in_fusion_0_img0->currentText().toStdString();
   // fusion_params.img1 = ui->in_fusion_1_img1->currentText().toStdString();
    //fusion_params.opacity = double(ui->in_fusion_2_opacity->value())/100.0;
    //fusion_params.fused_img_name = ui->in_fusion_3_fused_img_name->text().toStdString();
}

void MainWindow::on_in_fusion_2_opacity_valueChanged(int value)
{
    //ui->out_fusion_0_opacity->setText(QString::number(double(value)/100.0));
}

void MainWindow::on_pushButton_5_clicked()
{
//    TODO: Load fixed image
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("open a file."),
            "D:/",
            tr("All files(*.*)"));
    //ui->in_registration_0_fixed_img_name->setText(fileName);
}

void MainWindow::on_pushButton_6_clicked()
{
//    TODO: Load moving image
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("open a file."),
            "D:/",
            tr("All files(*.*)"));
    //ui->in_registration_1_moving_img_name->setText(fileName);
}

void MainWindow::on_pushButton_7_clicked()
{
//    TODO: Registration
    RegistrationParams registration_params;
   /* registration_params.fixed_img_name = ui->in_registration_0_fixed_img_name->text().toStdString();
    registration_params.moving_img_name = ui->in_registration_1_moving_img_name->text().toStdString();
    registration_params.registration_type = RegistrationType(ui->in_registration_4_regi_type->currentIndex());
    registration_params.metrics_type = MetricsType(ui->in_registration_5_metrics_type->currentIndex());*/
}

void MainWindow::on_pushButton_clicked()
{
//    TODO: Generated surface
    //ProcessParams process_params;
    //process_params.img_name = ui->in_gray_0_img->currentText().toStdString();
    //process_params.isosurface_value = ui->in_gray_1_isosurface_value->value();
    //process_params.surface_name = ui->in_gray_2_surface_name->text().toStdString();

}

void MainWindow::on_start_smoothing_button_clicked()
{
    //SmoothingParams params;
    //params.smooth_type = ui->smoothing_toolBox->currentIndex();
    //switch (params.smooth_type) {
    //case 0:
    //    params.kernel_size = ui->in_smooth_gaussian_0_kernel_size->value();
    //    params.sigma_x = ui->in_smooth_gaussian_1_sigma_x->value();
    //    break;
    //case 1:
    //    params.kernel_size = ui->in_smooth_mean_0_kernel_size->value();
    //    break;
    //case 2:
    //    params.kernel_size = ui->in_smooth_median_0_kernel_size->value();
    //    break;
    //default:
    //    break;
    //}
//    TODO
}

void MainWindow::on_detect_edge_button_clicked()
{
    EdgeDetectParams params;
    params.threshold_1 = ui->in_edge_0_thresh_1->value();
    params.threshold_2 = ui->in_edge_0_thresh_2->value();
//    TODO
}

void MainWindow::on_start_thresholding_button_clicked()
{
    ThresholdingParams params;
    params.lower_value = this->ui->in_lower_value->value();
    params.upper_value = this->ui->in_upper_value->value();

    vtkSmartPointer<vtkImageData> image_threshold_result;

    image_threshold(image_vtk_, image_threshold_result, params);

    //for (int i = 0; i < 3; i++)
    //{
    //	riw_[i]->SetSlice(this->dims[i] / 2);
    //	riw_[i]->SetInputData(image_threshold_result);
    //	riw_[i]->SetSliceOrientation(i);

    //	riw_[i]->Render();

    //	riw_[i]->GetRenderer()->ResetCamera();

    //}


}

/*
 * update_data_manager: udpate ui->data_manager according to this->image_tree_
 * image_tree_ is two layer: 1. folder 2. images, eg: image_tree_[0][0] saves the original image of first folder we opened
 * cur_selected_image_ind saves the index, !!! make sure to check if cur_selected_image_ind is >= 0 before using it !!!
 * cur_selected_image_ind[0] = -1 means there is no selected image in current stage
 */
void MainWindow::update_data_manager() {
    if (image_tree_.empty()) {
        return;
    }
    ui->data_manager->clear();
    QTreeWidgetItem *folder_name;
    for (vector<ImageDataItem> &vec : image_tree_) {
        if (vec.empty()) {
            continue;
        }
        folder_name = new QTreeWidgetItem(ui->data_manager, QStringList(vec[0].image_name));
        folder_name->setCheckState(0, Qt::Unchecked);
        int len = int(vec.size());
        for (int i = 1; i < len; i++) {
            QTreeWidgetItem *item = new QTreeWidgetItem(folder_name, QStringList(vec[i].image_name));
            item->setCheckState(0, Qt::Unchecked);
        }
    }
    folder_name->setCheckState(0, Qt::Checked);
    cur_selected_image_ind[0] = int(image_tree_.size())-1;
    cur_selected_image_ind[1] = 0;
//    update combo box
    ui->FixedImageSelector->clear();
    ui->MovingImageSelector->clear();
//    ui->maskImageSelector->clear();
    ui->greyScaleImageSelector->clear();
    ui->BlendImage0Selector->clear();
    ui->BlendImage1Selector->clear();
    for (vector<ImageDataItem> &vec: image_tree_) {
        for (ImageDataItem &item: vec) {
            ui->FixedImageSelector->addItem(item.image_name);
            ui->MovingImageSelector->addItem(item.image_name);
//            ui->maskImageSelector->addItem(item.image_name);
            ui->greyScaleImageSelector->addItem(item.image_name);
            ui->BlendImage0Selector->addItem(item.image_name);
            ui->BlendImage1Selector->addItem(item.image_name);
        }
    }
}

/*
 * on_data_manager_itemClicked: triggered when item is clicked
 * [in]   item: the item user clicked
 * [in] column:
 * make sure single checked item in data_manager
 */
void MainWindow::on_data_manager_itemClicked(QTreeWidgetItem *item, int column)
{
    if (item->checkState(column) == Qt::Checked) {
        QTreeWidgetItemIterator iter(ui->data_manager);
        while (*iter) {
//            qDebug()<<(*iter)->text(0);
            (*iter)->setCheckState(0, Qt::Unchecked);
            ++iter;
        }
        item->setCheckState(column, Qt::Checked);
        if (item->parent() == nullptr) {
            cur_selected_image_ind[0] = ui->data_manager->indexOfTopLevelItem(item);
            cur_selected_image_ind[1] = 0;
        } else {
            cur_selected_image_ind[0] = ui->data_manager->indexOfTopLevelItem(item->parent());
            cur_selected_image_ind[1] = column+1;
        }
//        TODO: switch img when user select
    } else {
        cur_selected_image_ind[0] = -1;
    }
    qDebug()<<"data manager item clicked index: ("<<cur_selected_image_ind[0]<<","
           <<cur_selected_image_ind[1]<<"), name["<<item->text(column)<<"]";
}

void MainWindow::on_addPatientBtn_clicked()
{
//    TODO: add patient
    PatientFormParams params;
    params.user_info = user;
    PatientForm patient_form(params, this);
    int ret = patient_form.exec();
    if (ret == QDialog::Accepted) {
        update_patients();
    }
}

void MainWindow::on_action_upload_file_triggered()
{
    UploadFormParams params;
//    params.communicator = communicator;
    params.user_info = user;
//    params.patients = patients_;
    UploadForm upload_form(params, this);
    upload_form.exec();
}

void MainWindow::on_action_download_file_triggered()
{
    DownloadFormParams params;
    params.user_info = user;
//    params.patients = patients_;
    DownloadForm download_form(params, this);
    download_form.exec();
}

void MainWindow::init()
{
//    init patient
    update_patients();
}

void MainWindow::on_patientSelector_currentTextChanged(const QString &arg1)
{
    qDebug()<<"patient selected: "<<arg1;
    image_requester.setToken(user._token());
    QVector<QString> image_names = image_requester.getCtimeHttp(arg1);
    ui->patientImageSelector->clear();
    for (QString &name: image_names) {
        ui->patientImageSelector->addItem(name);
    }
}

void MainWindow::update_patients()
{
    patient temp_patient;
    temp_patient.set_token(user._token());
    this->patients_ = temp_patient.http_get_all_patient(&communicator);
    ui->patientSelector->clear();
    for (patient &pat: patients_) {
        qDebug()<<pat._id()<<":"<<pat._name();
        ui->patientSelector->addItem(pat._id());
    }
}

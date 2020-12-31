//#ifndef __segmentation_worker_h__
//#define __segmentation_worker_h__
//
//// ITK
//#include <itkImage.h>
//#include <itkCurvatureFlowImageFilter.h>
//#include <itkConnectedThresholdImageFilter.h>
//
//class SegmentationWorker
//{
//public:
//
//    SegmentationWorker();
//    ~SegmentationWorker() {};
//
//    // image typedefs
//    using PixelType = float;
//    using ImageType = itk::Image<PixelType, 3>;
//    using IndexType = RegionGrowingFilterType::IndexType;
//
//    using CurvatureFlowFilter = itk::CurvatureFlowImageFilter<ImageType, ImageType>;
//    using RegionGrowingFilterType = itk::ConnectedThresholdImageFilter<ImageType, ImageType>;
//    using ThresholdingFilterType = itk::BinaryThresholdImageFilter<ImageType, ImageType>;
//
//public:
//    void set_image(ImageType::Pointer img);
//    void set_seeds(vector<IndexType> seeds);
//    void update();
//    ImageType::Pointer get_result_image();
//
//    void set_lower_value(double d) {
//        lower_value_ = d;
//    }
//
//    void set_upper_value(double d) {
//        upper_value_ = d;
//    }
//
//
//private:
//    ImageType::Pointer image_;
//    ImageType::Pointer result_image_;
//
//    CurvatureFlowFilter::Pointer     smoothing_filter_;
//    RegionGrowingFilterType::Pointer regiongrow_filter_;
//    ThresholdingFilterType::Pointer  thresholder_;
//
//    // parameters
//    double lower_value_;
//    double upper_value_;
//};
//
//
//
//#endif // __segmentation_worker_h__
//

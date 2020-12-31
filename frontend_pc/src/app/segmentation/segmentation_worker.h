#ifndef __segmentation_worker_h__
#define __segmentation_worker_h__

// ITK
#include <itkImage.h>
#include <itkCurvatureFlowImageFilter.h>
#include <itkConnectedThresholdImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkImageFileWriter.h>

#include <vector>

class SegmentationWorker
{
public:

    SegmentationWorker();
    ~SegmentationWorker();

    // image typedefs
    using PixelType = float;
    using ImageType = itk::Image<PixelType, 3>;

    using CurvatureFlowFilter = itk::CurvatureFlowImageFilter<ImageType, ImageType>;
    using RegionGrowingFilterType = itk::ConnectedThresholdImageFilter<ImageType, ImageType>;
    using ThresholdingFilterType = itk::BinaryThresholdImageFilter<ImageType, ImageType>;
    
    using IndexType = RegionGrowingFilterType::IndexType;

public:
    void set_input_image(ImageType::Pointer img);
    void set_seeds(std::vector<IndexType> seeds);
    void update();
    ImageType::Pointer get_output_image();

    void set_lower_value(double d) {
        lower_value_ = d;
    }

    void set_upper_value(double d) {
        upper_value_ = d;
    }


private:
    ImageType::Pointer image_;
    ImageType::Pointer result_image_;

    CurvatureFlowFilter::Pointer     smoothing_filter_;
    RegionGrowingFilterType::Pointer regiongrow_filter_;
    ThresholdingFilterType::Pointer  thresholder_;

    std::vector<IndexType> seeds_;

    // parameters
    double lower_value_;
    double upper_value_;
};



#endif // __segmentation_worker_h__


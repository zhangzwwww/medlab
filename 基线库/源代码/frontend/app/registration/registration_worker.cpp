#include <iostream>

#include <QMessageBox>
#include <thread>

#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"

#include "registration_worker.h"


RegistrationWorker::RegistrationWorker()
	: id(002), m_iteration(50), m_maximumStepLength(4), m_minimumStepLength(0.00001)
	, m_relaxationFactor(0.80), m_numberOfSamples(2000), m_defaultPixelValue(-1000)
	, m_fixedImage(nullptr), m_movingImage(nullptr)
{
	m_transform = TransformType::New();
	m_optimizer = OptimizerType::New();
	m_interpolator = InterpolatorType::New();
	m_metric = MIMetricType::New();
	m_registration = RegistrationType::New();

	m_metric->SetFixedImageStandardDeviation(0.4);
	m_metric->SetMovingImageStandardDeviation(0.4);
	m_optimizer->MaximizeOn();

	m_registration->SetMetric(m_metric);
	m_registration->SetOptimizer(m_optimizer);
	m_registration->SetTransform(m_transform);
	m_registration->SetInterpolator(m_interpolator);

	m_fixedNormalizer = FixedNormalizeFilterType::New();
	m_movingNormalizer = MovingNormalizeFilterType::New();

	m_fixedSmoother = GaussianFilterType::New();
	m_movingSmoother = GaussianFilterType::New();

	m_fixedSmoother->SetVariance(2.0);
	m_movingSmoother->SetVariance(2.0);

	using OptimizerScalesType = OptimizerType::ScalesType;
	OptimizerScalesType optimizerScales(m_transform->GetNumberOfParameters());
	const double translationScale = 1.0 / 1000.0;
	optimizerScales[0] = 1.0;
	optimizerScales[1] = 1.0;
	optimizerScales[2] = 1.0;
	optimizerScales[3] = translationScale;
	optimizerScales[4] = translationScale;
	optimizerScales[5] = translationScale;

	m_optimizer->SetScales(optimizerScales);

}

void RegistrationWorker::preparePipeline() 
{
	m_optimizer->SetNumberOfIterations(m_iteration);
	m_optimizer->SetMaximumStepLength(m_maximumStepLength);
	m_optimizer->SetMinimumStepLength(m_minimumStepLength);

	m_optimizer->SetRelaxationFactor(m_relaxationFactor);

	const auto numberOfSamples = static_cast<unsigned int>(m_numberOfSamples);

	m_metric->SetNumberOfSpatialSamples(numberOfSamples);
	m_metric->ReinitializeSeed(1000);

	FixedImageType::Pointer resampledImage = imageResample(m_fixedImage);

	m_fixedNormalizer->SetInput(resampledImage);
	m_movingNormalizer->SetInput(m_movingImage);

	m_fixedSmoother->SetInput(m_fixedNormalizer->GetOutput());
	m_movingSmoother->SetInput(m_movingNormalizer->GetOutput());


	using TransformInitializerType = itk::CenteredTransformInitializer<TransformType, FixedImageType, MovingImageType>;
	TransformInitializerType::Pointer initializer = TransformInitializerType::New();

	initializer->SetTransform(m_transform);
	initializer->SetFixedImage(m_fixedSmoother->GetOutput());
	initializer->SetMovingImage(m_movingSmoother->GetOutput());
	//initializer->MomentsOn(); 
	initializer->GeometryOn();
	initializer->InitializeTransform();

	using VersorType = TransformType::VersorType;
	using VectorType = VersorType::VectorType;
	VersorType  rotation;
	VectorType  axis;
	axis[0] = 0.0;
	axis[1] = 0.0;
	axis[2] = 1.0;
	constexpr double angle = 0;
	rotation.Set(axis, angle);
	m_transform->SetRotation(rotation);

	m_registration->SetFixedImage(m_fixedSmoother->GetOutput());
	m_registration->SetMovingImage(m_movingSmoother->GetOutput());

	m_fixedNormalizer->Update();
	FixedImageType::RegionType fixedImageRegion = m_fixedNormalizer->GetOutput()->GetBufferedRegion();
	m_registration->SetFixedImageRegion(fixedImageRegion);

	m_registration->SetInitialTransformParameters(m_transform->GetParameters());

	// add progress observer
	CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
	observer->SetCallbackWorker(this);
	
	//static_cast<CommandIterationUpdate*>(observer.GetPointer())->SetCallbackWorker(this);
	m_optimizer->AddObserver(itk::IterationEvent(), observer);

}

void RegistrationWorker::itkProgressCommandCallback(int iter) {
	float progress_f = float(iter) / m_iteration;
	emit progress(progress_f);
}

void RegistrationWorker::process() {

	try {
		preparePipeline();
		m_output = getRegistrationResult();

	}
	catch (itk::ExceptionObject& error)
	{
		QMessageBox::warning(nullptr,
			tr("Selection Error"),
			tr(error.GetDescription()),
			QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

	}

	emit finished((itk::DataObject::Pointer) m_output);
}

itk::Image<float, 3>::Pointer RegistrationWorker::readImageDICOM(const char* DICOMImagePath)
{
	using FixedImageReaderType = itk::ImageSeriesReader<FixedImageType>;
	FixedImageReaderType::Pointer  fixedImageReader = FixedImageReaderType::New();

	using ImageIOType = itk::GDCMImageIO;
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	fixedImageReader->SetImageIO(dicomIO);
	//fixedImageReader->ForceOrthogonalDirectionOff(); // properly read CTs with gantry tilt

	using NamesGeneratorType = itk::GDCMSeriesFileNames;
	NamesGeneratorType::Pointer fixedNameGenerator = NamesGeneratorType::New();
	fixedNameGenerator->SetUseSeriesDetails(true);
	fixedNameGenerator->SetDirectory(DICOMImagePath);
	fixedNameGenerator->AddSeriesRestriction("0008|0021"); 

	try
	{
		//std::cout << std::endl;

		// UID ( Unique Identifiers )
		using SeriesIdContainer = std::vector< std::string >;
		const SeriesIdContainer& fixedSeriesUID = fixedNameGenerator->GetSeriesUIDs();

		//std::cout << "Contains the following DICOM Series: ";
		auto seriesItr = fixedSeriesUID.begin();
		auto seriesEnd = fixedSeriesUID.end();

		if (seriesItr == seriesEnd)
		{

			QMessageBox::warning(nullptr,
				tr("Selection Error"),
				tr("please select the folder containing DICOM files."),
				QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

			return nullptr;
		}


		while (seriesItr != seriesEnd)
		{
			//std::cout << seriesItr->c_str() << std::endl << std::endl << std::endl;
			++seriesItr;
		}

		std::string fixedSeriesIdentifier;
		std::string movingSeriesIdentifier;

		fixedSeriesIdentifier = fixedSeriesUID.begin()->c_str();

		using FileNamesContainer = std::vector< std::string >;
		FileNamesContainer fixedFileNames;

		fixedFileNames = fixedNameGenerator->GetFileNames(fixedSeriesIdentifier);
		fixedImageReader->SetFileNames(fixedFileNames);

		try
		{
			fixedImageReader->Update();
		}
		catch (itk::ExceptionObject& ex)
		{
			QMessageBox::warning(nullptr,
				tr("Error"),
				tr(ex.GetDescription()),
				QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		}

		return fixedImageReader->GetOutput();

	}
	catch (itk::ExceptionObject& ex)
	{
		QMessageBox::warning(nullptr,
			tr("Error"),
			tr(ex.GetDescription()),
			QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	}

}

void RegistrationWorker::readFixedImageDICOM(const char* DICOMImagePath)
{
	using FixedImageReaderType = itk::ImageSeriesReader<FixedImageType>;
	FixedImageReaderType::Pointer  fixedImageReader = FixedImageReaderType::New();

	using ImageIOType = itk::GDCMImageIO;
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	fixedImageReader->SetImageIO(dicomIO);
	//fixedImageReader->ForceOrthogonalDirectionOff(); // properly read CTs with gantry tilt

	using NamesGeneratorType = itk::GDCMSeriesFileNames;
	NamesGeneratorType::Pointer fixedNameGenerator = NamesGeneratorType::New();
	fixedNameGenerator->SetUseSeriesDetails(true);
	fixedNameGenerator->SetDirectory(DICOMImagePath);
	fixedNameGenerator->AddSeriesRestriction("0008|0021"); 

	try
	{
		std::cout << std::endl;

		using SeriesIdContainer = std::vector< std::string >;
		const SeriesIdContainer& fixedSeriesUID = fixedNameGenerator->GetSeriesUIDs();

		std::cout << "Contains the following DICOM Series: ";
		auto seriesItr = fixedSeriesUID.begin();
		auto seriesEnd = fixedSeriesUID.end();

		if (seriesItr == seriesEnd) 
		{

			QMessageBox::warning(nullptr,
				tr("Selection Error"),
				tr("please select the folder containing DICOM files."),
				QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

			return;
		}


		while (seriesItr != seriesEnd)
		{
			std::cout << seriesItr->c_str() << std::endl << std::endl << std::endl;
			++seriesItr;
		}

		std::string fixedSeriesIdentifier;
		std::string movingSeriesIdentifier;

		fixedSeriesIdentifier = fixedSeriesUID.begin()->c_str();

		using FileNamesContainer = std::vector< std::string >;
		FileNamesContainer fixedFileNames;

		fixedFileNames = fixedNameGenerator->GetFileNames(fixedSeriesIdentifier);
		fixedImageReader->SetFileNames(fixedFileNames);

		try
		{
			fixedImageReader->Update();
		}
		catch (itk::ExceptionObject& ex)
		{
			std::cout << ex << std::endl;
		}

		FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

	}
	catch (itk::ExceptionObject& ex)
	{
		std::cout << ex << std::endl;
	}

}

void RegistrationWorker::readMovingImageDICOM(const char* DICOMImagePath)
{
	using MovingImageReaderType = itk::ImageSeriesReader<MovingImageType>;
	MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();

	using ImageIOType = itk::GDCMImageIO;
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	movingImageReader->SetImageIO(dicomIO);
	//movingImageReader->ForceOrthogonalDirectionOff(); // properly read CTs with gantry tilt

	using NamesGeneratorType = itk::GDCMSeriesFileNames;
	NamesGeneratorType::Pointer movingNameGenerator = NamesGeneratorType::New();
	movingNameGenerator->SetUseSeriesDetails(true);
	movingNameGenerator->SetDirectory(DICOMImagePath);
	movingNameGenerator->AddSeriesRestriction("0008|0021");

	try
	{
		std::cout << std::endl;

		using SeriesIdContainer = std::vector< std::string >;
		const SeriesIdContainer& movingSeriesUID = movingNameGenerator->GetSeriesUIDs();

		std::string movingSeriesIdentifier;

		movingSeriesIdentifier = movingSeriesUID.begin()->c_str();

		using FileNamesContainer = std::vector< std::string >;
		FileNamesContainer fixedFileNames;
		FileNamesContainer movingFileNames;

		movingFileNames = movingNameGenerator->GetFileNames(movingSeriesIdentifier);
		movingImageReader->SetFileNames(movingFileNames);

		try
		{
			movingImageReader->Update();
		}
		catch (itk::ExceptionObject& ex)
		{
			std::cout << ex << std::endl;

		}

		MovingImageType::Pointer movingImage = movingImageReader->GetOutput();

	}
	catch (itk::ExceptionObject& ex)
	{
		std::cout << ex << std::endl;
	}

}


RegistrationWorker::FixedImageType::Pointer RegistrationWorker::getRegistrationResult()
{
	try
	{
		m_registration->Update();
	}
	catch (itk::ExceptionObject& err)
	{
		std::cout << "ExceptionObject caught !" << std::endl;
		std::cout << err << std::endl;
	}

	using ParametersType = RegistrationType::ParametersType;
	ParametersType finalParameters = m_registration->GetLastTransformParameters();

	double TranslationAlong1 = finalParameters[0];
	double TranslationAlong2 = finalParameters[1];
	double TranslationAlong3 = finalParameters[2];
	double TranslationAlong4 = finalParameters[3];
	double TranslationAlong5 = finalParameters[4];
	double TranslationAlong6 = finalParameters[5];

	unsigned int numberOfIterations = m_optimizer->GetCurrentIteration();

	double bestValue = m_optimizer->GetValue();

	TransformType::Pointer finalTransform = TransformType::New();;
	finalTransform->SetParameters(finalParameters);
	finalTransform->SetFixedParameters(m_transform->GetFixedParameters());

	using ResampleFilterType = itk::ResampleImageFilter<MovingImageType, FixedImageType>;
	ResampleFilterType::Pointer resample = ResampleFilterType::New();

	resample->SetTransform(finalTransform);
	//resample->SetTransform(m_registration->GetOutput()->Get());
	resample->SetInput(m_movingImage);
	resample->SetSize(m_fixedImage->GetLargestPossibleRegion().GetSize());
	resample->SetOutputOrigin(m_fixedImage->GetOrigin());
	resample->SetOutputSpacing(m_fixedImage->GetSpacing());
	resample->SetOutputDirection(m_fixedImage->GetDirection());
	resample->SetDefaultPixelValue(m_defaultPixelValue);

	try
	{
		resample->Update();
	}
	catch (itk::ExceptionObject& error)
	{
		std::cerr << "Error: " << error << std::endl;
	}

	FixedImageType::Pointer finishedImage = resample->GetOutput();

	return finishedImage;

}


RegistrationWorker::FixedImageType::Pointer RegistrationWorker::imageResample(FixedImageType::Pointer& originalImage)
{
	typedef itk::ResampleImageFilter<FixedImageType, FixedImageType> ResampleImageFilterType;
	ResampleImageFilterType::Pointer resampleFilter = ResampleImageFilterType::New();

	resampleFilter->SetInput(originalImage);

	using SizeType = FixedImageType::SizeType;
	using PixelType = FixedImageType::PixelType;
	using PoingType = FixedImageType::PointType;
	using IndexType = FixedImageType::IndexType;
	using DirectionType = FixedImageType::DirectionType;
	using SpacingType = FixedImageType::SpacingType;

	SpacingType originspacing = originalImage->GetSpacing();
	SizeType size = originalImage->GetLargestPossibleRegion().GetSize();
	SizeType sizesample;
	sizesample[0] = 256;
	sizesample[1] = 256;
	sizesample[2] = size[2];
	SpacingType spacingresample;
	spacingresample[0] = size[0] * originspacing[0] / sizesample[0];
	spacingresample[1] = size[1] * originspacing[1] / sizesample[1];
	spacingresample[2] = size[2] * originspacing[2] / sizesample[2];

	InterpolatorType::Pointer interpolator = InterpolatorType::New();
	resampleFilter->SetInterpolator(interpolator);
	resampleFilter->SetOutputSpacing(spacingresample);
	resampleFilter->SetSize(sizesample);
	resampleFilter->SetOutputOrigin(originalImage->GetOrigin());
	resampleFilter->SetOutputDirection(originalImage->GetDirection());
	resampleFilter->SetDefaultPixelValue(m_defaultPixelValue);

	try
	{
		resampleFilter->Update();
	}
	catch (itk::ExceptionObject& exp)
	{
		std::cerr << "Resample Error!" << std::endl;
		std::cerr << exp << std::endl;
	}

	return resampleFilter->GetOutput();
}

void RegistrationWorker::showImageInfor()
{

	std::cout << std::endl <<
		" ----------------- Image Information ---------------- " << std::endl;

	// Fixed Image
	std::cout << std::endl << "Fixed Image Information: " << std::endl;
	std::cout << std::endl;

	const FixedImageType::SpacingType& spacing = m_fixedImage->GetSpacing();
	const FixedImageType::PointType& origin = m_fixedImage->GetOrigin();
	const FixedImageType::SizeType& size = m_fixedImage->GetLargestPossibleRegion().GetSize();

	std::cout << " Spacing = " << spacing[0] << " " << spacing[1] << " " << spacing[2] << std::endl;
	std::cout << " Origin = " << origin[0] << " " << origin[1] << " " << origin[2] << std::endl;
	std::cout << " Size = " << size[0] << " " << size[1] << " " << size[2] << std::endl;

	std::cout << std::endl << "Moving Image Information: " << std::endl;
	std::cout << std::endl;

	const MovingImageType::SpacingType& spacing1 = m_movingImage->GetSpacing();
	const MovingImageType::PointType& origin1 = m_movingImage->GetOrigin();
	const MovingImageType::SizeType& size1 = m_movingImage->GetLargestPossibleRegion().GetSize();

	std::cout << " Spacing = " << spacing1[0] << " " << spacing1[1] << " " << spacing1[2] << std::endl;
	std::cout << " Origin = " << origin1[0] << " " << origin1[1] << " " << origin1[2] << std::endl;
	std::cout << " Size = " << size1[0] << " " << size1[1] << " " << size1[2] << std::endl;


	std::cout << std::endl << "Image Center Information: " << std::endl;

	RegistrationType::ParametersType center = m_transform->GetFixedParameters();
	std::cout << std::endl << "Center of Fixed Image is: " << "[" << center[0] << ", "
		<< center[1] << ", " << center[2] << "]" << std::endl;

	FixedImageType::IndexType pixelIndex;
	FixedImageType::PointType point;
	point[0] = center[0];
	point[1] = center[1];
	point[2] = center[2];

	bool isInside = m_fixedSmoother->GetOutput()->TransformPhysicalPointToIndex(point, pixelIndex);
	if (isInside)
	{
		std::cout << "The Center of Fixed Image is: " << "[" << pixelIndex[0] << ", "
			<< pixelIndex[1] << ", " << pixelIndex[2] << "]" << std::endl << std::endl;
	}
	else
	{
		std::cout << "the point is outside of image !" << std::endl << std::endl;
	}


}


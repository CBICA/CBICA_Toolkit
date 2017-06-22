/**
\file  cbicaITKUtilities.h

\brief Some basic utility functions.

Dependecies: ITK (module_review enabled), OpenMP

https://www.cbica.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include <algorithm>
#include <functional>
#include <omp.h>

#include "itkImage.h"
#include "itkImageRegionIterator.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkAdaptiveHistogramEqualizationImageFilter.h"
#include "itkMultiResolutionPDEDeformableRegistration.h"
#include "itkDemonsRegistrationFilter.h"
#include "itkDiffeomorphicDemonsRegistrationFilter.h"
#include "itkFastSymmetricForcesDemonsRegistrationFilter.h"
#include "itkSymmetricForcesDemonsRegistrationFilter.h"

#include "itkLinearInterpolateImageFunction.h"
#include "itkBSplineInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"


#include "gdcmMD5.h"

using ImageTypeFloat3D = itk::Image< float, 3 >;
unsigned int RmsCounter = 0;
double MaxRmsE[4] = { 0.8, 0.75, 0.4, 0.2 };

enum DeformRegType
{
  Demons, DiffeomorphicDemons, SymmetricForcesDemons, FastSymmetricForcesDemons
};

enum InterpolatorType
{
  Linear, NearestNeighbor, BSpline
};

namespace cbica
{
  /**
  \brief Calculate and preserve the mask indeces

  \param inputModalitiesAndImages A collection of images which are stored in a per-modality basis (each entry corresponds to a subject, whose entries contain different modalities)
  \return A collection of indeces which constitute the non-zero locations per modality (each entry corresponds to a subject, which contains the locations of non-zero pixel values for all modalities)
  */
  template< class TImageType = ImageTypeFloat3D >
  std::vector< std::vector< typename TImageType::IndexType > > CreateMaskIndeces(const std::vector< std::vector< typename TImageType::Pointer > > &inputModalitiesAndImages)
  {
    std::vector< std::vector< typename TImageType::IndexType > > returnMaskIndeces;
    returnMaskIndeces.resize(inputModalitiesAndImages.size()); // pre-allocate data for speed

    // start data processing
    // made parallel for efficiency
    int threads = omp_get_max_threads(); // obtain maximum number of threads available on machine  
    threads > inputModalitiesAndImages.size() ? threads = inputModalitiesAndImages.size() : threads = threads;
//#pragma omp parallel for num_threads(threads)
    for (int i = 0; i < inputModalitiesAndImages.size(); i++)
    {
      VectorType means;
      std::vector< typename TImageType::IndexType > tempIndeces;
      TImageType::SizeType size = inputModalitiesAndImages[i][0]->GetLargestPossibleRegion().GetSize();
      size_t totalImageSize = size[0] * size[1] * size[2];
      means.resize(totalImageSize);
      //std::fill(means.begin(), means.end(), 0);
      means.assign(totalImageSize, 0);

      for (size_t j = 0; j < inputModalitiesAndImages[i].size(); j++)
      {
        VectorType tempVec;
        itk::ImageRegionIterator< TImageType > it(inputModalitiesAndImages[i][j], inputModalitiesAndImages[i][j]->GetLargestPossibleRegion());
        it.GoToBegin();

        while (!it.IsAtEnd())
        {
          tempVec.push_back(it.Get());
          tempIndeces.push_back(it.GetIndex());
          ++it;
        }
        if (tempVec.size() == means.size())
        {
          std::transform(means.begin(), means.end(), tempVec.begin(), means.begin(), std::plus< float >()); // add tempVec to means dector
        }
        else
        {
          std::cerr << "Mean vector calculation error.\n";
          exit(EXIT_FAILURE);
        }
      } // loop over all subjects in each modality

      //std::transform(means.begin(), means.end(), means.begin(), std::bind1st(std::divides< float >(), means.size())); // divide entire means vector by its size
      std::vector< typename TImageType::IndexType > tempMaskIndeces;
      for (size_t j = 0; j < means.size(); j++)
      {
        if (means[j] > 0)
        {
          tempMaskIndeces.push_back(tempIndeces[j]); // store indeces of non-zero mean values
        }
      }
      returnMaskIndeces[i] = tempMaskIndeces;
    } // loop over all modalities

    return returnMaskIndeces;
  }

  /**
  \brief Get Pixel Values of specified indeces of input Image

  \param inputImage The input image in itk::Image format
  \param indeced The indeces from which pixel values need to be extracted
  \return Vector of values whose data type is the same as image type
  */
  template < typename TImageType = ImageTypeFloat3D >
  std::vector< typename TImageType::PixelType > GetPixelValuesFromIndeces(const typename TImageType::Pointer inputImage, const std::vector< typename TImageType::IndexType > &indeces)
  {
    std::vector< typename TImageType::PixelType > returnVector;
    returnVector.resize(indeces.size()); // pre-allocation done for speed

    typedef itk::ImageRegionIterator< TImageType > IteratorType;
    IteratorType imageIterator(inputImage, inputImage->GetBufferedRegion());

    // made parallel for efficiency
    int threads = omp_get_max_threads(); // obtain maximum number of threads available on machine  
    threads > returnVector.size() ? threads = returnVector.size() : threads = threads;
//#pragma omp parallel for num_threads(threads)
    for (int i = 0; i < returnVector.size(); i++)
    {
      imageIterator.SetIndex(indeces[i]);
      returnVector[i] = imageIterator.Get();
    }

    return returnVector;
  }

  /**
  \brief Wrap of GetPixelValues
  */
  template < typename TImageType = ImageTypeFloat3D >
  std::vector< typename TImageType::PixelType > ExtractPixelValuesFromIndeces(const typename TImageType::Pointer inputImage, const std::vector< typename TImageType::IndexType > &indeces)
  {
    return GetPixelValuesFromIndeces< TImageType >(inputImage, indeces);
  }
  
  /**
  \brief Get MD5 sum of a supplied file

  \param fileName The input file
  \return The MD5 checksum
  */
  std::string GetMD5Sum(const std::string &fileName)
  {
    gdcm::MD5 md5Computer;
    char digStr[MAX_PATH];
    md5Computer.ComputeFile(fileName.c_str(), digStr);
    return std::string(digStr);
  }

  /**
  \brief Wrap of GetMD5Sum()
  */
  std::string ComputeMD5Sum(const std::string &fileName)
  {
    return GetMD5Sum(fileName);
  }

  /**
  \brief Get the indeces of the image which are not zero
  */
  template < typename TImageType = ImageTypeFloat3D >
  std::vector< typename TImageType::IndexType > GetIndexFromNonZeroPixels(const typename TImageType::Pointer inputImage, const std::string valuesToExclude = "0")
  {
    std::vector< typename TImageType::IndexType > returnVector;

    itk::ImageRegionConstIterator< TImageType > iterator(inputImage, inputImage->GetLargestPossibleRegion());
    for (iterator.GoToBegin(); !iterator.IsAtEnd();++iterator)
    {
      if (iterator.Get() != 0)
      {
        returnVector.push_back(iterator.GetIndex());
      }
    }

    return returnVector;
  }

  /**
  \brief Get the indeces of the image which are not zero

  \param inputImage The input image on which the matching needs to be done
  \param referenceImage The reference image based on which the 
  \param numberOfMatchPoints Governs the number of quantile values to be matched
  \param numberOfHistogramLevels Sets the number of bins used when creating histograms of the source and reference images
  */
  template < typename TImageType = ImageTypeFloat3D >
  typename TImageType::Pointer GetHistogramMatchedImage(const typename TImageType::Pointer inputImage, const typename TImageType::Pointer referenceImage, 
    const int numberOfMatchPoints = 40, const int numberOfHistogramLevels = 100)
  {
    auto filter = itk::HistogramMatchingImageFilter< TImageType, TImageType >::New();
    filter->SetInput(inputImage);
    filter->SetReferenceImage(referenceImage);
    if (numberOfHistogramLevels != 100)
    {
      filter->SetNumberOfHistogramLevels(numberOfHistogramLevels);
    }
    filter->ThresholdAtMeanIntensityOn();
    filter->SetNumberOfMatchPoints(numberOfMatchPoints);
    filter->Update();

    return filter->GetOutput();
  }

  /**
  \brief Get the indeces of the image which are not zero

  \param inputImage The input image on which the matching needs to be done
  \param referenceImage The reference image based on which the
  \param alpha Ranges between 0-1; with 1 giving result same as input image and lower values behaving as unsharp filters; default = 0.3
  \param beta Ranges between 0-1; with 1 giving result same as input image and lower values behaving as unsharp filters; default = 0.3
  \param radius Ranges between 1-10 with default = 1
  */
  template < typename TImageType = ImageTypeFloat3D >
  typename TImageType::Pointer GetAdaptiveHistogramEqualizedImage(const typename TImageType::Pointer inputImage, const typename TImageType::Pointer referenceImage,
    const float alpha = 0.3, const float beta = 0.3, const float radius = 1, const int numberOfHistogramLevels = 100)
  {
    auto filter = itk::AdaptiveHistogramEqualizationImageFilter< TImageType, TImageType >::New();
    filter->SetInput(inputImage);
    filter->SetAlpha(alpha);
    filter->SetBeta(beta);
    filter->SetRadius(radius);
    filter->Update();

    return filter->GetOutput();
  }

  /**
  \brief Perform the deformable registration

  \param movingImage The moving image for registration
  \param referenceImage The reference image for registration
  \param multiResLevels Number of multi-resolution levels for registration, defaults to 5
  \param iterationStart Start size of iteration for first multiResLevel, defaults to 10
  \param iterationStep Step size of the iterations increasing over each MultiResLevel, defaults to 10
  \param iterationEnd End size of iteration for first multiResLevel, defaults to 50
  \param regType The type of registration to perform, defaults to 'Demons'
  \param interpolatorType The type of interpolator to use, defaults to 'Linear'
  */
  template< class TImageType = ImageTypeFloat3D >
  typename TImageType::Pointer GetDeformRegisteredImage(const typename TImageType::Pointer movingImage, const typename TImageType::Pointer referenceImage,
    const unsigned int multiResLevels = 5, 
    const unsigned int iterationStart = 10, const unsigned int iterationStep = 10, const unsigned int iterationEnd = 50,
    const int regType = Demons, const int interpolatorType = Linear)
  {
    // do histogram matchin of the 2 images
    auto movingImage_histoMatched = GetHistogramMatchedImage< TImageType >(movingImage, referenceImage, 40, 1024);

    // setup the displacement field
    using VectorPixelType = itk::Vector< float, TImageType::ImageDimension >;
    using DisplacementFieldType =  itk::Image< VectorPixelType, TImageType::ImageDimension >;

    auto multiRes = typename itk::MultiResolutionPDEDeformableRegistration< TImageType, TImageType, DisplacementFieldType >::New();

    // set the registration type
    switch (regType)
    {
    case DiffeomorphicDemons:
    {
      auto filter = typename itk::DiffeomorphicDemonsRegistrationFilter< TImageType, TImageType, DisplacementFieldType >::New();
      filter->SetStandardDeviations(1.0);
      multiRes->SetRegistrationFilter(filter);
      break;
    }
    case SymmetricForcesDemons:
    {
      auto filter = typename itk::SymmetricForcesDemonsRegistrationFilter< TImageType, TImageType, DisplacementFieldType >::New();
      filter->SetStandardDeviations(1.0);
      multiRes->SetRegistrationFilter(filter);
      break;
    }
    case FastSymmetricForcesDemons:
    {
      auto filter = typename itk::FastSymmetricForcesDemonsRegistrationFilter< TImageType, TImageType, DisplacementFieldType >::New();
      filter->SetStandardDeviations(1.0);
      multiRes->SetRegistrationFilter(filter);
      break;
    }
    default: // does Demons
    {
      auto filter = typename itk::DemonsRegistrationFilter< TImageType, TImageType, DisplacementFieldType >::New();
      filter->SetStandardDeviations(1.0);
      multiRes->SetRegistrationFilter(filter);
      break;
    }
    }

    // set the different parameters of the multi resolution registration
    multiRes->SetNumberOfLevels(multiResLevels);
    multiRes->SetFixedImage(referenceImage);
    multiRes->SetMovingImage(movingImage_histoMatched);

    // set up the iterations
    std::vector< unsigned int > iterations_vector;
    for (size_t i = iterationStart; i <= iterationEnd; i += iterationStep)
    {
      iterations_vector.push_back(i);
    }
    unsigned int *iterations_array = &iterations_vector[0];
    multiRes->SetNumberOfIterations(iterations_array);

    // start the regisration
    try
    {
      multiRes->Update();
    }
    catch (itk::ExceptionObject & excp)
    {
      std::cerr << excp << std::endl;
      return referenceImage;
    }
    
    // warp the moving image
    auto warper = typename itk::WarpImageFilter< TImageType, TImageType, DisplacementFieldType >::New();
    warper->SetInput(movingImage);
    warper->SetOutputSpacing(referenceImage->GetSpacing());
    warper->SetOutputOrigin(referenceImage->GetOrigin());
    warper->SetOutputDirection(referenceImage->GetDirection());
    warper->SetDisplacementField(multiRes->GetOutput());

    // set up the interpolator type
    switch (interpolatorType)
    {
    case NearestNeighbor:
    {
      auto interpolator = typename itk::NearestNeighborInterpolateImageFunction< TImageType, double >::New();
      warper->SetInterpolator(interpolator);
      break;
    }
    case BSpline:
    {
      auto interpolator = typename itk::BSplineInterpolateImageFunction< TImageType, double >::New();
      warper->SetInterpolator(interpolator);
      break;
    }
    default: // linear by default
    {
      auto interpolator = typename itk::LinearInterpolateImageFunction< TImageType, double >::New();
      warper->SetInterpolator(interpolator);
      break;
    }
    }

    // perform the warping
    try
    {
      warper->Update();
    }
    catch (itk::ExceptionObject & excp)
    {
      std::cerr << excp << std::endl;
      return referenceImage;
    }

    return warper->GetOutput();
  }


}
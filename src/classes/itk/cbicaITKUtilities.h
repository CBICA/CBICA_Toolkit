/**
\file  cbicaITKUtilities.h

\brief Some basic utility functions.

Dependecies: ITK, OpenCV, OpenMP

https://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include <algorithm>
#include <functional>
#include <omp.h>

#include "itkImage.h"
#include "itkImageRegionIterator.h"
#include "gdcmMD5.h"

#include "opencv2/core/core.hpp"

//#include "cbicaDefinitions.h"

//typedef itk::Image<double, 3> TImageType;

namespace cbica
{
  /**
  \brief Vectorizes a set of images

  \param inputSubjectsAndImages The subjects and images which are to be vectorized
  \param maskImages The masks to be used for the input images
  \param columnMajor If true, image intensities are converted to column vectors; otherwise they are converted to row vectors; and then they are concatinated
  \param appendInputImagesFromSubjects If true, concatenate all image voxels together in a single column/row
  \param maskDefinedPerSubject If true, the mask is defined on a per-subject basis instead of per-modality
  \return An OpenCV Mat: size is [inputSubjectsAndImages[i][j]->GetLargestPossibleRegion().GetSize(), inputSubjectsAndImages[j].size()] if columnMajor is true; transpose otherwise
  */
  template< class TImageType >
  cv::Mat VectorizeImages(const std::vector< std::vector< typename TImageType::Pointer > > inputSubjectsAndImages, 
    const std::vector< typename TImageType::Pointer > maskImages, 
    const bool columnMajor, const bool appendInputImagesFromSubjects, const bool maskDefinedPerSubject)
  {
    if (inputSubjectsAndImages.size() != maskImages.size())
    {
      std::cerr << "The number of input and mask images do not match.\n";
      exit(EXIT_FAILURE);
    }
    cv::Mat returnMat;

    for (size_t i = 0; i < inputSubjectsAndImages.size(); i++)
    {
      std::vector< float > tempMat; //tempMat is ALWAYS initialized to float because OpenCV functions are defined for float
      for (size_t j = 0; j < inputSubjectsAndImages[i].size(); j++)
      {
        if (!appendInputImagesFromSubjects)
        {
          tempMat.clear(); // re-initialize tempMat if intensities from different modalities are not to be concatenated
        }
        int maskIndexToConsider = j;
        if (maskDefinedPerSubject)
        {
          maskIndexToConsider = i;
        }

        itk::ImageRegionIterator<TImageType>
          inputImageIterator(inputSubjectsAndImages[i][j], inputSubjectsAndImages[i][j]->GetLargestPossibleRegion()),
          maskImageIterator(maskImages[maskIndexToConsider], maskImages[maskIndexToConsider]->GetLargestPossibleRegion());

        maskImageIterator.GoToBegin();

        while (!maskImageIterator.IsAtEnd())
        {
          if (maskImageIterator.Get() != static_cast<typename TImageType::PixelType>(0))
          {
            inputImageIterator.SetIndex(maskImageIterator.GetIndex());
            tempMat.push_back(static_cast<float>(inputImageIterator.Get()));
          }
          ++maskImageIterator;
        }
        if (!appendInputImagesFromSubjects)
        {
          returnMat.push_back(tempMat); 
        }
      }
      if (appendInputImagesFromSubjects)
      {
        returnMat.push_back(tempMat);
      }
    }

    if (columnMajor)
    {
      cv::transpose(returnMat, returnMat);
    }
    return returnMat;
  }

  /**
  \brief Vectorizes a set of images

  \param inputSubjectsAndImages The subjects and images which are to be vectorized
  \param maskIndeces The mask indeces to be used for the input images
  \param columnMajor If true, image intensities are converted to column vectors; otherwise they are converted to row vectors; and then they are concatinated
  \param appendInputImagesFromSubjects If true, concatenate all image voxels together in a single column/row
  \param maskDefinedPerSubject If true, the mask is defined on a per-subject basis instead of per-modality
  \return An OpenCV Mat: size is [inputSubjectsAndImages[i][j]->GetLargestPossibleRegion().GetSize(), inputSubjectsAndImages[j].size()] if columnMajor is true; transpose otherwise
  */
  template< class TImageType >
  cv::Mat VectorizeImages(const std::vector< std::vector< typename TImageType::Pointer > > inputSubjectsAndImages,
    const std::vector< std::vector< typename TImageType::IndexType > > maskIndeces,
    const bool appendInputImagesFromSubjects = false, const bool columnMajor = false, const bool maskDefinedPerSubject = false)
  {
    if (maskDefinedPerSubject)
    {
      if (inputSubjectsAndImages.size() != maskIndeces.size())
      {
        std::cerr << "The number of input and mask images do not match.\n";
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      if (inputSubjectsAndImages[0].size() != maskIndeces.size())
      {
        std::cerr << "The number of input and mask images do not match.\n";
        exit(EXIT_FAILURE);
      }
    }
    cv::Mat returnMat; // pre-allocate this and then parallelize the next big i-loop

    for (size_t i = 0; i < inputSubjectsAndImages.size(); i++)
    {
      cv::Mat tempMat; //tempMat is ALWAYS initialized to float because OpenCV functions are defined for float
      for (size_t j = 0; j < inputSubjectsAndImages[i].size(); j++)
      {
        if (!appendInputImagesFromSubjects)
        {
          tempMat.empty(); // re-initialize tempMat if intensities from different modalities are not to be concatenated
        }
        itk::ImageRegionIterator<TImageType>
          inputImageIterator(inputSubjectsAndImages[i][j], inputSubjectsAndImages[i][j]->GetLargestPossibleRegion());

        int maskIndexToConsider = j;
        if (maskDefinedPerSubject)
        {
          maskIndexToConsider = i;
        }
        for (size_t k = 0; k < maskIndeces[maskIndexToConsider].size(); k++)
        {
          inputImageIterator.SetIndex(maskIndeces[maskIndexToConsider][k]);
          tempMat.push_back(static_cast<float>(inputImageIterator.Get()));
        }
        if (!appendInputImagesFromSubjects)
        {
          returnMat.push_back(tempMat.t());
        }
      }
      if (appendInputImagesFromSubjects)
      {
        returnMat.push_back(tempMat.t());
      }
    }

    if (columnMajor)
    {
      return returnMat.t();
    }
    return returnMat;
  }

  /**
  \brief Vectorizes a set of images

  \param inputSubjectsAndImages The images which are to be vectorized
  \param columnMajor If true, image intensities are converted to column vectors; otherwise they are converted to row vectors; and then they are concatinated
  \param appendInputImagesFromSubjects If true, concatenate all image voxels together in a single column/row
  \return An OpenCV Mat: size is [inputSubjectsAndImages[i]->GetLargestPossibleRegion().GetSize(), inputSubjectsAndImages.size()] if columnMajor is true; transpose otherwise
  */
  template< class TImageType >
  cv::Mat VectorizeImages(const std::vector< std::vector< typename TImageType::Pointer > > inputSubjectsAndImages,
    const bool columnMajor = false, const bool appendInputImagesFromSubjects = false)
  {
    cv::Mat returnMat; // pre-allocate this and then parallelize the next big i-loop

    for (size_t i = 0; i < inputSubjectsAndImages.size(); i++)
    {
      std::vector< float > tempMat; //tempMat is ALWAYS initialized to float because OpenCV functions are defined for float
      for (size_t j = 0; j < inputSubjectsAndImages[i].size(); j++)
      {
        if (!appendInputImagesFromSubjects)
        {
          tempMat.clear(); // re-initialize tempMat if intensities from different modalities are not to be concatenated
        }
        itk::ImageRegionIterator<TImageType>
          inputImageIterator(inputSubjectsAndImages[i][j], inputSubjectsAndImages[i][j]->GetLargestPossibleRegion());

        while (!inputImageIterator.IsAtEnd())
        {
          // should there be a check put here regarding loads of zero-intensities for images that don't have any mask?
          tempMat.push_back(static_cast<float>(inputImageIterator.Get()));
          ++inputImageIterator;
        }
        if (!appendInputImagesFromSubjects)
        {
          returnMat.push_back(tempMat);
        }
      }
      if (appendInputImagesFromSubjects)
      {
        returnMat.push_back(tempMat);
      }
    }

    if (columnMajor)
    {
      cv::transpose(returnMat, returnMat);
    }
    return returnMat;
  }

  /**
  \brief Calculate and preserve the mask indeces

  \param inputModalitiesAndImages A collection of images which are stored in a per-modality basis
  \return A collection of indeces which constitute the non-zero locations per modality
  */
  template< class TImageType >
  std::vector< std::vector< typename TImageType::IndexType > > createMaskIndeces(const std::vector< std::vector< typename TImageType::Pointer > > &inputModalitiesAndImages)
  {
    std::vector< std::vector< typename TImageType::IndexType > > returnMaskIndeces;
    returnMaskIndeces.resize(inputModalitiesAndImages.size());

    // start data processing
    // made parallel for efficiency
    int threads = omp_get_max_threads(); // obtain maximum number of threads available on machine  
    threads > inputModalitiesAndImages.size() ? threads = inputModalitiesAndImages.size() : threads = threads;
#pragma omp parallel for num_threads(threads)
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
  \brief Normalize a vector based on its L2 norm

  \param inputVector The vector to normalize
  \return The normalized vector
  */
  template< typename TDataType >
  std::vector< TDataType > L2normalize(const std::vector< TDataType > &inputVector)
  {
    std::vector< TDataType > returnVector;
    float norm = cv::norm(inputVector, cv::NORM_L2);
    if (norm == 0)
    {
      std::cerr << "Calculated norm = 0, please check data.\n";
      exit(EXIT_FAILURE);
    }
    returnVector.resize(inputVector.size());
    std::transform(inputVector.begin(), inputVector.end(), returnVector.begin(), std::bind1st(std::divides< float >(), norm));
    
    return returnVector;
  }

  /**
  \brief Wrap of OpenCV's multiply function
  */
  cv::Mat multiply(const cv::Mat &input1, const cv::Mat &input2)
  {
    cv::Mat returnMat;
    cv::multiply(input1, input2, returnMat);
    return returnMat;
  }

  /**
  \brief Wrap of OpenCV's multiply function
  */
  cv::Mat multiply(const cv::Mat &input1, const double input2)
  {
    cv::Mat returnMat;
    cv::multiply(input1, input2, returnMat);
    return returnMat;
  }

  /**
  \brief Wrap of OpenCV's add function
  */
  cv::Mat add(const cv::InputArray &input1, const cv::InputArray &input2)
  {
    cv::Mat returnMat;
    cv::add(input1, input2, returnMat);
    return returnMat;
  }

  /**
  \brief Wrap of OpenCV's invert function
  */
  cv::Mat invert(const cv::InputArray &input)
  {
    cv::Mat returnMat;
    cv::invert(input, returnMat);
    return returnMat;
  }

  /**
  \brief Wrap of OpenCV's transpose function
  */
  cv::Mat transpose(const cv::InputArray &input)
  {
    cv::Mat returnMat;
    cv::transpose(input, returnMat);
    return returnMat;
  }

  /**
  \brief Wrap of OpenCV's subtract function
  */
  cv::Mat subtract(const cv::InputArray &input1, const cv::InputArray &input2)
  {
    cv::Mat returnMat;
    cv::subtract(input1, input2, returnMat);
    return returnMat;
  }

  /**
  \brief Get Pixel Values of specified indeces of input Image

  \param inputImage The input image in itk::Image format
  \param indeced The indeces from which pixel values need to be extracted
  \return Vector of values whose data type is the same as image type
  */
  template < typename TImageType>
  std::vector< typename TImageType::PixelType > extractPixelValues(const typename TImageType::Pointer inputImage, const std::vector< itk::Index<TImageType::ImageDimension> > &indeces)
  {
    std::vector< typename TImageType::PixelType > returnVector;
    returnVector.resize(indeces.size());

    typedef itk::ImageRegionIterator< TImageType > IteratorType;
    IteratorType imageIterator(inputImage, inputImage->GetBufferedRegion());

    // made parallel for efficiency
    int threads = omp_get_max_threads(); // obtain maximum number of threads available on machine  
    threads > returnVector.size() ? threads = returnVector.size() : threads = threads;
#pragma omp parallel for num_threads(threads)
    for (int i = 0; i < returnVector.size(); i++)
    {
      imageIterator.SetIndex(indeces[i]);
      returnVector[i] = imageIterator.Get();
    }

    return returnVector;
  }

  std::string computeMD5Sum(const std::string &fileName)
  {
    gdcm::MD5 md5Computer;
    char digStr[_MAX_PATH];
    md5Computer.ComputeFile(fileName.c_str(), digStr);
    return std::string(digStr);
  }
}
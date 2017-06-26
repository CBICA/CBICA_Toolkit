/**
\file  cbicaITKOpenCVUtilities.h

\brief Some basic utility functions to be used along with OpenCV.

Dependecies: ITK, OpenCV, OpenMP

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
#include "gdcmMD5.h"

#include "opencv2/core/core.hpp"

//#include "cbicaDefinitions.h"

//typedef itk::Image<double, 3> TImageType;

/*
\namespace cbica
\brief Namespace for differentiating functions written for internal use
*/
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
  template< class TImageType
#if (_MSC_VER >= 1800) || (__GNUC__ > 4)
    = itk::Image< float, 3 >
#endif
  >
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
  template< class TImageType
#if (_MSC_VER >= 1800) || (__GNUC__ > 4)
    = itk::Image< float, 3 >
#endif
      >
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
  template< class TImageType
#if (_MSC_VER >= 1800) || (__GNUC__ > 4)
    = itk::Image< float, 3 >
#endif
        >
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
  \brief Normalize a vector based on its L2 norm

  \param inputVector The vector to normalize
  \return The normalized vector
  */
  template< typename TDataType
#if (_MSC_VER >= 1800) || (__GNUC__ > 4)
    = double
#endif
      >
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
  cv::Mat Multiply(const cv::Mat &input1, const cv::Mat &input2)
  {
    cv::Mat returnMat;
    cv::multiply(input1, input2, returnMat);
    return returnMat;
  }

  /**
  \brief Wrap of OpenCV's multiply function
  */
  cv::Mat Multiply(const cv::Mat &input1, const double input2)
  {
    cv::Mat returnMat;
    cv::multiply(input1, input2, returnMat);
    return returnMat;
  }

  /**
  \brief Wrap of OpenCV's add function
  */
  cv::Mat Add(const cv::InputArray &input1, const cv::InputArray &input2)
  {
    cv::Mat returnMat;
    cv::add(input1, input2, returnMat);
    return returnMat;
  }

  /**
  \brief Wrap of OpenCV's invert function
  */
  cv::Mat Invert(const cv::InputArray &input)
  {
    cv::Mat returnMat;
    cv::invert(input, returnMat);
    return returnMat;
  }

  /**
  \brief Wrap of OpenCV's transpose function
  */
  cv::Mat Transpose(const cv::InputArray &input)
  {
    cv::Mat returnMat;
    cv::transpose(input, returnMat);
    return returnMat;
  }

  /**
  \brief Wrap of OpenCV's subtract function
  */
  cv::Mat Subtract(const cv::InputArray &input1, const cv::InputArray &input2)
  {
    cv::Mat returnMat;
    cv::subtract(input1, input2, returnMat);
    return returnMat;
  }
}
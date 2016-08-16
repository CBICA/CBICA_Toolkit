/**
\file  cbicaITKUtilities.h

\brief Some basic utility functions.

Dependecies: ITK, OpenMP

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

//typedef itk::Image<double, 3> TImageType;

namespace cbica
{
  /**
  \brief Calculate and preserve the mask indeces

  \param inputModalitiesAndImages A collection of images which are stored in a per-modality basis (each entry corresponds to a subject, whose entries contain different modalities)
  \return A collection of indeces which constitute the non-zero locations per modality (each entry corresponds to a subject, which contains the locations of non-zero pixel values for all modalities)
  */
  template< class TImageType
#if (_MSC_VER >= 1800) || (__GNUC__ > 4)
    = itk::Image< float, 3 >
#endif
  >
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
  template < typename TImageType
#if (_MSC_VER >= 1800) || (__GNUC__ > 4)
    = itk::Image< float, 3 >
#endif
  >
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
  template < typename TImageType
#if (_MSC_VER >= 1800) || (__GNUC__ > 4)
    = itk::Image< float, 3 >
#endif
  >
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
}
/**
\file  cbicaDtiRecon.h

\brief Declaration of DtiRecon
 
https://www.cbica.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/stat.h>

#include "itkTensorFractionalAnisotropyImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMetaDataDictionary.h"
#include "itkImageSeriesReader.h"
#include "itkDisplacementFieldJacobianDeterminantFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkNiftiImageIO.h"
#include "itkNrrdImageIO.h"
#include "itkImageMaskSpatialObject.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkNumericTraits.h"
#include "itkCommand.h"
#include "itkDiffusionTensor3DReconstructionImageFilter.h"

#include "cbicaITKCommonHolder.h"
#include "cbicaLogging.h"

namespace cbica
{

  template< typename TOutputImageType, typename TTensorImageType >
  inline void allocateImage(typename TOutputImageType::Pointer scIm, const typename TTensorImageType::Pointer tenIm)
  {
    scIm->SetOrigin(tenIm->GetOrigin());
    scIm->SetSpacing(tenIm->GetSpacing());
    scIm->SetDirection(tenIm->GetDirection());
    scIm->SetLargestPossibleRegion(tenIm->GetLargestPossibleRegion());
    scIm->SetRequestedRegion(tenIm->GetRequestedRegion());
    scIm->SetBufferedRegion(tenIm->GetBufferedRegion());
    scIm->Allocate();
  }
  
  /**
  
  */
  template < typename TVectorImageType, typename TScalarImageType, typename TMaskImageType, typename TOutputTensorCompType>
  std::vector< typename TScalarImageType::Pointer > dtiRecon(typename TVectorImageType::Pointer inputImage, const typename TMaskImageType::Pointer maskImage, const std::vector< std::vector< float > >& gradValues, const float &bValue, std::string outputBasename, bool verbose, bool inputIsVectorImage)
  {
    const unsigned int Dimensions = 3;
    bool readb0 = false;
    double b0 = 0;
    typedef itk::VectorImage< typename TScalarImageType::PixelType, Dimensions > GradientImageType;
    //Set up the gradient image size
    GradientImageType::Pointer gradIm = inputImage; // why is this happening

    std::cout << "Converting DWI to vector image\n";

    typedef itk::DiffusionTensor3DReconstructionImageFilter< typename TScalarImageType::PixelType, 
      typename TScalarImageType::PixelType, TOutputTensorCompType > TensorReconstructionImageFilterType;
    // -------------------------------------------------------------------------

    TensorReconstructionImageFilterType::GradientDirectionType vect3d;
    TensorReconstructionImageFilterType::GradientDirectionContainerType::Pointer DiffusionVectors = TensorReconstructionImageFilterType::GradientDirectionContainerType::New();

    for (size_t i = 0; i < gradValues.size(); i++)
    {
      vec3d = gradValues[i];
      //vect3d[0] = gradValues[i][0];
      //vect3d[1] = gradValues[i][1];
      //vect3d[2] = gradValues[i][2];
      DiffusionVectors->InsertElement(i, vect3d);
    }
    // -------------------------------------------------------------------------

    TensorReconstructionImageFilterType::Pointer tensorReconstructionFilter = TensorReconstructionImageFilterType::New();

    typedef itk::ImageMaskSpatialObject< Dimensions > MaskType;
    MaskType::Pointer  spatialObjectMask = MaskType::New();
    typedef itk::BinaryThresholdImageFilter<TMaskImageType, MaskType::ImageType> ThresholderType;
    ThresholderType::Pointer thresholder = ThresholderType::New();
    thresholder->SetOutsideValue(itk::NumericTraits< MaskType::ImageType::PixelType>::Zero);
    thresholder->SetInsideValue(itk::NumericTraits< MaskType::ImageType::PixelType>::One);
    thresholder->SetLowerThreshold(itk::NumericTraits<unsigned short>::One);
    thresholder->InPlaceOn();

    thresholder->SetInput(maskImage);
    thresholder->Update();
    spatialObjectMask->SetImage(thresholder->GetOutput());
    //tensorReconstructionFilter->SetMaskSpatialObject(spatialObjectMask);

    //---------------------------------------------------------------------------------
    tensorReconstructionFilter->SetGradientImage(DiffusionVectors, gradIm);
    tensorReconstructionFilter->SetNumberOfThreads(1);
    tensorReconstructionFilter->SetBValue(static_cast<TensorReconstructionImageFilterType::TTensorPixelType>(bValue));
    //CommandProgressUpdate::Pointer observer = CommandProgressUpdate::New();
    //tensorReconstructionFilter->AddObserver(itk::ProgressEvent(), observer);
    tensorReconstructionFilter->Update();

    //----------------------------------------------------------------

    //typedef itk::ImageFileWriter<TensorReconstructionImageFilterType::OutputImageType > TensorWriterType;
    //TensorWriterType::Pointer tensorWriter = TensorWriterType::New();
    //tensorWriter->SetFileName("C:/Projects/SampleData/DTI/AALZOutput/tensorFile.nii.gz");
    //tensorWriter->SetInput(tensorReconstructionFilter->GetOutput());
    //tensorWriter->Update();

    //--------------------------computing scalars----------------------------
    std::string default_ext = ".nii.gz";
    static int writeFA = 1;
    static int writeTR = 1;
    static int writeEign = 0;
    static int writeGeo = 0;
    static int writeGordR = 0;
    static int writeGordK = 0;
    static int writeRadAx = 1;
    static int writeSkew = 0;
    static int writeKurt = 0;

    typedef itk::DiffusionTensor3D< typename TScalarImageType::PixelType > TensorPixelType;
    typedef itk::Image< TensorPixelType, Dimensions > TensorImageType;

    std::vector< TScalarImageType::Pointer > vectorOfDTIScalars;

    typedef itk::ImageRegionConstIteratorWithIndex < TensorImageType > ConstIterType;

    try
    {
      typename TensorImageType::Pointer tensorIm = TensorImageType::New();
      tensorIm = tensorReconstructionFilter->GetOutput();
      // Allocate each image that we will want to make..
      //FA
      typename TScalarImageType::Pointer faIm = ScalarImageType::New();

      //TR
      typename TScalarImageType::Pointer trIm = ScalarImageType::New();

      //Eigensys
      typename TScalarImageType::Pointer l1Im = ScalarImageType::New();
      typename TScalarImageType::Pointer l2Im = ScalarImageType::New();
      typename TScalarImageType::Pointer l3Im = ScalarImageType::New();
      typename TVectorImageType::Pointer v1Im = VectorImageType::New();
      typename TVectorImageType::Pointer v2Im = VectorImageType::New();
      typename TVectorImageType::Pointer v3Im = VectorImageType::New();

      //Skewness & Kurtosis
      typename TScalarImageType::Pointer skIm = ScalarImageType::New();
      typename TScalarImageType::Pointer kuIm = ScalarImageType::New();

      //Geometric Features
      typename TScalarImageType::Pointer clIm = ScalarImageType::New();
      typename TScalarImageType::Pointer cpIm = ScalarImageType::New();
      typename TScalarImageType::Pointer csIm = ScalarImageType::New();

      //Radial Axial
      typename TScalarImageType::Pointer rdIm = ScalarImageType::New();
      typename TScalarImageType::Pointer adIm = ScalarImageType::New();

      //Gordons R features
      typename TScalarImageType::Pointer r1Im = ScalarImageType::New();
      typename TScalarImageType::Pointer r2Im = ScalarImageType::New();
      typename TScalarImageType::Pointer r3Im = ScalarImageType::New();

      //Gordons K features
      typename TScalarImageType::Pointer k1Im = ScalarImageType::New();
      typename TScalarImageType::Pointer k2Im = ScalarImageType::New();
      typename TScalarImageType::Pointer k3Im = ScalarImageType::New();

      //Allocate all the images...
      if (writeFA)
        allocateImage<TScalarImageType, TensorImageType>(faIm, tensorIm);

      if (writeTR)
        allocateImage<TScalarImageType, TensorImageType>(trIm, tensorIm);

      if (writeEign)
      {
        allocateImage<TScalarImageType, TensorImageType>(l1Im, tensorIm);
        allocateImage<TScalarImageType, TensorImageType>(l2Im, tensorIm);
        allocateImage<TScalarImageType, TensorImageType>(l3Im, tensorIm);
        v1Im->SetVectorLength(3);
        v2Im->SetVectorLength(3);
        v3Im->SetVectorLength(3);
        allocateImage<TVectorImageType, TensorImageType>(v1Im, tensorIm);
        allocateImage<TVectorImageType, TensorImageType>(v2Im, tensorIm);
        allocateImage<TVectorImageType, TensorImageType>(v3Im, tensorIm);

      }

      if (writeSkew)
      {
        allocateImage<TScalarImageType, TensorImageType>(skIm, tensorIm);
      }

      if (writeKurt)
      {
        allocateImage<TScalarImageType, TensorImageType>(kuIm, tensorIm);
      }

      if (writeGeo)
      {
        allocateImage<TScalarImageType, TensorImageType>(clIm, tensorIm);
        allocateImage<TScalarImageType, TensorImageType>(cpIm, tensorIm);
        allocateImage<TScalarImageType, TensorImageType>(csIm, tensorIm);
      }
      if (writeRadAx)
      {
        allocateImage<TScalarImageType, TensorImageType>(rdIm, tensorIm);
        allocateImage<TScalarImageType, TensorImageType>(adIm, tensorIm);
      }
      if (writeGordR)
      {
        allocateImage<TScalarImageType, TensorImageType>(r1Im, tensorIm);
        allocateImage<TScalarImageType, TensorImageType>(r2Im, tensorIm);
        allocateImage<TScalarImageType, TensorImageType>(r3Im, tensorIm);
      }
      if (writeGordK)
      {
        allocateImage<TScalarImageType, TensorImageType>(k1Im, tensorIm);
        allocateImage<TScalarImageType, TensorImageType>(k2Im, tensorIm);
        allocateImage<TScalarImageType, TensorImageType>(k3Im, tensorIm);
      }
      //Loop though all the voxels and if compute the needed measures!
      ConstIterType iter(tensorIm, tensorIm->GetLargestPossibleRegion());
      for (iter.GoToBegin(); !iter.IsAtEnd(); ++iter)
      {
        TensorPixelType tmp = iter.Get();
        typename TensorPixelType::EigenValuesArrayType     lambda;
        typename TensorPixelType::EigenVectorsMatrixType   vMat;
        tmp.ComputeEigenAnalysis(lambda, vMat);

        typename ScalarImageType::IndexType index = iter.GetIndex();

        if (writeTR)
          trIm->SetPixel(index, lambda[0] + lambda[1] + lambda[2]);

        if (writeFA)
        {
          faIm->SetPixel(index, tmp.GetFractionalAnisotropy());
        }

        if (writeEign)
        {
          l1Im->SetPixel(index, lambda[2]);
          l2Im->SetPixel(index, lambda[1]);
          l3Im->SetPixel(index, lambda[0]);

          typename TVectorImageType::PixelType vec1(Dimensions);
          typename TVectorImageType::PixelType vec2(Dimensions);
          typename TVectorImageType::PixelType vec3(Dimensions);

          vec1[0] = vMat[2][0];
          vec1[1] = vMat[2][1];
          vec1[2] = vMat[2][2];

          vec2[0] = vMat[1][0];
          vec2[1] = vMat[1][1];
          vec2[2] = vMat[1][2];

          vec3[0] = vMat[0][0];
          vec3[1] = vMat[0][1];
          vec3[2] = vMat[0][2];

          v1Im->SetPixel(index, vec1);
          v2Im->SetPixel(index, vec2);
          v3Im->SetPixel(index, vec3);

        }

        if (writeSkew)
        {
          typename TScalarImageType::PixelType m1, m3, l1, l2, l3;
          l1 = abs(lambda[0]);
          l2 = abs(lambda[1]);
          l3 = abs(lambda[2]);
          m1 = (l1 + l2 + l3) / 3.0;

          if (m1 > 0)
          {
            m3 = (vcl_pow(l1 - m1, 3) + vcl_pow(l2 - m1, 3) + vcl_pow(l3 - m1, 3)) / (vcl_pow(l1, 3) + vcl_pow(l2, 3) + vcl_pow(l3, 3));
            if (m3 > 0)
            {
              skIm->SetPixel(index, vcl_pow(m3, static_cast<ScalarPixelType>(1.0 / 3.0)));
            }
            else
            {
              skIm->SetPixel(index, -1 * vcl_pow((-1 * m3), static_cast<ScalarPixelType>(1.0 / 3.0)));
            }
          }
          else
          {
            skIm->SetPixel(index, static_cast<ScalarPixelType>(0));
          }
        }

        if (writeKurt)
        {
          typename TScalarImageType::PixelType m1, m4, l1, l2, l3;
          l1 = abs(lambda[0]);
          l2 = abs(lambda[1]);
          l3 = abs(lambda[2]);
          m1 = (l1 + l2 + l3) / 3.0;
          if (m1 > 0)
          {
            m4 = (vcl_pow(l1 - m1, 4) + vcl_pow(l2 - m1, 4) + vcl_pow(l3 - m1, 4)) / (vcl_pow(l1, 4) + vcl_pow(l2, 4) + vcl_pow(l3, 4));
            kuIm->SetPixel(index, vcl_pow(m4, static_cast<ScalarPixelType>(1.0 / 4.0)));
          }
          else
          {
            kuIm->SetPixel(index, static_cast<ScalarPixelType>(0));
          }
        }

        if (writeGeo)
        {
          if (lambda[2] > 0)
          {
            clIm->SetPixel(index, (lambda[2] - lambda[1]) / lambda[2]);
            cpIm->SetPixel(index, (lambda[1] - lambda[0]) / lambda[2]);
            csIm->SetPixel(index, lambda[0] / lambda[2]);
          }
          else
          {
            clIm->SetPixel(index, 0);
            cpIm->SetPixel(index, 0);
            csIm->SetPixel(index, 0);
          }
        }

        if (writeRadAx)
        {
          rdIm->SetPixel(index, (lambda[1] + lambda[0]) / 2);
          adIm->SetPixel(index, lambda[2]);
        }

        if (writeGordR)
        {
          //Compute the moments...
          typename TScalarImageType::PixelType m1, m2, m3, r1, r2, r3;
          m1 = (lambda[0] + lambda[1] + lambda[2]) / 3.0;
          m2 = (vcl_pow(lambda[0] - m1, 2) + vcl_pow(lambda[1] - m1, 2)
            + vcl_pow(lambda[2] - m1, 2)) / 3.0;
          m3 = (vcl_pow(lambda[0] - m1, 3) + vcl_pow(lambda[1] - m1, 3)
            + vcl_pow(lambda[2] - m1, 3)) / 3.0;

          r1 = sqrt(3 * (vcl_pow(m1, 2) + m2));
          r2 = sqrt(3 * m2 / 2 / (vcl_pow(m1, 2) + m2));
          //        r3 = sqrt(2) * m3 / vcl_pow(static_cast<double>(m2), static_cast<double>(1.5));
          r3 = (lambda[0] * lambda[1] * lambda[2]) / vcl_pow(static_cast<double>(sqrt(3 * m2)), 3);

          r1Im->SetPixel(index, r1);
          r2Im->SetPixel(index, r2);
          r3Im->SetPixel(index, r3);
        }

        if (writeGordK)
        {
          //Compute the moments...
          typename TScalarImageType::PixelType m1, m2, m3, k1, k2, k3;

          m1 = (lambda[0] + lambda[1] + lambda[2]) / 3.0;
          m2 = (vcl_pow(lambda[0] - m1, 2) + vcl_pow(lambda[1] - m1, 2)
            + vcl_pow(lambda[2] - m1, 2)) / 3.0;
          m3 = (vcl_pow(lambda[0] - m1, 3) + vcl_pow(lambda[1] - m1, 3)
            + vcl_pow(lambda[2] - m1, 3)) / 3.0;

          k1 = 3 * m1;
          k2 = sqrt(3 * m2);
          //        k3 = sqrt(2) * m3 / vcl_pow(static_cast<double>(m2), static_cast<double>(1.5));
          k3 = (lambda[0] * lambda[1] * lambda[2]) / vcl_pow(static_cast<double>(sqrt(3 * m2)), 3);


          k1Im->SetPixel(index, k1);
          k2Im->SetPixel(index, k2);
          k3Im->SetPixel(index, k3);
        }
      }

      std::cout << "Done Computing Scalars\n";

      typedef itk::ImageFileWriter< TScalarImageType >  ScalarWriterType;
      typedef itk::ImageFileWriter< TVectorImageType >  VectorWriterType;

      if (writeFA)
        vectorOfDTIScalars.push_back(faIm);

      if (writeTR)
        vectorOfDTIScalars.push_back(trIm);

      if (writeRadAx)
      {
        vectorOfDTIScalars.push_back(rdIm);
        vectorOfDTIScalars.push_back(adIm);
      }
    }
    catch (itk::ExceptionObject & excp)
    {
    }
    return vectorOfDTIScalars;
  }

}

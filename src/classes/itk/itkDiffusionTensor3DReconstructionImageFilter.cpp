/**
\file  itkDiffusionTensor3DReconstructionImageFilter.cpp

\brief Implementation of DiffusionTensor3DReconstructionImageFilter
 
https://www.cbica.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/

#ifndef __itkDiffusionTensor3DReconstructionImageFilter_cpp
#define __itkDiffusionTensor3DReconstructionImageFilter_cpp

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkArray.h"
#include "vnl/vnl_vector.h"
#include "itkProgressReporter.h"

#include "itkDiffusionTensor3DReconstructionImageFilter.h"

namespace itk 
{

  template< class TReferenceImagePixelType,
            class TGradientImagePixelType, class TTensorPixelType >
  DiffusionTensor3DReconstructionImageFilter< TReferenceImagePixelType,
    TGradientImagePixelType, TTensorPixelType >
  ::DiffusionTensor3DReconstructionImageFilter()
  {
    // At least 1 inputs is necessary for a vector image.
    // For images added one at a time we need at least six
    this->SetNumberOfRequiredInputs( 1 );
    m_NumberOfGradientDirections = 0;
    m_NumberOfBaselineImages = 1;
    m_Threshold = NumericTraits< ReferencePixelType >::min();
    m_GradientImageTypeEnumeration = Else;
    m_GradientDirectionContainer = NULL;
    m_TensorBasis.set_identity();
    m_TensorBasisInverse.set_identity();
    m_BValue = 1.0;
    //~ m_CalculateResidualImage = false;
    m_CalculateResidualImage = true;
  }


  template< class TReferenceImagePixelType,
            class TGradientImagePixelType, class TTensorPixelType >
  void DiffusionTensor3DReconstructionImageFilter< TReferenceImagePixelType,
    TGradientImagePixelType, TTensorPixelType >
  ::BeforeThreadedGenerateData()
  {
    // If we have more than 2 inputs, then each input, except the first is a
    // gradient image. The number of gradient images must match the number of
    // gradient directions.
    const unsigned int numberOfInputs = this->GetNumberOfInputs();

    // There need to be at least 6 gradient directions to be able to compute the
    // tensor basis
    if( m_NumberOfGradientDirections < 6 )
      {
      itkExceptionMacro( << "At least 6 gradient directions are required" );
      }

    // If there is only 1 gradient image, it must be an itk::VectorImage. Otherwise
    // we must have a container of (numberOfInputs-1) itk::Image. Check to make sure
    if ( numberOfInputs == 1
        && m_GradientImageTypeEnumeration != GradientIsInASingleImage )
      {
      std::string gradientImageClassName(
          this->ProcessObject::GetInput(0)->GetNameOfClass());
      if ( strcmp(gradientImageClassName.c_str(),"VectorImage") != 0 )
        {
        itkExceptionMacro( <<
            "There is only one Gradient image. I expect that to be a VectorImage. "
            << "But its of type: " << gradientImageClassName );
        }
      }

    //Initialize the residualImage if we are going to calculate it.
    if (m_CalculateResidualImage)
    {
      std::cerr << "Allocating Residue Image" << std::endl;
      m_ResidualImage = ResidualImageType::New();
      m_ResidualImage->CopyInformation(this->ProcessObject::GetInput(0));
      m_ResidualImage->SetRegions(m_ResidualImage->GetLargestPossibleRegion() );
      m_ResidualImage->SetVectorLength( m_NumberOfGradientDirections );
    
      m_ResidualImage->Allocate();
      std::cerr << "Done Allocating Residue Image" << std::endl;
    }

    this->ComputeTensorBasis();
  }


  // POTENTIAL WARNING:
  //
  // Until we fix netlib svd routines, we will need to set the number of thread
  // to 1.
  template< class TReferenceImagePixelType,
            class TGradientImagePixelType, class TTensorPixelType >
  void DiffusionTensor3DReconstructionImageFilter< TReferenceImagePixelType,
    TGradientImagePixelType, TTensorPixelType >
  ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                         int threadId)
  {
    typename OutputImageType::Pointer outputImage =
              static_cast< OutputImageType * >(this->ProcessObject::GetOutput(0));

    ImageRegionIterator< OutputImageType > oit(outputImage, outputRegionForThread);
    oit.GoToBegin();

    ImageRegionIterator< ResidualImageType > rit;
  
    //if we are calculateing the Residual set up an iterator...
    if (m_CalculateResidualImage)
    {
      rit = ImageRegionIterator< ResidualImageType >( m_ResidualImage, outputRegionForThread);
      rit.GoToBegin();
    }

    vnl_vector<double> B(m_NumberOfGradientDirections);
    vnl_vector<double> D(6);

    // Support for progress methods/callbacks
    ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

    // Two cases here .
    // 1. If the Gradients have been specified in multiple images, we will create
    // 'n' iterators for each of the gradient images and solve the Stejskal-Tanner
    // equations for every pixel.
    // 2. If the Gradients have been specified in a single multi-component image,
    // one iterator will suffice to do the same.

    if( m_GradientImageTypeEnumeration == GradientIsInManyImages )
      {
      ImageRegionConstIteratorWithIndex< ReferenceImageType >
        it(static_cast< ReferenceImageType * >(this->ProcessObject::GetInput(0)),
           outputRegionForThread);
        it.GoToBegin();

      typedef ImageRegionConstIterator< GradientImageType > GradientIteratorType;
      std::vector< GradientIteratorType * > gradientItContainer;

      for( unsigned int i = 1; i<= m_NumberOfGradientDirections; i++ )
        {
        typename GradientImageType::Pointer gradientImagePointer = NULL;

        // Would have liked a dynamic_cast here, but seems SGI doesn't like it
        // The enum will ensure that an inappropriate cast is not done
        gradientImagePointer = static_cast< GradientImageType * >(
                                  this->ProcessObject::GetInput(i) );

        GradientIteratorType *git = new GradientIteratorType(
                            gradientImagePointer, outputRegionForThread );
        git->GoToBegin();
        gradientItContainer.push_back(git);
        }

      // Iterate over the reference and gradient images and solve the steskal
      // equations to reconstruct the Diffusion tensor.
      // See splweb.bwh.harvard.edu:8000/pages/papers/westin/ISMRM2002.pdf
      // "A Dual Tensor Basis Solution to the Stejskal-Tanner Equations for DT-MRI"

      while( !it.IsAtEnd() )
        {

        TensorPixelType tensor(0.0);

        //If we aren't in the mask move on!
        if ( this->m_ImageMask )
        {
          typename ReferenceImageType::IndexType index = it.GetIndex();
          typename ReferenceImageType::PointType inputPoint;
          (static_cast< ReferenceImageType * >
            (this->ProcessObject::GetInput(0)))
              ->TransformIndexToPhysicalPoint( index, inputPoint );

          if ( !this->m_ImageMask->IsInside( inputPoint ) )
          {
            oit.Set( tensor );
            ++oit;
            ++it;
            //Iterate all the gradient iterators
            for( unsigned int i = 0; i< m_NumberOfGradientDirections; i++ )
            {
              ++(*gradientItContainer[i]);
            }
          
            //iterate the residue iterator
            if (m_CalculateResidualImage)
            {
              ++rit;
            }

            progress.CompletedPixel();
            continue;
          }
        }
        ReferencePixelType b0 = it.Get();

        if( (b0 != 0) && (b0 >= m_Threshold) )
        {
          for( unsigned int i = 0; i< m_NumberOfGradientDirections; i++ )
          {
            GradientPixelType b = gradientItContainer[i]->Get();

            if( b == 0 )
            {
              B[i] = 0;
            }
            else
            {
              B[i] = -log( static_cast<double>(b) / static_cast<double>(b0) )/this->m_BValue;
            }

            ++(*gradientItContainer[i]);
          }

          //vnl_svd< double > pseudoInverseSolver( m_TensorBasis );
          if( m_NumberOfGradientDirections > 6 )
          {
  //          D = pseudoInverseSolver.solve( m_BMatrix * B );
            D =  m_TensorBasisInverse * ( m_BMatrix * B );
          }
          else
          {
  //          D = pseudoInverseSolver.solve( B );
            D =  m_TensorBasisInverse *  B;
          }

          tensor(0,0) = D[0];
          tensor(0,1) = D[1];
          tensor(0,2) = D[2];
          tensor(1,1) = D[3];
          tensor(1,2) = D[4];
          tensor(2,2) = D[5];

         //CHECK FOR SPD
          TensorEigenValuesType     lambda;
          TensorEigenVectorsType    eigenVectors;

          tensor.ComputeEigenAnalysis(lambda, eigenVectors);
          bool tensorIsNotSpd = false;
          //Check all the eigenvalues
          for (unsigned int r=0; r<3; ++r)
          {
            if (lambda[r] <= 0)
            {
              tensorIsNotSpd = true;
              lambda[r] = -lambda[r];
            }
          }

          //if its notSPD then reconstitue the tensor..
          if (tensorIsNotSpd)
          {
            TensorMatrixType  diag;
            for (unsigned int r=0; r<3; ++r)
            {
              diag[r][r] = lambda[r];
            }

            TensorMatrixType tmp
              = (static_cast<TensorMatrixType>(eigenVectors.GetTranspose()) ) * (diag * eigenVectors);

            tensor[0] = tmp[0][0];
            tensor[1] = tmp[0][1];
            tensor[2] = tmp[0][2];
            tensor[3] = tmp[1][1];
            tensor[4] = tmp[1][2];
            tensor[5] = tmp[2][2];
          }
        }
        else
          {
          for( unsigned int i = 0; i< m_NumberOfGradientDirections; i++ )
            {
            ++(*gradientItContainer[i]);
            }
          }
        if (m_CalculateResidualImage)
        {
          rit.Set( ComputeResidual( B, tensor, static_cast<double>(b0) ) );
          ++rit;
        }
        oit.Set( tensor );
        ++oit;
        ++it;
        progress.CompletedPixel();
        }

      for( unsigned int i = 0; i< gradientItContainer.size(); i++ )
        {
        delete gradientItContainer[i];
        }
      }
    // The gradients are specified in a single multi-component image
    else if( m_GradientImageTypeEnumeration == GradientIsInASingleImage )
      {
      typedef ImageRegionConstIteratorWithIndex< GradientImagesType > GradientIteratorType;
      typedef typename GradientImagesType::PixelType         GradientVectorType;
      typename GradientImagesType::Pointer gradientImagePointer = NULL;

      // Would have liked a dynamic_cast here, but seems SGI doesn't like it
      // The enum will ensure that an inappropriate cast is not done
      gradientImagePointer = static_cast< GradientImagesType * >(
                                this->ProcessObject::GetInput(0) );

      GradientIteratorType git(gradientImagePointer, outputRegionForThread );
      git.GoToBegin();

      // Compute the indicies of the baseline images and gradient images
      std::vector<unsigned int> baselineind; // contains the indicies of
                                             // the baseline images
      std::vector<unsigned int> gradientind; // contains the indicies of
                                             // the gradient images

      for(GradientDirectionContainerType::ConstIterator gdcit = this->m_GradientDirectionContainer->Begin();
          gdcit != this->m_GradientDirectionContainer->End(); ++gdcit)
        {
        if(gdcit.Value().one_norm() <= 0.0)
          {
          baselineind.push_back(gdcit.Index());
          }
        else
          {
          gradientind.push_back(gdcit.Index());
          }
        }

      while( !git.IsAtEnd() )
        {
        TensorPixelType tensor(0.0);

        //If we aren't in the mask move on!
        if ( this->m_ImageMask )
        {
          typename GradientImagesType::IndexType index = git.GetIndex();
          typename GradientImagesType::PointType inputPoint;
          gradientImagePointer->TransformIndexToPhysicalPoint( index, inputPoint );

          if ( !this->m_ImageMask->IsInside( inputPoint ) )
          {
            oit.Set( tensor );
            ++oit;
            ++git;
            //iterate the residue iterator
            if (m_CalculateResidualImage)
            {
              ++rit;
            }

            progress.CompletedPixel();
            continue;
          }
        }

        GradientVectorType b = git.Get();

        typename NumericTraits<ReferencePixelType>::AccumulateType b0 = NumericTraits<ReferencePixelType>::Zero;

        // Average the baseline image pixels
        for(unsigned int i = 0; i < baselineind.size(); ++i)
          {
          b0 += b[baselineind[i]];
          }
        b0 /= this->m_NumberOfBaselineImages;

        if( (b0 != 0) && (b0 >= m_Threshold) )
          {
          for( unsigned int i = 0; i< m_NumberOfGradientDirections; i++ )
            {
            if( b[gradientind[i]] == 0 )
              {
              B[i] = 0;
              }
            else
              {
              B[i] = -log( static_cast<double>(b[gradientind[i]]) / static_cast<double>(b0) ) / this->m_BValue;
              }
            }

  //        vnl_svd< double > pseudoInverseSolver( m_TensorBasis );
          if( m_NumberOfGradientDirections > 6 )
            {
  //          D = pseudoInverseSolver.solve( m_BMatrix * B );
            D = m_TensorBasisInverse * ( m_BMatrix * B );
            }
          else
            {
  //          D = pseudoInverseSolver.solve( B );
            D = m_TensorBasisInverse * B ;
            }

          tensor(0,0) = D[0];
          tensor(0,1) = D[1];
          tensor(0,2) = D[2];
          tensor(1,1) = D[3];
          tensor(1,2) = D[4];
          tensor(2,2) = D[5];

          //CHECK FOR SPD
          TensorEigenValuesType     lambda;
          TensorEigenVectorsType    eigenVectors;

          tensor.ComputeEigenAnalysis(lambda, eigenVectors);
          bool tensorIsNotSpd = false;
          //Check all the eigenvalues
          for (unsigned int r=0; r<3; ++r)
          {
            if (lambda[r] <= 0)
            {
              tensorIsNotSpd = true;
              lambda[r] = -lambda[r];
            }
          }

          //if its notSPD then reconstitue the tensor..
          if (tensorIsNotSpd)
          {
            TensorMatrixType  diag;
            for (unsigned int r=0; r<3; ++r)
            {
              diag[r][r] = lambda[r];
            }

            TensorMatrixType tmp
              = (static_cast<TensorMatrixType>(eigenVectors.GetTranspose()) ) * (diag * eigenVectors);

            tensor[0] = tmp[0][0];
            tensor[1] = tmp[0][1];
            tensor[2] = tmp[0][2];
            tensor[3] = tmp[1][1];
            tensor[4] = tmp[1][2];
            tensor[5] = tmp[2][2];
          }

        }
        if (m_CalculateResidualImage)
        {
          //~ ResidualPixelType res = ComputeResidual( B,tensor );
          //~ std::cerr << res << std::endl;
          rit.Set( ComputeResidual( B, tensor, static_cast<double>(b0) ) );
          ++rit;
        }
        oit.Set( tensor );
        ++oit; // Output (reconstructed tensor image) iterator
        ++git; // Gradient  image iterator
        progress.CompletedPixel();

        }
      }

  }


  template< class TReferenceImagePixelType,
            class TGradientImagePixelType, class TTensorPixelType >
  void DiffusionTensor3DReconstructionImageFilter< TReferenceImagePixelType,
    TGradientImagePixelType, TTensorPixelType >
  ::ComputeTensorBasis()
  {
    if( m_NumberOfGradientDirections < 6 )
      {
      itkExceptionMacro( << "Not enough gradient directions supplied. Need to supply at least 6" );
      }

    // This is only important if we are using a vector image.  For
    // images added one at a time, this is not needed but doesn't hurt.
    std::vector<unsigned int> gradientind;
    for(GradientDirectionContainerType::ConstIterator gdcit = this->m_GradientDirectionContainer->Begin();
        gdcit != this->m_GradientDirectionContainer->End(); ++gdcit)
      {
      if(gdcit.Value().one_norm() > 0.0)
        {
        gradientind.push_back(gdcit.Index());
        }
      }

    m_BMatrix.set_size( m_NumberOfGradientDirections, 6 );
    for (unsigned int m = 0; m < m_NumberOfGradientDirections; m++)
      {
      m_BMatrix[m][0] =     m_GradientDirectionContainer->ElementAt(gradientind[m])[0] * m_GradientDirectionContainer->ElementAt(gradientind[m])[0];
      m_BMatrix[m][1] = 2 * m_GradientDirectionContainer->ElementAt(gradientind[m])[0] * m_GradientDirectionContainer->ElementAt(gradientind[m])[1];
      m_BMatrix[m][2] = 2 * m_GradientDirectionContainer->ElementAt(gradientind[m])[0] * m_GradientDirectionContainer->ElementAt(gradientind[m])[2];
      m_BMatrix[m][3] =     m_GradientDirectionContainer->ElementAt(gradientind[m])[1] * m_GradientDirectionContainer->ElementAt(gradientind[m])[1];
      m_BMatrix[m][4] = 2 * m_GradientDirectionContainer->ElementAt(gradientind[m])[1] * m_GradientDirectionContainer->ElementAt(gradientind[m])[2];
      m_BMatrix[m][5] =     m_GradientDirectionContainer->ElementAt(gradientind[m])[2] * m_GradientDirectionContainer->ElementAt(gradientind[m])[2];
      }

    if( m_NumberOfGradientDirections > 6 )
      {
      m_TensorBasis = m_BMatrix.transpose() * m_BMatrix;
      }
    else
      {
      m_TensorBasis = m_BMatrix;
      }

    m_BMatrix.inplace_transpose();

    /*Speed up*/
    vnl_svd<double> solver(m_TensorBasis);
    m_TensorBasisInverse = solver.inverse();
  }

  template< class TReferenceImagePixelType,
            class TGradientImagePixelType, class TTensorPixelType >
  void DiffusionTensor3DReconstructionImageFilter< TReferenceImagePixelType,
    TGradientImagePixelType, TTensorPixelType >
  ::AddGradientImage( const GradientDirectionType &gradientDirection,
                          const GradientImageType *gradientImage )
  {
    // Make sure crazy users did not call both AddGradientImage and
    // SetGradientImage
    if( m_GradientImageTypeEnumeration == GradientIsInASingleImage)
      {
      itkExceptionMacro( << "Cannot call both methods:"
      << "AddGradientImage and SetGradientImage. Please call only one of them.");
      }

    // If the container to hold the gradient directions hasn't been allocated
    // yet, allocate it.
    if( !this->m_GradientDirectionContainer )
      {
      this->m_GradientDirectionContainer = GradientDirectionContainerType::New();
      }

    m_GradientDirectionContainer->InsertElement(
                m_NumberOfGradientDirections, gradientDirection / gradientDirection.two_norm() );
    ++m_NumberOfGradientDirections;
    this->ProcessObject::SetNthInput( m_NumberOfGradientDirections,
        const_cast< GradientImageType* >(gradientImage) );
    m_GradientImageTypeEnumeration = GradientIsInManyImages;
  }

  template< class TReferenceImagePixelType,
            class TGradientImagePixelType, class TTensorPixelType >
  void DiffusionTensor3DReconstructionImageFilter< TReferenceImagePixelType,
    TGradientImagePixelType, TTensorPixelType >
  ::SetGradientImage( GradientDirectionContainerType *gradientDirection,
                          const GradientImagesType *gradientImage )
  {
    // Make sure crazy users did not call both AddGradientImage and
    // SetGradientImage
    if( m_GradientImageTypeEnumeration == GradientIsInManyImages )
      {
      itkExceptionMacro( << "Cannot call both methods:"
      << "AddGradientImage and SetGradientImage. Please call only one of them.");
      }

    this->m_GradientDirectionContainer = gradientDirection;

    unsigned int numImages = gradientDirection->Size();
    this->m_NumberOfBaselineImages = 0;
    for(GradientDirectionContainerType::Iterator it = this->m_GradientDirectionContainer->Begin();
        it != this->m_GradientDirectionContainer->End(); it++)
      {
      if(it.Value().one_norm() <= 0.0)
        {
        this->m_NumberOfBaselineImages++;
        }
      else // Normalize non-zero gradient directions
        {
        it.Value() = it.Value() / it.Value().two_norm();
        }
      }

    this->m_NumberOfGradientDirections = numImages - this->m_NumberOfBaselineImages;

    // ensure that the gradient image we received has as many components as
    // the number of gradient directions
    if( gradientImage->GetVectorLength() != this->m_NumberOfBaselineImages + this->m_NumberOfGradientDirections )
      {
      itkExceptionMacro( << this->m_NumberOfGradientDirections << " gradients + " << this->m_NumberOfBaselineImages
                         << "baselines = " << this->m_NumberOfGradientDirections + this->m_NumberOfBaselineImages
                         << " directions specified but image has " << gradientImage->GetVectorLength()
        << " components.");
      }

    this->ProcessObject::SetNthInput( 0,
        const_cast< GradientImagesType* >(gradientImage) );
    m_GradientImageTypeEnumeration = GradientIsInASingleImage;
  }

  template< class TReferenceImagePixelType,
            class TGradientImagePixelType, class TTensorPixelType >
  typename DiffusionTensor3DReconstructionImageFilter< TReferenceImagePixelType,TGradientImagePixelType, TTensorPixelType >::ResidualPixelType
  DiffusionTensor3DReconstructionImageFilter< TReferenceImagePixelType,TGradientImagePixelType, TTensorPixelType >
  ::ComputeResidual( vnl_vector<double> adc, TensorPixelType tensor, double S0)
  {
    ResidualPixelType res( m_NumberOfGradientDirections );

    vnl_vector<double> adcFit; // the fitted adc profile!!!
    vnl_vector<double> signal; //recreate the input signal!
    vnl_vector_fixed<double,6> ten;

    for (unsigned int i=0;i<6;++i)
      ten[i] = tensor[i];

    if( m_NumberOfGradientDirections > 6 )
    {
      // m_BMatrix is tranposed at the end of ComputeTensorBasis() and therefore 
      // we need to transpose it back to the original one that is used to map
      // 6 tensor elements to ADC values
      adcFit = m_BMatrix.transpose() * ten;
    }
    else
    {
      adcFit = m_TensorBasis * ten;
    }

    for (unsigned int i=0;i<m_NumberOfGradientDirections;++i)
    {

      //compute the residue..
      res[i] = static_cast< typename ResidualPixelType::ValueType>(
          S0 * ( exp( - adc[i] * this->m_BValue ) - exp( - adcFit[i] * this->m_BValue ) )
        );

    }

    return res;
  }


  template< class TReferenceImagePixelType,
            class TGradientImagePixelType, class TTensorPixelType >
  void DiffusionTensor3DReconstructionImageFilter< TReferenceImagePixelType,
    TGradientImagePixelType, TTensorPixelType >
  ::PrintSelf(std::ostream& os, Indent indent) const
  {
    Superclass::PrintSelf(os,indent);

    os << indent << "TensorBasisMatrix: " << m_TensorBasis << std::endl;
    os << indent << "Coeffs: " << m_BMatrix << std::endl;
    if ( m_GradientDirectionContainer )
      {
      os << indent << "GradientDirectionContainer: "
         << m_GradientDirectionContainer << std::endl;
      }
    else
      {
      os << indent <<
      "GradientDirectionContainer: (Gradient directions not set)" << std::endl;
      }
    os << indent << "NumberOfGradientDirections: " <<
                m_NumberOfGradientDirections << std::endl;
    os << indent << "NumberOfBaselineImages: " <<
                m_NumberOfBaselineImages << std::endl;
    os << indent << "Threshold for reference B0 image: " << m_Threshold << std::endl;
    os << indent << "BValue: " << m_BValue << std::endl;
    if ( this->m_GradientImageTypeEnumeration == GradientIsInManyImages )
      {
      os << indent << "Gradient images haven been supplied " << std::endl;
      }
    else if ( this->m_GradientImageTypeEnumeration == GradientIsInManyImages )
      {
      os << indent << "A multicomponent gradient image has been supplied" << std::endl;
      }
  }

}

#endif

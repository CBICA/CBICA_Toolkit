/**
\file  itkNaryMeanDiffusionTensorImageFilter.h

\brief Declaration & Implementation of the NaryMeanDiffusionTensorImageFilter class

https://www.cbica.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include "itkNaryFunctorImageFilter.h"
#include "itkNumericTraits.h"
#include "itkDiffusionTensor3D.h"
#include "itkDTILogEuclideanCalculator.h"

namespace itk
{
  
  namespace Functor 
  {  
    /**
    \class DiffusionTensorMean

    \brief Helper class to compute the Tensor mean which interfaces with NaryMeanDiffusionTensorImageFilter
    */
    template< class TInput, class TOutput >
    class DiffusionTensorMean
    {
    public:

      // Enum to decide which Metric To use compute the means
      enum { EUCLIDEAN, LOGEUCLIDEAN };
      typedef typename NumericTraits< TInput >::RealType DtiCompType;
      typedef typename NumericTraits< TOutput >::RealType SymTensorCompType;

      typedef DTILogEuclideanCalculator< DtiCompType, SymTensorCompType > DTICalculatorType;

      typedef typename DTICalculatorType::DTType TensorType;
      typedef typename DTICalculatorType::SymMatType SymMatType;
  
      DiffusionTensorMean()
      {
        m_MetricType = LOGEUCLIDEAN;
        dtiCalc =  DTICalculatorType::New();
      }
  
      virtual ~DiffusionTensorMean() {}
      inline TOutput operator()( const std::vector< TInput > & B)
      {
        TensorType meanVal = NumericTraits< TensorType >::Zero;

        switch (m_MetricType)
        {
          case LOGEUCLIDEAN :
          {
        
            int stop = 0;
        
            // Check for any zero inputs and return a zero tensor if any are found.
            for( unsigned int i=0; i< B.size(); i++ )
            {
              if ( !dtiCalc->IsPositiveDefinite(static_cast<TensorType>(B[i])) ) 
              {
                stop = 1;
                break;
              }
            }
          
            if (stop) { break; }
        
            SymMatType sum = NumericTraits< SymMatType >::Zero;

            for( unsigned int i=0; i< B.size(); i++ )
            {
              sum += dtiCalc->CalculateMatrixLog(static_cast<TensorType>(B[i]));
            }
            meanVal = dtiCalc->CalculateMatrixExp(sum / B.size());
            break;
          }
          case EUCLIDEAN :
          {
            TensorType sum = NumericTraits< TensorType >::Zero;
            for( unsigned int i=0; i< B.size(); i++ )
            {
              sum += static_cast<TensorType>(B[i]);
            }
            meanVal = (sum / B.size());
            break;
          }
        }

        return static_cast<TOutput>( meanVal.GetInnerScalarProduct() );
      }
  
      bool operator== (const DiffusionTensorMean&) const
      {
        return true;
      }
      bool operator!= (const DiffusionTensorMean&) const
      {
        return false;
      }
  
      virtual void SetMetricType (int _arg)
      {
        if (this->m_MetricType !=
          (bool)(_arg<EUCLIDEAN ? EUCLIDEAN : (_arg>LOGEUCLIDEAN ? LOGEUCLIDEAN : _arg)))
        {
          this->m_MetricType =
            (bool)(_arg<EUCLIDEAN ? EUCLIDEAN : (_arg>LOGEUCLIDEAN ? LOGEUCLIDEAN : _arg));
        }
      }

    private:
      bool m_MetricType; // Boolean to use LogEuc or Euclidean
      typename DTICalculatorType::Pointer dtiCalc;
 
    }; 
  }
  
  /**
  \class NaryMeanDiffusionTensorImageFilter

  \brief Implements an operator for pixel-wise averaging of two 
  Diffusion Tensor images.

  This class is parametrized over the types of the two 
  input images and the type of the output image. 
  Numeric conversions (castings) are done by the C++ defaults.

  The pixel type of the input 1 image must have a valid defintion of
  the operator+ with a pixel type of the image 2. This condition is 
  required because internally this filter will perform the operation

  <code>pixel_from_image_1 + pixel_from_image_2</code>

  Additionally the type resulting from the sum, will be cast to
  the pixel type of the output image.

  \warning No numeric overflow checking is performed in this filter.

  \todo We should implement a mask to define the region of interest.

  \ingroup IntensityImageFilters  Multithreaded
  */

  template <class TInputImage, class TOutputImage>
  class ITK_EXPORT NaryMeanDiffusionTensorImageFilter :
      public
  NaryFunctorImageFilter<TInputImage,TOutputImage, 
                         Functor::DiffusionTensorMean<typename TInputImage::PixelType,
                           typename TOutputImage::PixelType > > 
  {
  public:

    //! Enum to decide which Metric To use compute the means
    enum { EUCLIDEAN, LOGEUCLIDEAN };

    //! Standard class typedefs. 
    typedef NaryMeanDiffusionTensorImageFilter  Self;
    typedef NaryFunctorImageFilter<TInputImage,TOutputImage, 
                                   Functor::DiffusionTensorMean<typename TInputImage::PixelType,  
                                   typename TInputImage::PixelType  > >  Superclass;
    typedef SmartPointer<Self>                  Pointer;
    typedef SmartPointer<const Self>            ConstPointer;

    //! Method for creation through the object factory. 
    itkNewMacro(Self);

    //! Runtime information support. 
    itkTypeMacro(NaryMeanDiffusionTensorImageFilter, 
                 NaryFunctorImageFilter);


  #ifdef ITK_USE_CONCEPT_CHECKING
    //! Begin concept checking 
    itkConceptMacro(InputConvertibleToOutputCheck,
      (Concept::Convertible<typename TInputImage::PixelType,
                            typename TOutputImage::PixelType>));
    itkConceptMacro(InputHasZeroCheck,
      (Concept::HasZero<typename TInputImage::PixelType>));
    //! End concept checking 
  #endif

    virtual void SetMetricType (int _arg)
      { this->GetFunctor().SetMetricType(_arg);} 

  protected:
    NaryMeanDiffusionTensorImageFilter() {}
    virtual ~NaryMeanDiffusionTensorImageFilter() {}

  private:
    NaryMeanDiffusionTensorImageFilter(const Self&); // purposely not implemented
    void operator=(const Self&); // purposely not implemented

  };

} // end namespace itk

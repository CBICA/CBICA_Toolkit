/**
\file  itkNaryVarianceImageFilter.h

\brief Declaration & Implementation of the NaryFunctorImageFilter class

https://www.cbica.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include "itkNaryFunctorImageFilter.h"
#include "itkNumericTraits.h"
#include "itkDiffusionTensor3D.h"

//#include "itkSymRealSphericalHarmonicRep.h"

namespace itk
{
  
  namespace Functor 
  {  
    /**
    \class Variance

    \brief Helper class for calculating variance of images which interfaces with NaryVarianceImageFilter
    */
    template< class TInput, class TOutput >
    class Variance
    {
    public:
      typedef typename NumericTraits< TOutput >::RealType RealType;
      Variance() {}
      ~Variance() {}
      inline TOutput operator()( const std::vector< TInput > & B)
      {
        RealType sum = NumericTraits< TOutput >::Zero;
        RealType sum_sqr = NumericTraits< TOutput >::Zero;
          
        for( unsigned int i=0; i< B.size(); i++ )
        {
          sum_sqr += static_cast< TOutput >(B[i]) * static_cast< TOutput >(B[i]);
          sum += static_cast< TOutput >(B[i]);
        }       
        return static_cast<TOutput>( (sum_sqr - (sum * sum) / B.size() ) /B.size() );
      }
      bool operator== (const Variance&) const
      {
        return true;
      }
      bool operator!= (const Variance&) const
      {
        return false;
      }
    };
    //
    //template <class TCompType, unsigned int TOrder, class TOutput>
    //class ITK_EXPORT Variance
    //  <SymRealSphericalHarmonicRep<TCompType,TOrder>, TOutput>
    //{
    //public:
    //  typedef typename NumericTraits< TOutput >::RealType   RealType;
    //  typedef SymRealSphericalHarmonicRep<TCompType,TOrder> TInput;
    //  typedef typename NumericTraits< TInput >::RealType    InputRealType;
    //  
    //  Variance() {}
    //  ~Variance() {}
    //  inline TOutput operator()( const std::vector< TInput > & B)
    //  {
    //    //First compute the mean
    //    InputRealType mean = NumericTraits<InputRealType>::Zero;
    //    for (unsigned int i=0;i<B.size();i++ )
    //    {
    //      mean += B[i];
    //    }
    //    mean /= B.size();
    //    //compute the variance
    //    RealType variance = NumericTraits<RealType>::Zero;
    //    for (unsigned int j=0;j<B.size();j++ )
    //    {
    //      for ( unsigned int i=0; i<TInput::Dimension; ++i )
    //      {
    //        variance += vcl_pow( (B[j])[i] - mean[i], 2);
    //      }
    //    }
    //    variance /= B.size();
    //    
    //    return static_cast<TOutput>( variance );
    //  }
    //  bool operator== (const Variance&) const
    //  {
    //    return true;
    //  }
    //  bool operator!= (const Variance&) const
    //  {
    //    return false;
    //  }
    //};

  }

  /**
  \class NaryVarianceImageFilter

  \brief Implements an operator for calculating pixel-wise variance of two images.
  
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
  
  \ingroup IntensityImageFilters  Multithreaded
   */

  template <class TInputImage, class TOutputImage>
  class ITK_EXPORT NaryVarianceImageFilter :
      public
  NaryFunctorImageFilter<TInputImage,TOutputImage, 
                         Functor::Variance<typename TInputImage::PixelType,  typename TOutputImage::PixelType > > 
  {
  public:
    /** Standard class typedefs. */
    typedef NaryVarianceImageFilter  Self;
    typedef NaryFunctorImageFilter<TInputImage,TOutputImage, 
                                   Functor::Variance<typename TInputImage::PixelType,  
                                   typename TInputImage::PixelType  > >  Superclass;
    typedef SmartPointer<Self>   Pointer;
    typedef SmartPointer<const Self>  ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Runtime information support. */
    itkTypeMacro(NaryVarianceImageFilter, 
                 NaryFunctorImageFilter);

  #ifdef ITK_USE_CONCEPT_CHECKING
    /** Begin concept checking */
    itkConceptMacro(InputConvertibleToOutputCheck,
      (Concept::Convertible<typename TInputImage::PixelType,
                            typename TOutputImage::PixelType>));
    itkConceptMacro(InputHasZeroCheck,
      (Concept::HasZero<typename TInputImage::PixelType>));
    /** End concept checking */
  #endif

  protected:
    NaryVarianceImageFilter() {}
    virtual ~NaryVarianceImageFilter() {}

  private:
    NaryVarianceImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

  };

} // end namespace itk


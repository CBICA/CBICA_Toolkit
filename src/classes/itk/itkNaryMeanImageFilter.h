/**
\file  itkNaryMeanImageFilter.h

\brief Declaration & Implementation of the NaryMeanImageFilter class

https://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include "itkNaryFunctorImageFilter.h"
#include "itkNumericTraits.h"

namespace itk
{
  
  namespace Functor 
  {  
    /**
    \class Mean

    \brief Helper class for calculating mean of images which interfaces with NaryMeanImageFilter
    */
    template< class TInput, class TOutput >
    class Mean
    {
    public:
      typedef typename NumericTraits< TOutput >::RealType RealType;
      Mean() {}
      ~Mean() {}
      inline TOutput operator()( const std::vector< TInput > & B)
      {
        RealType meanVal = NumericTraits< TOutput >::Zero;
        for( unsigned int i=0; i< B.size(); i++ )
        {
          meanVal += static_cast< RealType >(B[i]);
        }
        meanVal = meanVal / B.size();
    
        return static_cast<TOutput>( meanVal );
      }
      bool operator== (const Mean&) const
      {
        return true;
      }
      bool operator!= (const Mean&) const
      {
        return false;
      }
    }; 
  }
    
  /**
  \class NaryMeanImageFilter

  \brief Implements an operator for pixel-wise averaging of two images.

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
  class ITK_EXPORT NaryMeanImageFilter :
      public
  NaryFunctorImageFilter<TInputImage,TOutputImage, 
                         Functor::Mean<typename TInputImage::PixelType,  typename TOutputImage::PixelType > > 
  {
  public:
    //! Standard class typedefs. 
    typedef NaryMeanImageFilter  Self;
    typedef NaryFunctorImageFilter<TInputImage,TOutputImage, 
                                   Functor::Mean<typename TInputImage::PixelType,  
                                   typename TInputImage::PixelType  > >  Superclass;
    typedef SmartPointer<Self>   Pointer;
    typedef SmartPointer<const Self>  ConstPointer;

    //! Method for creation through the object factory. 
    itkNewMacro(Self);

    //! Runtime information support. 
    itkTypeMacro(NaryMeanImageFilter, 
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

  protected:
    NaryMeanImageFilter() {}
    virtual ~NaryMeanImageFilter() {}

  private:
    NaryMeanImageFilter(const Self&); // purposely not implemented
    void operator=(const Self&); // purposely not implemented

  };

} // end namespace itk


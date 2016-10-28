/**
\file  itkDTILogEuclideanCalculator.h

\brief Declaration of the DTILogEuclideanCalculator class

https://www.cbica.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkDiffusionTensor3D.h"
#include "itkSymmetricSecondRankTensor.h"

#if WIN32
__declspec(dllexport) inline void getRidOfLNK4221(){};
#endif

namespace itk
{
  /**
  \class DTILogEuclideanCalculator

  \brief The Purpose of this class is to provide common computations for
  diffusion tensors specifically

  1) Matrix Log Computation
  2) Matrix Exp Computaiton
  3) Tests for positive definitness 
  */
  template <class TDtiCompType , class TSymCompType>
  class ITK_EXPORT DTILogEuclideanCalculator:public Object
  {
  public:

    //! Standard class typedefs
    typedef DTILogEuclideanCalculator                         Self;
    typedef Object                                            Superclass;

    typedef SmartPointer<Self>                                Pointer;
    typedef SmartPointer<const Self>                          ConstPointer;

    //! Method for creation through the object factory
    itkNewMacro(Self);  

    //! Run-time type information (and related methods)
    itkTypeMacro(DTILogEuclideanCalculator, Object);

    //! Typedefs for methods
    typedef DiffusionTensor3D< TDtiCompType >                 DTType;
    typedef SymmetricSecondRankTensor< TSymCompType, 3>       SymMatType;

    static const unsigned int MatrixDimension = 3;

    //! Compute the MatrixExponential
    DTType CalculateMatrixExp( SymMatType ) const;
    //! Compute the MatrixLogarithm
    SymMatType CalculateMatrixLog( DTType ) const;
    //! Check if the eigenvalues are > 0 
    bool IsPositiveDefinite( SymMatType ) const;
  
  protected:
    DTILogEuclideanCalculator();
    ~DTILogEuclideanCalculator() {};

  private:
    DTILogEuclideanCalculator(const Self&); // purposely not implemented
    void operator=(const Self&); // purposely not implemented

  };
  
} // end namespace itk
   
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkDTILogEuclideanCalculator.cpp"
#endif
  
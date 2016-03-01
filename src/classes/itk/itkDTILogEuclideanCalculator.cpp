/**
\file  itkDTILogEuclideanCalculator.cpp

\brief Implementation of the DTILogEuclideanCalculator class

https://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#ifndef ITKDTILOGEUCLIDEANCALCULATOR_CPP
#define ITKDTILOGEUCLIDEANCALCULATOR_CPP

#include "itkDTILogEuclideanCalculator.h"

namespace itk
{

/**
\brief The constructor

Initialize new instance
*/
template <class TDtiCompType , class TSymCompType>
DTILogEuclideanCalculator< TDtiCompType, TSymCompType >
::DTILogEuclideanCalculator()
{

}


/**
\brief Compute the matrix Exponential
*/
template <class TDtiCompType , class TSymCompType>
typename DTILogEuclideanCalculator< TDtiCompType, TSymCompType >::DTType
DTILogEuclideanCalculator< TDtiCompType, TSymCompType >
::CalculateMatrixExp( SymMatType symMatrix) const
{
  DTType result;

  typedef typename SymMatType::MatrixType                   MatrixType;
  typedef typename SymMatType::EigenValuesArrayType         EigenValuesArrayType;
  typedef typename SymMatType::EigenVectorsMatrixType       EigenVectorsMatrixType;

  MatrixType diag;
  EigenValuesArrayType eigenValues;
  EigenVectorsMatrixType eigenVectors;

  //! Diagnolize...
  symMatrix.ComputeEigenAnalysis( eigenValues,eigenVectors );

  for(unsigned int r=0; r<MatrixDimension; r++)
  {
    diag[r][r] = vcl_exp(eigenValues[r]);
  }

  MatrixType tmp = (static_cast<MatrixType>(eigenVectors.GetTranspose()) ) * (diag * eigenVectors);

  for(unsigned int r=0; r<MatrixDimension; r++)
    {
    for(unsigned int c=0; c<MatrixDimension; c++)
      {
      result(r,c) = static_cast<TDtiCompType>( tmp(r,c) );
      }
    }

  return result;
}


/**
\brief Compute the matrix Log
*/
template <class TDtiCompType , class TSymCompType>
typename DTILogEuclideanCalculator< TDtiCompType, TSymCompType >::SymMatType
DTILogEuclideanCalculator< TDtiCompType, TSymCompType >
::CalculateMatrixLog( DTType dt) const
{
  SymMatType result;
  
  typedef typename DTType::MatrixType                       MatrixType;
  typedef typename DTType::EigenValuesArrayType             EigenValuesArrayType;
  typedef typename DTType::EigenVectorsMatrixType           EigenVectorsMatrixType;
  
  MatrixType diag;
  EigenValuesArrayType eigenValues;
  EigenVectorsMatrixType eigenVectors;

  //! Diagnolize...
  dt.ComputeEigenAnalysis( eigenValues,eigenVectors );

  for(unsigned int r=0; r<MatrixDimension; r++)
  {
    diag[r][r] = vcl_log(eigenValues[r]);
  }

  MatrixType tmp = (static_cast<MatrixType>(eigenVectors.GetTranspose()) ) * (diag * eigenVectors);

  for(unsigned int r=0; r<MatrixDimension; r++)
    {
    for(unsigned int c=0; c<MatrixDimension; c++)
      {
      result(r,c) = static_cast<TSymCompType>( tmp(r,c) );
      }
    }

  return result;
}

/**
\brief Determine if suuplied tensor is positive definite
*/
template <class TDtiCompType , class TSymCompType>
bool
DTILogEuclideanCalculator< TDtiCompType, TSymCompType >
::IsPositiveDefinite( SymMatType dt) const
{
  
  typedef typename SymMatType::EigenValuesArrayType         EigenValuesArrayType;
  typedef typename SymMatType::EigenVectorsMatrixType       EigenVectorsMatrixType;

  EigenValuesArrayType eigenValues;
  EigenVectorsMatrixType eigenVectors;

  //! Diagnolize...
  dt.ComputeEigenAnalysis( eigenValues,eigenVectors );

  for(unsigned int r=0; r<MatrixDimension; r++)
  {
    if (eigenValues[r] <= 0)
      return false;
  }

  return true;
}

} // end namespace itk

#endif // ITKDTILOGEUCLIDEANCALCULATOR_CPP
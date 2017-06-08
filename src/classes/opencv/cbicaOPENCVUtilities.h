/**
\file  cbicaOPENCVUtilities.h

\brief Some basic utility functions.

Dependecies: OpenCV

https://www.cbica.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2017 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include <algorithm>
#include <functional>

#include "opencv2/core.hpp"

#include "cbicaUtilities.h"

namespace cbica
{
  /**
  \brief Convert a 1-channel cv::Mat to a std::vector
  */
  template< class TPixelType = float >
  std::vector< TPixelType > GetVectorFromMat(const cv::Mat& inputMat)
  {
    std::vector< TPixelType > returnVector;

    if (inputMat.isContinuous())
    {
      returnVector.assign((TPixelType*)inputMat.datastart, (TPixelType*)inputMat.dataend);
    }
    else
    {
      for (int i = 0; i < inputMat.rows; ++i)
      {
        returnVector.insert(returnVector.end(), inputMat.ptr< TPixelType >(i), inputMat.ptr< TPixelType >(i) +inputMat.cols);
      }
    }
    return returnVector;
  }

  /**
  \brief Convert an std::vector to a 1-channel cv::Mat

  \param inputVector The input vector that needs to be converted to cv::Mat
  \param outputSize The expected size of the output cv::Mat (used to call reshape inside); defaults to [1xinputVector.size()]
  */
  template< class TPixelType = float >
  cv::Mat GetMatFromVector(const std::vector< TPixelType >& inputVector, const cv::Size &outputSize)
  {
    cv::Mat returnMat;

    // auto initialization doesn't work inside the for-loop
    auto it = inputVector.begin();
    auto jt = returnMat.begin< TPixelType >();

    for (; it != inputVector.end(); ++it)
    {
      *jt++ = static_cast< TPixelType >(*it);
    }

    returnMat.reshape(1, outputSize.height);

    return returnMat;
  }
  /**
  \brief Calculates the Confusion Matrix for a set of real and predicted labels

  Values returned: True Positive (TP), False Positive (FP), True Negative (TN), False Negative (FN), Real Positive (RP), Preditcted Positive (PP)

  \param inputRealAndPredictedLabels A 2xN or Nx2 matrix containing real labels in the first column/row and then the predicted labels

  \return std::map< string, size_t > A map of string and corresponding non-negative values 
  */
  std::map< std::string, size_t > ConfusionMatrix(const cv::Mat &inputRealAndPredictedLabels);

  /**
  \brief Calculates the Confusion Matrix for a set of real and predicted labels

  Calls cbica::Utilities::Roc_Values inside; see that function for details.

  \param inputRealAndPredictedLabels A 2xN or Nx2 matrix containing real labels in the first column/row and then the predicted labels

  \return std::map< string, float > A map of string and corresponding float values
  */
  std::map< std::string, float > ROC_Values(const cv::Mat &inputRealAndPredictedLabels);

  /**
  \brief Calculates the Confusion Matrix for a set of real and predicted labels

  Calls cbica::Utilities::Roc_Values inside; see that function for details.

  \param inputRealValues A 1xN or Nx1 matrix containing real labels 
  \param inputPredictedValues A 1xN or Nx1 matrix containing predicted labels 

  \return std::map< string, float > A map of string and corresponding float values
  */
  std::map< std::string, float > ROC_Values(const cv::Mat &inputRealValues, const cv::Mat &inputPredictedValues);

  /**
  \brief Calculate z-scores for the supplied vector

  \param inputVector Vector in a cv::Mat structure
  */
  cv::Mat ZScores(const cv::Mat &inputMat);

}
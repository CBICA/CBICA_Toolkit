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
  \brief Calculates the Confusion Matrix for a set of real and predicted labels

  Values returned: True Positive (TP), False Positive (FP), True Negative (TN), False Negative (FN), Real Positive (RP), Preditcted Positive (PP)

  \param inputRealAndPredictedLabels A 2xN or Nx2 matrix containing real labels in the first column/row and then the predicted labels

  \return std::map< string, size_t > A map of string and corresponding non-negative values 
  */
  std::map< std::string, size_t > ConfusionMatrix(const cv::Mat &inputRealAndPredictedLabels);

  /**
  \brief Calculates the Confusion Matrix for a set of real and predicted labels

  Values returned: 
  True Positive (TP), False Positive (FP), True Negative (TN), False Negative (FN), Real Positive (RP), Preditcted Positive (PP) [all from ConfusionMatrix()],
  Accuracy, Positive Predictive Value (PPV) [aka Precision], False Discovery Rate (FDR), False Omission Rate (FOR), 
  Negative Predictive Value (NPV), Prevalence, True Positive Rate (TPR) [aka Sensitivity, Recall, Probability of Detection (POD)], 
  False Positive Rate (FPR) [aka Fall-out], False Negative Rate (FNR) [aka Miss Rate],  True Negative Rate (TNR) [aka Specificity], 
  Positive Likelihood Ratio (LR+), Negative Likelihood Ratio (LR−), Diagnostic Odds Ratio (DOR)

  \param inputRealAndPredictedLabels A 2xN or Nx2 matrix containing real labels in the first column/row and then the predicted labels

  \return std::map< string, float > A map of string and corresponding float values
  */
  std::map< std::string, float > ROC_Values(const cv::Mat &inputRealAndPredictedLabels);

  /**
  \brief Calculate z-scores for the supplied vector

  \param inputVector Vector in a cv::Mat structure
  */
  cv::Mat ZScores(const cv::Mat &inputVector);
}
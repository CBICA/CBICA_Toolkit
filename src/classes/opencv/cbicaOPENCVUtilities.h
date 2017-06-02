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
  /*
  template <class T>
  double Z_Score ( T array[], int size )
  {
	
	  int i;
	  double mean = Mean( array, size ), Std = Pop_Std_Dev( array, size, mean );
	  double * Z_array = new double [size];
	
	  for ( i = 0; i < size; i++ )
	  {
		
		  Z_array[i] = ( ( array[i] - mean ) / Std );
		
	  }
	
	  return Z_array;	
  }  
  */

  /**
  \brief Calculates the Confusion Matrix for a set of real and predicted labels

  Values returned: True Positive (TP), False Positive (FP), True Negative (TN), False Negative (FN), Real Positive (RP), Preditcted Positive (PP)

  \param inputRealAndPredictedLabels A 2xN or Nx2 matrix containing real labels in the first column/row and then the predicted labels

  \return std::map< string, size_t > A map of string and corresponding non-negative values 
  */
  std::map< std::string, size_t > ConfusionMatrix(const cv::Mat &inputRealAndPredictedLabels);

}
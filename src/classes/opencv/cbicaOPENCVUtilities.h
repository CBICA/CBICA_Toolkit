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

  // ROC: https://github.com/vbalnt/cppROC
}
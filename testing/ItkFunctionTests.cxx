/**
\file  ItkFunctionTests.cxx

\brief File for testing the ITK classes.

https://www.cbica.upenn.edu/sbia/software/
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html.

*/

#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <string>
#include <algorithm>

#include "classes/itk/cbicaITKImageInfo.h"

int main(int argc, char** argv)
{
  try
  {
    if( (std::string( "-itkImageInfo").compare(argv[1]) == 0) ||
        (std::string("--itkImageInfo").compare(argv[1]) == 0) )
    {
      cbica::ITKImageInfo test_image = cbica::ITKImageInfo(argv[2]);
      itk::SmartPointer<itk::ImageIOBase> io_base = test_image.getImageIOBase();
      std::vector<itk::SizeValueType> size = test_image.getImageSize();
      std::vector<double> spacings = test_image.getImageSpacings();
      const int dimensions = test_image.getImageDimensions();

      if( dimensions == 0 )
        return EXIT_FAILURE;
      if( size.empty() )
        return EXIT_FAILURE;
      else if( spacings.empty() )
        return EXIT_FAILURE;
      if( !io_base )
        return EXIT_FAILURE;
    }
  }
  catch( itk::ExceptionObject &ex )
  {
    std::cout << ex << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
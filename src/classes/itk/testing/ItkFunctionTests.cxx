/**
\file  ItkFunctionTests.cxx

\brief File for testing the ITK classes.

https://www.cbica.upenn.edu/sbia/software/
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/

#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <string>
#include <algorithm>

#include "itkImage.h"

#include "classes/itk/cbicaITKImageInfo.h"
#include "classes/itk/cbicaITKWriteImage.h"

#include "itkImage.h"
#include "gdcmImage.h"

int main(int argc, char** argv)
{
  try
  {
    if( (std::string( "-itkImageInfo").compare(argv[1]) == 0) ||
        (std::string("--itkImageInfo").compare(argv[1]) == 0) )
    {
      cbica::ImageInfo test_image = cbica::ImageInfo(argv[2]);
      itk::SmartPointer<itk::ImageIOBase> io_base = test_image.getImageIOBase();
      std::vector<itk::SizeValueType> size = test_image.getImageSize();
      std::vector<double> spacings = test_image.getImageSpacings();
      const int dimensions = test_image.getImageDimensions();

      if( dimensions == 0 )
        return EXIT_FAILURE;
      if( size.empty() )
        return EXIT_FAILURE;
      if( spacings.empty() )
        return EXIT_FAILURE;
      if( !io_base )
        return EXIT_FAILURE;
    }

    else if( (std::string( "-writeImage").compare(argv[1]) == 0) ||
             (std::string("--writeImage").compare(argv[1]) == 0) )
    {
      typedef itk::Image<int, 3> ExpectedImageType;
      typedef itk::ImageFileReader< ExpectedImageType > ReaderType;
      ReaderType::Pointer reader = ReaderType::New();
      reader->SetFileName(argv[2]);

      ExpectedImageType::Pointer inputImage = reader->GetOutput();

      typedef itk::Image<float, 3> OutputImageType;
      cbica::WriteImage<ExpectedImageType, OutputImageType>(inputImage, argv[3]);
      
      typedef itk::ImageFileReader< OutputImageType > OutputReaderType;
      OutputReaderType::Pointer reader2 = OutputReaderType::New();
      reader2->SetFileName(argv[2]);

      OutputImageType::Pointer writtenImage = reader2->GetOutput();

      if (writtenImage->GetImageDimension() == 0)
        return EXIT_FAILURE;
      if (writtenImage->GetLargestPossibleRegion().GetSize()[0] == 0)
        return EXIT_FAILURE;
      if (writtenImage->GetSpacing()[0] == 0)
        return EXIT_FAILURE;
    }
    // write tests for computeaveragemap, computedtiscalars and computevariancemap

  }
  catch( itk::ExceptionObject &ex )
  {
    std::cout << ex << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
/**
\file  ItkFunctionTests.cxx

\brief File for testing the ITK classes.

https://www.cbica.upenn.edu/sbia/software/
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/

#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <string>
#include <algorithm>

#include "itkImage.h"

#include "classes/cbicaUtilities.h"
#include "classes/cbicaCmdParser.h"

#include "classes/itk/cbicaITKImageInfo.h"
#include "classes/itk/cbicaITKSafeImageIO.h"

#include "itkImage.h"
#include "gdcmImage.h"

#include "itkTranslationTransform.h"
#include "itkImageFileReader.h"
#include "itkNormalizeImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkImageFileWriter.h"

int main(int argc, char** argv)
{
  cbica::CmdParser parser(argc, argv);
  parser.addOptionalParameter("i", "imageInfo", cbica::Parameter::NONE, "", "ImageInfo Test");
  parser.addOptionalParameter("w", "writeImage", cbica::Parameter::NONE, "", "writeImage Test");
  parser.addOptionalParameter("r", "readImage", cbica::Parameter::NONE, "", "readImage Test");

  int tempPosition;
  if (parser.compareParameter("imageInfo", tempPosition))
  {
    cbica::ImageInfo test_image = cbica::ImageInfo(argv[tempPosition + 1]);
    itk::SmartPointer<itk::ImageIOBase> io_base = test_image.getImageIOBase();
    std::vector<itk::SizeValueType> size = test_image.getImageSize();
    std::vector<double> spacings = test_image.getImageSpacings();
    const int dimensions = test_image.getImageDimensions();

    if (dimensions == 0)
      return EXIT_FAILURE;
    if (size.empty())
      return EXIT_FAILURE;
    if (spacings.empty())
      return EXIT_FAILURE;
    if (!io_base)
      return EXIT_FAILURE;
  }

  if (parser.compareParameter("readImage", tempPosition))
  {
    const std::string inputFile = argv[tempPosition + 1];
    typedef float ExpectedPixelType;
    typedef itk::Image<ExpectedPixelType, 3> ExpectedImageType;
    ExpectedImageType::Pointer inputImage = cbica::ReadImage<ExpectedImageType>(inputFile);

    if (inputImage->GetImageDimension() == 0)
      return EXIT_FAILURE;
    for (size_t i = 0; i < inputImage->GetImageDimension(); i++)
    {
      if (inputImage->GetLargestPossibleRegion().GetSize()[i] == 0)
        return EXIT_FAILURE;
      if (inputImage->GetSpacing()[i] == 0)
        return EXIT_FAILURE;
    }
  }

  if (parser.compareParameter("writeImage", tempPosition))
  {
    const std::string inputFile = argv[tempPosition + 1], fileToWrite = argv[tempPosition + 2];
    typedef itk::Image<int, 3> ExpectedImageType;
    typedef itk::ImageFileReader< ExpectedImageType > ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(inputFile);
    reader->Update();
    ExpectedImageType::Pointer inputImage = reader->GetOutput();

    typedef itk::Image<float, 3> OutputImageType;
    cbica::WriteImage<ExpectedImageType, OutputImageType>(inputImage, fileToWrite);

    OutputImageType::Pointer writtenImage = cbica::ReadImage<OutputImageType>(fileToWrite);

    if (writtenImage->GetImageDimension() != inputImage->GetImageDimension())
      return EXIT_FAILURE;
    for (size_t i = 0; i < writtenImage->GetImageDimension(); i++)
    {
      if (writtenImage->GetLargestPossibleRegion().GetSize()[i] != inputImage->GetLargestPossibleRegion().GetSize()[i])
        return EXIT_FAILURE;
      if (writtenImage->GetSpacing()[i] != inputImage->GetSpacing()[i])
        return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
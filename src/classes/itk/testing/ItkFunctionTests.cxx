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
#include <fstream>

#include "itkImage.h"

#include "classes/cbicaUtilities.h"
#include "classes/cbicaCmdParser.h"

#include "classes/itk/cbicaITKImageInfo.h"
#include "classes/itk/cbicaITKSafeImageIO.h"
#include "classes/itk/cbicaITKUtilities.h"

#include "itkImage.h"

#include "itkMetaImageIO.h"
#include "itkTranslationTransform.h"
#include "itkImageFileReader.h"
#include "itkNormalizeImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkDiffusionTensor3DReconstructionImageFilter.h"
#include "itkTestingComparisonImageFilter.h"

int main(int argc, char** argv)
{
  cbica::CmdParser parser(argc, argv);
  parser.addOptionalParameter("i", "imageInfo", cbica::Parameter::NONE, "", "ImageInfo Test");
  parser.addOptionalParameter("w", "writeImage", cbica::Parameter::NONE, "", "writeImage Test");
  parser.addOptionalParameter("r", "readImage", cbica::Parameter::NONE, "", "readImage Test");
  parser.addOptionalParameter("d", "deform", cbica::Parameter::NONE, "", "Deformable registration Test");
  parser.addOptionalParameter("s", "skullStrip", cbica::Parameter::NONE, "", "Skull stripping Test");
  parser.addOptionalParameter("l", "labelDist", cbica::Parameter::DIRECTORY, "", "Label distance calculator Test");
  parser.addOptionalParameter("dcm", "dicom", cbica::Parameter::STRING, "", "DICOM reading test");

  int tempPosition;
  if (parser.compareParameter("imageInfo", tempPosition))
  {
    auto test_image = cbica::ImageInfo(argv[tempPosition + 1]);
    auto io_base = test_image.GetImageIOBase();
    auto size = test_image.GetImageSize();
    auto spacings = test_image.GetImageSpacings();
    const int dimensions = test_image.GetImageDimensions();

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
    using ExpectedImageType = itk::Image<float, 3>;
    auto inputImage = cbica::ReadImage<ExpectedImageType>(inputFile);

    auto calculator = itk::MinimumMaximumImageCalculator< ExpectedImageType >::New();
    calculator->SetImage(inputImage);
    calculator->Compute();

    if (inputImage->GetImageDimension() == 0)
      return EXIT_FAILURE;
    for (size_t i = 0; i < inputImage->GetImageDimension(); i++)
    {
      if (inputImage->GetLargestPossibleRegion().GetSize()[i] == 0)
        return EXIT_FAILURE;
      if (inputImage->GetSpacing()[i] == 0)
        return EXIT_FAILURE;
    }
    if (calculator->GetMaximum() == 0)
    {
      return EXIT_FAILURE;
    }
  }

  if (parser.compareParameter("writeImage", tempPosition))
  {
    const std::string inputFile = argv[tempPosition + 1], fileToWrite = argv[tempPosition + 2];
    typedef itk::Image<int, 3> ExpectedImageType;
    auto inputImage = cbica::ReadImage< ExpectedImageType >(inputFile);

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

  if (parser.compareParameter("deform", tempPosition))
  {
    const std::string file_referenceImage = argv[tempPosition + 1], file_movingImage = argv[tempPosition + 2], 
      file_referenceOutput = argv[tempPosition + 3];

    auto refImage = cbica::ReadImage< ImageTypeFloat3D >(file_referenceImage);
    auto movImage = cbica::ReadImage< ImageTypeFloat3D >(file_movingImage);

    // run deformable registration with default values
    //auto deformedImage = cbica::GetDeformRegisteredImage(movImage, refImage);

    //// run image comparator with default values
    //if (!cbica::GetResultOfImageComparasion(cbica::ReadImage< ImageTypeFloat3D >(file_referenceOutput), deformedImage)) 
    //{
    //  return EXIT_FAILURE;
    //}

  }

  if (parser.compareParameter("skullStrip", tempPosition))
  {
    const std::string file_inputImage = argv[tempPosition + 1], file_atlasImage = argv[tempPosition + 2],
      file_atlasLabel = argv[tempPosition + 3], file_referenceOutput = argv[tempPosition + 4];

    auto inputImage = cbica::ReadImage< ImageTypeFloat3D >(file_inputImage);
    auto atlasImage = cbica::ReadImage< ImageTypeFloat3D >(file_atlasImage);
    auto atlasLabel = cbica::ReadImage< ImageTypeFloat3D >(file_atlasLabel);
    auto referenceOutput = cbica::ReadImage< ImageTypeFloat3D >(file_referenceOutput);

    // run skull stripping with default values
    auto skullStrippedImage = cbica::GetSkullStrippedImage(inputImage, atlasImage, atlasLabel);

    // run image comparator with default values
    if (!cbica::GetResultOfImageComparasion(referenceOutput, skullStrippedImage))
    {
      return EXIT_FAILURE;
    }
  }

  if (parser.compareParameter("labelDist", tempPosition))
  {
    const std::string dataDir = argv[tempPosition + 1], file_inputLabel = dataDir + argv[tempPosition + 2],
      file_inputSeeds = dataDir + "inputSeeds.txt";

    using ImageType = ImageTypeFloat3D;

    auto image_inputLabel = cbica::ReadImage< ImageType >(file_inputLabel);

    // read the coordinates from file and put them as a vector of ImageType::IndexType
    std::vector< std::string > string_coordinates;
    std::ifstream myfile(file_inputSeeds);
    if (myfile.is_open())
    {
      std::string line;
      while (getline(myfile, line))
      {
        string_coordinates.push_back(line);
      }
      myfile.close();
    }
    std::vector< ImageType::IndexType > coordinates, coordinates_real;
    coordinates.resize(string_coordinates.size());
    coordinates_real.resize(string_coordinates.size());
    for (size_t i = 0; i < string_coordinates.size(); i++)
    {
      auto temp_coords = cbica::stringSplit(string_coordinates[i], " ");
      for (size_t j = 0; j < ImageType::ImageDimension; j++)
      {
        // gets the index of the point in question
        coordinates[i][j] = std::abs((std::atof(temp_coords[j].c_str()) * image_inputLabel->GetSpacing()[j]) + image_inputLabel->GetOrigin()[j]);
        coordinates_real[i][j] = std::atof(temp_coords[j].c_str());
      }
    }

    std::vector< std::pair< float, ImageType::IndexType > > distsAndIndeces;
    for (size_t i = 0; i < coordinates.size(); i++)
    {
      distsAndIndeces.push_back(cbica::GetMaxDistanceInLabelMap(image_inputLabel, coordinates[i]));
    }

    auto blah = 1;
  }

  if (parser.compareParameter("dicom", tempPosition))
  {
    const std::string stringToCheck = argv[tempPosition + 1];

    auto stringToCheck_wrap = cbica::normPath(stringToCheck);
    auto stringToCheck_wrap_dir = cbica::getFilenamePath(stringToCheck_wrap);

    //auto dir_uids = cbica::GetDICOMSeriesInDir(stringToCheck_wrap_dir);

    auto seriesReader = itk::ImageSeriesReader< itk::Image< float, 3 > >::New();
    auto dicomIO = itk::DCMTKImageIO::New();
    auto inputNames = itk::DCMTKSeriesFileNames::New();
    //inputNames->SetLoadPrivateTags(true);
    //// doesn't work: 0020|0011, 0020|000E, 
    ////inputNames->AddSeriesRestriction("0020|0011"); // check with both '0020|0011-Series number' and '0020|000E-Series UID'
    ////inputNames->AddSeriesRestriction("0020|000E"); // check with both '0020|0011-Series number' and '0020|000E-Series UID'
    //inputNames->AddSeriesRestriction(restriction); // check with both '0020|0011-Series number' and '0020|000E-Series UID'
    //inputNames->SetUseSeriesDetails(true);
    //inputNames->SetDirectory(stringToCheck_wrap_dir);
    //auto test = inputNames->GetInputFileNames();

    // works: 0008|103E, 0020|0011, 0020|000E
    // not works: 
    auto inputNames_2 = itk::GDCMSeriesFileNames::New();
    inputNames_2->SetLoadPrivateTags(true);
    //inputNames_2->AddSeriesRestriction("0020|0011"); // check with both '0020|0011-Series number' and '0020|000E-Series UID'
    //inputNames_2->AddSeriesRestriction("0020|000E"); // check with both '0020|0011-Series number' and '0020|000E-Series UID'
    //inputNames_2->AddSeriesRestriction("0020|000E"); // check with both '0020|0011-Series number' and '0020|000E-Series UID'
    inputNames_2->SetUseSeriesDetails(true);
    inputNames_2->SetInputDirectory(stringToCheck_wrap_dir);
    auto filesToRead = inputNames_2->GetInputFileNames();

    auto uniqueUIDsAndFiles = cbica::GetDICOMSeriesAndFilesInDir(stringToCheck_wrap_dir);

    seriesReader->SetFileNames(filesToRead);
    seriesReader->SetImageIO(dicomIO);
    seriesReader->Update();

    auto image = seriesReader->GetOutput();

    bool singleFile = false;

    if (cbica::isFile(stringToCheck_wrap))
    {
      auto stringToCheck_wrap_dir = cbica::getFilenamePath(stringToCheck_wrap);

      singleFile = true;
      using ImageType = itk::Image< float, 2 >;
      auto image = cbica::ReadImage< ImageType >(stringToCheck_wrap);

      auto spacing = image->GetSpacing();

      int blah = 1;
    }
    else
    {
      std::cout << "Multi-file DICOM detected.\n";
      using ImageType = itk::Image< float, 3 >;

      auto imageInfo = cbica::ImageInfo(stringToCheck_wrap);

      auto image = cbica::ReadDicomImage< ImageType >(stringToCheck_wrap);

      auto spacing = image->GetSpacing();


      int blah = 1;
    }
  }

  return EXIT_SUCCESS;
}
/**
\file  OpenCVFunctionTests.cxx

\brief File for testing the OpenCV classes.

https://www.cbica.upenn.edu/sbia/software/
software@cbica.upenn.edu

Copyright (c) 2017 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/

#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <string>
#include <algorithm>

#include "opencv2/ml.hpp"

#include "classes/cbicaUtilities.h"
#include "classes/cbicaCmdParser.h"

#include "cbicaOPENCVUtilities.h"

int main(int argc, char** argv)
{
  cbica::CmdParser parser(argc, argv);
  parser.addOptionalParameter("r", "roc", cbica::Parameter::NONE, "", "ROC test");

  if (parser.isPresent("r"))
  {
    const std::string rocFile = argv[2];

    auto samplesAndLabels = cbica::readCSVDataFile< float >(rocFile, true);

    std::vector< float > inputScores, inputLabels;
    cv::Mat inputData = cv::Mat(static_cast< int >(samplesAndLabels[0].size()), static_cast< int >(samplesAndLabels.size()), CV_32F);

    for (int i = 0; i < inputData.rows; i++)
    {
      inputData.ptr< float >(i)[0] = static_cast< float >(samplesAndLabels[0][i]);
      inputData.ptr< float >(i)[1] = static_cast< float >(samplesAndLabels[1][i]);
    }

    auto output = cbica::ROC_Values(inputData);

    int blah = 1;
  }

  return EXIT_SUCCESS;
}
#include "cbicaOPENCVUtilities.h"

namespace cbica
{
  std::map< std::string, size_t > ConfusionMatrix(const cv::Mat &inputRealAndPredictedLabels)
  {
    std::vector< float > returnTP, returnFP;
    cv::Mat inputData = inputRealAndPredictedLabels;
    // consider the case of a column-major data being provided
    if (inputData.cols > inputData.rows)
    {
      inputData = inputData.t();
    }

    auto depthCondition = CV_32F;
    if (inputData.depth() != depthCondition)
    {
      //std::cerr << "Changing depth of input to CV_32F\n";
      inputData.convertTo(inputData, depthCondition);
    }

    std::vector< float > realLabels, predictedLabels;
    for (size_t i = 0; i < inputData.rows; i++)
    {
      realLabels.push_back(inputData.ptr< float >(i)[0]);
      predictedLabels.push_back(inputData.ptr< float >(i)[1]);
    }

    return cbica::ConfusionMatrix(realLabels, predictedLabels); // call ROC from cbica Utilities
  }

}
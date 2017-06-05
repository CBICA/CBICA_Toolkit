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

  std::map< std::string, float > ROC_Values(const cv::Mat &inputRealAndPredictedLabels)
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
    realLabels.resize(inputData.rows);
    predictedLabels.resize(inputData.rows);
    for (size_t i = 0; i < inputData.rows; i++)
    {
      realLabels[i] = inputData.ptr< float >(i)[0];
      predictedLabels[i] = inputData.ptr< float >(i)[1];
    }

    return cbica::ROC_Values(realLabels, predictedLabels); // call ROC from cbica Utilities
  }

  cv::Mat Z_Scores(const cv::Mat &inputVector)
  {
    cv::Mat inputData = inputVector;
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

    // construct the returnMat with the appropriate size
    cv::Mat returnMat = cv::Mat::zeros(inputData.size(), inputData.depth());

    // populate data in a standard data structure
    std::vector< float > actualData;
    actualData.resize(inputData.rows);
    for (size_t i = 0; i < inputData.rows; i++)
    {
      actualData[i] = inputData.ptr< float >(i)[0];
    }

    // get output from cbica::Utilities
    auto outputZScores = cbica::Z_Scores(actualData);
    
    // popoulate returnMat with the output
    for (size_t i = 0; i < returnMat.rows; i++)
    {
      returnMat.ptr< float >(i)[0] = outputZScores[i];
    }

    return returnMat;
  }
}
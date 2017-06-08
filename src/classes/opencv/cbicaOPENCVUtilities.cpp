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
    for (int i = 0; i < inputData.rows; i++)
    {
      realLabels.push_back(inputData.ptr< float >(i)[0]);
      predictedLabels.push_back(inputData.ptr< float >(i)[1]);
    }

    return cbica::ConfusionMatrix(realLabels, predictedLabels); // call ROC from cbica Utilities
  }

  std::map< std::string, float > ROC_Values(const cv::Mat &inputRealValues, const cv::Mat &inputPredictedValues)
  {
    std::vector< float > realLabels = GetVectorFromMat(inputRealValues),
      predictedLabels = GetVectorFromMat(inputPredictedValues);

    return cbica::ROC_Values(realLabels, predictedLabels); // call ROC from cbica Utilities
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

    return ROC_Values(inputData.col(0), inputData.col(1));
  }

  cv::Mat Z_Scores(const cv::Mat &inputMat)
  {
    auto inputData = GetVectorFromMat< float >(inputMat);

    // construct the returnMat with the appropriate size
    cv::Mat returnMat = cv::Mat::zeros(inputMat.size(), inputMat.depth());

    // get output from cbica::Utilities
    auto outputZScores = cbica::ZScores(inputData);
    
    // popoulate returnMat with the output
    for (int i = 0; i < returnMat.rows; i++)
    {
      returnMat.ptr< float >(i)[0] = outputZScores[i];
    }

    return GetMatFromVector(outputZScores, inputMat.size());
  }
}
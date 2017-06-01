#include "cbicaOPENCVUtilities.h"

namespace cbica
{
  std::pair< std::vector< float >, std::vector< float > > ROC(const cv::Mat &inputScoresAndLabels)
  {
    std::vector< float > returnTP, returnFP;
    cv::Mat inputData = inputScoresAndLabels;
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

    auto P = cv::countNonZero(inputData.col(1)); // should this be checking for 1s only?
    auto N = inputData.cols - P; // should this be checking for 0s or !1 only?

    // initialize the threshold to check
    auto f_prev = -std::numeric_limits< float >::infinity();
    double TP = 0, FP = 0;

    for (size_t i = 0; i < inputData.rows; i++)
    {
      auto f_i = inputData.ptr< float >(i)[0];
      auto label = inputData.ptr< float >(i)[1];

      // add points to ROC curve
      if (f_i != f_prev)
      {
        returnTP.push_back(TP / P);
        returnFP.push_back(FP / N);

        f_prev = f_i;
      }

      if (label == 1)
      {
        TP++;
      }
      else
      {
        FP++;
      }
    }

    // add point 1-1
    returnTP.push_back(TP / P);
    returnFP.push_back(FP / N);

    return std::make_pair(returnTP, returnFP);
  }
}
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
    return GetMatFromVector(
      cbica::ZScores(GetVectorFromMat< float >(inputMat)) // vectorize the input and then calculate the zscores from cbica::utilities
      , inputMat.size()); // adjust the size of the return in accordance with the input
  }

  void SaveAsCSV(const cv::Mat &inputMat, const std::string &filename)
  {
    std::ofstream myfile;
    myfile.open(filename.c_str());
    myfile << cv::format(inputMat, cv::Formatter::FMT_CSV) << std::endl;
    myfile.close();
  }

  cv::Mat ReadFromCSV(const std::string &filename)
  {
    std::ifstream inputfile(filename.c_str());

    std::string current_line;

    // vector allows you to add data without knowing the exact size beforehand
    std::vector< std::vector<float> > all_data;

    // Start reading lines as long as there are lines in the file
    while (getline(inputfile, current_line))
    {
      // Now inside each line we need to seperate the cols
      std::vector< float > values;
      std::stringstream temp(current_line);
      std::string single_value;
      while (getline(temp, single_value, ','))
      {
        // convert the string element to a integer value
        values.push_back(atoi(single_value.c_str()));
      }
      // add the row to the complete data vector
      if (!values.empty())
      {
        all_data.push_back(values);
      }
    }

    // Now add all the data into a Mat element
    cv::Mat vect = cv::Mat::zeros((int)all_data.size(), (int)all_data[0].size(), CV_32FC1);
    // Loop over vectors and add the data
    for (int rows = 0; rows < (int)all_data.size(); rows++)
    {
      for (int cols = 0; cols< (int)all_data[0].size(); cols++)
      {
        vect.ptr< float >(rows)[cols] = all_data[rows][cols];
      }
    }

    return vect;
  }
}
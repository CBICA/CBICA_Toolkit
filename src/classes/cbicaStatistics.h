#pragma once

#include <cmath>
#include <algorithm>

namespace cbica
{
  /**
  \brief Stand-alone helper class to generate statistics in a efficient manner

  Usage:
  std::vector< int > myArray;
  cbica::Statistics< int > calculator(myArray); // OR use 'calculator.SetInput(myArray)' after doing 'cbica::Statistics< int > calculator;'
  std::cout << "Sum = " << calculator.GetSum() << "\n;
  std::cout << "Mean = " << calculator.GetMean() << "\n;
  std::cout << "Variance = " << calculator.GetVariance() << "\n;
  std::cout << "StandardDev = " << calculator.GetStandardDeviation() << "\n;
  std::cout << "Kurtosis = " << calculator.GetKurtosis() << "\n;
  std::cout << "Skewness = " << calculator.GetSkewness() << "\n;

  */
  template< class TDataType = float >
  class Statistics
  {
  public:
    //! Default Constructor
    Statistics() {};

    //! Constructor with input
    Statistics(std::vector< TDataType >& inputArray)
    {
      InitializeClass(inputArray);
    }

    //! Set new input
    void SetInput(std::vector< TDataType >& inputArray)
    {
      InitializeClass(inputArray);
    }

    //! Default Destructor
    ~Statistics()
    {
      m_input.clear(); // not really needed 
    }
    
    //! Does exactly what it says
    double GetMaximum()
    {
      return m_max;
    }

    //! Does exactly what it says
    double GetMinimum()
    {
      return m_min;
    }

    //! Does exactly what it says
    double GetSum()
    {
      return m_sum;
    }

    //! Does exactly what it says
    double GetMean()
    {
      return m_mean;
    }

    //! Does exactly what it says
    double GetVariance()
    {
      return m_variance;
    }

    TDataType GetMode()
    {
      if (!mode_calculated)
      {
        m_mode = m_input_Sorted[0];
        auto number = m_input_Sorted[0];
        int count = 1;
        int countMode = 1;

        for (size_t i = 1; i < m_input_Sorted.size(); i++)
        {
          if (m_input_Sorted[i] == number)
          { // count occurrences of the current number
            ++count;
          }
          else
          { // now this is a different number
            if (count > countMode)
            {
              countMode = count; // mode is the biggest ocurrences
              m_mode = number;
            }
            count = 1; // reset count for the new number
            number = m_input_Sorted[i];
          }
        }
        mode_calculated = true;
      }

      return m_mode;
    }

    TDataType GetMedian()
    {
      if (!median_calculated)
      {
        if (!m_input_Sorted.empty())
        {
          auto size = m_input_Sorted.size();
          if (size % 2 == 0)
          {
            m_median = (m_input_Sorted[size / 2 - 1] + m_input_Sorted[size / 2]) / 2;
          }
          else
          {
            m_median = m_input_Sorted[size / 2];
          }
        }
        else
        {
          std::cerr << "Array cannot be empty.\n";
          return std::numeric_limits< TDataType >::min();
        }
        median_calculated = true;
      }

      return m_median;
    }

    //! Does exactly what it says
    double GetStandardDeviation()
    {
      CalculateSTD();
      return m_stdDev;
    }

    //! Does exactly what it says
    double GetKurtosis()
    {
      if (!kurtosis_calculated)
      {
        CalculateSTD();

        m_kurtosis = weirdStatistics(4);
        kurtosis_calculated = true;
      }

      return m_kurtosis;
    }

    //! Does exactly what it says
    double GetSkewness()
    {
      if (!skewness_calculated)
      {
        CalculateSTD();

        m_skewness = weirdStatistics(3);
        skewness_calculated = true;
      }

      return m_skewness;
    }

    //! Does exactly what it says
    std::vector< double > GetZScores()
    {
      if (!zscores_calculated)
      {
        m_zscores.clear();
        for (const TDataType& element : m_input)
        {
          m_zscores.push_back((element - m_mean) / m_stdDev);
        }
        zscores_calculated = true;
      }

      return m_zscores;
    }

  private:
    std::vector< TDataType > m_input;
    std::vector< TDataType > m_input_Sorted;

    //! actual outputs
    double m_sum = 0.0, m_mean = 0.0, m_variance = 0.0, m_stdDev = 0.0, m_kurtosis = 0.0, m_skewness = 0.0, m_max = 0.0, m_min = 0.0;

    TDataType m_mode, m_median;

    std::vector< double > m_zscores;

    //! flags to check if something has been calculated or not
    bool stdDev_calculated = false, kurtosis_calculated = false, skewness_calculated = false, zscores_calculated = false, 
      mode_calculated = false, median_calculated = false;

    //! This function gets called every time the input is set, for obvious reasons
    void InitializeClass(std::vector< TDataType >& inputArray)
    {
      m_input = inputArray;
      m_input_Sorted = inputArray;
      std::sort(m_input_Sorted.begin(), m_input_Sorted.end());

      stdDev_calculated = false;
      kurtosis_calculated = false;
      skewness_calculated = false;
      zscores_calculated = false;
      mode_calculated = false; 
      median_calculated = false;
      m_sum = std::accumulate(m_input.begin(), m_input.end(), 0.0);
      m_mean = m_sum / m_input.size();
      auto result = std::minmax_element(m_input.begin(), m_input.end());
      m_min = *result.first;
      m_max = *result.second;
      for (const TDataType& element : m_input)
      {
        m_variance += std::pow(static_cast<double>(element - m_mean), 2);
      }

      m_variance = m_variance / (m_input.size() - 1);
    }

    //! Helper function for calculating kurtosis and skewness
    double weirdStatistics(size_t power)
    {
      double returnStat = 0.0;
      for (size_t x = 0; x < m_input.size(); x++)
      {
        returnStat += std::pow(((m_input[x] - m_mean) / m_stdDev), power);
      }
      return (returnStat / m_input.size());
    }

    //! Single place to calculate STD Dev to reduce number of moving parts
    void CalculateSTD()
    {
      if (!stdDev_calculated)
      {
        m_stdDev = std::sqrt(static_cast<double>(m_variance));
        stdDev_calculated = true;
      }
    }
  };

}
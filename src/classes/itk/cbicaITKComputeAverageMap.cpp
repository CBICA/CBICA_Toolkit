/**
\file  cbicaITKComputeAverageMap.cpp

\brief Implementation of the ComputeAverageMap class

https://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#include "cbicaITKComputeAverageMap.h"
#include "cbicaITKImageInfo.h"

namespace cbica
{  
  inline void ComputeAverageMap::runAlgorithm()
  {
    switch( m_dimensions )
    {
    case 2:
      {
        ComputeAverageMap::invokeRunner<2>();
        break;
      }
    case 3:
      {
        ComputeAverageMap::invokeRunner<3>();
        break;
      }
    default:
      {
        std::cerr << "Unsupported dimension size.\n";
        break;
      }
    }    
  }
  
  ComputeAverageMap::ComputeAverageMap()
  {

  }
  
  ComputeAverageMap::ComputeAverageMap( const std::vector<std::string> &inputFiles,
                                        const std::string &output ) : 
                                        CommonHolder(inputFiles, output)
  {
    m_verbose = false;
    ComputeAverageMap::runAlgorithm();
  }
  
  ComputeAverageMap::ComputeAverageMap( const std::vector<std::string> &inputFiles,
                                        const std::string &output, 
                                        const bool verbose ) : 
                                        CommonHolder(inputFiles, output)
  {
    m_verbose = verbose;
    ComputeAverageMap::runAlgorithm();
  }
  
  ComputeAverageMap::ComputeAverageMap( const std::vector<std::string> &inputFiles,
                                        const std::string &output, 
                                        const std::string &prefix ) : 
                                        CommonHolder(inputFiles, output, prefix)
  {
    m_verbose = false;
    ComputeAverageMap::runAlgorithm();
  }
  
  ComputeAverageMap::ComputeAverageMap( const std::vector<std::string> &inputFiles,
                                        const std::string &output, 
                                        const bool verbose, const std::string &prefix ) : 
                                        CommonHolder(inputFiles, output, prefix)
  {
    m_verbose = verbose;
    ComputeAverageMap::runAlgorithm();
  }

  ComputeAverageMap::~ComputeAverageMap()
  {

  }
  
  void ComputeAverageMap::SetParameters( const std::vector<std::string> &inputFileNames, 
                                         const std::string &output, const std::string &prefix )
  {
    CommonHolder(inputFileNames, output, prefix);
    ComputeAverageMap::runAlgorithm();
  }
}
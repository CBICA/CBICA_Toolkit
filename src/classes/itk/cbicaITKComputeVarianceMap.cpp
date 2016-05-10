/**
\file  cbicaITKComputeVarianceMap.cpp

\brief Implementation of the ComputeVarianceMap class

https://www.cbica.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/

#include "cbicaITKComputeVarianceMap.h"

namespace cbica
{
  ComputeVarianceMap::ComputeVarianceMap()
  {

  }
  
  ComputeVarianceMap::ComputeVarianceMap( const std::string &inputFileName, 
                                          const std::string &output ) :
                                          CommonHolder(inputFileName, output)
  {
    
    ComputeVarianceMap::runAlgorithm();
  }

  ComputeVarianceMap::ComputeVarianceMap( const std::vector<std::string> &inputFileNames, 
                                          const std::string &output ) :
                                          CommonHolder(inputFileNames, output)
  {
    
    ComputeVarianceMap::runAlgorithm();
  }
  
  ComputeVarianceMap::ComputeVarianceMap( const std::string &inputFileName, 
                                          const std::string &output, 
                                          const std::string &prefix ) :
                                          CommonHolder(inputFileName, output, prefix)
  {
    
    ComputeVarianceMap::runAlgorithm();
  }
  
  ComputeVarianceMap::ComputeVarianceMap( const std::vector<std::string> &inputFileNames, 
                                          const std::string &output, 
                                          const std::string &prefix ) :
                                          CommonHolder(inputFileNames, output, prefix)
  {
    
    ComputeVarianceMap::runAlgorithm();
  }
  
  ComputeVarianceMap::~ComputeVarianceMap()
  {

  }
  
  void ComputeVarianceMap::SetParameters( const std::vector<std::string> &inputFileNames, 
                        const std::string &output, const std::string &prefix )
  {
    CommonHolder(inputFileNames, output, prefix);
    ComputeVarianceMap::runAlgorithm();
  }

  inline void ComputeVarianceMap::runAlgorithm()
  {
    if ( m_pixelType == itk::ImageIOBase::SCALAR )
    {
      switch ( m_componentType )
      {
        case itk::ImageIOBase::CHAR:
        {
          computeVarianceRunner<char,3>(m_inputFiles,m_outputFiles[0]);
          break;
        }
        case itk::ImageIOBase::UCHAR:
        {
          computeVarianceRunner<unsigned char,3>(m_inputFiles,m_outputFiles[0]);
          break;
        }
        case itk::ImageIOBase::USHORT:
        {
          computeVarianceRunner<unsigned short,3>(m_inputFiles,m_outputFiles[0]);
          break;
        }
        case itk::ImageIOBase::SHORT:
        {
          computeVarianceRunner<short,3>(m_inputFiles,m_outputFiles[0]);
          break;
        }
        case itk::ImageIOBase::UINT:
        {
          computeVarianceRunner<unsigned int,3>(m_inputFiles,m_outputFiles[0]);
          break;
        }
        case itk::ImageIOBase::INT:
        {
          computeVarianceRunner<int,3>(m_inputFiles,m_outputFiles[0]);
          break;
        }
        case itk::ImageIOBase::ULONG:
        {
          computeVarianceRunner<unsigned long,3>(m_inputFiles,m_outputFiles[0]);
          break;
        }
        case itk::ImageIOBase::LONG:
        {
          computeVarianceRunner<long,3>(m_inputFiles,m_outputFiles[0]);
          break;
        }
        case itk::ImageIOBase::FLOAT:
        {
          computeVarianceRunner<float,3>(m_inputFiles,m_outputFiles[0]);
          break;
        }
        case itk::ImageIOBase::DOUBLE:
        {
          computeVarianceRunner<double,3>(m_inputFiles,m_outputFiles[0]);
          break;
        }      
        default:
        {
          std::cerr << "Unsupported component Type: " << m_componentType_asString << "\n";
        }
      }
    }
  }
  
}
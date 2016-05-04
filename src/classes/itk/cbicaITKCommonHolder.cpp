/**
\file  cbicaITKCommonHolder.cpp

\brief Implementation of the CommonHolder class

This class computes the average of a series of 2 or 3 dimension images.

https://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#include "cbicaITKCommonHolder.h"
#include "cbicaITKImageInfo.h"

namespace cbica
{
  inline void CommonHolder::initializeClass( const std::vector<std::string> &inputFileNames, 
                                             const std::string &output, 
                                             const std::string &prefix )
  {
    if (inputFileNames.empty())
    {
      std::cerr << "Please provide at least one input.\n";
      return;
    }

    m_outputDir = "";
    m_extension = "";
    if (output.empty())
    {
      m_outputDir = cbica::createTmpDir();
      std::cout << "No output folder has been specified. Writing to temporary directory: '"
        << m_outputDir << "'\n";
    }
    m_inputFiles = inputFileNames;
    m_prefix = prefix;
    std::string extName, baseName, path;

    m_componentType_asString = "";

    for (unsigned int i = 0; i < m_inputFiles.size(); ++i)
    {
      if (!fileExists(m_inputFiles[i]))
      {
        std::cerr << "The " << i << "'th Input file does not exist. Please check.\n";
        return;
      }
      
      replaceString(m_inputFiles[i], "\\", "/");

      if (splitFileName(m_inputFiles[i], path, baseName, extName))
      {
        if (extName == "img")
        {
          std::cerr << "Header has been supplied as '.img' in file #" << i << 
            ". Please supply it as '.hdr'.\n";
          return;
        }
        
        if (m_extension == "")
        {
          m_extension = extName;
        }
        else if (m_extension != extName)
        {
          std::cerr << "Extension mismatch in input files #" << i-1 << " and #" << i <<
            ". Please check.\n";
          return;
        }
      }
      
      cbica::ImageInfo imageIO = cbica::ImageInfo(m_inputFiles[i]);

      if (m_componentType_asString == "")
      {
        m_componentType = imageIO.getComponentType();
        m_pixelType = imageIO.getPixelType();
        m_componentType_asString = imageIO.getComponentTypeAsString();
        m_pixelType_asString = imageIO.getPixelTypeAsString();
      }
      
      if (m_componentType != imageIO.getComponentType())
      {
        std::cerr << "Component type mismatch between images #" << i-1 << " and #" << i <<
          ". Please check.\n";
        return;
      }
      
      if (m_pixelType != imageIO.getPixelType()) 
      {
        std::cerr << "Pixel type mismatch between images #" << i-1 << " and #" << i <<
          ". Please check.\n";
        return;
      }

      
      if (m_outputDir == "")
      {
        cbica::replaceString(output, "\\", "/");
        //if (std::string(1, output.back()) == "/")
        if (output.at(output.size()-1) == '/')
        {
          std::cout << "An output directory has been defined. Using the default output file name " << 
            "'o+<input_file>.hdr'\n";
          m_outputDir = output;
          m_outputFiles.push_back(m_outputDir + m_prefix + baseName + "." + m_extension);
          m_outputBaseNames.push_back(m_outputDir + m_prefix + baseName);
        }
        else if (output.find("/") != std::string::npos)
        {
          std::cout << "An output file has been defined with path. Discarding default prefix 'o'.\n";
          std::string extName_2, baseName_2, path_2;
          if (splitFileName(output, path_2, baseName_2, extName_2))
          {
            if (extName_2 != "hdr")
              std::cout << "Unsupported output file format provided, will write to 'hdr' file instead.\n";
          }
          m_outputBaseNames.push_back(baseName_2);
          m_outputDir = path_2;
          m_outputFiles.push_back(path_2 + m_prefix + baseName_2);
        }
        else
        {
          m_outputDir = cbica::createTmpDir();
          std::cout << "An output file has been defined without path. Saving in directory: "
            << m_outputDir << "\n";
          m_outputBaseNames.push_back(m_outputDir + m_prefix + baseName);
          m_outputFiles.push_back(m_outputDir + output);
        }
      }
      else
      {
        m_outputFiles.push_back(m_outputDir + m_prefix + baseName + "." + m_extension);
        m_outputBaseNames.push_back(m_outputDir + m_prefix + baseName);
      }

    }
    //m_extension = extName;
    m_extension = "hdr";
    
    //runAlgorithm();
  }
  
  CommonHolder::CommonHolder()
  {

  }
  
  CommonHolder::CommonHolder( const std::string &inputFileName )
  {
    std::vector<std::string> inputFilesWrap;
    inputFilesWrap.push_back(inputFileName);
    initializeClass(inputFilesWrap, "", "o");
  }
  
  CommonHolder::CommonHolder( const std::vector<std::string> &inputFileNames )
  {
    m_outputDir = cbica::createTmpDir();
    initializeClass(inputFileNames, m_outputDir, "o");
  }
  
  CommonHolder::CommonHolder( const std::string &inputFileName, 
                              const std::string &output )
  {
    std::vector<std::string> inputFilesWrap;
    inputFilesWrap.push_back(inputFileName);
    initializeClass(inputFilesWrap, output, "o");
  }
  
  CommonHolder::CommonHolder( const std::string &inputFileName, 
                              const std::string &output, 
                              const std::string &prefix )
  {
    std::vector<std::string> inputFilesWrap;
    inputFilesWrap.push_back(inputFileName);
    initializeClass(inputFilesWrap, output, prefix);
  }
  
  CommonHolder::CommonHolder( const std::vector<std::string> &inputFileNames,  
                              const std::string &output )
  {
    initializeClass(inputFileNames, output, "o");
  }
  
  CommonHolder::CommonHolder( const std::vector<std::string> &inputFileNames, 
                              const std::string &output, 
                              const std::string &prefix )
  {
    initializeClass(inputFileNames, output, prefix);
  }
  
  CommonHolder::~CommonHolder()
  {

  }
  
  void CommonHolder::SetParameters( const std::vector<std::string> &inputFileNames, 
                                    const std::string &output, const std::string &prefix )
  {
    //initializeClass(inputFileNames, output, prefix);
  }

  void CommonHolder::checkInputs()
  {

  }
  
  void CommonHolder::runAlgorithm()
  {

  }
  
}
/**
\file  cbicaITKCommonHolder.h

\brief Declaration of the CommonHolder class

This class computes the average of a series of 2 or 3 dimension images.

https://www.cbica.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "itkImageIOBase.h"
#include "itkImageIOFactory.h"
#include "itkDiffusionTensor3D.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkNiftiImageIO.h"

#include "cbicaUtilities.h"

using namespace itk;

namespace cbica
{
  /**
  \class CommonHolder

  \brief Common data container for all statistical computations of images
  */
  class CommonHolder
  {
  public:
    
    /**
    \brief Default Constructor

    Use the SetParameters() method if this is used
    */
    explicit CommonHolder();
    
    /**
    \brief Actual Constructor

    ALL scalars are computed. Output is saved using cbica::createTmpDir()

    \param inputFileName The Input File Name
    */
    explicit CommonHolder( const std::string &inputFileName );
    
    /**
    \brief Actual Constructor

    ALL scalars are computed. Output is saved using cbica::createTmpDir()

    \param inputFileNames The Input File Names
    */
    explicit CommonHolder( const std::vector<std::string> &inputFileNames );
    
    /**
    \brief Actual Constructor

    ALL scalars are computed.

    \param inputFileName The Input File Name
    \param output The output file or directory
    */
    explicit CommonHolder( const std::string &inputFileName, const std::string &output );
    
    /**
    \brief Actual Constructor

    ALL scalars are computed.

    \param inputFileNames Vector of input file names
    \param output The output file or directory
    */
    explicit CommonHolder( const std::vector<std::string> &inputFileNames, const std::string &output );
    
    /**
    \brief Actual Constructor

    \param inputFileName The Input File Name
    \param output The output file or directory
    \param prefix Prefix for output. Disregarded if output is file
    */
    explicit CommonHolder( const std::string &inputFileName, const std::string &output, 
                           const std::string &prefix );
    
    /**
    \brief Actual Constructor

    \param inputFileNames Vector of input file names
    \param output The output file or directory
    \param prefix Prefix for output. Disregarded if output is file
    */
    explicit CommonHolder( const std::vector<std::string> &inputFileNames, const std::string &output, 
                           const std::string &prefix );
    
    /**
    \brief The Destructor
    */
    virtual ~CommonHolder();

  protected:
    // vector of input file names
    std::vector<FileNameParts> m_inputFiles_parts;
    std::vector< std::string > m_inputFiles;

    // output directory (to write multiple files)
    std::string m_outputDir;
    
    // output files (includes path as prefix)
    //std::string m_outputFile;

    // output basenames (include path as prefix)
    //std::string m_outputBaseName;
    
    // output files (includes path as prefix)
    std::vector<std::string> m_outputFiles;

    // output basenames (include path as prefix)
    std::vector<std::string> m_outputBaseNames;

    // other parameters
    std::string m_prefix, m_suffix, m_extension;

    itk::ImageIOBase::IOComponentType m_componentType;
    itk::ImageIOBase::IOPixelType m_pixelType;
    std::string m_componentType_asString, m_pixelType_asString;

    /**
    \brief Exception handling for input file names
    */
    virtual void checkInputs();

    /**
    \brief Set parameters after using default constructor

    \param inputFileNames Vector of input file(s)
    \param output Output file/dir
    \param prefix For output
    */
    virtual void SetParameters( const std::vector<std::string> &inputFileNames, 
                                const std::string &output, const std::string &prefix );

    /**
    \brief Main function to call different algorithms

    All inheriting classes need to implement this method. It should interface with initializeClass().
    */
    virtual void runAlgorithm();

    /**
    \brief Checks the input and throws exception if there is an issue

    This verifies the input image file and throws an exception if there is an issue. For variable (but required)
    inputs, please use checkInputs() function. If the inherited class doesn't have any required parameter other than 
    the input, then checkInputs() can be left as an empty void function. If the output is empty, then 
    cbica::createTmpDir() is invoked and the resultant directory is shown as output in the command line.

    \param inputFileNames Wrap for m_inputFiles
    \param output Can have the output file or directory. If file is specified without path, it is written in temp dir created using cbica::createTmpDir()
    */
    inline void initializeClass( const std::vector<std::string> &inputFileNames, 
                                 const std::string &output, 
                                 const std::string &prefix);

  };

}
/**
\file  cbicaITKComputeDtiScalars.h

\brief Declaration of the ComputeDtiScalars class

https://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include <string>
#include <iostream>
#include <sys/stat.h> 

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkDiffusionTensor3D.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNiftiImageIO.h"
#include "itkVector.h"

#include "cbicaITKCommonHolder.h"

namespace cbica
{
  /**
  \class ComputeDtiScalars
  
  \brief Computes the scalars of a single DTI image and writes the output

  \todo Reuqired for multiple images?
  */
  class ComputeDtiScalars : public CommonHolder
  {
  public:

    /**
    \brief Default Constructor

    Use the SetParameters() method if this is used
    */
    explicit ComputeDtiScalars();
    
    /**
    \brief Actual Constructor

    ALL scalars are computed.

    \param inputFileName The Input File Name
    \param output The output file or directory
    */
    explicit ComputeDtiScalars( const std::string &inputFileName, const std::string &output );
    
    /**
    \brief Actual Constructor

    Available options: 
    - FA   : FA images
    - EIG  : Eigensystem images
    - GEO  : Geoemtric features
    - SKEW : Tensor Skewness
    - KURT : Tensor kurtosis
    - RADAX: Radial and Axial diffusivity
    - GORDR: Gordon's R measures
    - GORDK: Gordon's K measures
    - ALL  : All of the above (default)

    \param inputFileName The Input File Name
    \param output The output file or directory
    \param parameters A string of requested parameters (mixed case characters allowed)
    */
    explicit ComputeDtiScalars( const std::string &inputFileName, const std::string &output,   
                                const std::string &parameters );
    
    /**
    \brief Actual Constructor

    \param inputFileName The Input File Name
    \param output The output file or directory
    \param parameters A string of requested parameters (mixed case characters allowed)
    \param prefix Prefix for output. Disregarded if output is file
    */
    explicit ComputeDtiScalars( const std::string &inputFileName, const std::string &output, 
                                const std::string &parameters, const std::string &prefix );
    
    /**
    \brief The Destructor
    */
    virtual ~ComputeDtiScalars();

    /**
    \brief Set the object parameters for the default () constructor

    \param inputFileName The Input File Name
    \param output Output file/dir
    \param parameters A string of requested parameters (mixed case characters allowed)
    \param prefix For written file(s)
    */
    void SetParameters( const std::string &inputFileName, const std::string &output, 
                        const std::string &parameters, const std::string &prefix );
    
    /**
    \brief Set the object parameters for the default () constructor

    Since this class utilizes only a single image (for multiple images, it can be called in a loop), 
    setting the input as a vector will tell the algorithm to take the first file only.

    \param inputFileNames Vector of input files
    \param output Output file/dir
    */
    void SetParameters( const std::vector<std::string> &inputFileNames, 
                        const std::string &output, const std::string &prefix );
    

  private:
    // string of parameters
    std::string m_parameters;

    bool m_verbose;

    itk::ImageIOBase::IOComponentType m_component;
    std::string m_component_asString;
    
    /**
    \brief Run the algorithm
    */
    inline void runAlgorithm();

    /**
    \brief Extra check for input files
    */
    inline void checkInputs() { /* all required parameters are checked by CommonHolder */ };

  };
}
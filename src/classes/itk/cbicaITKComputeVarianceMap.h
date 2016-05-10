/**
\file  cbicaITKComputeVarianceMap.h

\brief Declaration of the ComputeVarianceMap class

https://www.cbica.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h> 

#include "itkImageIOBase.h"
#include "itkImageIOFactory.h"
#include "itkDiffusionTensor3D.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkNiftiImageIO.h"

#include "itkNaryVarianceImageFilter.h"

#include "cbicaITKCommonHolder.h"

namespace cbica
{
  /**
  \class ComputeVarianceMap

  \brief Computes the variance map for a single image.

  \todo Support for calculating variance for multiple images (groundwork is ready, 
  just need to change the runAlgorithm() function
  */
  class ComputeVarianceMap : public CommonHolder
  {
  public:
    
    /**
    \brief Default Constructor

    Use the SetParameters() method if this is used
    */
    explicit ComputeVarianceMap();
    
    /**
    \brief Actual Constructor

    ALL scalars are computed.

    \param inputFileName The Input File Name
    \param output The output file or directory
    */
    explicit ComputeVarianceMap( const std::string &inputFileName, const std::string &output );
    
    /**
    \brief Actual Constructor

    ALL scalars are computed.

    \param inputFileNames Vector of input file names
    \param output The output file or directory
    */
    explicit ComputeVarianceMap( const std::vector<std::string> &inputFileNames, const std::string &output );
    
    /**
    \brief Actual Constructor

    \param inputFileName The Input File Name
    \param output The output file or directory
    \param prefix Prefix for output. Disregarded if output is file
    */
    explicit ComputeVarianceMap( const std::string &inputFileName, const std::string &output, 
                                 const std::string &prefix );
    
    /**
    \brief Actual Constructor

    \param inputFileNames Vector of input file names
    \param output The output file or directory
    \param prefix Prefix for output. Disregarded if output is file
    */
    explicit ComputeVarianceMap( const std::vector<std::string> &inputFileNames, const std::string &output, 
                                 const std::string &prefix );
    
    /**
    \brief The Destructor
    */
    virtual ~ComputeVarianceMap();
    
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
    bool m_verbose;

    /**
    \brief Function to call the Variance filter
    */
    template <typename PixelType,unsigned int Dimension>
    void computeVarianceRunner( std::vector<std::string> inpFiles, std::string outputFile )
    {    
      /*bool isScalar;
      if ( m_pixelType == itk::ImageIOBase::SCALAR )
        isScalar = true;
      else
        isScalar = false;*/
    
      // typedefs
      typedef typename itk::Image< PixelType,  Dimension >   InputImageType;
      typedef typename itk::ImageFileReader< InputImageType  >  ReaderType;
      typedef typename ReaderType::Pointer  ReaderPointerType;
      typedef typename itk::ImageFileWriter< InputImageType  >  WriterType;
      typedef typename itk::NaryVarianceImageFilter< InputImageType, InputImageType > FilterType;
      typename FilterType::Pointer varianceFilter = FilterType::New();
      
      // Lets hold on to all the readers?
      typename std::vector< ReaderPointerType > readers;
      for (size_t i=0; i<inpFiles.size() ;i++)
      {
        typename ReaderType::Pointer reader = ReaderType::New();
        itk::NiftiImageIO::Pointer imageIOr = itk::NiftiImageIO::New();
        reader->SetFileName( inpFiles[i] );
    
        readers.push_back(reader);
        
        reader->SetImageIO( imageIOr );
        varianceFilter->SetInput(i,reader->GetOutput());
      }
    
      // Write out the result
      typename WriterType::Pointer writer = WriterType::New();
      itk::NiftiImageIO::Pointer imageIOw = itk::NiftiImageIO::New();
      writer->SetImageIO( imageIOw );
      writer->SetFileName( outputFile );       
      writer->SetInput( varianceFilter->GetOutput() );      
      writer->Update();  
      
    }
    
    /**
    \brief Runs the algorithm right after the constructor
    */
    inline void runAlgorithm();
    
    /**
    \brief Extra check for input files
    */
    inline void checkInputs() { /* all required parameters are checked by CommonHolder */ };

  };

}
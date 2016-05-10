/**
\file  cbicaITKComputeAverageMap.h

\brief Declaration of the ComputeAverageMap class

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

#include "cbicaITKCommonHolder.h"
#include "itkDTILogEuclideanCalculator.h"
#include "itkNaryMeanImageFilter.h"
#include "itkNaryMeanDiffusionTensorImageFilter.h"
#include "itkDiffusionTensor3D.h"


using namespace itk;

namespace cbica
{
  /**
  \class ComputeAverageMap

  \brief Computes the average of a series of images and writes the output
  */
  class ComputeAverageMap : public CommonHolder
  {
  public:

    /**
    \brief Default Constructor
    */
    explicit ComputeAverageMap();

    /**
    \brief Actual Constructor
    
    \param inputFiles A vector of strings containing file names
    \param output The directory/file in which to write the files
    */
    explicit ComputeAverageMap( const std::vector<std::string> &inputFiles, 
                                const std::string &output);
    
    /**
    \brief Actual Constructor
    
    \param inputFiles A vector of strings containing file names
    \param output The directory/file in which to write the files
    \param verbose A boolean to show verbose output
    */
    explicit ComputeAverageMap( const std::vector<std::string> &inputFiles, 
                                const std::string &output, 
                                const bool verbose);
    
    /**
    \brief Actual Constructor
    
    \param inputFiles A vector of strings containing file names
    \param output The directory/file in which to write the files
    \param prefix A string which is a prefix for all written files
    */
    explicit ComputeAverageMap( const std::vector<std::string> &inputFiles, 
                                const std::string &output, 
                                const std::string &prefix);

    /**
    \brief Actual Constructor

    \param inputFiles A vector of strings containing file names
    \param output The directory/file in which to write the files
    \param verbose A boolean to show verbose output
    \param prefix A string which is a prefix for all written files
    */
    explicit ComputeAverageMap( const std::vector<std::string> &inputFiles, 
                                const std::string &output, 
                                const bool verbose, const std::string &prefix);

    /**
    \brief The Destructor
    */
    virtual ~ComputeAverageMap();
    
    /**
    \brief Set the object parameters for the default () constructor

    Since this class utilizes only a single image (for multiple images, it can be called in a loop), 
    setting the input as a vector will tell the algorithm to take the first file only.

    \param inputFileNames Vector of input files
    \param output Output file/dir
    */
    void SetParameters( const std::vector<std::string> &inputFileNames, 
                        const std::string &output, const std::string &prefix );
    
  protected:    
    
    /**
    \brief Main mean algorithm
    */
    template< typename PixelType, unsigned int Dimension >
    inline void computeMeanRunner( std::vector<std::string> &inpFiles, std::string &outputBase )
    {
      typedef typename itk::Image< PixelType,  Dimension >   InputImageType;
      typedef typename itk::ImageFileReader< InputImageType  >  ReaderType;
      typedef typename ReaderType::Pointer  ReaderPointerType;
      typedef typename itk::ImageFileWriter< InputImageType  >  WriterType;
      typedef typename itk::NaryMeanImageFilter< InputImageType, InputImageType > FilterType;
      typename FilterType::Pointer meanFilter = FilterType::New();
    
      // Lets hold on to all the readers
      typename std::vector< ReaderPointerType > readers;
      for( unsigned int i=0; i<inpFiles.size(); i++ )
      {
        typename ReaderType::Pointer reader = ReaderType::New();
        itk::NiftiImageIO::Pointer imageIOr = itk::NiftiImageIO::New();
        reader->SetFileName( inpFiles[i] );
    
        readers.push_back(reader);
    
        reader->SetImageIO( imageIOr );
        meanFilter->SetInput(i,reader->GetOutput());
      }
    
      // Write out the result
      typename WriterType::Pointer writer = WriterType::New();
      itk::NiftiImageIO::Pointer imageIOw = itk::NiftiImageIO::New();
      writer->SetImageIO( imageIOw );
      writer->SetFileName( outputBase );
    
      writer->SetInput( meanFilter->GetOutput() );
    
      writer->Update();
    
    }
    
    /**
    \brief Main mean algorithm for DTI images
    */
    template <typename PixelType,unsigned int Dimension>
    inline void computeMeanDTIRunner( std::vector<std::string> &inpFiles, std::string &outputBase )
    {
      typedef typename itk::Image< PixelType,  Dimension >   InputImageType;
      typedef typename itk::ImageFileReader< InputImageType  >  ReaderType;
      typedef typename ReaderType::Pointer  ReaderPointerType;
      typedef typename itk::ImageFileWriter< InputImageType  >  WriterType;
      typedef typename itk::NaryMeanDiffusionTensorImageFilter
                                              < InputImageType, InputImageType > FilterType;
      typename FilterType::Pointer meanFilter = FilterType::New();
      
      // Lets hold on to all the readers
      typename std::vector< ReaderPointerType > readers;
      for( unsigned int i=0; i<inpFiles.size(); i++ )
      {
        typename ReaderType::Pointer reader = ReaderType::New();
        itk::NiftiImageIO::Pointer imageIOr = itk::NiftiImageIO::New();
        reader->SetFileName( inpFiles[i] );
      
        readers.push_back(reader);
      
        reader->SetImageIO( imageIOr );
        meanFilter->SetInput(i,reader->GetOutput());
      }
      
      // Write out the result
      typename WriterType::Pointer writer = WriterType::New();
      itk::NiftiImageIO::Pointer imageIOw = itk::NiftiImageIO::New();
      writer->SetImageIO( imageIOw );
      writer->SetFileName( outputBase );
      
      writer->SetInput( meanFilter->GetOutput() );
      
      writer->Update();
    }

    /**
    \brief Runs the algorithm
    */
    inline void runAlgorithm();
    
  private:
    bool m_verbose, m_isScalar;
    int m_dimensions;
    
    /**
    \brief Invokes the computeRunner functions
    */
    template <const unsigned int Dimension>
    inline void invokeRunner()
    {
      if( m_isScalar )
      {
        std::cout << "Computing normal Average Map...\n";
        switch( m_componentType )
        {
        case itk::ImageIOBase::CHAR:
          {
            computeMeanRunner<char, Dimension>(m_inputFiles, m_outputFiles[0]);
            break;
          }
        case itk::ImageIOBase::UCHAR:
          {
            computeMeanRunner<unsigned char, Dimension>(m_inputFiles, m_outputFiles[0]);
            break;
          }
        case itk::ImageIOBase::SHORT:
          {
            computeMeanRunner<short, Dimension>(m_inputFiles, m_outputFiles[0]);
            break;
          }
        case itk::ImageIOBase::USHORT:
          {
            computeMeanRunner<unsigned short, Dimension>(m_inputFiles, m_outputFiles[0]);
            break;
          }
        case itk::ImageIOBase::INT:
          {
            computeMeanRunner<int, Dimension>(m_inputFiles, m_outputFiles[0]);
            break;
          }
        case itk::ImageIOBase::UINT:
          {
            computeMeanRunner<unsigned int, Dimension>(m_inputFiles, m_outputFiles[0]);
            break;
          }
        case itk::ImageIOBase::LONG:
          {
            computeMeanRunner<long, Dimension>(m_inputFiles, m_outputFiles[0]);
            break;
          }
        case itk::ImageIOBase::ULONG:
          {
            computeMeanRunner<unsigned long, Dimension>(m_inputFiles, m_outputFiles[0]);
            break;
          }
        case itk::ImageIOBase::FLOAT:
          {
            computeMeanRunner<float, Dimension>(m_inputFiles, m_outputFiles[0]);
            break;
          }
        case itk::ImageIOBase::DOUBLE:
          {
            computeMeanRunner<double, Dimension>(m_inputFiles, m_outputFiles[0]);
            break;
          }
        default:
          {
            std::cerr << "Unsupported component type: " << m_componentType_asString << std::endl;
            break;
          }
        }
      }
      
      else if( m_pixelType == itk::ImageIOBase::SYMMETRICSECONDRANKTENSOR ||
               m_pixelType == itk::ImageIOBase::DIFFUSIONTENSOR3D )
      {
        std::cout << "Computing Average Map from Tensor...\n";
        switch( m_componentType )
        {
        /*case itk::ImageIOBase::CHAR:
          {
            computeMeanDTIRunner<char, Dimension>(m_inputFiles, m_output);
            break;
          }
        case itk::ImageIOBase::UCHAR:
          {
            computeMeanDTIRunner<unsigned char, Dimension>(m_inputFiles, m_output);
            break;
          }
        case itk::ImageIOBase::SHORT:
          {
            computeMeanDTIRunner<short, Dimension>(m_inputFiles, m_output);
            break;
          }
        case itk::ImageIOBase::USHORT:
          {
            computeMeanDTIRunner<unsigned short, Dimension>(m_inputFiles, m_output);
            break;
          }
        case itk::ImageIOBase::INT:
          {
            computeMeanDTIRunner<int, Dimension>(m_inputFiles, m_output);
            break;
          }
        case itk::ImageIOBase::UINT:
          {
            computeMeanDTIRunner<unsigned int, Dimension>(m_inputFiles, m_output);
            break;
          }
        case itk::ImageIOBase::LONG:
          {
            computeMeanDTIRunner<long, Dimension>(m_inputFiles, m_output);
            break;
          }
        case itk::ImageIOBase::ULONG:
          {
            computeMeanDTIRunner<unsigned long, Dimension>(m_inputFiles, m_output);
            break;
          }*/
        case itk::ImageIOBase::FLOAT:
          {
            computeMeanDTIRunner<float, Dimension>(m_inputFiles, m_outputFiles[0]);
            break;
          }
        case itk::ImageIOBase::DOUBLE:
          {
            computeMeanDTIRunner<double, Dimension>(m_inputFiles, m_outputFiles[0]);
            break;
          }
        default:
          {
            std::cerr << "Unsupported component type: " << m_componentType_asString << std::endl;
            break;
          }
        }
      }
      
      else
      {
        std::cerr << "Unsupported pixel type: " << m_pixelType_asString << std::endl <<
          "ITKComputeAverageMap class can only work with Scalar or diffusion tensor images.\n";
      }
    }
    
    /**
    \brief Extra check for input files
    */
    inline void checkInputs() { /* all required parameters are checked by CommonHolder */ };

  };
}
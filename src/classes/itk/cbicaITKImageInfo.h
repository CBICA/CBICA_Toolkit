/**
\file  cbicaITKImageInfo.h

\brief Declaration of the ImageInfo class

Extracts basic information from any image (like dimensions, spacing, pixel type, etc.).

https://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include <algorithm>
#include <string>
//#include <tuple>

#include "itkImage.h"
#include "itkImageFileReader.h"

namespace cbica
{  
  /**
  \class ImageInfo

  \brief Reads any image from file name and generates relevant data.

  It can give information like dimensions, pixel type, component type, spacing, etc. about 
  the image which mitigates the requirement of explicit declration in the program.
  */
  class ImageInfo
  {
  public:
    
    /**
    \brief The Constructor
    
    Note: There the class "itk::MetaDataDictionary" does a far better job of obtaining the metadata of
    an image, if that is what the user requires. This function just obtains some basic information, 
    namely, the spacing and dimensions of the image.
    
    \param fName The image file name for which information is required
    */
    explicit ImageInfo(const std::string &fName);
    
    /**
    \brief The Destructor
    */
    virtual ~ImageInfo();
    
    /**
    \brief Get the imageIOBase of the specified image
    
    \return An itk::ImageIOBase which is overwritten with information
    */
    itk::SmartPointer<itk::ImageIOBase> getImageIOBase();
    
    /**
    \brief Get the Size of the specified image
    
    \return A vector of itk::SizeValueType which gets overwritten with information
    */
    std::vector<itk::SizeValueType> getImageSize();
    
    /**
    \brief Get the dimensions of the specified image
    
    \return A const unsigned int with the number of dimensions of the image
    */
    const unsigned int getImageDimensions()
    { 
      m_dimensions = m_itkImageIOBase->GetNumberOfDimensions(); 
      return m_itkImageIOBase->GetNumberOfDimensions();
    };
    
    /**
    \brief Get the Spacings of the specified image
    
    \return A vector of double which gets overwritten with information
    */
    std::vector<double> getImageSpacings();
    
    /**
    \brief Get the type of pixel in the image as a string

    \return Pixel type as a std::string
    */
    std::string getComponentTypeAsString();
    
    /**
    \brief Get the type of pixel in the image as an itk IOComponentType

    \return Pixel type as an itk IOComponentType
    */
    itk::ImageIOBase::IOComponentType getComponentType();
    
    /**
    \brief Get the type of pixel in the image as an itk IOComponentType

    \return Pixel type as an itk IOComponentType
    */
    std::string getPixelTypeAsString();

    /**
    \brief Get the type of pixel in the image as an itk IOComponentType

    \return Pixel type as an itk IOComponentType
    */
    itk::ImageIOBase::IOPixelType getPixelType();
        
  protected:
    std::string m_fileName;
    itk::SmartPointer<itk::ImageIOBase> m_itkImageIOBase;
    std::vector<double> m_spacings;
    std::vector<itk::SizeValueType> m_size;
    unsigned int m_dimensions;
    std::string m_pixelType_asString, m_IOComponentType_asString;
    itk::ImageIOBase::IOComponentType m_IOComponentType;
    itk::ImageIOBase::IOPixelType m_pixelType;
  };
}
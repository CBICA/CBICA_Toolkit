/**
\file  cbicaITKImageInfo.cpp

\brief Implementation of the ImageInfo class

https://www.cbica.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#include "cbicaITKImageInfo.h"

#include "cbicaUtilities.h"

namespace cbica
{
  // ============================================================================ //
  
  ImageInfo::ImageInfo( const std::string &fName )
  {
    m_fileName = fName;
    m_itkImageIOBase = itk::ImageIOFactory::CreateImageIO( 
                       fName.c_str(), itk::ImageIOFactory::ReadMode );
    
    // exception handling in case of NULL pointer initialization
    if ( m_itkImageIOBase )
    {
      m_itkImageIOBase->SetFileName(fName);
      m_itkImageIOBase->ReadImageInformation();
    }
    else
    {
      itkGenericExceptionMacro("Could not read the input image information from '" << fName << "'\n");
    }

    m_itkImageIOBase->SetFileName(fName);
    m_itkImageIOBase->ReadImageInformation();

    m_IOComponentType = m_itkImageIOBase->GetComponentType();
    m_pixelType = m_itkImageIOBase->GetPixelType();
    m_IOComponentType_asString = m_itkImageIOBase->GetComponentTypeAsString(m_IOComponentType);
    m_pixelType_asString = m_itkImageIOBase->GetPixelTypeAsString(m_pixelType);

    for (size_t i = 0; i<m_itkImageIOBase->GetNumberOfDimensions(); i++)
    {
      m_spacings.push_back(m_itkImageIOBase->GetSpacing(i));
      m_size.push_back(m_itkImageIOBase->GetDimensions(i));
    }
  }
  
  ImageInfo::~ImageInfo()
  {

  }

  itk::SmartPointer<itk::ImageIOBase> ImageInfo::getImageIOBase()
  {
    return m_itkImageIOBase;
  }

  std::vector<itk::SizeValueType> ImageInfo::getImageSize()
  {
    return m_size;
  }
  
  std::vector<double> ImageInfo::getImageSpacings()
  {
    return m_spacings;
  }

  std::string ImageInfo::getComponentTypeAsString()
  {
    return m_IOComponentType_asString;
  }
  
  itk::ImageIOBase::IOComponentType ImageInfo::getComponentType()
  {
    return m_IOComponentType;
  }
  
  std::string ImageInfo::getPixelTypeAsString()
  {
    return m_pixelType_asString;
  }
  
  itk::ImageIOBase::IOPixelType ImageInfo::getPixelType()
  {
    return m_pixelType;
  }
  
}
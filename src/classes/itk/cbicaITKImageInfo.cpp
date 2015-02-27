#include "cbicaITKImageInfo.h"

#include "itkImageIOFactory.h"

namespace cbica
{
  // ============================================================================ //
  
  /**
  \brief The Constructor

  Note: There the class "itk::MetaDataDictionary" does a far better job of obtaining the metadata of
  an image, if that is what the user requires. This function just obtains some basic information, 
  namely, the spacing and dimensions of the image.

  \param fName The image file name for which information is required
  */
  ITKImageInfo::ITKImageInfo( const std::string &fName )
  {
    m_itkImageIOBase = itk::ImageIOFactory::CreateImageIO( fName.c_str() , itk::ImageIOFactory::ReadMode);
    if ( m_itkImageIOBase )
    {
      m_itkImageIOBase->SetFileName(fName);
      m_itkImageIOBase->ReadImageInformation();
    }
    else
    {
      itkGenericExceptionMacro(<<"Could not read the input image information from '" << fName << "'\n");
    }

    for( int i=0; i<3; i++ )
    {
      m_spacings[i] = m_itkImageIOBase->GetSpacing(i);
      m_dimensions[i] = m_itkImageIOBase->GetDimensions(i);
    }
  }
  
  /**
  \brief The Destructor
  */
  ITKImageInfo::~ITKImageInfo()
  {

  }

  /**
  \brief Get the imageIOBase of the specified image

  \return An itk::ImageIOBase which is overwritten with information
  */
  itk::SmartPointer<itk::ImageIOBase> ITKImageInfo::getImageIOBase()
  {
    return m_itkImageIOBase;
  }

  /**
  \brief Get the Dimensions of the specified image

  \return A vector of itk::SizeValueType which gets overwritten with information
  */
  std::vector<itk::SizeValueType> ITKImageInfo::getImageDimensions()
  {
    return m_dimensions;
  }
  
  /**
  \brief Get the Spacings of the specified image

  \return A vector of double which gets overwritten with information
  */
  std::vector<double> ITKImageInfo::getImageSpacings()
  {
    return m_spacings;
  }
  
}
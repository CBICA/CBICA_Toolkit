/*!
 * \file  sbiaITK.h
 * \brief Some basic functions that use ITK backend.
 *

https://www.cbica.upenn.edu/sbia/software/
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html.

 */
#pragma once

#include <algorithm>
#include <string>
#include <tuple>

#include "itkImageIOBase.h"
#include "itkSmartPointer.h"


namespace cbica
{  
  class ITKImageInfo
  {
  public:
    explicit ITKImageInfo(const std::string &fName);
    virtual ~ITKImageInfo();

    itk::SmartPointer<itk::ImageIOBase> getImageIOBase();
    std::vector<itk::SizeValueType> getImageDimensions();
    std::vector<double> getImageSpacings();

  private:
    itk::SmartPointer<itk::ImageIOBase> m_itkImageIOBase;
    std::vector<double> m_spacings;
    std::vector<itk::SizeValueType> m_dimensions;
  };
}
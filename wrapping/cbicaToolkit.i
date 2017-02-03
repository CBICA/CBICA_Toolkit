%module cbicaToolkit
%{
#define SWIG_FILE_WITH_INIT

#include "cbicaUtilities.h"
#include "cbicaLogging.h"

#include "itkImage.h"
#include "itkImageFileReader.h"

#include "cbicaITKImageInfo.h"

%}

// system includes
%include "std_string.i"
//%include "std_set.i"
%include "std_vector.i"
//%include "std_pair.i"
//%include "std_map.i"
//%include "exception.i"
//%include "stdint.i"
%include stl.i



%include "cbicaUtilities.h"
%include "cbicaLogging.h"

%include "cbicaITKImageInfo.h"
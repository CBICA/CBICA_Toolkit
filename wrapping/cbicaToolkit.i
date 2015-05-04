%module cbicaToolkit
%{
#define SWIG_FILE_WITH_INIT

#include "cbicaUtilities.h"
#include "cbicaLogging.h"
#include "cbicaCmdParser.h"

%}

// system includes
%include stl.i
%include "std_string.i"
//%include "std_set.i"
%include "std_vector.i"
//%include "std_pair.i"
//%include "std_map.i"
//%include "exception.i"
//%include "stdint.i"


%include "cbicaUtilities.h"
%include "cbicaLogging.h"
%include "cbicaCmdParser.h"


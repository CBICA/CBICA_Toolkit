# CBICA Toolkit

<p align="center">
    <a href="https://dev.azure.com/CBICA/CaPTk/_build?definitionId=6" alt="Build Status"><img src="https://dev.azure.com/CBICA/CaPTk/_apis/build/status/CBICA.CBICA_Toolkit?branchName=master" /></a>
    <img src="https://img.shields.io/badge/language-c%2B%2B11-blue.svg" />
</p>

This project houses the common classes and functions that are used throughout CaPTk and other C++ projects under the CBICA umbrella (including collaborators). All functions/classes are cross-platform.

## Dependencies

- ITK
- OpenCV

Changes to dependencies should be discussed in the group so that any downstream effects can be rectified.

## Documentation

https://cbica.github.io/CBICA_Toolkit/

## Functionality Overview

- Command line parsing
- Safe Image I/O (NIfTI, DICOM)
- Image Utilities and wrappers to ITK filters
  - CreateMaskIndeces
  - GetPixelValuesFromIndeces
  - Preprocessing: histogram matching, smoothing, image comparison, orientation fix, skull stripping, resize, resample
  - Image sanity checking
  - Distance calculations: image and world coordinates
  - Create new image based on existing image
- Generic functionalities: file/directory check, sym-link creation, sub-directories, filename splitting, create/delete/copy directory, file/folder size, normalize path, get current working and/or executable directory, get/set environment variables, CSV parsing, confusion matrix, ROC, randn, 
- First order statistics using a standard vector

### Full Function List

https://cbica.github.io/CBICA_Toolkit/functions_func.html

### Full Class List

https://cbica.github.io/CBICA_Toolkit/annotated.html

## Usage 

Add this repo as a **git submodule** (see [this](https://git-scm.com/book/en/v2/Git-Tools-Submodules) page for details on how to do this).

### Root CMakeLists File:

```cmake
# for projects within CaPTk/src/applications
ADD_SUBDIRECTORY( ${PROJECT_SOURCE_DIR}/../../cbica_toolkit ) 

# for projects that add CBICA_Toolkit as a sub-module
ADD_SUBDIRECTORY( ${MAIN_PROJECT_SOURCE_DIR}/src/CBICA_Toolkit ) 

INCLUDE_DIRECTORIES( ${CACHED_INCLUDE_DIRS} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} CBICA_Basic CBICA_)
```

### Source code:

```cpp
#include "cbicaUtilities.h" // or the relevant header file

int main()
{
  // use functions as needed
}
```
\mainpage CBICA Toolkit

  Section of Biomedical Image Analysis <BR>
  Department of Radiology <BR>
  University of Pennsylvania <BR>
  3600 Market Street, Suite 380 <BR>
  Philadelphia, PA 19104 <BR>

  Web:   https://www.cbica.upenn.edu/sbia/ <BR>
  Email: sbia-software at uphs.upenn.edu

  Copyright (c) 2015 University of Pennsylvania. All rights reserved. <BR>
  See https://www.cbica.upenn.edu/sbia/software/license.html or COPYING file.

<b> Author: </b>
Sarthak Pati

===============
1. INTRODUCTION
===============

This project provides a common interface for some popular functions/methods used in the CBICA research center. It is actively maintained and tested on
Windows x64 and Linux (CentOS6) x64. 

All the functions are standalone in nature; i.e., they are header-only and can be exported individually for use/distribution in projects.

Currently, the Toolkit has been tested to be working on Windows x64 using MS Visual Studio 2012 while linking against ITK/4.4.2 built with options
specified in SBIA internal Wiki (https://sbia-wiki.uphs.upenn.edu/wiki/index.php/Third-party_Software#ITK).

<b>TO DO:</b>
- Check installation and usage on cluster with gcc/4.9.2
- Compatibility with ITK/4.7.1
- Individual executables for functions to make Python wrapping easier

==========
2. INSTALL    
==========

----------------
2.1 Dependencies
----------------

- C++ complier
- CMake
- Doxygen (for documentation only)
- GIT (for superbuild system only)
- ITK (required for ITK based classes)

Make sure all dependencies are met before proceeding with install.

-------------------------------------
2.2 Generating standalone executables
-------------------------------------

Use CMake to generate files for the compiler in use (MSVC/GCC) and then run accordingly.

An example under UNIX using CMake command line:

Go to the <b>build</b> directory:<BR>
<CODE>cd build</CODE>

Run CMake (note you can use <CODE>ccmake ../</CODE> to configure the installation directory):<BR>
<CODE>cmake ../</CODE>

Build and install:<BR>
<CODE>make 
make install</CODE>

----------------------------
2.3 Generating documentation
----------------------------

Generate the project files. Then follow appropriate instructions:

2.3.1 For Internal use (integrates with SBIA website):

Enable the option <b>BUILD_DOCUMENTATION_INTERNAL</b> while running cmake.<BR>
UNIX: <CODE>make doc_internal</CODE><BR>
WINDOWS: Build the <CODE>doc_internal</CODE> project.

The documentation is generated into the binary directory by default (<CODE>"BIN_DIR/doc_internal/"</CODE>) and it required Doxygen to run.

<b>NOTE:</b> The folder <CODE>"docs_internal"</CODE> should be never be present in a tag. It is for internal use only.

2.3.1 For External use (to be published along with source code):

Enable the option <b>BUILD_DOCUMENTATION</b> while running cmake.<BR>
UNIX: <CODE>make doc</CODE><BR>
WINDOWS: Build the <CODE>doc</CODE> project.

The documentation is generated into the binary directory by default (<CODE>"BIN_DIR/docs/"</CODE>) and it required Doxygen to run.

-----------
2.4 Testing
-----------

After generating project files, run the following commands:<BR>
UNIX: <CODE>make tests</CODE><BR>
WINDOWS: Build the <CODE>RUN_TESTS</CODE> project.

The test executables (if any) are generated into the binary directory by default (<CODE>"BIN_DIR/testing/"</CODE>).


==============================
3. AVAILABLE FUNCTIONS/CLASSES    
==============================

-------------------
3.1 Basic Functions
-------------------

- <b>fileExists</b>: check if specified file exists or not
- <b>directoryExists</b>: check if specified directory exists or not
- <b>createTmpDir</b>: creates a temporary directory
- <b>createDir</b>: creates a directory
- <b>removeDirectoryRecursively</b>: removes a directory and all its contents
- <b>splitFileName</b>: splits a supplied file name between path, base file name and extension
- <b>findInVector</b>: checks for an element in a vector and returns true or false along with its position in vector (-1 if not found)
- <b>getExeName</b>: get current executable name
- <b>getUserName</b>: get current user name
- <b>convertString</b>: convert a character to its corresponding ASCII code
- <b>stringSplit</b>: split the supplied string with the deliminator and store results in a vector

TO DO:

-----------------
3.1 ITK Classes
-----------------

- <b>ITKImageInfo</b>: obtains the data regarding spacing and dimensions of specified image in 
<code>itk::ImageIOBase</code>; look in class documentation for details

	getImageIOBase		: get the ImageIOBase class wrapped around a smart pointer
	getImageDimensions	: get the dimensions
	getImageSpacings	: get the spacings

TO DO:


===========
4 LICENSING
===========

  See https://www.cbica.upenn.edu/sbia/software/license.html or "licences/COPYING" file.
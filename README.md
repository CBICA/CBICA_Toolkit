\mainpage CBICA Toolkit

  Section of Biomedical Image Analysis <br>
  Department of Radiology <br>
  University of Pennsylvania <br>
  3600 Market Street, Suite 380 <br>
  Philadelphia, PA 19104 <br>

  Web:   https://www.cbica.upenn.edu/sbia/ <br>
  Email: sbia-software at uphs.upenn.edu

  Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
  See https://www.cbica.upenn.edu/sbia/software/license.html or COPYING file.

<b> Authors: </b><br>
  Sarthak – primary developer, integration of everything <br>
	Drew    – DTI algorithms <br>
	Michael – sort + rename <br>
	Martin  – sort + rename <br>
	Jimit   – Overall testing <br>
	Guray   – Overall testing <br>


===============
1. INTRODUCTION
===============

This project provides a common interface for some popular functions/methods used in the CBICA research center. It is actively maintained and tested on
Windows x64 and Linux (CentOS6) x64. 

All the functions are standalone in nature; i.e., they are header-only and can be exported individually for use/distribution in projects.

Currently, the Toolkit has been tested to be working on Windows x64 using MS Visual Studio 2012 while linking against ITK/4.7.0 built with options
specified in SBIA internal Wiki (https://sbia-wiki.uphs.upenn.edu/wiki/index.php/Third-party_Software#ITK).


==========
2. INSTALL    
==========

----------------
2.1 Dependencies
----------------

- C++ complier (MSVC/12, GCC/4.9.2)
- CMake 2.8.12.1
- Doxygen (for documentation only)
- GIT (for superbuild system only) -- ?
- ITK 4.7.0 (required for ITK based classes) - check cmake/ExternalITK for parameters
- SWIG (required for Python Wrapping) [tested on 3.0.0-3.0.4]

Make sure all dependencies are met before proceeding with install.

-------------------------------------
2.2 Generating standalone executables
-------------------------------------

Use CMake to generate files for the compiler in use (MSVC/GCC) and then run accordingly.

An example under UNIX using CMake command line:

Go to the <b>build</b> directory:<br>
<code>cd build</code>

Run CMake (note you can use <code>ccmake ../</code> to configure the installation directory):<br>
<code>cmake ../</code>

Build and install:<br>
<code>make 
make install</code>

----------------------------
2.3 Generating documentation
----------------------------

Generate the project files. Then follow appropriate instructions:

2.3.1 For Internal use (integrates with SBIA website):

Enable the option <b>BUILD_DOCUMENTATION_INTERNAL</b> while running cmake.<br>
UNIX: <code>make doc_internal</code><br>
WINDOWS: Build the <code>doc_internal</code> project.

The documentation is generated into the binary directory by default (<code>"BIN_DIR/doc_internal/"</code>) and it required Doxygen to run.

<b>NOTE:</b> The folder <code>"docs_internal"</code> should be never be present in a tag. It is for internal use only.

2.3.1 For External use (to be published along with source code):

Enable the option <b>BUILD_DOCUMENTATION</b> while running cmake.<br>
UNIX: <code>make doc</code><br>
WINDOWS: Build the <code>doc</code> project.

The documentation is generated into the binary directory by default (<code>"BIN_DIR/docs/"</code>) and it required Doxygen to run.

-----------
2.4 Testing
-----------

After generating project files, run the following commands:<br>
UNIX: <code>make tests</code><br>
WINDOWS: Build the <code>RUN_TESTS</code> project.

The test executables (if any) are generated into the binary directory by default (<code>"BIN_DIR/testing/"</code>).


==============================
3. AVAILABLE FUNCTIONS/CLASSES    
==============================

-------------------
3.1 Basic Functions
-------------------

Folder/File operations:

- <b>fileExists</b>: Check if specified file exists or not
- <b>directoryExists</b>: Check if specified directory exists or not
- <b>createTmpDir</b>: Creates a temporary directory
- <b>createDir</b>: Creates a directory
- <b>makeDir</b>: Wrap for <code>createDir</code>
- <b>removeDirectoryRecursively</b>: Removes a directory and all its contents
- <b>removeDir</b>: Wrap for <code>removeDirectoryRecursively</code>
- <b>deleteDir</b>: Wrap for <code>removeDirectoryRecursively</code>

OS operations:

- <b>getFilenameExtension</b>: Get the extension of the supplied file name
- <b>getExecutableName</b>: Get current executable name
- <b>getFullPath</b>: Get path of current executable
- <b>getUserName</b>: Get current user name
- <b>getCWD</b>: Get current working directory
- <b>normalizePath</b>: Wrap for normPath
- <b>relativePath</b>: Wrap for relPath
- <b>realPath</b>: Reimplementation of python's "os.path.realpath"
- <b>exists</b>: Reimplementation of python's "os.path.exists"
- <b>isLink</b>: Reimplementation of python's "os.path.islink"
- <b>isSymbolicLink</b>: Wrap for <code>isLink</code>
- <b>makeSymbolicLink</b>: Make a symbolic link of a file
- <b>setEnvironmentVariable</b>: Sets the environment variable
- <b>deleteEnvironmentVariable</b>: Delete the environment variable

Python re-implementations: 

- <b>isFile</b>: Reimplementation of "os.path.isfile"--Return True if path is an existing regular file
- <b>isDir</b>: Reimplementation of "os.path.isdir"--Return True if path is an existing directory
- <b>relPath</b>: Reimplementation of "os.path.relpath"--Return a relative filepath to path
- <b>realPath</b>: Reimplementation of "os.path.realpath"--Return the canonical path of the specified filename
- <b>exists</b>: Reimplementation of "os.path.exists"--Return True if path exists and false for broken symbolic links
- <b>isLink</b>: Reimplementation of "os.path.islink"--Return True if path refers to a directory entry that is a symbolic link
- <b>normPath</b>: Reimplementation of "os.path.normpath"--Normalize a pathname by collapsing redundant separators and up-level references

String operations:

- <b>splitFileName</b>: Splits a supplied file name between path, base file name and extension
- <b>stringSplit</b>: Split the supplied string with the deliminator and store results in a vector
- <b>replaceString</b>: Replaces specified string with another string in a larger input string
- <b>constCharToChar</b>: Converts from "const char*" to "char*"
- <b>findInVector</b>: Checks for an element in a vector and returns true or false along with its position in vector (-1 if not found)
- <b>convertString</b>: Convert a character to its corresponding ASCII code
- <b>convertCharacter</b>: Convert first character to integer, double, unsigned int, etc.

TO DO:


[Request a new function via eLog.]

-------------------
3.2 Basic Classes
-------------------

- <b>Logging</b>: A logging class for internal use. Generated log is in the format: <br>
<CODE><4 digit year>:<2 digit month>:<2 digit date>,<2 digit 24 hour>:<2 digit minute>:<2 digit second>;<exe name>;<user name></CODE>

- <b>CmdParser</b>: Universal command line parser. Add parameters, descriptions and call on them from the command line. Details in header file.

	addParameter		: Add parameters one by one<br>
	compareParamter	: Compare added parameters with command line input<br>
	getDescription	: Get description for a particular added parameter<br>
	echoUsage			  : Give usage of the executable<br>
	echoVersion			: Give version details of the executable<br>

TO DO:


[Request a new function via eLog.]

-----------------
3.3 ITK Classes
-----------------



[Request a new function via eLog.]

===========
4 LICENSING
===========

  See https://www.cbica.upenn.edu/sbia/software/license.html or "licences/COPYING" file.
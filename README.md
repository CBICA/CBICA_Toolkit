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
  Sarthak – primary dev, integration of everything <br>
	Drew – DTI algos <br>
	Michael – sort + rename <br>
	Martin – sort + rename <br>
	Jimit – python wraps <br>
	Guray – Overall testing <br>


===============
1. INTRODUCTION
===============

This project provides a common interface for some popular functions/methods used in the CBICA research center. It is actively maintained and tested on
Windows x64 and Linux (CentOS6) x64. 

All the functions are standalone in nature; i.e., they are header-only and can be exported individually for use/distribution in projects.

Currently, the Toolkit has been tested to be working on Windows x64 using MS Visual Studio 2012 while linking against ITK/4.4.2 built with options
specified in SBIA internal Wiki (https://sbia-wiki.uphs.upenn.edu/wiki/index.php/Third-party_Software#ITK).

<b>TO DO:</b>
- Compatibility with ITK/4.7.1
- Individual executables for functions to make Python wrapping easier

[Request a new function via eLog.]

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

Folder/File:

- <b>fileExists</b>: check if specified file exists or not
- <b>directoryExists</b>: check if specified directory exists or not
- <b>createTmpDir</b>: creates a temporary directory
- <b>createDir</b>: creates a directory
- <b>makeDir</b>: wrap for <code>createDir</code>
- <b>removeDirectoryRecursively</b>: removes a directory and all its contents
- <b>removeDir</b>: wrap for <code>removeDirectoryRecursively</code>
- <b>deleteDir</b>: wrap for <code>removeDirectoryRecursively</code>

OS:

- <b>getFilenameExtension</b>: get the extension of the supplied file name
- <b>getExecutableName</b>: get current executable name
- <b>getFullPath</b>: get path of current executable
- <b>getUserName</b>: get current user name
- <b>getCWD</b>: get current working directory
- <b>normPath</b>: Wrap of python's "os.path.normpath"--Normalize a pathname by collapsing redundant separators and up-level references
- <b>normalizePath</b>: Wrap for normPath
- <b>relPath</b>: Wrap for python's "os.path.relpath"--Return a relative filepath to path
- <b>realPath</b>: Wrap of python's "os.path.realpath"--Return the canonical path of the specified filename
- <b>isFile</b>: Wrap for python's "os.path.isfile"--Return True if path is an existing regular file
- <b>isDir</b>: Wrap of python's "os.path.isdir"--Return True if path is an existing directory
- <b>exists</b>: Wrap of python's "os.path.exists"--Return True if path exists and false for broken symbolic links
- <b>isLink</b>: Wrap of python's "os.path.islink"--Return True if path refers to a directory entry that is a symbolic link
- <b>makeSymbolicLink</b>: Make a symbolic link of a file

String:

- <b>findInVector</b>: checks for an element in a vector and returns true or false along with its position in vector (-1 if not found)
- <b>splitFileName</b>: splits a supplied file name between path, base file name and extension
- <b>convertString</b>: convert a character to its corresponding ASCII code
- <b>stringSplit</b>: split the supplied string with the deliminator and store results in a vector
- <b>replaceString</b>: replaces specified string with another string in a larger input string

TO DO:


[Request a new function via eLog.]

-------------------
3.2 Basic Classes
-------------------

- <b>Logging</b>: A logging class for internal use. Generated log is in the format: <br>
<CODE><4 digit year>:<2 digit month>:<2 digit date>,<2 digit 24 hour>:<2 digit minute>:<2 digit second>;<exe name>;<user name></CODE>

- <b>CmdParser</b>: Universal command line parser. Add parameters, descriptions and call on them from the command line. Details in header file.

	addParameter		: add parameters one by one<br>
	compareParamter	: compare added parameters with command line input<br>
	getDescription	: get description for a particular added parameter<br>
	echoUsage			  : give usage of the executable<br>
	echoVersion			: give version details of the executable<br>

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
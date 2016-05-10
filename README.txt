/**

\mainpage Overview

This project provides a common interface for some popular functions/methods used in the CBICA research center. It is actively maintained and tested on
Windows x64 and Linux (CentOS6) x64. 

All the functions are standalone in nature; i.e., they are header-only and can be exported individually for use/distribution in projects.

Currently, the Toolkit has been tested to be working on Windows x64 using MS Visual Studio 2012 while linking against ITK/4.7.1 built with options
specified in SBIA internal Wiki (http://sbia-wiki.uphs.upenn.edu/wiki/index.php/Third-party_Software#ITK).

<b>TO DO:</b>
- Individual executables for functions to make Python wrapping easier

[Request a new function via eLog.]

---

# 1. Installation

## Requirements

- CMake (for project configuration)
- C++ compiler
- ITK (for ITK-based classes)
- OpenCV (for classes that do machine learning)

See the <a href="Installation.html">Installation Page</a> for details.

---

# 2. Available Functions and Classes

## 2.1 Basic Functions

For a full list, please see the documentation of cbicaUtilities.h file.

### 2.1.1 Folder/File operations

<table border="0">
	<tr>
		<td width="200px"><b>fileExists</b></td>
		<td>Check if specified file exists or not</td>
	</tr>
	<tr>
		<td><b>isFile</b></td>
		<td>Wrap of <code>fileExists</code></td>
	</tr>
	<tr>
		<td><b>directoryExists</b></td>
		<td>Check if specified directory exists or not</td>
	</tr>
	<tr>
		<td><b>isDir</b></td>
		<td>Wrap of <code>directoryExists</code></td>
	</tr>
	<tr>
		<td><b>createTemporaryDirectory</b></td>
		<td>Creates a temporary directory</td>
	</tr>
	<tr>
		<td><b>makeTemporaryDirectory</b></td>
		<td>Wrap of <code>createTemporaryDirectory</code></td>
	</tr>
	<tr>
		<td><b>createTmpDir</b></td>
		<td>Wrap of<code>createTemporaryDirectory</code></td>
	</tr>
	<tr>
		<td><b>createDir</b></td>
		<td>Creates a directory</td>
	</tr>
	<tr>
		<td><b>makeDir</b></td>
		<td>Wrap for <code>createDir</code></td>
	</tr>
	<tr>
		<td><b>removeDirectoryRecursively</b></td>
		<td>Removes a directory and all its contents recursively</td>
	</tr>
	<tr>
		<td><b>removeDir</b></td>
		<td>Wrap for <code>removeDirectoryRecursively</code></td>
	</tr>
	<tr>
		<td><b>deleteDir</b></td>
		<td>Wrap for <code>removeDirectoryRecursively</code></td>
	</tr>
	<tr>
		<td><b>numberOfRowsInFile</b></td>
		<td>Number of rows in CSV file</td>
	</tr>
	<tr>
		<td><b>numberOfColsInFile</b></td>
		<td>Number of cols in CSV file</td>
	</tr>
	<tr>
		<td><b>parseCSVFile</b></td>
		<td>Parse a list CSV file which contains file names and labels</td>
	</tr>
</table>

### 2.1.2 OS operations

<table border="0">
	<tr>
		<td width="200px"><b>getFilenamePath</b></td>
		<td>Get the path of the supplied file name (uses <code>splitFileName</code>)</td>
	</tr>
	<tr>
		<td><b>getFilenameBase</b></td>
		<td>Get the base of the supplied file name (uses <code>splitFileName</code>)</td>
	</tr>
	<tr>
		<td><b>getFilenameExtension</b></td>
		<td>Get the extension of the supplied file name (uses <code>splitFileName</code>)</td>
	</tr>
	<tr>
		<td><b>getExecutableName</b></td>
		<td>Get current executable name</td>
	</tr>
	<tr>
		<td><b>getFullPath</b></td>
		<td>Get full path of current executable</td>
	</tr>
	<tr>
		<td><b>getUserName</b></td>
		<td>Get current user name</td>
	</tr>
	<tr>
		<td><b>getCWD</b></td>
		<td>Get current working directory</td>
	</tr>
	<tr>
		<td><b>normalizePath</b></td>
		<td>Wrap for <code>normPath</code></td>
	</tr>
	<tr>
		<td><b>relativePath</b></td>
		<td>Wrap for <code>relPath</code></td>
	</tr>
	<tr>
		<td><b>realPath</b></td>
		<td>Reimplementation of python's <code>os.path.realpath</code></td>
	</tr>
	<tr>
		<td><b>isLink</b></td>
		<td>Reimplementation of python's <code>os.path.islink</code></td>
	</tr>
	<tr>
		<td><b>isSymbolicLink</b></td>
		<td>Wrap for <code>islink</code></td>
	</tr>
	<tr>
		<td><b>makeSymbolicLink</b></td>
		<td>Make a symbolic link of a file</td>
	</tr>
	<tr>
		<td><b>setEnvironmentVariable</b></td>
		<td>Sets the environment variable</td>
	</tr>
	<tr>
		<td><b>deleteEnvironmentVariable</b></td>
		<td>Delete the environment variable</td>
	</tr>
	<tr>
		<td><b>filesInDirectory</b></td>
		<td>Find all files in the specified directory (recursion available)</td>
	</tr>
	<tr>
		<td><b>subdirectoriesInDirectory</b></td>
		<td>Find all directories - recursive turned off by default</td>
	</tr>
</table>

### 2.1.3 Python re-implementations

<table border="0">
	<tr>
		<td width="200px"><b>isFile</b></td>
		<td>Return True if path is an existing regular file</td>
	</tr>
	<tr>
		<td><b>isDir</b></td>
		<td>Return True if path is an existing directory</td>
	</tr>
	<tr>
		<td><b>relPath</b></td>
		<td>Return a relative filepath to path</td>
	</tr>
	<tr>
		<td><b>realPath</b></td>
		<td>Return the canonical path of the specified filename</td>
	</tr>
	<tr>
		<td><b>exists</b></td>
		<td>Return True if path exists and false for broken symbolic links</td>
	</tr>
	<tr>
		<td><b>exists</b></td>
		<td>Return True if path refers to a directory entry that is a symbolic link</td>
	</tr>
	<tr>
		<td><b>normPath</b></td>
		<td>Normalize a pathname by collapsing redundant separators and up-level references</td>
	</tr>
</table>

### 2.1.4 String operations

<table border="0">
	<tr>
		<td width="200px"><b>splitFileName</b></td>
		<td>Splits a supplied file name between path, base file name and extension</td>
	</tr>
	<tr>
		<td><b>stringSplit</b></td>
		<td>Split the supplied string with the deliminator and store results in a vector</td>
	</tr>
	<tr>
		<td><b>replaceString</b></td>
		<td>Replaces specified string with another string in a larger input string</td>
	</tr>
	<tr>
		<td><b>constCharToChar</b></td>
		<td>Converts from <code>const char*</code> to <code>char*</code></td>
	</tr>
	<tr>
		<td><b>findInVector</b></td>
		<td>Checks for an element in a vector and returns true or false along with its position in vector (-1 if not found)</td>
	</tr>
	<tr>
		<td><b>convertString</b></td>
		<td>Convert a character to its corresponding ASCII code</td>
	</tr>
	<tr>
		<td><b>convertCharacter</b></td>
		<td>Convert first character to integer, double, unsigned int, etc.</td>
	</tr>
</table>

### 2.1.5 Miscellaneous 

<table border="0">
	<tr>
		<td width="200px"><b>compareEqual</b></td>
		<td>Compare if multiple inputs are equal</td>
	</tr>
	<tr>
		<td><b>compareGreater</b></td>
		<td>Compare if first input is greater than rest</td>
	</tr>
	<tr>
		<td><b>compareLesser</b></td>
		<td>Compare if first input is smaller than rest</td>
	</tr>
</table>

### 2.1.6 STD Wraps (these are present for GCC < 5)

<table border="0">
	<tr>
		<td width="200px"><b>round</b></td>
		<td>std::round</td>
	</tr>
	<tr>
		<td><b>to_string</b></td>
		<td>std::to_string</td>
	</tr>
</table>

### 2.1.6 TO DO:

[Request a new function via eLog.]


## 2.2 Basic Classes

### 2.2.1 Logging

A logging class which generates machine-parseable logs. This can output to both console (default behavior) and a file. For full documentation, see the cbica::Logging.

Generated log is in the format: <br>
<CODE><4 digit year>:<2 digit month>:<2 digit date>,<2 digit 24 hour>:<2 digit minute>:<2 digit second>;<exe name>;<user name></CODE>

#### Available functions:

<table border="0">
	<tr>
		<td width="200px"><b>EnableTextLogging</b></td>
		<td>Switches from console to specific text file</td>
	</tr>
	<tr>
		<td><b>Write</b></td>
		<td>Writes free text to console/file as std::cout</td>
	</tr>
	<tr>
		<td><b>WriteError</b></td>
		<td>Writes free text to console/file as std::cerr - used for error reporting</td>
	</tr>
	<tr>
		<td><b>UseNewFile</b></td>
		<td>Change Logging file after initializing class</td>
	</tr>
</table>

### 2.2.2 CmdParser

Universal command line parser. Add parameters, descriptions and call on them from the command line. Details in header file. It picks up exe name using getExecutableName() but it can be set in the class. The parameters "u/usage", "v/version" and "h/help" are automatically set. 'usage' is basic usage details, 'help' is verbose help and 'version' is version details (picked up automatically from parent CMake file). For full documentation, see the cbica::CmdParser.
  
#### Available functions: 

<table border="0">
	<tr>
		<td width="200px"><b>addRequiredParameter</b></td>
		<td>Add required parameter (no '-' is necessary) - exception is thrown if a required parameter is missing</td>
	</tr>
	<tr>
		<td><b>addOptionalParameter</b></td>
		<td>Add optional parameter (no '-' is necessary)</td>
	</tr>
	<tr>
		<td><b>addParameter</b></td>
		<td>Defaults to <code>addOptionalParameter()</code></td>
	</tr>
	<tr>
		<td><b>compareParameter</b></td>
		<td>Check if specific parameter is present in the argv</td>
	</tr>
	<tr>
		<td><b>getDescription</b></td>
		<td>Get description of the specific parameter</td>
	</tr>
	<tr>
		<td><b>exampleUsage</b></td>
		<td>Add an example usage which comes up in <code>echoHelp()</code></td>
	</tr>
	<tr>
		<td><b>setExeName</b></td>
		<td>Set a custom executable name</td>
	</tr>
	<tr>
		<td><b>writeConfigFile</b></td>
		<td> Writes a machine readable configuration file for the executable</td>
	</tr>
	<tr>
		<td><b>echoUsage</b></td>
		<td> Based on the parameters added, it automatically constructs an <code>echoUsage()</code> console output which includes contact information and copyright details</td>
	</tr>
	<tr>
		<td><b>echoHelp</b></td>
		<td> Based on the parameters added, it automatically constructs an <code>echoHelp()</code> console output which includes contact information and copyright details. This is supposed to be more verbose than <code>echoUsage()</code></td>
	</tr>
</table>

### 2.2.3 TO DO

[Request a new function via eLog.]


## 2.3 ITK Classes

### 2.3.1 ImageInfo

Obtains the data regarding spacing and dimensions of specified image in <code>itk::ImageIOBase</code>; look in class documentation for details.

#### Available functions: 

<table border="0">
	<tr>
		<td width="200px"><b>getImageIOBase</b></td>
		<td>Get the ImageIOBase class wrapped around a smart pointer</td>
	</tr>
	<tr>
		<td><b>getImageDimensions</b></td>
		<td>Get the dimensions</td>
	</tr>
	<tr>
		<td><b>getImageSpacings</b></td>
		<td>Get the spacings</td>
	</tr>
</table>

### 2.3.2 CommonHolder

Common interface class for all algorithmic classes.

### 2.3.3 ComputeAverageMap

Computes the average of a series of images and writes the output. Set input files as a vector of strings and the output directory in the constructor itself.

### 2.3.4 ComputeDtiScalars

Computes and saves the specified scalar image for an image. Input image and output (director/file) are specified in the constructor.


#### Available Options: 

<table border="0">
	<tr>
		<td width="200px"><b>FA</b></td>
		<td>FA images</td>
	</tr>
	<tr>
		<td><b>EIG</b></td>
		<td>Eigensystem images</td>
	</tr>
	<tr>
		<td><b>GEO</b></td>
		<td>Geoemtric features</td>
	</tr>
	<tr>
		<td><b>SKEW</b></td>
		<td>Tensor Skewness</td>
	</tr>
	<tr>
		<td><b>KURT</b></td>
		<td>Tensor kurtosis</td>
	</tr>
	<tr>
		<td><b>RADAX</b></td>
		<td>Radial and Axial diffusivity</td>
	</tr>
	<tr>
		<td><b>GORDR</b></td>
		<td>Gordon's R measures</td>
	</tr>
	<tr>
		<td><b>GORDK</b></td>
		<td>Gordon's K measures</td>
	</tr>
	<tr>
		<td><b>ALL</b></td>
		<td>All of the above (default)</td>
	</tr>
</table>

### 2.3.5 ComputeVarianceMap

Computes the variance and saves the output for a single image (can be changed easily by modifying the class to be more in line with ComputeAverageMap).

### 2.3.6 DtiRecon

Reconstruction of DTI image

### 2.3.7 ITKUtilities

Functions to make handling of images easier.

<table border="0">
	<tr>
		<td width="200px"><b>VectorizeImages</b></td>
		<td>Vectorize a set of images in either column or row major format</td>
	</tr>
	<tr>
		<td><b>CreateMaskIndeces</b></td>
		<td>Eigensystem images</td>
	</tr>
</table>

NOTE: There are a lot of back-end classes which have been implemented for compatibility between itk versions. These are not meant to be for standalone usage.

TO DO:
- dti Stuff

[Request a new function via eLog.]


---

# 3 Developer Information

The CBICATk has a certain code guideline. For details, please see the <a href="ForDevelopers.html">For Developers Page</a>

---

# 4 Licensing information

See the <a href="http://www.cbica.upenn.edu/sbia/software/license.html">SBIA License</a>  or "licences/COPYING" file.

The CBICATk code incorporates 3rd party libraries and toolkits, potentially including but not limited to:

- ITK
- OpenCV
		
All rights to these components are subject to the terms and licenses of their respective owners. All trademarks are the property of their respective owners. 

*/
/**

\mainpage Overview

This project provides a common interface for some popular functions/methods used in the CBICA research center. It is actively maintained and tested on
Windows x64 and Linux (CentOS6) x64. 

All the functions are standalone in nature; i.e., they are header-only and can be exported individually for use/distribution in projects.

Currently, the Toolkit has been tested to be working on Windows x64 using MS Visual Studio 2012 while linking against ITK/4.7.1 and ITK/4.9.0 built with options
specified in SBIA internal Wiki (http://sbia-wiki.uphs.upenn.edu/wiki/index.php/Third-party_Software#ITK).

<b>TO DO:</b>
- Individual executables for functions to make Python wrapping easier

[Request a new function via eLog.]

---

#  Installation

## Requirements

- CMake (for project configuration)
- C++ compiler
- ITK (for ITK-based classes)
- OpenCV (for classes that do machine learning)

See the <a href="Installation.html">Installation Page</a> for details.

---

# Available Functions and Classes

1. [Full Functions List](functions_func.html)

2. [Full Class List](annotated.html)

## Overview

1. Basic Utilities (no third party library dependency): These are in the **Functions** section of the [cbicaUtilities.h](cbicaUtilities_8h.html) file.

2. Basic Classes: These make a C++ developer's life easier; no third party dependency.

  1. [CmdParser](cbicaCmdParser_8h.html): This is a universal command line parser. Add parameters, descriptions and call on them from the command line. Details in the header file. It picks up exe name using getExecutableName() but it can be set in the class. The parameters "u/usage", "v/version" and "h/help" are automatically set. 'usage' is basic usage details, 'help' is verbose help and 'version' is version details (picked up automatically from parent CMake file). For full documentation, see the [cbica::CmdParser](cbicaCmdParser_8h.html) documentation.
  
  2. [Logging](cbicaLogging_8h.html): A logging class which generates machine-parseable logs. This can output to both console (default behavior) and a file. For full documentation, see the [cbica::Logging](cbicaLogging_8h.html) documentation. Generated log is in the following format: 
```
<4 digit year>:<2 digit month>:<2 digit date>,<2 digit 24 hour>:<2 digit minute>:<2 digit second>;<exe name>;<user name>
```

3. ITK Utilities (dependency on ITK): These are in the **Functions** section of the [cbicaITKUtilities.h](cbicaITKUtilities_8h.html) file.

4. ITK-based Safe I/O Functions: These functions are used for safe I/O operations for various medical images. Details are in the [cbicaITKUtilities.h](cbicaITKSafeImageIO_8h.html) file.

5. Host of other specialized functions from the `src/classes/itk` folder.

## Benchmarking Executable

This is a shell executable that is used for benchmarking different algorithms and applications. This provides a ready-to-use way for comparing different algorithm outputs with the one that needs to be tested for multiple inputs. See <code>src/Benchmarking.cxx</code> for details.

## TO DO

Request a new function by emailing dev@cbica.upenn.edu


---

# Developer Information

The CBICATk has a certain code guideline. For details, please see the [For Developers](ForDevelopers.html)

---

# Licensing information

See the [CBICA License](http://www.cbica.upenn.edu/sbia/software/license.html)  or "LICENSE" file.

The CBICATk code incorporates 3rd party libraries and toolkits, potentially including but not limited to:

- ITK
- OpenCV
		
All rights to these components are subject to the terms and licenses of their respective owners. All trademarks are the property of their respective owners. 

*/